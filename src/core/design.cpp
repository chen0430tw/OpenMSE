#include "design.hpp"

#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <set>

namespace openmse::core {

namespace {

// ---------------------------------------------------------------- shared rules

const std::set<std::string>& allowed_controls()
{
    static const std::set<std::string> s = {
        "Button", "Label", "TextBox", "RichTextBox", "CheckBox", "RadioButton", "ComboBox", "ListBox",
        "CheckedListBox", "GroupBox", "Panel", "PictureBox", "ProgressBar", "TrackBar", "NumericUpDown",
        "DateTimePicker", "MonthCalendar", "TreeView", "ListView", "TabControl", "LinkLabel", "DataGridView" };
    return s;
}

// Default event per control kind: what double-clicking the control in the designer opens.
struct EventSpec { const char* name; const char* args; const char* delegate; };
EventSpec default_event(const std::string& kind)
{
    static const char* eh = "System.EventHandler";
    static const char* ea = "System.EventArgs";
    if (kind == "Form") return { "Load", ea, eh };
    if (kind == "CheckBox" || kind == "RadioButton") return { "CheckedChanged", ea, eh };
    if (kind == "TextBox" || kind == "RichTextBox") return { "TextChanged", ea, eh };
    if (kind == "ComboBox" || kind == "ListBox" || kind == "CheckedListBox" || kind == "ListView" || kind == "TabControl") return { "SelectedIndexChanged", ea, eh };
    if (kind == "NumericUpDown" || kind == "DateTimePicker") return { "ValueChanged", ea, eh };
    if (kind == "TrackBar") return { "Scroll", ea, eh };
    if (kind == "GroupBox") return { "Enter", ea, eh };
    if (kind == "Panel") return { "Paint", "System.Windows.Forms.PaintEventArgs", "System.Windows.Forms.PaintEventHandler" };
    if (kind == "MonthCalendar") return { "DateChanged", "System.Windows.Forms.DateRangeEventArgs", "System.Windows.Forms.DateRangeEventHandler" };
    if (kind == "TreeView") return { "AfterSelect", "System.Windows.Forms.TreeViewEventArgs", "System.Windows.Forms.TreeViewEventHandler" };
    if (kind == "LinkLabel") return { "LinkClicked", "System.Windows.Forms.LinkLabelLinkClickedEventArgs", "System.Windows.Forms.LinkLabelLinkClickedEventHandler" };
    if (kind == "DataGridView") return { "CellContentClick", "System.Windows.Forms.DataGridViewCellEventArgs", "System.Windows.Forms.DataGridViewCellEventHandler" };
    return { "Click", ea, eh };
}
// These controls parse Text as a value and throw (or ignore it) for a caption.
bool has_caption(const std::string& kind) { return kind != "DateTimePicker" && kind != "NumericUpDown" && kind != "MonthCalendar"; }
// HTML attribute used for an element's default event.
const char* html_event(const std::string& kind)
{
    return kind == "TextBox" || kind == "RichTextBox" || kind == "ComboBox" || kind == "ListBox" ? "onchange" : "onclick";
}

constexpr std::size_t max_nodes = 200;

[[noreturn]] void invalid(const std::string& message) { throw KernelError(ErrorCode::invalid_argument, message); }

std::string lower(std::string s)
{
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

bool iequals(const std::string& a, const std::string& b) { return lower(a) == lower(b); }

bool identifier(const std::string& s)
{
    if (s.empty() || !(std::isalpha(static_cast<unsigned char>(s[0])) || s[0] == '_')) return false;
    for (unsigned char c : s) if (!(std::isalnum(c) || c == '_')) return false;
    return true;
}

// "#RGB", "#RRGGBB", a colour name, or empty. Names are resolved by the UI adapter's colour table.
bool colour(const std::string& s)
{
    if (s.empty()) return true;
    if (s[0] == '#') {
        if (s.size() != 4 && s.size() != 7) return false;
        for (std::size_t i = 1; i < s.size(); ++i) if (!std::isxdigit(static_cast<unsigned char>(s[i]))) return false;
        return true;
    }
    for (unsigned char c : s) if (!std::isalpha(c)) return false;
    return true;
}

void validate_node(const DesignNode& n)
{
    if (!identifier(n.name)) invalid("Name 必须是由英文字母、数字或下划线组成的标识符。");
    if (n.x < 0 || n.y < 0 || n.x > 10000 || n.y > 10000 || n.width < 12 || n.height < 12 || n.width > 4096 || n.height > 4096)
        invalid("位置范围为 0–10000，尺寸范围为 12–4096。");
    if (!colour(n.back_color) || !colour(n.fore_color)) invalid("颜色必须是 #RGB、#RRGGBB 或颜色名称。");
}

std::string node_id(std::size_t i) { return "n" + std::to_string(i); }

std::size_t node_index(const JsonValue& op, std::size_t count)
{
    const JsonValue* v = op.find("node");
    if (!v || v->type != JsonValue::Type::string || v->text.size() < 2 || v->text[0] != 'n') invalid("缺少节点 ID。");
    std::size_t i = 0;
    for (std::size_t k = 1; k < v->text.size(); ++k) {
        if (!std::isdigit(static_cast<unsigned char>(v->text[k])) || i > 100000) invalid("节点 ID 无效。");
        i = i * 10 + static_cast<std::size_t>(v->text[k] - '0');
    }
    if (i >= count) invalid("节点不存在，请刷新设计视图。");
    return i;
}

const JsonValue& properties_of(const JsonValue& op)
{
    const JsonValue* p = op.find("properties");
    if (!p || p->type != JsonValue::Type::object) invalid("缺少 properties 对象。");
    return *p;
}

long long int_prop(const JsonValue& v, const std::string& key)
{
    if (v.type != JsonValue::Type::number || !v.integer || v.int_value < -100000000 || v.int_value > 100000000) invalid(key + " 必须是整数。");
    return v.int_value;
}

std::string string_prop(const JsonValue& v, const std::string& key)
{
    if (v.type == JsonValue::Type::null) return std::string();
    if (v.type != JsonValue::Type::string) invalid(key + " 必须是字符串。");
    if (!valid_utf8(v.text)) invalid(key + " 不是有效的 UTF-8。");
    return v.text;
}

// Merges a "set" op's properties into a node; returns whether text was given.
bool merge_properties(DesignNode& n, const JsonValue& props)
{
    bool text = false;
    for (const auto& m : props.members) {
        const std::string& k = m.key;
        if (k == "name") n.name = string_prop(m.value, k);
        else if (k == "text") { n.text = string_prop(m.value, k); text = true; }
        else if (k == "x") n.x = int_prop(m.value, k);
        else if (k == "y") n.y = int_prop(m.value, k);
        else if (k == "width") n.width = int_prop(m.value, k);
        else if (k == "height") n.height = int_prop(m.value, k);
        else if (k == "back_color") n.back_color = string_prop(m.value, k);
        else if (k == "fore_color") n.fore_color = string_prop(m.value, k);
        else invalid("不支持的属性：" + k);
    }
    return text;
}

struct Patch { std::size_t begin, end; std::string text; };

std::string apply_patches(std::string source, std::vector<Patch> patches)
{
    std::stable_sort(patches.begin(), patches.end(), [](const Patch& a, const Patch& b) { return a.begin > b.begin; });
    for (const auto& p : patches) source.replace(p.begin, p.end - p.begin, p.text);
    return source;
}

// ---------------------------------------------------------------- HTML source adapter
// Edits source spans only; it never executes HTML and never re-serialises a DOM.

struct HtmlToken {
    std::size_t start = 0, open_end = 0, content_end = 0, end = 0;
    std::string tag, open, id;
};

bool is_space(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }
bool name_char(char c) { unsigned char u = static_cast<unsigned char>(c); return std::isalnum(u) || c == '_' || c == ':' || c == '-' || u >= 0x80; }

struct AttributeMatch { std::size_t begin, end; std::string name, value; };

// Equivalent of the former regex  ([\w:-]+)\s*=\s*("[^"]*"|'[^']*'|[^\s>]+)  applied with Matches().
std::vector<AttributeMatch> attributes(const std::string& open)
{
    std::vector<AttributeMatch> out;
    std::size_t p = 0;
    while (p < open.size()) {
        if (!name_char(open[p])) { ++p; continue; }
        std::size_t n = p;
        while (n < open.size() && name_char(open[n])) ++n;
        std::size_t q = n;
        while (q < open.size() && is_space(open[q])) ++q;
        if (q >= open.size() || open[q] != '=') { ++p; continue; }
        ++q;
        while (q < open.size() && is_space(open[q])) ++q;
        if (q >= open.size()) { ++p; continue; }
        std::size_t vb = q, ve = q, end = q;
        if ((open[q] == '"' || open[q] == '\'') && open.find(open[q], q + 1) != std::string::npos) {
            std::size_t close = open.find(open[q], q + 1);
            vb = q + 1; ve = close; end = close + 1;
        } else {
            while (end < open.size() && !is_space(open[end]) && open[end] != '>') ++end;
            if (end == q) { ++p; continue; }
            vb = q; ve = end;
        }
        out.push_back({ p, end, open.substr(p, n - p), open.substr(vb, ve - vb) });
        p = end;
    }
    return out;
}

std::string html_decode(const std::string& s)
{
    std::string out;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] != '&') { out.push_back(s[i]); continue; }
        std::size_t semi = s.find(';', i);
        if (semi == std::string::npos || semi - i > 10) { out.push_back('&'); continue; }
        std::string name = s.substr(i + 1, semi - i - 1);
        unsigned long cp = 0;
        bool ok = true;
        if (name == "amp") cp = '&'; else if (name == "lt") cp = '<'; else if (name == "gt") cp = '>';
        else if (name == "quot") cp = '"'; else if (name == "apos") cp = '\''; else if (name == "nbsp") cp = 0xA0;
        else if (name.size() > 1 && name[0] == '#') {
            bool hex = name[1] == 'x' || name[1] == 'X';
            std::string digits = name.substr(hex ? 2 : 1);
            char* endp = nullptr;
            cp = digits.empty() ? 0 : std::strtoul(digits.c_str(), &endp, hex ? 16 : 10);
            ok = !digits.empty() && endp && *endp == '\0' && cp > 0 && cp <= 0x10FFFF && !(cp >= 0xD800 && cp <= 0xDFFF);
        } else ok = false;
        if (!ok) { out.push_back('&'); continue; }
        if (cp < 0x80) out.push_back(static_cast<char>(cp));
        else if (cp < 0x800) { out.push_back(static_cast<char>(0xC0 | (cp >> 6))); out.push_back(static_cast<char>(0x80 | (cp & 0x3F))); }
        else if (cp < 0x10000) { out.push_back(static_cast<char>(0xE0 | (cp >> 12))); out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F))); out.push_back(static_cast<char>(0x80 | (cp & 0x3F))); }
        else { out.push_back(static_cast<char>(0xF0 | (cp >> 18))); out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F))); out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F))); out.push_back(static_cast<char>(0x80 | (cp & 0x3F))); }
        i = semi;
    }
    return out;
}

