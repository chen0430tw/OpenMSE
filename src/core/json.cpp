#include "json.hpp"

#include <cerrno>
#include <cstdlib>

namespace openmse::core {

namespace {
bool ieq(std::string_view a, std::string_view b)
{
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        char x = a[i], y = b[i];
        if (x >= 'A' && x <= 'Z') x = static_cast<char>(x + 32);
        if (y >= 'A' && y <= 'Z') y = static_cast<char>(y + 32);
        if (x != y) return false;
    }
    return true;
}

void append_utf8(std::string& out, unsigned cp)
{
    if (cp < 0x80) out.push_back(static_cast<char>(cp));
    else if (cp < 0x800) { out.push_back(static_cast<char>(0xC0 | (cp >> 6))); out.push_back(static_cast<char>(0x80 | (cp & 0x3F))); }
    else if (cp < 0x10000) { out.push_back(static_cast<char>(0xE0 | (cp >> 12))); out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F))); out.push_back(static_cast<char>(0x80 | (cp & 0x3F))); }
    else { out.push_back(static_cast<char>(0xF0 | (cp >> 18))); out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F))); out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F))); out.push_back(static_cast<char>(0x80 | (cp & 0x3F))); }
}

class Parser {
public:
    explicit Parser(std::string_view s) : s_(s) {}
    JsonValue document()
    {
        JsonValue v = value(0);
        ws();
        if (p_ != s_.size()) fail("JSON 末尾有多余内容");
        return v;
    }
private:
    std::string_view s_;
    std::size_t p_ = 0;

    [[noreturn]] void fail(const char* what) const { throw JsonError(std::string(what) + "（位置 " + std::to_string(p_) + "）。"); }
    void ws() { while (p_ < s_.size() && (s_[p_] == ' ' || s_[p_] == '\t' || s_[p_] == '\n' || s_[p_] == '\r')) ++p_; }
    bool literal(std::string_view word) { if (s_.substr(p_, word.size()) != word) return false; p_ += word.size(); return true; }

    unsigned hex4()
    {
        if (p_ + 4 > s_.size()) fail("\\u 转义不完整");
        unsigned v = 0;
        for (int i = 0; i < 4; ++i) {
            char c = s_[p_++];
            v <<= 4;
            if (c >= '0' && c <= '9') v |= static_cast<unsigned>(c - '0');
            else if (c >= 'a' && c <= 'f') v |= static_cast<unsigned>(c - 'a' + 10);
            else if (c >= 'A' && c <= 'F') v |= static_cast<unsigned>(c - 'A' + 10);
            else fail("\\u 转义无效");
        }
        return v;
    }

    std::string string()
    {
        ++p_; // opening quote
        std::string out;
        while (true) {
            if (p_ >= s_.size()) fail("字符串未结束");
            unsigned char c = static_cast<unsigned char>(s_[p_++]);
            if (c == '"') return out;
            if (c < 0x20) fail("字符串包含未转义的控制字符");
            if (c != '\\') { out.push_back(static_cast<char>(c)); continue; }
            if (p_ >= s_.size()) fail("转义不完整");
            char e = s_[p_++];
            switch (e) {
            case '"': out.push_back('"'); break;
            case '\\': out.push_back('\\'); break;
            case '/': out.push_back('/'); break;
            case 'b': out.push_back('\b'); break;
            case 'f': out.push_back('\f'); break;
            case 'n': out.push_back('\n'); break;
            case 'r': out.push_back('\r'); break;
            case 't': out.push_back('\t'); break;
            case 'u': {
                unsigned cp = hex4();
                if (cp >= 0xD800 && cp <= 0xDBFF) {
                    if (!literal("\\u")) fail("缺少低位代理项");
                    unsigned low = hex4();
                    if (low < 0xDC00 || low > 0xDFFF) fail("代理项无效");
                    cp = 0x10000 + ((cp - 0xD800) << 10) + (low - 0xDC00);
                } else if (cp >= 0xDC00 && cp <= 0xDFFF) fail("代理项无效");
                append_utf8(out, cp);
                break;
            }
            default: fail("未知转义");
            }
        }
    }

