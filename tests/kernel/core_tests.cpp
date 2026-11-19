// Behaviour tests for OpenMSE.Core through its public C ABI only.
// Usage: core-tests.exe <empty test directory>
#include <openmse/kernel/abi.h>

#include <cstdio>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#include <tlhelp32.h>

namespace {

int failures = 0, passes = 0;
void check(bool ok, const char* name, const std::string& detail = std::string())
{
    if (ok) ++passes; else { ++failures; std::printf("FAIL %s\n", name); if (!detail.empty()) std::printf("  %s\n", detail.substr(0, 3000).c_str()); }
}

struct Call {
    omk_status status = -1;
    std::string json;
    bool ok() const { return status == OMK_OK; }
    bool has(const std::string& s) const { return json.find(s) != std::string::npos; }
    // Extracts an unsigned number value for "key":N (first occurrence).
    unsigned long long num(const std::string& key) const
    {
        auto p = json.find("\"" + key + "\":");
        return p == std::string::npos ? ~0ull : std::strtoull(json.c_str() + p + key.size() + 3, nullptr, 10);
    }
};

Call take(omk_status s, omk_buffer& b)
{
    Call c{ s, b.data ? std::string(b.data, b.size) : std::string() };
    omk_buffer_free(&b);
    return c;
}

omk_kernel* k = nullptr;
Call create(const std::string& name, const std::string& text) { omk_buffer b{}; return take(omk_document_create(k, name.data(), name.size(), text.data(), text.size(), &b), b); }
Call open(const std::string& path) { omk_buffer b{}; return take(omk_document_open(k, path.data(), path.size(), &b), b); }
Call snapshot(unsigned long long id) { omk_buffer b{}; return take(omk_document_snapshot(k, id, &b), b); }
Call replace(unsigned long long id, unsigned long long rev, uint32_t bl, uint32_t bc, uint32_t el, uint32_t ec, const std::string& text, uint32_t flags = 0)
{
    omk_buffer b{};
    return take(omk_document_replace(k, id, rev, bl, bc, el, ec, text.data(), text.size(), flags, &b), b);
}
Call undo(unsigned long long id, unsigned long long rev) { omk_buffer b{}; return take(omk_document_undo(k, id, rev, &b), b); }
Call redo(unsigned long long id, unsigned long long rev) { omk_buffer b{}; return take(omk_document_redo(k, id, rev, &b), b); }
Call save(unsigned long long id, unsigned long long rev, const std::string& path = std::string()) { omk_buffer b{}; return take(omk_document_save(k, id, rev, path.data(), path.size(), &b), b); }
Call close(unsigned long long id, uint32_t policy) { omk_buffer b{}; return take(omk_document_close(k, id, policy, &b), b); }
Call toolbox(const std::string& path) { omk_buffer b{}; return take(omk_toolbox_load(k, path.data(), path.size(), &b), b); }
Call design(unsigned long long id) { omk_buffer b{}; return take(omk_design_snapshot(k, id, &b), b); }
Call apply(unsigned long long id, unsigned long long rev, const std::string& op) { omk_buffer b{}; return take(omk_design_apply(k, id, rev, op.data(), op.size(), &b), b); }
Call export_form(unsigned long long id, const std::string& language) { omk_buffer b{}; return take(omk_design_export(k, id, language.data(), language.size(), &b), b); }
std::size_t count(const std::string& s, const std::string& what)
{
    std::size_t n = 0;
    for (std::size_t p = s.find(what); p != std::string::npos; p = s.find(what, p + 1)) ++n;
    return n;
}
Call tools() { omk_buffer b{}; return take(omk_tools_list(k, &b), b); }
Call ws_open(const std::string& path) { omk_buffer b{}; return take(omk_workspace_open(k, path.data(), path.size(), &b), b); }
Call ws_close(unsigned long long id) { omk_buffer b{}; return take(omk_workspace_close(k, id, &b), b); }
Call build(unsigned long long ws, const std::string& config, const std::string& project = std::string())
{ omk_buffer b{}; return take(omk_build_start(k, ws, config.data(), config.size(), project.data(), project.size(), &b), b); }
Call run(unsigned long long ws, const std::string& config, const std::string& project = std::string())
{ omk_buffer b{}; return take(omk_run_start(k, ws, config.data(), config.size(), project.data(), project.size(), &b), b); }
Call task_wait(unsigned long long task, uint32_t ms) { omk_buffer b{}; return take(omk_task_wait(k, task, ms, &b), b); }
Call task_cancel(unsigned long long task) { omk_buffer b{}; return take(omk_task_cancel(k, task, &b), b); }
int count_processes(const wchar_t* name);
Call dbg_start(unsigned long long ws, const std::string& config, const std::string& breakpoints)
{ omk_buffer b{}; std::string project; return take(omk_debug_start(k, ws, config.data(), config.size(), project.data(), 0, breakpoints.data(), breakpoints.size(), &b), b); }
Call dbg_wait(unsigned long long s, uint32_t ms) { omk_buffer b{}; return take(omk_debug_wait(k, s, ms, &b), b); }
Call dbg(unsigned long long s, const std::string& action) { omk_buffer b{}; return take(omk_debug_control(k, s, action.data(), action.size(), &b), b); }
Call dbg_vars(unsigned long long s, uint32_t frame) { omk_buffer b{}; return take(omk_debug_variables(k, s, frame, &b), b); }
Call dbg_eval(unsigned long long s, uint32_t frame, const std::string& e) { omk_buffer b{}; return take(omk_debug_evaluate(k, s, frame, e.data(), e.size(), &b), b); }
Call dbg_bp(unsigned long long s, const std::string& file, uint32_t line) { omk_buffer b{}; return take(omk_debug_breakpoint_add(k, s, file.data(), file.size(), line, &b), b); }
Call capabilities() { omk_buffer b{}; return take(omk_capabilities(k, &b), b); }
std::string json_path(const std::string& p) { std::string o; for (char c : p) { if (c == '\\') o += "\\\\"; else o.push_back(c); } return o; }
std::string project_xml(const std::string& guid, const std::string& assembly, const std::string& type, bool vb, const std::string& compile, const std::string& reference)
{
    std::string x = "<Project DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n  <PropertyGroup>\n"
        "    <Configuration Condition=\" '$(Configuration)' == '' \">Debug</Configuration>\n    <Platform Condition=\" '$(Platform)' == '' \">AnyCPU</Platform>\n"
        "    <ProjectGuid>" + guid + "</ProjectGuid>\n    <OutputType>" + type + "</OutputType>\n    <AssemblyName>" + assembly + "</AssemblyName>\n"
        "    <RootNamespace>" + assembly + "</RootNamespace>\n  </PropertyGroup>\n"
        "  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Debug|AnyCPU' \">\n    <OutputPath>bin\\Debug\\</OutputPath>\n    <DebugSymbols>true</DebugSymbols>\n    <DebugType>full</DebugType>\n    <PlatformTarget>x86</PlatformTarget>\n  </PropertyGroup>\n"
        "  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Release|AnyCPU' \">\n    <OutputPath>bin\\Release\\</OutputPath>\n    <Optimize>true</Optimize>\n    <PlatformTarget>x86</PlatformTarget>\n  </PropertyGroup>\n"
        "  <ItemGroup>\n    <Reference Include=\"System\" />\n    <Compile Include=\"" + compile + "\" />\n  </ItemGroup>\n";
    if (!reference.empty()) x += "  <ItemGroup>\n    <ProjectReference Include=\"" + reference + "\">\n      <Project>{11111111-2222-3333-4444-555555555555}</Project>\n      <Name>Lib</Name>\n    </ProjectReference>\n  </ItemGroup>\n";
    return x + "  <Import Project=\"$(MSBuildBinPath)\\Microsoft." + (vb ? "VisualBasic" : "CSharp") + ".targets\" />\n</Project>\n";
}
Call poll() { omk_buffer b{}; return take(omk_event_poll(k, &b), b); }
Call command(const std::string& id, unsigned long long doc, unsigned long long rev, bool run)
{
    omk_buffer b{};
    return take(run ? omk_command_execute(k, id.data(), id.size(), doc, rev, &b) : omk_command_query(k, id.data(), id.size(), doc, &b), b);
}

// Test paths are UTF-8 like the ABI; narrow std::fstream paths would use the ANSI code page.
std::wstring wide(const std::string& utf8)
{
    std::wstring w(static_cast<std::size_t>(MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0)), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), w.data(), static_cast<int>(w.size()));
    return w;
}
std::string read_bytes(const std::string& path)
{
    std::ifstream f(wide(path), std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}
void write_bytes(const std::string& path, const std::string& data)
{
    std::ofstream f(wide(path), std::ios::binary | std::ios::trunc);
    f.write(data.data(), static_cast<std::streamsize>(data.size()));
}
std::string text_of(const Call& c)
{
    auto p = c.json.find("\"text\":\"");
    if (p == std::string::npos) return {};
    std::string out;
    for (std::size_t i = p + 8; i < c.json.size() && c.json[i] != '"'; ++i) {
        if (c.json[i] == '\\') { ++i; out.push_back(c.json[i] == 'n' ? '\n' : c.json[i]); }
        else out.push_back(c.json[i]);
    }
    return out;
}

int count_processes(const wchar_t* name)
{
    int n = 0;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W e{ sizeof(e) };
    for (BOOL ok = Process32FirstW(snap, &e); ok; ok = Process32NextW(snap, &e)) n += _wcsicmp(e.szExeFile, name) == 0;
    CloseHandle(snap);
    return n;
}

} // namespace