std::string html_encode(const std::string& s, bool attribute)
{
    std::string out;
    for (char c : s) {
        switch (c) {
        case '&': out += "&amp;"; break;
        case '<': out += "&lt;"; break;
        case '>': if (attribute) out.push_back(c); else out += "&gt;"; break;
        case '"': out += "&quot;"; break;
        case '\'': out += "&#39;"; break;
        default: out.push_back(c);
        }
    }
    return out;
}

std::string get_attribute(const std::string& open, const std::string& name)
{
    for (const auto& m : attributes(open)) if (iequals(m.name, name)) return html_decode(m.value);
    return std::string();
}

std::string set_attribute(std::string open, const std::string& name, const std::string& value)
{
    std::string pair = name + "=\"" + html_encode(value, true) + "\"";
    for (const auto& m : attributes(open)) if (iequals(m.name, name)) return open.replace(m.begin, m.end - m.begin, pair);
    std::size_t end = open.rfind('>');
    if (end == std::string::npos) invalid("标签不完整。");
    if (end > 0 && open[end - 1] == '/') --end;
    return open.insert(end, " " + pair);
}

std::string trim(const std::string& s)
{
    std::size_t b = 0, e = s.size();
    while (b < e && is_space(s[b])) ++b;
    while (e > b && is_space(s[e - 1])) --e;
    return s.substr(b, e - b);
}

std::vector<std::string> split(const std::string& s, char sep)
{
    std::vector<std::string> out;
    std::size_t start = 0;
    while (true) {
        std::size_t p = s.find(sep, start);
        out.push_back(s.substr(start, p == std::string::npos ? std::string::npos : p - start));
        if (p == std::string::npos) return out;
        start = p + 1;
    }
}

std::string position_style(const std::string& open, const DesignNode& n)
{
    static const char* managed[] = { "position", "left", "top", "width", "height", "background-color", "color" };
    std::string kept;
    for (const auto& declaration : split(get_attribute(open, "style"), ';')) {
        if (trim(declaration).empty()) continue;
        std::size_t colon = declaration.find(':');
        std::string key = colon == std::string::npos ? std::string() : lower(trim(declaration.substr(0, colon)));
        bool drop = false;
        for (const char* m : managed) drop |= colon != std::string::npos && key == m;
        if (drop) continue;
        if (!kept.empty()) kept.push_back(';');
        kept += declaration;
    }
    std::string style = kept + ";position:absolute;left:" + std::to_string(n.x) + "px;top:" + std::to_string(n.y) + "px;width:" + std::to_string(n.width)
        + "px;height:" + std::to_string(n.height) + "px;background-color:" + n.back_color + ";color:" + n.fore_color + ";";
    std::size_t first = style.find_first_not_of(';');
    return set_attribute(open, "style", first == std::string::npos ? std::string() : style.substr(first));
}

