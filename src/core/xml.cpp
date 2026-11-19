#include "xml.hpp"

#include <cstdlib>
#include <stdexcept>

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

std::string local(const std::string& name)
{
    std::size_t colon = name.find(':');
    return colon == std::string::npos ? name : name.substr(colon + 1);
}

void append_utf8(std::string& out, unsigned long cp)
{
    if (cp < 0x80) out.push_back(static_cast<char>(cp));
    else if (cp < 0x800) { out.push_back(static_cast<char>(0xC0 | (cp >> 6))); out.push_back(static_cast<char>(0x80 | (cp & 0x3F))); }
    else if (cp < 0x10000) { out.push_back(static_cast<char>(0xE0 | (cp >> 12))); out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F))); out.push_back(static_cast<char>(0x80 | (cp & 0x3F))); }
    else { out.push_back(static_cast<char>(0xF0 | (cp >> 18))); out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F))); out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F))); out.push_back(static_cast<char>(0x80 | (cp & 0x3F))); }
}

class Reader {
public:
    explicit Reader(std::string_view s) : s_(s) {}
    XmlNode document()
    {
        misc();
        if (p_ >= s_.size() || s_[p_] != '<') fail("缺少根元素");
        XmlNode root = element(0);
        misc();
        if (p_ != s_.size()) fail("根元素之后有多余内容");
        return root;
    }
private:
    std::string_view s_;
    std::size_t p_ = 0;
    [[noreturn]] void fail(const char* what) const { throw std::runtime_error(std::string("XML 无效：") + what + "（位置 " + std::to_string(p_) + "）。"); }
    bool starts(std::string_view t) const { return s_.substr(p_, t.size()) == t; }
    void ws() { while (p_ < s_.size() && (s_[p_] == ' ' || s_[p_] == '\t' || s_[p_] == '\r' || s_[p_] == '\n')) ++p_; }
    void skip_to(std::string_view end)
    {
        std::size_t e = s_.find(end, p_);
        if (e == std::string_view::npos) fail("结构未结束");
        p_ = e + end.size();
    }
    void misc()
    {
        while (true) {
            ws();
            if (starts("<?")) skip_to("?>");
            else if (starts("<!--")) skip_to("-->");
            else if (starts("<!DOCTYPE")) fail("不支持 DTD");
            else return;
        }
    }
    std::string name()
    {
        std::size_t b = p_;
        while (p_ < s_.size()) {
            char c = s_[p_];
            if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '/' || c == '>' || c == '=') break;
            ++p_;
        }
        if (b == p_) fail("缺少名称");
        return std::string(s_.substr(b, p_ - b));
    }
    std::string decode(std::string_view raw)
    {
        std::string out;
        for (std::size_t i = 0; i < raw.size(); ++i) {
            if (raw[i] != '&') { out.push_back(raw[i]); continue; }
            std::size_t semi = raw.find(';', i);
            if (semi == std::string_view::npos) fail("实体未结束");
            std::string_view e = raw.substr(i + 1, semi - i - 1);
            if (e == "lt") out.push_back('<'); else if (e == "gt") out.push_back('>'); else if (e == "amp") out.push_back('&');
            else if (e == "quot") out.push_back('"'); else if (e == "apos") out.push_back('\'');
            else if (e.size() > 1 && e[0] == '#') {
                bool hex = e[1] == 'x' || e[1] == 'X';
                std::string digits(e.substr(hex ? 2 : 1));
                char* end = nullptr;
                unsigned long cp = std::strtoul(digits.c_str(), &end, hex ? 16 : 10);
                if (digits.empty() || *end || cp == 0 || cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) fail("数字实体无效");
                append_utf8(out, cp);
            } else fail("未知实体");
            i = semi;
        }
        return out;
    }
    XmlNode element(int depth)
    {
        if (depth > 128) fail("嵌套过深");
        ++p_; // '<'
        XmlNode n;
        std::string raw = name();
        n.name = local(raw);
        while (true) {
            ws();
            if (p_ >= s_.size()) fail("标签未结束");
            if (starts("/>")) { p_ += 2; return n; }
            if (s_[p_] == '>') { ++p_; break; }
            std::string key = local(name());
            ws();
            if (p_ >= s_.size() || s_[p_] != '=') fail("属性缺少 =");
            ++p_; ws();
            if (p_ >= s_.size() || (s_[p_] != '"' && s_[p_] != '\'')) fail("属性值必须加引号");
            char q = s_[p_++];
            std::size_t e = s_.find(q, p_);
            if (e == std::string_view::npos) fail("属性值未结束");
            n.attributes.emplace_back(key, decode(s_.substr(p_, e - p_)));
            p_ = e + 1;
        }
        while (true) {
            if (p_ >= s_.size()) fail("元素未结束：" );
            if (starts("</")) {
                p_ += 2;
                std::string closing = name();
                if (closing != raw) fail("结束标签不匹配");
                ws();
                if (p_ >= s_.size() || s_[p_] != '>') fail("结束标签未结束");
                ++p_;
                return n;
            }
            if (starts("<!--")) { skip_to("-->"); continue; }
            if (starts("<![CDATA[")) {
                p_ += 9;
                std::size_t e = s_.find("]]>", p_);
                if (e == std::string_view::npos) fail("CDATA 未结束");
                n.text.append(s_.substr(p_, e - p_));
                p_ = e + 3;
                continue;
            }
            if (starts("<?")) { skip_to("?>"); continue; }
            if (s_[p_] == '<') { n.children.push_back(element(depth + 1)); continue; }
            std::size_t e = s_.find('<', p_);
            if (e == std::string_view::npos) fail("元素未结束");
            n.text += decode(s_.substr(p_, e - p_));
            p_ = e;
        }
    }
};
} // namespace

std::string XmlNode::attribute(std::string_view key) const
{
    for (const auto& a : attributes) if (ieq(a.first, key)) return a.second;
    return {};
}

std::vector<const XmlNode*> XmlNode::all(std::string_view child) const
{
    std::vector<const XmlNode*> out;
    for (const auto& c : children) if (ieq(c.name, child)) out.push_back(&c);
    return out;
}

const XmlNode* XmlNode::first(std::string_view child) const
{
    for (const auto& c : children) if (ieq(c.name, child)) return &c;
    return nullptr;
}

XmlNode parse_xml(std::string_view utf8) { return Reader(utf8).document(); }

} // namespace openmse::core