int main(int argc, char** argv)
{
    if (argc != 2) { std::printf("usage: core-tests <empty directory>\n"); return 2; }
    (void)argv;
    int wargc = 0;
    LPWSTR* wargv = CommandLineToArgvW(GetCommandLineW(), &wargc);
    std::wstring wdir = wargv[1];
    LocalFree(wargv);
    CreateDirectoryW(wdir.c_str(), nullptr);
    std::string dir(static_cast<std::size_t>(WideCharToMultiByte(CP_UTF8, 0, wdir.data(), static_cast<int>(wdir.size()), nullptr, 0, nullptr, nullptr)), '\0');
    WideCharToMultiByte(CP_UTF8, 0, wdir.data(), static_cast<int>(wdir.size()), dir.data(), static_cast<int>(dir.size()), nullptr, nullptr);
    if (dir.back() != '\\') dir += '\\';

    // Handshake.
    check(omk_abi_version() == OMK_ABI_VERSION, "abi version");
    omk_kernel* wrong = nullptr;
    check(omk_create(OMK_ABI_VERSION + 1, &wrong) == OMK_E_ABI_MISMATCH && wrong == nullptr, "abi mismatch refused");
    check(omk_create(OMK_ABI_VERSION, &k) == OMK_OK && k, "kernel created");

    // Untitled document: newline normalisation, Chinese and a surrogate pair (U+1F600).
    Call d = create(u8"未命名1.txt", "a\r\n\xE4\xB8\xAD\xE6\x96\x87\xF0\x9F\x98\x80\rb");
    check(d.ok() && text_of(d) == "a\n\xE4\xB8\xAD\xE6\x96\x87\xF0\x9F\x98\x80\nb", "create normalises CRLF/CR to LF");
    check(d.has("\"dirty\":true") && d.has("\"path\":null"), "seeded untitled document is dirty and has no path");
    unsigned long long id = d.num("id"), rev = d.num("revision");

    // Line 1 = "中文😀": UTF-16 length 4; column 3 is inside the surrogate pair.
    check(replace(id, rev, 1, 3, 1, 3, "X").status == OMK_E_INVALID_ARGUMENT, "column inside surrogate pair rejected");
    check(replace(id, rev, 1, 5, 1, 5, "X").status == OMK_E_INVALID_ARGUMENT, "column past line end rejected");
    check(replace(id, rev, 9, 0, 9, 0, "X").status == OMK_E_INVALID_ARGUMENT, "line past end rejected");
    check(replace(id, rev, 1, 2, 1, 0, "X").status == OMK_E_INVALID_ARGUMENT, "reversed range rejected");
    check(replace(id, rev, 0, 0, 0, 0, std::string("\xC3\x28", 2)).status == OMK_E_INVALID_ARGUMENT, "invalid UTF-8 rejected");
    Call r = replace(id, rev, 1, 4, 1, 4, "!");
    check(r.ok() && r.num("revision") == rev + 1, "insert after surrogate pair");
    check(replace(id, rev, 0, 0, 0, 0, "stale").status == OMK_E_CONFLICT, "stale revision conflicts");
    rev = r.num("revision");
    r = replace(id, rev, 1, 2, 2, 0, "|");  // replace "😀!\n" across a line boundary
    check(r.ok() && text_of(snapshot(id)) == "a\n\xE4\xB8\xAD\xE6\x96\x87|b", "multi-line range replace");
    rev = r.num("revision");

    // Typing coalesces into one undo step; undo reports an edit in pre-undo coordinates.
    Call fresh = snapshot(id);
    check(fresh.ok(), "snapshot after typing");
    std::string before_typing = text_of(fresh);
    rev = fresh.num("revision");
    unsigned long long base = rev;
    std::string start = before_typing;
    r = replace(id, rev, 0, 1, 0, 1, "1", OMK_EDIT_COALESCE); rev = r.num("revision");
    r = replace(id, rev, 0, 2, 0, 2, "2", OMK_EDIT_COALESCE); rev = r.num("revision");
    r = replace(id, rev, 0, 3, 0, 3, "3", OMK_EDIT_COALESCE); rev = r.num("revision");
    check(rev == base + 3 && text_of(snapshot(id)).substr(0, 4) == "a123", "three typing edits applied");
    Call u = undo(id, rev);
    check(u.ok() && u.has("\"begin\":{\"line\":0,\"column\":1}") && u.has("\"end\":{\"line\":0,\"column\":4}") && u.has("\"text\":\"\""), "coalesced typing undone as one step with view edit");
    rev = u.num("revision");
    check(text_of(snapshot(id)) == start, "undo restores text");
    Call re = redo(id, rev);
    check(re.ok() && re.has("\"text\":\"123\"") && text_of(snapshot(id)).substr(0, 4) == "a123", "redo reapplies typing");
    rev = re.num("revision");
    r = replace(id, rev, 0, 4, 0, 4, "\n", OMK_EDIT_COALESCE); rev = r.num("revision");
    u = undo(id, rev); rev = u.num("revision");
    check(text_of(snapshot(id)).substr(0, 4) == "a123", "newline is a separate undo step");
    check(redo(id, rev).ok(), "redo newline");
    rev = snapshot(id).num("revision");
    r = replace(id, rev, 0, 0, 0, 0, "new");
    check(r.has("\"can_redo\":false"), "new edit clears redo");
    rev = r.num("revision");

    // Commands route to the same operations.
    check(command("edit.undo", id, 0, false).has("\"enabled\":true"), "undo command enabled");
    check(command("file.save", id, 0, false).has("\"enabled\":false"), "save command disabled for untitled");
    check(command("no.such", id, 0, false).status == OMK_E_UNSUPPORTED, "unknown command");
    Call cu = command("edit.undo", id, rev, true);
    check(cu.ok() && cu.has("\"edit\""), "undo command executes");
    rev = cu.num("revision");
    check(save(id, rev).status == OMK_E_INVALID_STATE, "untitled save needs a path");

    // Save as: UTF-8 with BOM and CRLF by default; then clean.
    std::string chinese = dir + u8"中文.txt";
    Call s = save(id, rev, chinese);
    std::string disk = read_bytes(chinese);
    check(s.ok() && s.has("\"dirty\":false"), "save as succeeds and cleans");
    check(disk.size() > 3 && disk.compare(0, 3, "\xEF\xBB\xBF") == 0 && disk.find("\r\n") != std::string::npos && disk.find("\n", 0) == disk.find("\r\n") + 1, "default UTF-8 BOM with CRLF");
    check(close(id, OMK_CLOSE_REFUSE_DIRTY).ok(), "clean document closes");
    check(snapshot(id).status == OMK_E_INVALID_ARGUMENT, "closed document is gone");

    // Open dedupe by path case and identity; LF without BOM preserved.
    std::string lf = dir + "lf.txt";
    write_bytes(lf, "a\n\xE4\xB8\xAD\xE6\x96\x87\n");
    Call o = open(lf);
    id = o.num("id");
    check(o.ok() && o.has("\"encoding\":\"utf-8\"") && o.has("\"newline\":\"lf\"") && o.has("\"dirty\":false"), "open LF UTF-8 without BOM");
    std::string upper = lf;
    for (auto& c : upper) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    Call o2 = open(upper);
    check(o2.ok() && o2.num("id") == id && o2.has("\"already_open\":true"), "open deduplicates by path/file identity");
    rev = o.num("revision");
    r = replace(id, rev, 2, 0, 2, 0, "b\n");
    check(r.has("\"dirty\":true"), "edit makes dirty");
    rev = r.num("revision");
    u = undo(id, rev);
    check(u.has("\"dirty\":false"), "undo back to persisted state is clean");
    rev = u.num("revision");
    re = redo(id, rev); rev = re.num("revision");
    check(close(id, OMK_CLOSE_REFUSE_DIRTY).status == OMK_E_UNSAVED_DOCUMENT, "dirty close refused");
    check(save(id, rev).ok() && read_bytes(lf) == "a\n\xE4\xB8\xAD\xE6\x96\x87\nb\n", "LF and no BOM preserved on save");

    // External change protection: the file on disk must not be overwritten.
    write_bytes(lf, "external");
    r = replace(id, rev, 0, 0, 0, 0, "mine"); rev = r.num("revision");
    check(save(id, rev).status == OMK_E_CONFLICT && read_bytes(lf) == "external", "external edits preserved");
    DeleteFileW(wide(lf).c_str());
    check(save(id, rev).status == OMK_E_CONFLICT, "deleted file needs save-as");
    check(close(id, OMK_CLOSE_DISCARD).ok(), "discard close");

    // Two documents may not save over each other.
    std::string one = dir + "one.txt", two = dir + "two.txt";
    write_bytes(one, "1"); write_bytes(two, "2");
    unsigned long long a = open(one).num("id"), b2 = open(two).num("id");
    check(save(a, snapshot(a).num("revision"), two).status == OMK_E_CONFLICT && read_bytes(two) == "2", "save-as onto another open document refused");
    close(a, OMK_CLOSE_DISCARD); close(b2, OMK_CLOSE_DISCARD);

    // UTF-16 LE round trip with mixed newline reporting.
    std::string u16 = dir + "u16.txt";
    write_bytes(u16, std::string("\xFF\xFE" "a\0\r\0\n\0" "-N\n\0", 12));
    o = open(u16);
    check(o.ok() && o.has("\"encoding\":\"utf-16le\"") && o.has("\"newline\":\"mixed\"") && text_of(o) == "a\n\xE4\xB8\xAD\n", "UTF-16LE decode, mixed newlines");
    id = o.num("id");
    check(save(id, o.num("revision")).ok() && read_bytes(u16) == std::string("\xFF\xFE" "a\0\r\0\n\0" "-N\r\0\n\0", 14), "UTF-16LE save normalises to CRLF");
    close(id, OMK_CLOSE_DISCARD);

    // Binary files are not opened as text.
    std::string bin = dir + "bin.dat";
    write_bytes(bin, std::string("MZ\0\0\x90", 5));
    check(open(bin).status == OMK_E_UNSUPPORTED, "binary file refused");

    // Legacy ANSI code page: characters it cannot represent must fail, not be replaced.
    if (GetACP() == 936) {
        std::string gbk = dir + "gbk.txt";
        write_bytes(gbk, "\xD6\xD0\xCE\xC4");
        o = open(gbk);
        id = o.num("id");
        check(o.has("\"encoding\":\"cp936\"") && text_of(o) == "\xE4\xB8\xAD\xE6\x96\x87", "GBK decode");
        r = replace(id, o.num("revision"), 0, 2, 0, 2, "\xF0\x9F\x98\x80");
        check(save(id, r.num("revision")).status == OMK_E_IO_ERROR && read_bytes(gbk) == "\xD6\xD0\xCE\xC4", "unrepresentable GBK character refused, file unchanged");
        r = undo(id, r.num("revision"));
        r = replace(id, r.num("revision"), 0, 2, 0, 2, "A");
        check(save(id, r.num("revision")).ok() && read_bytes(gbk) == "\xD6\xD0\xCE\xC4" "A", "GBK save");
        close(id, OMK_CLOSE_DISCARD);
    } else std::printf("SKIP GBK checks (ACP %u)\n", GetACP());

    // ---- Toolbox service: stable IDs, validated catalog.
    std::string catalog = dir + "toolbox.json";
    write_bytes(catalog, "\xEF\xBB\xBF[{\"Name\":\"Button\",\"Family\":\"HTML\",\"Markup\":\"<INPUT type=\\\"button\\\" value=\\\"Button\\\">\",\"ControlType\":\"Button\"},"
        "{\"Name\":\"Text Area\",\"Family\":\"HTML\",\"Markup\":\"<TEXTAREA rows=2 cols=20>\\\\n\\\\n</TEXTAREA>\",\"ControlType\":\"RichTextBox\"},"
        "{\"Name\":\"Button\",\"Family\":\"Windows Forms\",\"Markup\":null,\"ControlType\":\"Button\"},"
        "{\"Name\":\"Panel\",\"Family\":\"Windows Forms\",\"Markup\":null,\"ControlType\":\"Panel\"}]");
    Call tb = toolbox(catalog);
    check(tb.ok() && tb.has("\"id\":\"html.button\"") && tb.has("\"id\":\"html.text-area\"") && tb.has("\"id\":\"winforms.button\"") && tb.has("\"backend\":\"omform-source\""), "toolbox loads with stable IDs");
    std::string bad_catalog = dir + "bad-toolbox.json";
    write_bytes(bad_catalog, "[{\"Name\":\"X\",\"Family\":\"Windows Forms\",\"ControlType\":\"WebBrowser\"}]");
    check(toolbox(bad_catalog).status == OMK_E_INVALID_ARGUMENT && toolbox(catalog).ok(), "unsupported toolbox control refused, catalog kept");

    // ---- HTML design transactions: source patches, preservation, undo, guards.
    std::string page = "<html><body>\n<!-- keep -->\n<script>var s = '<button id=\"x\">';</script>\n<p id=\"intro\">Hi &amp; bye</p>\n<div id=\"box\"><span>a</span></div>\n</body></html>\n";
    d = create("page.html", page);
    id = d.num("id"); rev = d.num("revision");
    Call ds = design(id);
    check(ds.ok() && ds.has("\"kind\":\"html\"") && count(ds.json, "\"kind\":\"Label\"") == 2 && ds.has("\"name\":\"intro\"") && ds.has("\"text\":\"Hi & bye\""), "HTML design snapshot");
    Call ad = apply(id, rev, "{\"op\":\"add\",\"toolbox_id\":\"html.button\",\"x\":60,\"y\":100}");
    std::string after_add = text_of(snapshot(id));
    check(ad.ok() && ad.num("revision") == rev + 1 && ad.has("\"node\":\"n3\"") && ad.has("\"edit\":{"), "HTML add is one revision and selects the new node");
    check(after_add.find("<INPUT type=\"button\" value=\"Button\" id=\"control1\" style=\"position:absolute;left:60px;top:100px;width:140px;height:30px;background-color:#FFFFFF;color:#202835;\">\n</body>") != std::string::npos, "HTML add inserts positioned markup before </body>");
    check(after_add.find("<!-- keep -->\n<script>var s = '<button id=\"x\">';</script>") != std::string::npos, "comments and scripts preserved");
    rev = ad.num("revision");
    Call st = apply(id, rev, u8"{\"op\":\"set\",\"node\":\"n3\",\"properties\":{\"text\":\"按钮 \\\"中文\\\" & 保存\",\"x\":180}}");
    check(st.ok() && text_of(snapshot(id)).find(u8"value=\"按钮 &quot;中文&quot; &amp; 保存\"") != std::string::npos && st.has(u8"\"text\":\"按钮 \\\"中文\\\" & 保存\"") && st.has("\"x\":180"), "HTML property writeback encodes and round-trips");
    rev = st.num("revision");
    check(apply(id, rev, "{\"op\":\"set\",\"node\":\"n0\",\"properties\":{\"name\":\"control1\"}}").status == OMK_E_INVALID_ARGUMENT, "duplicate HTML id refused");
    check(apply(id, rev, "{\"op\":\"set\",\"node\":\"n0\",\"properties\":{\"name\":\"1bad\"}}").status == OMK_E_INVALID_ARGUMENT, "invalid name refused");
    check(apply(id, rev, "{\"op\":\"set\",\"node\":\"n1\",\"properties\":{\"text\":\"x\"}}").status == OMK_E_INVALID_ARGUMENT, "text change on nested markup refused");
    check(design(id).has("\"name\":\"box\",\"text\":\"a\",\"x\":24,\"y\":") && design(id).has("\"text_editable\":false"), "nested markup marked as not text-editable");
    check(apply(id, rev, "{\"op\":\"set\",\"node\":\"n9\",\"properties\":{\"x\":1}}").status == OMK_E_INVALID_ARGUMENT, "unknown node refused");
    check(apply(id, rev - 1, "{\"op\":\"delete\",\"node\":\"n3\"}").status == OMK_E_CONFLICT, "stale design revision conflicts");
    check(apply(id, rev, "{\"op\":\"add\",\"toolbox_id\":\"winforms.button\"}").status == OMK_E_INVALID_STATE, "cross-family toolbox entry refused");
    check(snapshot(id).num("revision") == rev, "refused operations commit nothing");
    Call ta = apply(id, rev, "{\"op\":\"add\",\"toolbox_id\":\"html.text-area\",\"x\":0,\"y\":0}");
    check(ta.ok() && text_of(snapshot(id)).find("<TEXTAREA rows=2 cols=20 id=\"control2\"") != std::string::npos && text_of(snapshot(id)).find(">\n\n</TEXTAREA>") != std::string::npos, "toolbox markup newline escapes expanded");
    rev = ta.num("revision");
    Call del = apply(id, rev, "{\"op\":\"delete\",\"node\":\"n4\"}");
    check(del.ok() && del.has("\"node\":null") && count(del.json, "\"id\":\"n") == 4, "HTML delete");
    rev = del.num("revision");
    u = undo(id, rev); rev = u.num("revision");
    check(count(design(id).json, "\"id\":\"n") == 5, "design delete undone as one step");
    u = undo(id, rev); rev = u.num("revision");
    u = undo(id, rev); rev = u.num("revision");
    check(design(id).has("\"text\":\"Button\"") && !design(id).has(u8"保存"), "property change undone");
    u = undo(id, rev); rev = u.num("revision");
    check(text_of(snapshot(id)) == page, "all design transactions undone to the original source");
    close(id, OMK_CLOSE_DISCARD);

    // ---- .omform: in-place value patches keep formatting and unknown members.
    std::string pretty = "{\n  \"FormatVersion\": 1,\n  \"Title\": \"OpenMSE Form\",\n  \"Width\": 720,\n  \"Height\": 480,\n  \"Extra\": {\"keep\": true},\n  \"Controls\": [\n    {\n      \"Kind\": \"Label\",\n      \"Name\": \"titleLabel\",\n      \"Text\": \"标题\",\n      \"X\": 30,\n      \"Y\": 25,\n      \"Width\": 320,\n      \"Height\": 35,\n      \"Tag\": \"custom\"\n    }\n  ]\n}\n";
    d = create("f.omform", pretty);
    id = d.num("id"); rev = d.num("revision");
    ds = design(id);
    check(ds.has("\"kind\":\"form\"") && ds.has("\"title\":\"OpenMSE Form\"") && ds.has("\"name\":\"titleLabel\"") && ds.has("\"back_color\":\"#FFFFFF\""), "form snapshot with defaults for missing members");
    st = apply(id, rev, "{\"op\":\"set\",\"node\":\"n0\",\"properties\":{\"x\":50,\"fore_color\":\"#FF0000\"}}");
    std::string expected = pretty;
    expected.replace(expected.find("\"X\": 30"), 7, "\"X\": 50");
    expected.insert(expected.find("\"custom\"") + 8, ",\n      \"ForeColor\": \"#FF0000\"");
    check(st.ok() && text_of(snapshot(id)) == expected, "form set patches one value and appends a missing member in the file's style");
    rev = st.num("revision");
    ad = apply(id, rev, "{\"op\":\"add\",\"toolbox_id\":\"winforms.panel\",\"x\":10,\"y\":20}");
    std::string formed = text_of(snapshot(id));
    check(ad.ok() && ad.has("\"node\":\"n1\"") && formed.find("},\n    {\n      \"Kind\": \"Panel\",\n      \"Name\": \"panel1\",\n      \"Text\": \"Panel\",\n      \"X\": 10,") != std::string::npos && formed.find("\"Extra\": {\"keep\": true}") != std::string::npos, "form add follows the array's indentation");
    rev = ad.num("revision");
    st = apply(id, rev, u8"{\"op\":\"set_form\",\"properties\":{\"title\":\"测试<窗体>\",\"width\":800}}");
    check(st.ok() && text_of(snapshot(id)).find(u8"\"Title\": \"测试\\u003c窗体\\u003e\"") != std::string::npos && st.has("\"width\":800"), "form properties written JavaScriptSerializer-style");
    rev = st.num("revision");
    check(apply(id, rev, "{\"op\":\"set\",\"node\":\"n0\",\"properties\":{\"width\":5}}").status == OMK_E_INVALID_ARGUMENT, "form range validation");
    check(apply(id, rev, "{\"op\":\"set\",\"node\":\"n1\",\"properties\":{\"name\":\"TITLELABEL\"}}").status == OMK_E_INVALID_ARGUMENT, "form names unique ignoring case");
    check(apply(id, rev, "{\"op\":\"set_form\",\"properties\":{\"width\":50}}").status == OMK_E_INVALID_ARGUMENT, "form size validation");
    Call cs = export_form(id, "csharp"), vb = export_form(id, "vb");
    check(cs.ok() && cs.has("Panel control_panel1 = new Panel();") && cs.has("control_titleLabel.SetBounds(50, 25, 320, 35);") && vb.has("Dim control_panel1 As New Panel()"), "C# and VB export");
    del = apply(id, rev, "{\"op\":\"delete\",\"node\":\"n0\"}");
    check(del.ok() && text_of(snapshot(id)).find("titleLabel") == std::string::npos && design(id).has("\"name\":\"panel1\"") && !design(id).has("\"error\":\""), "form delete leaves valid JSON");
    rev = del.num("revision");
    std::string form_path = dir + u8"窗体.omform";
    check(save(id, rev, form_path).ok() && close(id, OMK_CLOSE_REFUSE_DIRTY).ok(), "form saved and closed");
    o = open(form_path);
    check(o.ok() && design(o.num("id")).has("\"name\":\"panel1\"") && design(o.num("id")).has(u8"\"title\":\"测试<窗体>\""), "form reopened with the same design");
    close(o.num("id"), OMK_CLOSE_DISCARD);

    // Compact layouts stay compact; a missing Controls member is created.
    d = create("c.omform", "{\"FormatVersion\":1,\"Title\":\"Form1\",\"Width\":720,\"Height\":480,\"Controls\":[]}");
    id = d.num("id");
    ad = apply(id, d.num("revision"), "{\"op\":\"add\",\"toolbox_id\":\"winforms.button\",\"x\":1,\"y\":2}");
    ad = apply(id, ad.num("revision"), "{\"op\":\"add\",\"toolbox_id\":\"winforms.button\",\"x\":3,\"y\":4}");
    check(text_of(snapshot(id)) == "{\"FormatVersion\":1,\"Title\":\"Form1\",\"Width\":720,\"Height\":480,\"Controls\":[{\"Kind\":\"Button\",\"Name\":\"button1\",\"Text\":\"Button\",\"X\":1,\"Y\":2,\"Width\":140,\"Height\":32,\"BackColor\":\"#FFFFFF\",\"ForeColor\":\"#202835\",\"Enabled\":true,\"Family\":\"Windows Forms\"},{\"Kind\":\"Button\",\"Name\":\"button2\",\"Text\":\"Button\",\"X\":3,\"Y\":4,\"Width\":140,\"Height\":32,\"BackColor\":\"#FFFFFF\",\"ForeColor\":\"#202835\",\"Enabled\":true,\"Family\":\"Windows Forms\"}]}", "compact layout stays compact");
    close(id, OMK_CLOSE_DISCARD);
    d = create("m.omform", "{\"Title\":\"T\"}");
    id = d.num("id");
    ad = apply(id, d.num("revision"), "{\"op\":\"add\",\"toolbox_id\":\"winforms.button\",\"x\":0,\"y\":0}");
    check(ad.ok() && design(id).has("\"name\":\"button1\""), "missing Controls member created");
    close(id, OMK_CLOSE_DISCARD);
    d = create("bad.omform", "{ invalid layout");
    id = d.num("id");
    check(design(id).has("\"error\":\"") && apply(id, d.num("revision"), "{\"op\":\"add\",\"toolbox_id\":\"winforms.button\"}").status == OMK_E_INVALID_ARGUMENT && text_of(snapshot(id)) == "{ invalid layout", "invalid layout reported, source untouched");
    close(id, OMK_CLOSE_DISCARD);
    d = create("t.txt", "x");
    check(design(d.num("id")).has("\"kind\":\"none\"") && apply(d.num("id"), d.num("revision"), "{\"op\":\"delete\",\"node\":\"n0\"}").status == OMK_E_INVALID_STATE, "plain text has no design view");
    close(d.num("id"), OMK_CLOSE_DISCARD);

    // ---- Phase 3: tools, project graph, asynchronous build/run tasks, failure and cancellation.
    Call tl = tools();
    check(tl.ok() && tl.has("\"id\":\"msbuild-2.0\""), "MSBuild 2.0 discovered");
    std::string multi = dir + u8"多项目\\";
    CreateDirectoryW(wide(multi).c_str(), nullptr);
    CreateDirectoryW(wide(multi + "Lib").c_str(), nullptr);
    CreateDirectoryW(wide(multi + "App").c_str(), nullptr);
    const std::string lib_guid = "{11111111-2222-3333-4444-555555555555}", app_guid = "{AAAAAAAA-BBBB-CCCC-DDDD-EEEEEEEEEEEE}";
    write_bytes(multi + "Lib\\Lib.vbproj", "\xEF\xBB\xBF" + project_xml(lib_guid, "Lib", "Library", true, "Greeter.vb", ""));
    write_bytes(multi + "Lib\\Greeter.vb", std::string("\xEF\xBB\xBF") + u8"Public Class Greeter\r\n    Public Shared Function Greeting() As String\r\n        Return \"Hello, OpenMSE 中文\"\r\n    End Function\r\nEnd Class\r\n");
    write_bytes(multi + "App\\App.csproj", "\xEF\xBB\xBF" + project_xml(app_guid, "App", "Exe", false, "Program.cs", "..\\Lib\\Lib.vbproj"));
    std::string good_program = "\xEF\xBB\xBFusing System;\r\ninternal static class Program\r\n{\r\n    private static int Main()\r\n    {\r\n        Console.WriteLine(Lib.Greeter.Greeting());\r\n        return 0;\r\n    }\r\n}\r\n";
    write_bytes(multi + "App\\Program.cs", good_program);
    std::string sln = "\xEF\xBB\xBF\r\nMicrosoft Visual Studio Solution File, Format Version 9.00\r\n# Visual Studio 2005\r\n"
        "Project(\"{F184B08F-C81C-45F6-A57F-5ABD9991F28F}\") = \"Lib\", \"Lib\\Lib.vbproj\", \"" + lib_guid + "\"\r\nEndProject\r\n"
        "Project(\"{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}\") = \"App\", \"App\\App.csproj\", \"" + app_guid + "\"\r\nEndProject\r\n"
        "Global\r\n\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\r\n\t\tDebug|Any CPU = Debug|Any CPU\r\n\t\tRelease|Any CPU = Release|Any CPU\r\n\tEndGlobalSection\r\n"
        "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\r\n";
    for (const auto& g : { lib_guid, app_guid })
        for (const char* c : { "Debug", "Release" })
            sln += "\t\t" + g + "." + c + "|Any CPU.ActiveCfg = " + c + "|Any CPU\r\n\t\t" + g + "." + c + "|Any CPU.Build.0 = " + c + "|Any CPU\r\n";
    sln += "\tEndGlobalSection\r\nEndGlobal\r\n";
    write_bytes(multi + "Multi.sln", sln);
    Call w = ws_open(multi + "Multi.sln");
    unsigned long long wid = w.num("id");
    check(w.ok() && w.has("\"configurations\":[\"Debug|Any CPU\",\"Release|Any CPU\"]") && w.has("\"id\":\"" + app_guid + "\",\"name\":\"App\",\"kind\":\"csharp\"")
        && w.has("\"project_references\":[\"" + lib_guid + "\"]") && w.has("\"startup_project\":\"" + app_guid + "\"") && w.has("\"msbuild\":{\"id\":\"msbuild-2.0\""), "solution project graph");
    check(w.has("bin\\\\Release\\\\App.exe") && w.has("Lib\\\\bin\\\\Debug\\\\Lib.dll") && w.has("Greeter.vb"), "per-configuration artifacts and files");
    check(build(wid, "Nope|Any CPU").status == OMK_E_INVALID_ARGUMENT, "unknown configuration refused");
    check(run(wid, "Debug|Any CPU", lib_guid).status == OMK_E_INVALID_STATE, "library is not runnable");
    while (!poll().has("\"event\":null")) {}
    Call b = build(wid, "Debug|Any CPU");
    check(b.ok() && b.has("\"state\":\"running\"") && b.has("\"exit_code\":null"), "build starts asynchronously");
    Call bw = task_wait(b.num("id"), 120000);
    check(bw.has("\"finished\":true") && bw.has("\"state\":\"succeeded\"") && bw.has("\"exit_code\":0") && bw.has("\"errors\":0")
        && bw.has("App\\\\bin\\\\Debug\\\\App.exe") && bw.has("Lib\\\\bin\\\\Debug\\\\Lib.dll"), "solution build succeeds with both artifacts", bw.json);
    {
        Call e = poll();
        bool started = e.has("\"topic\":\"task.started\"");
        int output = 0; bool finished = false;
        for (int guard2 = 0; guard2 < 5000 && !e.has("\"event\":null"); ++guard2) { e = poll(); output += e.has("\"topic\":\"task.output\""); finished |= e.has("\"topic\":\"task.finished\""); }
        check(started && output > 3 && finished, "task events: started, output lines, finished");
    }
    Call rr = run(wid, "Debug|Any CPU");
    Call rw = task_wait(rr.num("id"), 30000);
    check(rw.has("\"state\":\"succeeded\"") && rw.has(u8"Hello, OpenMSE 中文"), "run task executes the startup project and captures output", rw.json);
    Call rb = build(wid, "Release|Any CPU", app_guid);
    rw = task_wait(rb.num("id"), 120000);
    check(rw.has("\"state\":\"succeeded\"") && rw.has("App\\\\bin\\\\Release\\\\App.exe") && rw.has("Lib\\\\bin\\\\Release\\\\Lib.dll"), "project build in Release builds its reference too");
    write_bytes(multi + "App\\Program.cs", "using System;\r\ninternal static class Program\r\n{\r\n    private static int Main()\r\n    {\r\n        int x = ;\r\n        return 0;\r\n    }\r\n}\r\n");
    Call fb = build(wid, "Debug|Any CPU");
    rw = task_wait(fb.num("id"), 120000);
    check(rw.has("\"state\":\"failed\"") && !rw.has("\"exit_code\":0") && rw.num("errors") >= 1 && rw.has("App\\\\Program.cs\",\"line\":6,\"column\":17,\"code\":\"CS1525\""), "failed build: state, exit code and located diagnostic", rw.json);
    write_bytes(multi + "App\\Program.cs", good_program);
    std::string slow = dir + "Slow\\";
    CreateDirectoryW(wide(slow).c_str(), nullptr);
    write_bytes(slow + "Slow.csproj", "<Project DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n"
        "  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Debug|AnyCPU' \"><OutputPath>bin\\</OutputPath></PropertyGroup>\n"
        "  <Target Name=\"Build\"><Exec Command=\"ping -n 60 127.0.0.1\" /></Target>\n</Project>\n");
    Call sw = ws_open(slow + "Slow.csproj");
    check(sw.ok() && sw.has("\"configurations\":[\"Debug|AnyCPU\"]"), "single project opened as a workspace");
    int pings = count_processes(L"PING.EXE");
    Call sb = build(sw.num("id"), "Debug|AnyCPU");
    Call early = task_wait(sb.num("id"), 2500);
    check(early.has("\"finished\":false") && count_processes(L"PING.EXE") == pings + 1, "slow build still running with its child process");
    check(ws_close(sw.num("id")).status == OMK_E_INVALID_STATE, "workspace with a running task cannot close");
    check(task_cancel(sb.num("id")).ok(), "cancel accepted");
    Call cw = task_wait(sb.num("id"), 10000);
    check(cw.has("\"finished\":true") && cw.has("\"state\":\"cancelled\"") && count_processes(L"PING.EXE") == pings, "cancelled build ends the whole process tree");
    check(task_cancel(sb.num("id")).status == OMK_E_INVALID_STATE, "finished task cannot be cancelled again");
    // ---- Phase 5: managed debugging with evidence for breakpoints, stepping, stack, variables and source locations.
    write_bytes(multi + "App\\Program.cs", "using System;\r\ninternal static class Program\r\n{\r\n    private static int counter = 5;\r\n    private static int Main()\r\n    {\r\n"
        "        int answer = 41;\r\n        answer = answer + 1;\r\n        string text = Lib.Greeter.Greeting();\r\n        Console.WriteLine(text + answer);\r\n"
        "        return answer + counter - 5;\r\n    }\r\n}\r\n");
    Call db = task_wait(build(wid, "Debug|Any CPU").num("id"), 120000);
    check(db.has("\"state\":\"succeeded\""), "debug build", db.json);
    std::string program_cs = json_path(multi + "App\\Program.cs"), greeter_vb = json_path(multi + "Lib\\Greeter.vb");
    Call ds1 = dbg_start(wid, "Debug|Any CPU", "[{\"file\":\"" + program_cs + "\",\"line\":8}]");
    unsigned long long sid = ds1.num("id");
    check(ds1.ok() && ds1.has("\"adapter\":\"x86\""), "debug session started with the x86 adapter for an x86 target", ds1.json);
    Call st1 = dbg_wait(sid, 30000);
    check(st1.has("\"state\":\"stopped\"") && st1.has("\"reason\":\"breakpoint\"") && st1.has("\"function\":\"Program.Main\"") && st1.has("App\\\\Program.cs\",\"line\":8,")
        && st1.has("\"verified\":true,\"bound_line\":8"), "breakpoint hit at Program.cs:8 with source location", st1.json);
    Call vars = dbg_vars(sid, 0);
    check(vars.has("{\"name\":\"answer\",\"kind\":\"local\",\"type\":\"int\",\"value\":\"41\"}") && vars.has("\"name\":\"text\""), "locals read at the breakpoint", vars.json);
    check(dbg_eval(sid, 0, "answer").has("\"value\":\"41\"") && dbg_eval(sid, 0, "nosuch").status == OMK_E_INVALID_ARGUMENT && dbg_eval(sid, 0, "1+1").status == OMK_E_UNSUPPORTED, "evaluation of locals; unknown and unsupported expressions refused");
    check(dbg(sid, "step_over").ok(), "step over accepted");
    Call st2 = dbg_wait(sid, 15000);
    check(st2.has("\"reason\":\"step\"") && st2.has("Program.cs\",\"line\":9,") && dbg_eval(sid, 0, "answer").has("\"value\":\"42\""), "step over moves to line 9 and the variable changes", st2.json);
    check(dbg(sid, "step_into").ok(), "step into accepted");
    Call st3 = dbg_wait(sid, 15000);
    check(st3.has("\"function\":\"Lib.Greeter.Greeting\"") && st3.has("Lib\\\\Greeter.vb\"") && st3.has("\"index\":1,\"function\":\"Program.Main\"") , "step into crosses into the VB library with a two-frame stack", st3.json);
    check(dbg(sid, "step_out").ok(), "step out accepted");
    Call st4 = dbg_wait(sid, 15000);
    check(st4.has("\"index\":0,\"function\":\"Program.Main\"") && st4.has("Program.cs\",\"line\":9,"), "step out returns to the caller", st4.json);
    dbg(sid, "step_over");
    Call st5 = dbg_wait(sid, 15000);
    check(st5.has("Program.cs\",\"line\":10,") && dbg_eval(sid, 0, "text").has(u8"\"value\":\"\\\"Hello, OpenMSE 中文\\\"\"") && dbg_eval(sid, 0, "counter").status == OMK_E_INVALID_ARGUMENT, "string local after the call", st5.json + dbg_eval(sid, 0, "text").json);
    Call bp2 = dbg_bp(sid, multi + "App\\Program.cs", 11);
    check(bp2.has("\"verified\":true,\"bound_line\":11"), "breakpoint added while stopped is bound", bp2.json);
    dbg(sid, "continue");
    Call st6 = dbg_wait(sid, 15000);
    check(st6.has("\"reason\":\"breakpoint\"") && st6.has("Program.cs\",\"line\":11,") && st6.has(u8"Hello, OpenMSE 中文42"), "continue runs to the new breakpoint; program output captured", st6.json);
    dbg(sid, "continue");
    Call ex1 = dbg_wait(sid, 15000);
    check(ex1.has("\"state\":\"exited\"") && ex1.has("\"exit_code\":42") && ex1.has("\"reason\":\"exited\""), "program exits with its real exit code", ex1.json);
    check(dbg(sid, "continue").status == OMK_E_INVALID_STATE && dbg_vars(sid, 0).status == OMK_E_INVALID_STATE && dbg_eval(sid, 0, "answer").status == OMK_E_INVALID_STATE, "no stepping, variables or evaluation after exit");

    // AnyCPU target: runs as 64-bit, so the x64 adapter is used; pause and terminate a running program.
    std::string anycpu = dir + "AnyCpu\\";
    CreateDirectoryW(wide(anycpu).c_str(), nullptr);
    write_bytes(anycpu + "AnyCpu.csproj", "<Project DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n  <PropertyGroup>\n    <OutputType>Exe</OutputType>\n    <AssemblyName>AnyCpu</AssemblyName>\n  </PropertyGroup>\n"
        "  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Debug|AnyCPU' \">\n    <OutputPath>bin\\Debug\\</OutputPath>\n    <DebugSymbols>true</DebugSymbols>\n    <DebugType>full</DebugType>\n  </PropertyGroup>\n"
        "  <ItemGroup>\n    <Reference Include=\"System\" />\n    <Compile Include=\"Loop.cs\" />\n  </ItemGroup>\n  <Import Project=\"$(MSBuildBinPath)\\Microsoft.CSharp.targets\" />\n</Project>\n");
    write_bytes(anycpu + "Loop.cs", "internal static class Program\r\n{\r\n    private static int Main()\r\n    {\r\n        int i = 0;\r\n        while (i >= 0)\r\n        {\r\n            i = i + 1;\r\n            System.Threading.Thread.Sleep(20);\r\n        }\r\n        return i;\r\n    }\r\n}\r\n");
    Call aw = ws_open(anycpu + "AnyCpu.csproj");
    Call ab = task_wait(build(aw.num("id"), "Debug|AnyCPU").num("id"), 120000);
    Call ds2 = dbg_start(aw.num("id"), "Debug|AnyCPU", "");
    unsigned long long sid2 = ds2.num("id");
    check(ab.has("\"state\":\"succeeded\"") && ds2.ok() && ds2.has("\"adapter\":\"x64\""), "AnyCPU target debugged by the x64 adapter", ab.json + ds2.json);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    check(dbg(sid2, "step_over").status == OMK_E_INVALID_STATE && dbg_eval(sid2, 0, "i").status == OMK_E_INVALID_STATE, "stepping and evaluation refused while running");
    check(dbg(sid2, "pause").ok(), "pause accepted");
    Call ps = dbg_wait(sid2, 10000);
    check(ps.has("\"state\":\"stopped\"") && ps.has("\"reason\":\"pause\"") && ps.has("\"function\":\"Program.Main\""), "paused program shows its managed stack", ps.json);
    unsigned long long main_frame = 0;
    for (unsigned long long f = 0; f < 10; ++f) { if (dbg_eval(sid2, static_cast<uint32_t>(f), "i").ok()) { main_frame = f; break; } }
    Call iv = dbg_eval(sid2, static_cast<uint32_t>(main_frame), "i");
    check(iv.ok() && iv.has("\"type\":\"int\"") && std::strtoll(iv.json.c_str() + iv.json.find("\"value\":\"") + 9, nullptr, 10) > 0, "loop counter read from the paused 64-bit program", iv.json);
    check(dbg(sid2, "stop").ok(), "stop accepted");
    Call te = dbg_wait(sid2, 15000);
    check(te.has("\"state\":\"exited\"") && te.has("\"reason\":\"terminated\""), "terminated session ends as exited/terminated", te.json);
    Call caps = capabilities();
    check(caps.has("\"name\":\"debug.clr2\",\"provider\":\"OpenMSE.DebugHost\",\"evidence\":\"behavior_verified\"") && caps.has("\"name\":\"host.vs8.debug\",\"provider\":\"vs8\",\"evidence\":\"blocked\""), "capability registry", caps.json);
    ws_close(aw.num("id"));

    check(ws_close(sw.num("id")).ok() && ws_close(wid).ok(), "workspaces closed");
    while (!poll().has("\"event\":null")) {}

    // Ordered events.
    d = create("events.txt", "");
    close(d.num("id"), OMK_CLOSE_REFUSE_DIRTY);
    Call e1 = poll(), e2 = poll();
    check(e1.has("\"topic\":\"document.opened\"") && e2.num("sequence") == e1.num("sequence") + 1, "ordered event queue");
    Call e;
    int guard = 0;
    do { e = poll(); } while (!e.has("\"event\":null") && ++guard < 10000);
    check(e.ok() && e.has("\"event\":null"), "event queue drains");

    omk_destroy(k);
    std::printf("%d passed, %d failed\n", passes, failures);
    return failures == 0 ? 0 : 1;
}