bool is_void(const std::string& tag)
{
    static const std::set<std::string> v = { "input", "img", "hr", "br", "meta", "link", "area", "base", "embed", "param", "source", "track", "wbr" };
    return v.count(tag) != 0;
}

// Case-insensitive search for </tag\s*> starting at `from`; returns {position, length}.
std::pair<std::size_t, std::size_t> find_close(const std::string& s, std::size_t from, const std::string& tag)
{
    for (std::size_t p = s.find('<', from); p != std::string::npos; p = s.find('<', p + 1)) {
        std::size_t q = p + 1;
        if (q >= s.size() || s[q] != '/') continue;
        ++q;
        while (q < s.size() && is_space(s[q])) ++q;
        if (q + tag.size() > s.size() || lower(s.substr(q, tag.size())) != tag) continue;
        q += tag.size();
        while (q < s.size() && is_space(s[q])) ++q;
        if (q < s.size() && s[q] == '>') return { p, q + 1 - p };
    }
    return { std::string::npos, 0 };
}

std::string html_kind(const HtmlToken& t)
{
    const std::string& g = t.tag;
    if (g == "button") return "Button";
    if (g == "input") {
        std::string type = lower(get_attribute(t.open, "type"));
        if (type == "checkbox") return "CheckBox";
        if (type == "radio") return "RadioButton";
        if (type == "button" || type == "submit" || type == "reset") return "Button";
        return "TextBox";
    }
    if (g == "textarea") return "RichTextBox";
    if (g == "select") return get_attribute(t.open, "size").empty() ? "ComboBox" : "ListBox";
    if (g == "span" || g == "label" || g == "p" || g == "h1" || g == "h2" || g == "h3" || g == "a") return "Label";
    if (g == "div" || g == "hr") return "Panel";
    if (g == "img") return "PictureBox";
    if (g == "table") return "DataGridView";
    return std::string();
}

bool parse_px(const std::string& value, long long& out)
{
    std::string v;
    for (std::size_t i = 0; i < value.size(); ++i) {
        if (value.compare(i, 2, "px") == 0) { ++i; continue; }
        v.push_back(value[i]);
    }
    v = trim(v);
    if (v.empty()) return false;
    std::size_t i = (v[0] == '-' || v[0] == '+') ? 1 : 0;
    if (i == v.size() || v.size() > 10) return false;
    for (std::size_t k = i; k < v.size(); ++k) if (!std::isdigit(static_cast<unsigned char>(v[k]))) return false;
    out = std::strtoll(v.c_str(), nullptr, 10);
    return true;
}

std::string strip_tags(const std::string& s)
{
    std::string out;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '<') { std::size_t e = s.find('>', i); if (e != std::string::npos) { i = e; continue; } }
        out.push_back(s[i]);
    }
    return out;
}

struct HtmlScan {
    std::vector<HtmlToken> tokens;
    std::vector<std::size_t> item_tokens; // token index per design node
    std::vector<DesignNode> nodes;
    std::size_t insert_at = 0;
    struct Script { std::string open; std::size_t begin, end; };
    std::vector<Script> scripts;

    explicit HtmlScan(const std::string& source)
    {
        insert_at = source.size();
        std::vector<std::size_t> stack;
        for (std::size_t i = 0; i < source.size();) {
            std::size_t start = source.find('<', i);
            if (start == std::string::npos) break;
            if (source.compare(start, 4, "<!--") == 0) {
                std::size_t e = source.find("-->", start + 4);
                i = e == std::string::npos ? source.size() : e + 3;
                continue;
            }
            std::size_t end = start + 1;
            char quote = 0;
            for (; end < source.size(); ++end) {
                char c = source[end];
                if (quote) { if (c == quote) quote = 0; }
                else if (c == '\'' || c == '"') quote = c;
                else if (c == '>') break;
            }
            if (end == source.size()) break;
            std::string open = source.substr(start, end - start + 1);
            i = end + 1;
            std::size_t p = 1;
            while (p < open.size() && is_space(open[p])) ++p;
            bool closing = p < open.size() && open[p] == '/';
            if (closing) { ++p; while (p < open.size() && is_space(open[p])) ++p; }
            if (p >= open.size() || !std::isalpha(static_cast<unsigned char>(open[p]))) continue;
            std::size_t q = p;
            while (q < open.size() && name_char(open[q]) && static_cast<unsigned char>(open[q]) < 0x80) ++q;
            std::string tag = lower(open.substr(p, q - p));
            if (closing) {
                if (tag == "body") insert_at = start;
                for (std::size_t k = stack.size(); k-- > 0;) {
                    if (tokens[stack[k]].tag != tag) continue;
                    tokens[stack[k]].content_end = start;
                    tokens[stack[k]].end = end + 1;
                    stack.resize(k);
                    break;
                }
                continue;
            }
            if (tag == "script" || tag == "style") {
                auto close = find_close(source, i, tag);
                if (tag == "script" && close.first != std::string::npos) scripts.push_back({ open, i, close.first });
                i = close.first == std::string::npos ? source.size() : close.first + close.second;
                continue;
            }
            HtmlToken t;
            t.start = start; t.open_end = t.content_end = t.end = end + 1;
            t.tag = tag; t.open = open; t.id = get_attribute(open, "id");
            tokens.push_back(t);
            std::size_t index = tokens.size() - 1;
            bool self_closing = open.size() >= 2 && open.compare(open.size() - 2, 2, "/>") == 0;
            if (!is_void(tag) && !self_closing) stack.push_back(index);
            if (tag == "textarea") {
                auto close = find_close(source, i, "textarea");
                if (close.first != std::string::npos) {
                    tokens[index].content_end = close.first;
                    tokens[index].end = close.first + close.second;
                    stack.erase(std::remove(stack.begin(), stack.end(), index), stack.end());
                    i = tokens[index].end;
                }
            }
        }
        long long flow = 24;
        for (std::size_t k = 0; k < tokens.size(); ++k) {
            const HtmlToken& t = tokens[k];
            std::string kind = html_kind(t);
            if (kind.empty()) continue;
            DesignNode n;
            n.kind = kind; n.name = t.id; n.x = 24; n.y = flow;
            n.width = kind == "Panel" ? 300 : 140;
            n.height = kind == "Panel" || kind == "DataGridView" ? 100 : 30;
            if (n.name.empty()) n.name = "element" + std::to_string(nodes.size() + 1);
            std::string content = t.content_end > t.open_end ? source.substr(t.open_end, t.content_end - t.open_end) : std::string();
            n.text = html_decode(strip_tags(content));
            if (t.tag == "input") n.text = get_attribute(t.open, "value");
            if (trim(n.text).empty()) n.text = kind == "Panel" || kind == "TextBox" ? std::string() : kind;
            n.text_editable = t.tag == "input" || (!is_void(t.tag) && content.find('<') == std::string::npos);
            n.password = t.tag == "input" && lower(get_attribute(t.open, "type")) == "password";
            for (const auto& declaration : split(get_attribute(t.open, "style"), ';')) {
                std::size_t colon = declaration.find(':');
                if (colon == std::string::npos) continue;
                std::string key = lower(trim(declaration.substr(0, colon))), value = trim(declaration.substr(colon + 1));
                long long number;
                if (parse_px(value, number)) {
                    if (key == "left") n.x = std::max(0LL, number);
                    if (key == "top") n.y = std::max(0LL, number);
                    if (key == "width") n.width = std::max(12LL, std::min(4096LL, number));
                    if (key == "height") n.height = std::max(12LL, std::min(4096LL, number));
                }
                if (key == "background-color") n.back_color = value;
                if (key == "color") n.fore_color = value;
            }
            n.event = html_event(kind);
            n.handler = get_attribute(t.open, n.event);
            n.id = node_id(nodes.size());
            nodes.push_back(n);
            item_tokens.push_back(k);
            flow = std::min(10000LL, n.y + n.height + 14);
        }
    }
};