    void number(JsonValue& v)
    {
        std::size_t start = p_;
        bool fractional = false;
        if (s_[p_] == '-') ++p_;
        if (p_ >= s_.size() || s_[p_] < '0' || s_[p_] > '9') fail("数字无效");
        if (s_[p_] == '0') ++p_; else while (p_ < s_.size() && s_[p_] >= '0' && s_[p_] <= '9') ++p_;
        if (p_ < s_.size() && s_[p_] == '.') {
            fractional = true; ++p_;
            if (p_ >= s_.size() || s_[p_] < '0' || s_[p_] > '9') fail("数字无效");
            while (p_ < s_.size() && s_[p_] >= '0' && s_[p_] <= '9') ++p_;
        }
        if (p_ < s_.size() && (s_[p_] == 'e' || s_[p_] == 'E')) {
            fractional = true; ++p_;
            if (p_ < s_.size() && (s_[p_] == '+' || s_[p_] == '-')) ++p_;
            if (p_ >= s_.size() || s_[p_] < '0' || s_[p_] > '9') fail("数字无效");
            while (p_ < s_.size() && s_[p_] >= '0' && s_[p_] <= '9') ++p_;
        }
        v.type = JsonValue::Type::number;
        if (!fractional) {
            std::string digits(s_.substr(start, p_ - start));
            errno = 0;
            long long n = std::strtoll(digits.c_str(), nullptr, 10);
            if (errno == 0) { v.integer = true; v.int_value = n; }
        }
    }

    JsonValue value(int depth)
    {
        if (depth > 64) fail("JSON 嵌套过深");
        ws();
        if (p_ >= s_.size()) fail("缺少 JSON 值");
        JsonValue v;
        v.begin = p_;
        char c = s_[p_];
        if (c == '{') {
            v.type = JsonValue::Type::object; ++p_; ws();
            if (p_ < s_.size() && s_[p_] == '}') { v.close = p_++; }
            else while (true) {
                ws();
                if (p_ >= s_.size() || s_[p_] != '"') fail("对象成员名必须是字符串");
                JsonMember m;
                m.key_begin = p_;
                m.key = string();
                ws();
                if (p_ >= s_.size() || s_[p_] != ':') fail("缺少冒号");
                ++p_;
                m.value = value(depth + 1);
                v.members.push_back(std::move(m));
                ws();
                if (p_ < s_.size() && s_[p_] == ',') { ++p_; continue; }
                if (p_ < s_.size() && s_[p_] == '}') { v.close = p_++; break; }
                fail("对象缺少逗号或右花括号");
            }
        } else if (c == '[') {
            v.type = JsonValue::Type::array; ++p_; ws();
            if (p_ < s_.size() && s_[p_] == ']') { v.close = p_++; }
            else while (true) {
                v.items.push_back(value(depth + 1));
                ws();
                if (p_ < s_.size() && s_[p_] == ',') { ++p_; continue; }
                if (p_ < s_.size() && s_[p_] == ']') { v.close = p_++; break; }
                fail("数组缺少逗号或右方括号");
            }
        } else if (c == '"') {
            v.type = JsonValue::Type::string; v.text = string();
        } else if (c == '-' || (c >= '0' && c <= '9')) {
            number(v);
        } else if (literal("true")) { v.type = JsonValue::Type::boolean; v.boolean = true; }
        else if (literal("false")) { v.type = JsonValue::Type::boolean; }
        else if (literal("null")) { v.type = JsonValue::Type::null; }
        else fail("无法识别的 JSON 值");
        v.end = p_;
        return v;
    }
};
} // namespace

const JsonMember* JsonValue::find_member(std::string_view key) const
{
    for (const auto& m : members) if (ieq(m.key, key)) return &m;
    return nullptr;
}

const JsonValue* JsonValue::find(std::string_view key) const
{
    const JsonMember* m = find_member(key);
    return m ? &m->value : nullptr;
}

JsonValue parse_json(std::string_view text) { return Parser(text).document(); }

std::string json_quote(std::string_view s, bool js_safe)
{
    static const char hex[] = "0123456789abcdef";
    std::string out;
    out.reserve(s.size() + 2);
    out.push_back('"');
    for (unsigned char c : s) {
        switch (c) {
        case '"': out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\b': out += "\\b"; break;
        case '\f': out += "\\f"; break;
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        default:
            if (c < 0x20 || (js_safe && (c == '<' || c == '>' || c == '&' || c == '\''))) {
                out += "\\u00"; out.push_back(hex[c >> 4]); out.push_back(hex[c & 15]);
            } else out.push_back(static_cast<char>(c));
        }
    }
    out.push_back('"');
    return out;
}

} // namespace openmse::core