DesignResult html_apply(const std::string& source, const JsonValue& op, const std::string& name, const ToolboxService& toolbox)
{
    HtmlScan scan(source);
    if (name == "add") {
        const JsonValue* id = op.find("toolbox_id");
        if (!id || id->type != JsonValue::Type::string) invalid("缺少 toolbox_id。");
        const ToolboxEntry& entry = toolbox.find(id->text);
        if (entry.family != "html") throw KernelError(ErrorCode::invalid_state, "请在对应的 HTML 页面或 Windows 窗体布局中使用此控件。");
        if (scan.nodes.size() >= max_nodes) throw KernelError(ErrorCode::invalid_state, "当前设计器最多显示 200 个元素。");
        DesignNode n;
        const JsonValue* x = op.find("x");
        const JsonValue* y = op.find("y");
        n.x = x ? int_prop(*x, "x") : 0; n.y = y ? int_prop(*y, "y") : 0;
        n.width = 140; n.height = entry.control_type == "Panel" ? 100 : 30;
        std::string snippet;
        for (std::size_t i = 0; i < entry.markup.size(); ++i) {
            if (entry.markup.compare(i, 2, "\\n") == 0) { snippet.push_back('\n'); ++i; } else snippet.push_back(entry.markup[i]);
        }
        HtmlScan fragment(snippet);
        if (fragment.tokens.empty()) invalid("工具箱条目没有可插入的标签。");
        const HtmlToken& first = fragment.tokens[0];
        std::string control;
        for (int i = 1;; ++i) {
            control = "control" + std::to_string(i);
            bool used = false;
            for (const auto& t : scan.tokens) used |= t.id == control;
            if (!used) break;
        }
        n.name = control;
        validate_node(n);
        std::string open = position_style(set_attribute(first.open, "id", control), n);
        snippet.replace(first.start, first.open_end - first.start, open);
        std::size_t at = scan.insert_at;
        std::string text = source;
        text.insert(at, "\n" + snippet + "\n");
        HtmlScan after(text);
        DesignResult r{ text, std::nullopt, std::nullopt };
        for (std::size_t k = 0; k < after.nodes.size(); ++k)
            if (after.tokens[after.item_tokens[k]].start == at + 1) r.node = after.nodes[k].id;
        return r;
    }
    if (name == "set") {
        std::size_t index = node_index(op, scan.nodes.size());
        DesignNode n = scan.nodes[index];
        std::string original_text = n.text;
        bool text_given = merge_properties(n, properties_of(op));
        validate_node(n);
        const HtmlToken& t = scan.tokens[scan.item_tokens[index]];
        for (const auto& other : scan.tokens)
            if (&other != &t && other.id == n.name) invalid("Name 已被其他元素使用。");
        std::string open = position_style(set_attribute(t.open, "id", n.name), n);
        std::string tail = source.substr(t.open_end, t.end - t.open_end);
        if (text_given && n.text != original_text) {
            if (t.tag == "input") open = set_attribute(open, "value", n.text);
            else {
                std::string inner = source.substr(t.open_end, t.content_end - t.open_end);
                if (inner.find('<') != std::string::npos || is_void(t.tag)) invalid("此元素包含嵌套内容或不支持文本，请在 HTML 视图修改。");
                tail = html_encode(n.text, false) + source.substr(t.content_end, t.end - t.content_end);
            }
        }
        std::string text = source;
        text.replace(t.start, t.end - t.start, open + tail);
        return { text, node_id(index), std::nullopt };
    }
    if (name == "set_event") {
        std::size_t index = node_index(op, scan.nodes.size());
        const DesignNode& n = scan.nodes[index];
        const HtmlToken& t = scan.tokens[scan.item_tokens[index]];
        std::string handler;
        if (!n.handler.empty()) {
            // Already wired: a plain call leads to its function, anything else to the element itself.
            std::string call = trim(n.handler);
            if (call.rfind("return ", 0) == 0) call = trim(call.substr(7));
            std::size_t paren = call.find('(');
            handler = paren == std::string::npos ? std::string() : trim(call.substr(0, paren));
            if (!identifier(handler)) return { source, n.id, t.start };
        } else {
            for (unsigned char c : n.name) handler.push_back(std::isalnum(c) ? static_cast<char>(c) : '_');
            if (handler.empty() || std::isdigit(static_cast<unsigned char>(handler[0]))) handler = "_" + handler;
            handler += "_" + n.event;
        }
        // Offset of the first line inside "function <handler>(...) {", or npos.
        auto body = [&](const std::string& text) -> std::size_t {
            HtmlScan again(text);
            for (const auto& s : again.scripts) {
                for (std::size_t p = text.find("function", s.begin); p != std::string::npos && p < s.end; p = text.find("function", p + 8)) {
                    std::size_t q = p + 8;
                    if (q >= s.end || !is_space(text[q])) continue;
                    while (q < s.end && is_space(text[q])) ++q;
                    if (text.compare(q, handler.size(), handler) != 0) continue;
                    q += handler.size();
                    while (q < s.end && is_space(text[q])) ++q;
                    if (q >= s.end || text[q] != '(') continue;
                    std::size_t brace = text.find('{', q);
                    if (brace == std::string::npos || brace >= s.end) return p;
                    std::size_t line = text.find('\n', brace);
                    if (line == std::string::npos || line >= s.end) return brace + 1;
                    std::size_t at = line + 1;
                    while (at < s.end && (text[at] == ' ' || text[at] == '\t')) ++at;
                    return at;
                }
            }
            return std::string::npos;
        };
        std::vector<Patch> patches;
        if (n.handler.empty()) patches.push_back({ t.start, t.open_end, set_attribute(t.open, n.event, handler + "()") });
        if (body(source) == std::string::npos) {
            std::string function = "\nfunction " + handler + "() {\n    \n}\n";
            const HtmlScan::Script* target = nullptr;
            for (const auto& s : scan.scripts) {
                std::string type = lower(get_attribute(s.open, "type") + get_attribute(s.open, "language"));
                if (get_attribute(s.open, "src").empty() && (type.empty() || type.find("javascript") != std::string::npos || type.find("jscript") != std::string::npos)) target = &s;
            }
            if (target) {
                if (target->end > target->begin && source[target->end - 1] == '\n') function.erase(0, 1);
                patches.push_back({ target->end, target->end, function });
            } else patches.push_back({ scan.insert_at, scan.insert_at, "<script>" + function + "</script>\n" });
        }
        std::string text = apply_patches(source, patches);
        std::size_t at = body(text);
        return { text, n.id, at == std::string::npos ? t.start : at };
    }
    if (name == "delete") {
        std::size_t index = node_index(op, scan.nodes.size());
        const HtmlToken& t = scan.tokens[scan.item_tokens[index]];
        std::string text = source;
        text.erase(t.start, t.end - t.start);
        return { text, std::nullopt, std::nullopt };
    }
    if (name == "set_form") throw KernelError(ErrorCode::invalid_state, "HTML 页面没有窗体属性。");
    invalid("未知的设计操作：" + name);
}

// ---------------------------------------------------------------- .omform adapter
// Values are patched in place, so formatting and members this version does not know survive edits.

struct FormDoc {
    JsonValue root;
    long long format = 1, width = 720, height = 480;
    std::string title = "Form1", handler;
    std::vector<std::pair<std::string, std::string>> handlers;
    const JsonValue* controls = nullptr;
    std::vector<DesignNode> nodes;
};

[[noreturn]] void bad_layout() { invalid("无效的窗体布局或版本。"); }

long long member_int(const JsonValue& obj, const char* key, long long fallback)
{
    const JsonValue* v = obj.find(key);
    if (!v) return fallback;
    if (v->type != JsonValue::Type::number || !v->integer || v->int_value < INT32_MIN || v->int_value > INT32_MAX) bad_layout();
    return v->int_value;
}

std::string member_string(const JsonValue& obj, const char* key, const std::string& fallback)
{
    const JsonValue* v = obj.find(key);
    if (!v) return fallback;
    if (v->type == JsonValue::Type::null) return std::string();
    if (v->type != JsonValue::Type::string) bad_layout();
    return v->text;
}

// "Events": {"Click": "button1_Click", "MouseMove": "button1_MouseMove"}: every bound event of a control or the form.
std::vector<std::pair<std::string, std::string>> event_handlers(const JsonValue& obj)
{
    std::vector<std::pair<std::string, std::string>> out;
    const JsonValue* events = obj.find("Events");
    if (!events || events->type == JsonValue::Type::null) return out;
    if (events->type != JsonValue::Type::object) bad_layout();
    for (const auto& m : events->members) {
        if (m.value.type == JsonValue::Type::null) continue;
        if (m.value.type != JsonValue::Type::string || !identifier(m.key) || !identifier(m.value.text)) invalid("事件绑定无效：" + m.key);
        out.emplace_back(m.key, m.value.text);
    }
    return out;
}
std::string handler_for(const std::vector<std::pair<std::string, std::string>>& handlers, const std::string& event)
{
    for (const auto& h : handlers) if (h.first == event) return h.second;
    return {};
}
// Class names come from file names, so letters outside ASCII (for example Chinese) are allowed, as C# and VB allow them.
bool class_identifier(const std::string& s)
{
    if (s.empty() || std::isdigit(static_cast<unsigned char>(s[0]))) return false;
    for (unsigned char c : s) if (!(std::isalnum(c) || c == '_' || c >= 0x80)) return false;
    return valid_utf8(s);
}

void parse_form(const std::string& text, FormDoc& f)
{
    try { f.root = parse_json(text); }
    catch (const JsonError& e) { invalid(std::string("布局 JSON 无效：") + e.what()); }
    if (f.root.type == JsonValue::Type::null) invalid("布局不能为空。");
    if (f.root.type != JsonValue::Type::object) bad_layout();
    f.format = member_int(f.root, "FormatVersion", 1);
    f.title = member_string(f.root, "Title", "Form1");
    f.width = member_int(f.root, "Width", 720);
    f.height = member_int(f.root, "Height", 480);
    f.handlers = event_handlers(f.root);
    f.handler = handler_for(f.handlers, "Load");
    f.controls = f.root.find("Controls");
    if (f.format != 1 || f.width < 120 || f.height < 100 || f.width > 4096 || f.height > 4096) bad_layout();
    if (f.controls && f.controls->type != JsonValue::Type::array) bad_layout();
    if (f.controls && f.controls->items.size() > max_nodes) bad_layout();
    std::set<std::string> names;
    if (!f.controls) return;
    for (const auto& c : f.controls->items) {
        if (c.type != JsonValue::Type::object) bad_layout();
        DesignNode n;
        n.kind = member_string(c, "Kind", "");
        n.name = member_string(c, "Name", "");
        n.text = member_string(c, "Text", "");
        n.x = member_int(c, "X", 0); n.y = member_int(c, "Y", 0);
        n.width = member_int(c, "Width", 120); n.height = member_int(c, "Height", 30);
        n.back_color = member_string(c, "BackColor", "#FFFFFF");
        n.fore_color = member_string(c, "ForeColor", "#202835");
        const JsonValue* enabled = c.find("Enabled");
        if (enabled && enabled->type != JsonValue::Type::boolean) bad_layout();
        n.enabled = enabled ? enabled->boolean : true;
        validate_node(n);
        if (!allowed_controls().count(n.kind) || !names.insert(lower(n.name)).second) invalid("控件类型不受支持或 Name 重复。");
        n.event = default_event(n.kind).name;
        n.event_args = default_event(n.kind).args;
        n.handlers = event_handlers(c);
        n.handler = handler_for(n.handlers, n.event);
        n.id = node_id(f.nodes.size());
        f.nodes.push_back(n);
    }
}

std::string line_indent(const std::string& s, std::size_t pos)
{
    std::size_t nl = s.rfind('\n', pos == 0 ? 0 : pos - 1);
    std::size_t b = nl == std::string::npos ? 0 : nl + 1, e = b;
    while (e < s.size() && (s[e] == ' ' || s[e] == '\t')) ++e;
    return s.substr(b, e - b);
}

bool spans_lines(const std::string& s, std::size_t b, std::size_t e) { return s.find('\n', b) < e; }

std::string literal(const std::string& value) { return json_quote(value, true); }
std::string literal(long long value) { return std::to_string(value); }
struct RawJson { std::string text; };
std::string literal(const RawJson& value) { return value.text; }

template<class T> void set_member(const std::string& src, const JsonValue& obj, const char* key, const T& value, std::vector<Patch>& patches)
{
    if (const JsonMember* m = obj.find_member(key)) { patches.push_back({ m->value.begin, m->value.end, literal(value) }); return; }
    if (obj.members.empty()) { patches.push_back({ obj.close, obj.close, json_quote(key) + ":" + literal(value) }); return; }
    const JsonMember& last = obj.members.back();
    std::string text = spans_lines(src, obj.begin, obj.close)
        ? ",\n" + line_indent(src, last.key_begin) + json_quote(key) + ": " + literal(value)
        : "," + json_quote(key) + ":" + literal(value);
    patches.push_back({ last.value.end, last.value.end, text });
}

std::string control_object(const DesignNode& n, const std::string& indent, const std::string& member_indent, bool pretty)
{
    std::vector<std::pair<std::string, std::string>> m = {
        { "Kind", literal(n.kind) }, { "Name", literal(n.name) }, { "Text", literal(n.text) },
        { "X", literal(n.x) }, { "Y", literal(n.y) }, { "Width", literal(n.width) }, { "Height", literal(n.height) },
        { "BackColor", literal(n.back_color) }, { "ForeColor", literal(n.fore_color) },
        { "Enabled", "true" }, { "Family", literal(std::string("Windows Forms")) } };
    std::string out = pretty ? "{\n" : "{";
    for (std::size_t i = 0; i < m.size(); ++i) {
        if (i) out += pretty ? ",\n" : ",";
        out += pretty ? member_indent + json_quote(m[i].first) + ": " + m[i].second : json_quote(m[i].first) + ":" + m[i].second;
    }
    return out + (pretty ? "\n" + indent + "}" : "}");
}

DesignResult form_apply(const std::string& source, const JsonValue& op, const std::string& name, const ToolboxService& toolbox)
{
    FormDoc f;
    parse_form(source, f);
    std::vector<Patch> patches;
    std::optional<std::string> node;
    if (name == "add") {
        const JsonValue* id = op.find("toolbox_id");
        if (!id || id->type != JsonValue::Type::string) invalid("缺少 toolbox_id。");
        const ToolboxEntry& entry = toolbox.find(id->text);
        if (entry.family != "winforms") throw KernelError(ErrorCode::invalid_state, "请在对应的 HTML 页面或 Windows 窗体布局中使用此控件。");
        if (f.nodes.size() >= max_nodes) throw KernelError(ErrorCode::invalid_state, "当前设计器最多支持 200 个控件。");
        DesignNode n;
        n.kind = entry.control_type;
        for (int i = 1;; ++i) {
            n.name = lower(entry.control_type) + std::to_string(i);
            bool used = false;
            for (const auto& c : f.nodes) used |= iequals(c.name, n.name);
            if (!used) break;
        }
        n.text = entry.name;
        const JsonValue* x = op.find("x");
        const JsonValue* y = op.find("y");
        n.x = x ? int_prop(*x, "x") : 0; n.y = y ? int_prop(*y, "y") : 0;
        bool box = n.kind == "Panel" || n.kind == "GroupBox";
        n.width = box ? 240 : 140;
        n.height = box || n.kind == "DataGridView" ? 120 : 32;
        validate_node(n);
        if (!f.controls) {
            set_member(source, f.root, "Controls", std::string(), patches);
            patches.back().text.replace(patches.back().text.size() - 2, 2, "[" + control_object(n, "", "", false) + "]");
        } else if (f.controls->items.empty() || !spans_lines(source, f.controls->begin, f.controls->close)) {
            std::size_t at = f.controls->items.empty() ? f.controls->close : f.controls->items.back().end;
            patches.push_back({ at, at, (f.controls->items.empty() ? "" : ",") + control_object(n, "", "", false) });
        } else {
            const JsonValue& first = f.controls->items.front();
            std::string indent = line_indent(source, first.begin);
            std::string member_indent = first.members.empty() ? indent + "  " : line_indent(source, first.members.front().key_begin);
            std::size_t at = f.controls->items.back().end;
            patches.push_back({ at, at, ",\n" + indent + control_object(n, indent, member_indent, true) });
        }
        node = node_id(f.nodes.size());
    } else if (name == "set") {
        std::size_t index = node_index(op, f.nodes.size());
        DesignNode n = f.nodes[index];
        const JsonValue& props = properties_of(op);
        merge_properties(n, props);
        validate_node(n);
        const JsonValue& obj = f.controls->items[index];
        for (const auto& m : props.members) {
            if (m.key == "name") set_member(source, obj, "Name", n.name, patches);
            else if (m.key == "text") set_member(source, obj, "Text", n.text, patches);
            else if (m.key == "x") set_member(source, obj, "X", n.x, patches);
            else if (m.key == "y") set_member(source, obj, "Y", n.y, patches);
            else if (m.key == "width") set_member(source, obj, "Width", n.width, patches);
            else if (m.key == "height") set_member(source, obj, "Height", n.height, patches);
            else if (m.key == "back_color") set_member(source, obj, "BackColor", n.back_color, patches);
            else if (m.key == "fore_color") set_member(source, obj, "ForeColor", n.fore_color, patches);
        }
        node = node_id(index);
    } else if (name == "delete") {
        std::size_t index = node_index(op, f.nodes.size());
        const auto& items = f.controls->items;
        if (items.size() == 1) patches.push_back({ f.controls->begin + 1, f.controls->close, std::string() });
        else if (index + 1 < items.size()) patches.push_back({ items[index].begin, items[index + 1].begin, std::string() });
        else patches.push_back({ items[index - 1].end, items[index].end, std::string() });
    } else if (name == "set_event") {
        // node "form" binds the form's Load event.
        const JsonValue* target = op.find("node");
        bool form = target && target->type == JsonValue::Type::string && target->text == "form";
        std::size_t index = form ? 0 : node_index(op, f.nodes.size());
        const JsonValue& obj = form ? f.root : f.controls->items[index];
        const std::string kind = form ? std::string("Form") : f.nodes[index].kind;
        const JsonValue* h = op.find("handler");
        std::string handler = h ? string_prop(*h, "handler") : std::string();
        if (!identifier(handler)) invalid("事件处理函数名无效。");
        const JsonValue* ev = op.find("event");
        std::string event_name = ev ? string_prop(*ev, "event") : std::string(default_event(kind).name);
        if (!identifier(event_name)) invalid("事件名无效。");
        const JsonValue* events = obj.find("Events");
        if (events && events->type == JsonValue::Type::object) set_member(source, *events, event_name.c_str(), handler, patches);
        else set_member(source, obj, "Events", RawJson{ "{" + json_quote(event_name) + ":" + literal(handler) + "}" }, patches);
        if (!form) node = node_id(index);
    } else if (name == "set_form") {
        const JsonValue& props = properties_of(op);
        for (const auto& m : props.members) {
            if (m.key == "title") set_member(source, f.root, "Title", string_prop(m.value, "title"), patches);
            else if (m.key == "width") set_member(source, f.root, "Width", int_prop(m.value, "width"), patches);
            else if (m.key == "height") set_member(source, f.root, "Height", int_prop(m.value, "height"), patches);
            else invalid("不支持的窗体属性：" + m.key);
        }
    } else invalid("未知的设计操作：" + name);

    std::string text = apply_patches(source, patches);
    FormDoc check;
    parse_form(text, check); // the whole result must still be a valid layout, or nothing is committed
    return { text, node, std::nullopt };
}

std::string extension(const Document& d)
{
    std::string name = d.path.empty() ? d.name : narrow(d.path);
    std::size_t slash = name.find_last_of("\\/"), dot = name.rfind('.');
    if (dot == std::string::npos || (slash != std::string::npos && dot < slash)) return std::string();
    return lower(name.substr(dot));
}

std::string cs_string(const std::string& v)
{
    std::string out = "\"";
    for (char c : v) {
        switch (c) {
        case '\\': out += "\\\\"; break; case '"': out += "\\\""; break; case '\r': out += "\\r"; break;
        case '\n': out += "\\n"; break; case '\t': out += "\\t"; break; case '\0': out += "\\0"; break;
        default: out.push_back(c);
        }
    }
    return out + "\"";
}

std::string vb_string(const std::string& v)
{
    std::string out = "\"";
    for (char c : v) {
        switch (c) {
        case '"': out += "\"\""; break; case '\r': out += "\" & ChrW(13) & \""; break;
        case '\n': out += "\" & ChrW(10) & \""; break; case '\0': out += "\" & ChrW(0) & \""; break;
        default: out.push_back(c);
        }
    }
    return out + "\"";
}

} // namespace

const char* design_kind_name(DesignKind k) { return k == DesignKind::html ? "html" : k == DesignKind::form ? "form" : "none"; }

DesignKind design_kind(const Document& d)
{
    std::string ext = extension(d);
    if (ext == ".html" || ext == ".htm") return DesignKind::html;
    if (ext == ".omform") return DesignKind::form;
    return DesignKind::none;
}

DesignSnapshot design_snapshot(const Document& d)
{
    DesignSnapshot s;
    s.kind = design_kind(d);
    if (s.kind == DesignKind::html) {
        HtmlScan scan(d.text);
        s.canvas_width = 900; s.canvas_height = 650;
        s.truncated = scan.nodes.size() > max_nodes;
        s.nodes.assign(scan.nodes.begin(), scan.nodes.begin() + static_cast<std::ptrdiff_t>(std::min(max_nodes, scan.nodes.size())));
    } else if (s.kind == DesignKind::form) {
        FormDoc f;
        try {
            parse_form(d.text, f);
            s.handler = f.handler;
            s.handlers = f.handlers;
            s.title = f.title; s.width = s.canvas_width = f.width; s.height = s.canvas_height = f.height;
            s.nodes = f.nodes;
        } catch (const KernelError& e) { s.error = e.what(); }
    } else s.error = "该文档类型没有设计视图。";
    return s;
}

DesignResult design_apply(const Document& d, const JsonValue& op, const ToolboxService& toolbox)
{
    if (op.type != JsonValue::Type::object) invalid("设计操作必须是 JSON 对象。");
    const JsonValue* name = op.find("op");
    if (!name || name->type != JsonValue::Type::string) invalid("缺少 op。");
    DesignKind kind = design_kind(d);
    if (kind == DesignKind::html) return html_apply(d.text, op, name->text, toolbox);
    if (kind == DesignKind::form) return form_apply(d.text, op, name->text, toolbox);
    throw KernelError(ErrorCode::invalid_state, "该文档类型没有设计视图。");
}

std::string design_export(const Document& d, const std::string& language)
{
    if (design_kind(d) != DesignKind::form) throw KernelError(ErrorCode::invalid_state, "HTML 页面直接保存为 .html；窗体源码导出适用于 .omform 布局。");
    bool vb = language == "vb";
    if (!vb && language != "csharp") invalid("导出语言必须是 csharp 或 vb。");
    FormDoc f;
    parse_form(d.text, f);
    std::string code;
    auto line = [&](const std::string& s) { code += s + "\r\n"; };
    if (vb) {
        line("' Generated by OpenMSE\r\nImports System\r\nImports System.Drawing\r\nImports System.Windows.Forms\r\n\r\nPublic Class DesignedForm\r\n    Inherits Form\r\n    Public Sub New()");
        line("        Me.Text = " + vb_string(f.title));
        line("        Me.ClientSize = New Size(" + std::to_string(f.width) + ", " + std::to_string(f.height) + ")");
    } else {
        line("// Generated by OpenMSE\r\nusing System;\r\nusing System.Drawing;\r\nusing System.Windows.Forms;\r\n\r\npublic class DesignedForm : Form\r\n{\r\n    public DesignedForm()\r\n    {");
        line("        this.Text = " + cs_string(f.title) + ";");
        line("        this.ClientSize = new Size(" + std::to_string(f.width) + ", " + std::to_string(f.height) + ");");
    }
    for (const auto& n : f.nodes) {
        std::string v = "control_" + n.name;
        std::string bounds = std::to_string(n.x) + ", " + std::to_string(n.y) + ", " + std::to_string(n.width) + ", " + std::to_string(n.height);
        if (vb) {
            line("        Dim " + v + " As New " + n.kind + "()");
            line("        " + v + ".Name = " + vb_string(n.name));
            if (has_caption(n.kind)) line("        " + v + ".Text = " + vb_string(n.text));
            line("        " + v + ".SetBounds(" + bounds + ")");
            line("        " + v + ".Enabled = " + (n.enabled ? "True" : "False"));
            line("        " + v + ".BackColor = ColorTranslator.FromHtml(" + vb_string(n.back_color) + ")");
            line("        " + v + ".ForeColor = ColorTranslator.FromHtml(" + vb_string(n.fore_color) + ")");
            line("        Me.Controls.Add(" + v + ")");
        } else {
            line("        " + n.kind + " " + v + " = new " + n.kind + "();");
            line("        " + v + ".Name = " + cs_string(n.name) + ";");
            if (has_caption(n.kind)) line("        " + v + ".Text = " + cs_string(n.text) + ";");
            line("        " + v + ".SetBounds(" + bounds + ");");
            line("        " + v + ".Enabled = " + (n.enabled ? "true" : "false") + ";");
            line("        " + v + ".BackColor = ColorTranslator.FromHtml(" + cs_string(n.back_color) + ");");
            line("        " + v + ".ForeColor = ColorTranslator.FromHtml(" + cs_string(n.fore_color) + ");");
            line("        this.Controls.Add(" + v + ");");
        }
    }
    if (vb) line("    End Sub\r\n    <STAThread()> Public Shared Sub Main()\r\n        Application.EnableVisualStyles()\r\n        Application.Run(New DesignedForm())\r\n    End Sub\r\nEnd Class");
    else line("    }\r\n    [STAThread]\r\n    public static void Main() { Application.EnableVisualStyles(); Application.Run(new DesignedForm()); }\r\n}");
    return code;
}

std::string design_generate(const Document& d, const std::string& language, const std::string& class_name)
{
    if (design_kind(d) != DesignKind::form) throw KernelError(ErrorCode::invalid_state, "只有 .omform 布局可以生成窗体代码。");
    bool vb = language == "vb";
    if (!vb && language != "csharp") invalid("语言必须是 csharp 或 vb。");
    if (!class_identifier(class_name)) invalid("窗体类名无效：" + class_name);
    FormDoc f;
    parse_form(d.text, f);
    // Escaped identifiers keep names such as "this" or "Select" valid.
    auto id = [&](const std::string& n) { return vb ? "[" + n + "]" : "@" + n; };
    auto text = [&](const std::string& s) { return vb ? vb_string(s) : cs_string(s); };
    const std::string self = vb ? "Me." : "this.", base = vb ? "MyBase." : "base.", end = vb ? "\r\n" : ";\r\n";
    const std::string forms = "System.Windows.Forms.", drawing = "System.Drawing.", make = vb ? "New " : "new ";
    std::string code;
    // Method-group conversion (C# 2.0) and AddressOf need no delegate type, so any event can be wired.
    auto wire = [&](const std::string& owner, const std::vector<std::pair<std::string, std::string>>& handlers) {
        for (const auto& h : handlers) {
            if (vb) code += "        AddHandler " + owner + "." + h.first + ", AddressOf Me." + id(h.second) + "\r\n";
            else code += "        " + owner + "." + h.first + " += this." + id(h.second) + ";\r\n";
        }
    };
    if (vb) code = "' <auto-generated>OpenMSE designer: changes to this file are overwritten.</auto-generated>\r\nPartial Public Class " + id(class_name) + "\r\n    Inherits " + forms + "Form\r\n\r\n";
    else code = "// <auto-generated>OpenMSE designer: changes to this file are overwritten.</auto-generated>\r\npublic partial class " + id(class_name) + " : " + forms + "Form\r\n{\r\n";
    for (const auto& n : f.nodes)
        code += vb ? "    Private " + id(n.name) + " As " + forms + n.kind + "\r\n" : "    private " + forms + n.kind + " " + id(n.name) + ";\r\n";
    code += vb ? "\r\n    Private Sub InitializeComponent()\r\n" : "\r\n    private void InitializeComponent()\r\n    {\r\n";
    code += "        " + base + "Text = " + text(f.title) + end;
    code += "        " + base + "ClientSize = " + make + drawing + "Size(" + std::to_string(f.width) + ", " + std::to_string(f.height) + ")" + end;
    for (const auto& n : f.nodes) {
        std::string v = self + id(n.name);
        code += "        " + v + " = " + make + forms + n.kind + "()" + end;
        code += "        " + v + ".Name = " + text(n.name) + end;
        if (has_caption(n.kind)) code += "        " + v + ".Text = " + text(n.text) + end;
        code += "        " + v + ".SetBounds(" + std::to_string(n.x) + ", " + std::to_string(n.y) + ", " + std::to_string(n.width) + ", " + std::to_string(n.height) + ")" + end;
        code += "        " + v + ".Enabled = " + (vb ? (n.enabled ? "True" : "False") : (n.enabled ? "true" : "false")) + end;
        code += "        " + v + ".BackColor = " + drawing + "ColorTranslator.FromHtml(" + text(n.back_color) + ")" + end;
        code += "        " + v + ".ForeColor = " + drawing + "ColorTranslator.FromHtml(" + text(n.fore_color) + ")" + end;
        wire(v, n.handlers);
        code += "        " + base + "Controls.Add(" + v + ")" + end;
    }
    wire(vb ? "MyBase" : "base", f.handlers);
    return code + (vb ? "    End Sub\r\nEnd Class\r\n" : "    }\r\n}\r\n");
}

// ---------------------------------------------------------------- toolbox

void ToolboxService::load(const std::string& utf8_path)
{
    std::wstring path;
    try { path = widen(utf8_path); } catch (const std::invalid_argument& e) { invalid(e.what()); }
    std::ifstream file(path, std::ios::binary);
    if (!file) throw KernelError(ErrorCode::io_error, "无法读取工具箱目录：" + utf8_path);
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (text.compare(0, 3, "\xEF\xBB\xBF") == 0) text.erase(0, 3);
    if (!valid_utf8(text)) invalid("工具箱目录不是有效的 UTF-8。");
    JsonValue root;
    try { root = parse_json(text); } catch (const JsonError& e) { invalid(std::string("工具箱目录 JSON 无效：") + e.what()); }
    if (root.type != JsonValue::Type::array) invalid("工具箱目录必须是数组。");
    std::vector<ToolboxEntry> loaded;
    std::set<std::string> ids;
    for (const auto& item : root.items) {
        if (item.type != JsonValue::Type::object) invalid("工具箱条目必须是对象。");
        ToolboxEntry e;
        e.name = member_string(item, "Name", "");
        e.family_label = member_string(item, "Family", "");
        e.control_type = member_string(item, "ControlType", "");
        const JsonValue* markup = item.find("Markup");
        e.has_markup = markup && markup->type == JsonValue::Type::string;
        if (e.has_markup) e.markup = markup->text;
        if (e.name.empty() || !allowed_controls().count(e.control_type)) invalid("工具箱条目缺少名称或控件类型不受支持：" + e.name);
        if (e.family_label == "HTML") {
            e.family = "html"; e.backend = "html-source";
            if (!e.has_markup || HtmlScan(e.markup).tokens.empty()) invalid("HTML 工具箱条目缺少可插入的标签：" + e.name);
        } else if (e.family_label == "Windows Forms") {
            e.family = "winforms"; e.backend = "omform-source";
        } else invalid("未知的工具箱分类：" + e.family_label);
        std::string slug;
        for (unsigned char c : e.name) slug.push_back(std::isalnum(c) ? static_cast<char>(std::tolower(c)) : '-');
        e.id = e.family + "." + slug;
        if (!ids.insert(e.id).second) invalid("工具箱条目 ID 重复：" + e.id);
        loaded.push_back(std::move(e));
    }
    entries_ = std::move(loaded);
    loaded_ = true;
}

const ToolboxEntry& ToolboxService::find(const std::string& id) const
{
    for (const auto& e : entries_) if (e.id == id) return e;
    throw KernelError(ErrorCode::invalid_argument, "未知的工具箱条目：" + id);
}

} // namespace openmse::core
