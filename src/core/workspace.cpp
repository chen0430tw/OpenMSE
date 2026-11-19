#include "workspace.hpp"

#include "documents.hpp"
#include "process.hpp"
#include "xml.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>
#include <sstream>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace openmse::core {

namespace {

std::string lower(std::string s) { for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c))); return s; }
std::string trim(const std::string& s)
{
    std::size_t b = s.find_first_not_of(" \t\r\n"), e = s.find_last_not_of(" \t\r\n");
    return b == std::string::npos ? std::string() : s.substr(b, e - b + 1);
}

std::wstring full(const std::wstring& path)
{
    DWORD n = GetFullPathNameW(path.c_str(), 0, nullptr, nullptr);
    if (!n) throw KernelError(ErrorCode::invalid_argument, "无法解析路径：" + narrow(path));
    std::wstring out(n, L'\0');
    out.resize(GetFullPathNameW(path.c_str(), n, out.data(), nullptr));
    return out;
}

std::wstring parent(const std::wstring& path)
{
    std::size_t s = path.find_last_of(L"\\/");
    return s == std::wstring::npos ? std::wstring() : path.substr(0, s);
}

std::wstring combine(const std::wstring& dir, const std::wstring& rel)
{
    if (rel.size() > 1 && (rel[1] == L':' || (rel[0] == L'\\' && rel[1] == L'\\'))) return full(rel);
    return full(dir + L"\\" + rel);
}

std::wstring to_backslashes(std::wstring s) { std::replace(s.begin(), s.end(), L'/', L'\\'); return s; }

bool exists(const std::wstring& p) { return GetFileAttributesW(p.c_str()) != INVALID_FILE_ATTRIBUTES; }

std::string read_text_file(const std::wstring& path)
{
    std::ifstream f(path, std::ios::binary);
    if (!f) throw KernelError(ErrorCode::io_error, "无法读取：" + narrow(path));
    std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    try { return normalize_newlines(decode_disk_bytes(bytes).utf8); }
    catch (const std::runtime_error& e) { throw KernelError(ErrorCode::unsupported, narrow(path) + "：" + e.what()); }
}

std::string replace_all(std::string s, const std::string& from, const std::string& to)
{
    for (std::size_t p = 0; (p = s.find(from, p)) != std::string::npos; p += to.size()) s.replace(p, from.size(), to);
    return s;
}

std::string ireplace_vars(std::string value, const std::string& configuration, const std::string& platform)
{
    for (std::size_t p; (p = lower(value).find("$(configuration)")) != std::string::npos;) value.replace(p, 16, configuration);
    for (std::size_t p; (p = lower(value).find("$(platform)")) != std::string::npos;) value.replace(p, 11, platform);
    return value;
}

// Evaluates the VS2005 project condition forms; returns false for anything it does not understand.
bool condition_matches(const std::string& condition, const std::string& configuration, const std::string& platform)
{
    std::string c = lower(condition);
    std::size_t eq = c.find("==");
    if (eq == std::string::npos) return false;
    std::string left = trim(c.substr(0, eq)), right = trim(c.substr(eq + 2));
    auto unquote = [](std::string s) { if (s.size() >= 2 && s.front() == '\'' && s.back() == '\'') return s.substr(1, s.size() - 2); return s; };
    left = unquote(left); right = trim(unquote(right));
    if (right.empty()) return false; // "'$(Configuration)' == ''" defaults: we always supply a configuration
    std::string actual = replace_all(replace_all(left, "$(configuration)", lower(configuration)), "$(platform)", lower(platform));
    if (actual.find("$(") != std::string::npos) return false;
    return trim(actual) == right;
}

void collect_project_configurations(const XmlNode& root, std::vector<std::string>& out)
{
    for (const XmlNode* group : root.all("PropertyGroup")) {
        std::string c = lower(group->attribute("Condition"));
        std::size_t eq = c.find("==");
        if (c.find("$(configuration)|$(platform)") == std::string::npos || eq == std::string::npos) continue;
        std::string original = group->attribute("Condition");
        std::size_t q1 = original.find('\'', eq + 2), q2 = q1 == std::string::npos ? q1 : original.find('\'', q1 + 1);
        if (q2 == std::string::npos) continue;
        std::string value = trim(original.substr(q1 + 1, q2 - q1 - 1));
        if (std::find(out.begin(), out.end(), value) == out.end()) out.push_back(value);
    }
}

std::map<std::string, std::string> evaluate(const XmlNode& root, const std::string& configuration, const std::string& platform)
{
    std::map<std::string, std::string> props{ { "configuration", configuration }, { "platform", platform } };
    for (const XmlNode* group : root.all("PropertyGroup")) {
        std::string gc = group->attribute("Condition");
        if (!gc.empty() && !condition_matches(gc, configuration, platform)) continue;
        for (const auto& p : group->children) {
            std::string pc = p.attribute("Condition");
            if (!pc.empty() && !condition_matches(pc, configuration, platform)) continue;
            std::string key = lower(p.name);
            if (key == "configuration" || key == "platform") continue;
            props[key] = ireplace_vars(trim(p.text), configuration, platform);
        }
    }
    return props;
}

void load_project(Project& p)
{
    p.directory = parent(p.path);
    std::wstring ext = p.path.size() > 7 ? p.path.substr(p.path.size() - 7) : L"";
    for (auto& c : ext) c = static_cast<wchar_t>(towlower(c));
    p.kind = ext == L".csproj" ? "csharp" : ext == L".vbproj" ? "vb" : "other";
    if (p.kind == "other") return;
    XmlNode root;
    try { root = parse_xml(read_text_file(p.path)); }
    catch (const KernelError& e) { p.load_error = e.what(); return; }
    catch (const std::runtime_error& e) { p.load_error = e.what(); return; }
    if (lower(root.name) != "project") { p.load_error = "不是 MSBuild 项目文件。"; return; }
    p.tools_version = root.attribute("ToolsVersion");
    auto base = evaluate(root, "Debug", "AnyCPU");
    p.assembly_name = base.count("assemblyname") ? base["assemblyname"] : narrow(p.path.substr(p.path.find_last_of(L'\\') + 1, p.path.size() - p.path.find_last_of(L'\\') - 8));
    p.output_type = lower(base.count("outputtype") ? base["outputtype"] : "library");
    p.framework = base.count("targetframeworkversion") ? base["targetframeworkversion"] : "v2.0";
    for (const XmlNode* items : root.all("ItemGroup")) {
        for (const auto& item : items->children) {
            std::string include = item.attribute("Include");
            if (include.empty()) continue;
            std::string kind = lower(item.name);
            if (kind == "compile" || kind == "content" || kind == "none" || kind == "embeddedresource") {
                if (include.find_first_of("*?") != std::string::npos || include.find("$(") != std::string::npos) continue;
                p.files.push_back(combine(p.directory, to_backslashes(widen(include))));
            } else if (kind == "projectreference") {
                p.unresolved_references.push_back(combine(p.directory, to_backslashes(widen(include))));
            } else if (kind == "reference") {
                p.assembly_references.push_back(include.substr(0, include.find(',')));
            }
        }
    }
}

// Fills configuration details for a project; solution_configuration -> project configuration mapping given.
void configure(Project& p, const std::string& solution_configuration, const std::string& project_configuration, bool build)
{
    ProjectConfiguration pc{ solution_configuration, project_configuration, build, {}, {} };
    if (p.kind != "other" && p.load_error.empty()) {
        std::size_t bar = project_configuration.find('|');
        std::string configuration = project_configuration.substr(0, bar), platform = bar == std::string::npos ? "AnyCPU" : project_configuration.substr(bar + 1);
        XmlNode root = parse_xml(read_text_file(p.path));
        auto props = evaluate(root, configuration, platform);
        pc.platform_target = props.count("platformtarget") ? props["platformtarget"] : "AnyCPU";
        std::string output = props.count("outputpath") ? props["outputpath"] : "bin\\" + configuration + "\\";
        std::string name = props.count("assemblyname") ? props["assemblyname"] : p.assembly_name;
        pc.artifact = combine(p.directory, to_backslashes(widen(output)) + L"\\" + widen(name) + (p.executable() ? L".exe" : L".dll"));
    }
    p.configurations.push_back(pc);
}

std::string upper(std::string s) { for (auto& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c))); return s; }

void load_solution(Workspace& w)
{
    std::istringstream lines(read_text_file(w.path));
    std::string line, section;
    std::vector<std::pair<std::string, std::string>> mapping; // "{GUID}.Debug|Any CPU.ActiveCfg" -> value
    std::string header;
    std::getline(lines, header);
    bool has_header = false;
    for (int i = 0; i < 3 && !has_header; ++i) {
        if (header.find("Microsoft Visual Studio Solution File") != std::string::npos) has_header = true;
        else if (!std::getline(lines, header)) break;
    }
    if (!has_header) throw KernelError(ErrorCode::unsupported, "不是 Visual Studio 解决方案文件。");
    while (std::getline(lines, line)) {
        std::string t = trim(line);
        if (t.rfind("Project(\"", 0) == 0) {
            // Project("{TYPE}") = "Name", "Path", "{GUID}"
            std::vector<std::string> quoted;
            for (std::size_t p = 0; (p = t.find('"', p)) != std::string::npos;) {
                std::size_t e = t.find('"', p + 1);
                if (e == std::string::npos) break;
                quoted.push_back(t.substr(p + 1, e - p - 1));
                p = e + 1;
            }
            if (quoted.size() < 4) continue;
            if (upper(quoted[0]) == "{2150E333-8FDC-42A3-9474-1A3956D46DE8}") continue; // solution folder
            Project p;
            p.type_guid = upper(quoted[0]);
            p.name = quoted[1];
            p.id = upper(quoted[3]);
            p.path = combine(w.root, to_backslashes(widen(quoted[2])));
            w.projects.push_back(p);
        } else if (t.rfind("GlobalSection(", 0) == 0) {
            section = t.substr(14, t.find(')') - 14);
        } else if (t == "EndGlobalSection") {
            section.clear();
        } else if (!section.empty()) {
            std::size_t eq = t.find(" = ");
            if (eq == std::string::npos) continue;
            std::string key = trim(t.substr(0, eq)), value = trim(t.substr(eq + 3));
            if (section == "SolutionConfigurationPlatforms" || section == "SolutionConfiguration") {
                if (std::find(w.configurations.begin(), w.configurations.end(), key) == w.configurations.end()) w.configurations.push_back(key);
            } else if (section == "ProjectConfigurationPlatforms" || section == "ProjectConfiguration") {
                mapping.emplace_back(upper(key.substr(0, key.find('}') + 1)) + key.substr(key.find('}') + 1), value);
            }
        }
    }
    for (auto& p : w.projects) {
        load_project(p);
        for (const auto& sc : w.configurations) {
            std::string active, prefix = p.id + "." + sc;
            bool build = false;
            for (const auto& m : mapping) {
                if (m.first == prefix + ".ActiveCfg") active = m.second;
                if (m.first == prefix + ".Build.0") build = true;
            }
            if (active.empty()) { active = sc; build = true; }
            configure(p, sc, replace_all(active, "Any CPU", "AnyCPU"), build);
        }
    }
}

} // namespace

const ProjectConfiguration* Project::configuration(const std::string& sc) const
{
    for (const auto& c : configurations) if (c.solution_configuration == sc) return &c;
    return nullptr;
}

const Project* Workspace::project(const std::string& project_id) const
{
    for (const auto& p : projects) if (p.id == project_id) return &p;
    return nullptr;
}

std::vector<Tool> discover_tools()
{
    std::vector<Tool> tools;
    std::wstring framework = windows_directory() + L"\\Microsoft.NET\\Framework\\";
    struct Candidate { const char* id; const char* kind; const wchar_t* dir; const char* fx; const wchar_t* exe; };
    const Candidate candidates[] = {
        { "msbuild-2.0", "msbuild", L"v2.0.50727", "v2.0", L"MSBuild.exe" }, { "msbuild-3.5", "msbuild", L"v3.5", "v3.5", L"MSBuild.exe" },
        { "msbuild-4.0", "msbuild", L"v4.0.30319", "v4.0", L"MSBuild.exe" },
        { "csc-2.0", "csc", L"v2.0.50727", "v2.0", L"csc.exe" }, { "vbc-2.0", "vbc", L"v2.0.50727", "v2.0", L"vbc.exe" },
        { "csc-3.5", "csc", L"v3.5", "v3.5", L"csc.exe" }, { "vbc-3.5", "vbc", L"v3.5", "v3.5", L"vbc.exe" },
    };
    for (const auto& c : candidates) {
        std::wstring path = framework + c.dir + L"\\" + c.exe;
        if (!exists(path)) continue;
        tools.push_back({ c.id, c.kind, c.fx, file_version(path), path });
    }
    return tools;
}

const Tool* choose_msbuild(const std::vector<Tool>& tools, const Workspace& w)
{
    // VS2005 projects carry no ToolsVersion and import $(MSBuildBinPath) targets: MSBuild 2.0 matches them.
    std::string wanted = "msbuild-2.0";
    for (const auto& p : w.projects) {
        if (p.tools_version == "3.5" && wanted == "msbuild-2.0") wanted = "msbuild-3.5";
        if (p.tools_version == "4.0" || p.tools_version.rfind("4", 0) == 0 || p.tools_version.rfind("1", 0) == 0) wanted = "msbuild-4.0";
    }
    const char* order[] = { "msbuild-2.0", "msbuild-3.5", "msbuild-4.0" };
    bool reached = false;
    for (const char* id : order) {
        if (id == wanted) reached = true;
        if (!reached) continue;
        for (const auto& t : tools) if (t.id == id) return &t;
    }
    return nullptr;
}

Workspace& WorkspaceService::open(const std::string& utf8_path)
{
    Workspace w;
    try { w.path = full(widen(utf8_path)); } catch (const std::invalid_argument& e) { throw KernelError(ErrorCode::invalid_argument, e.what()); }
    if (!exists(w.path)) throw KernelError(ErrorCode::io_error, "文件不存在：" + narrow(w.path));
    w.root = parent(w.path);
    std::wstring ext = lower(narrow(w.path.substr(w.path.find_last_of(L'.') == std::wstring::npos ? w.path.size() : w.path.find_last_of(L'.')))) == ".sln" ? L".sln" : L"";
    if (!ext.empty()) {
        w.solution = true;
        load_solution(w);
    } else {
        Project p;
        p.path = w.path;
        p.id = "P1";
        std::wstring file = w.path.substr(w.path.find_last_of(L'\\') + 1);
        p.name = narrow(file.substr(0, file.find_last_of(L'.')));
        load_project(p);
        if (p.kind == "other") throw KernelError(ErrorCode::unsupported, "只支持 .sln、.csproj 和 .vbproj。");
        if (!p.load_error.empty()) throw KernelError(ErrorCode::invalid_argument, p.load_error);
        XmlNode root = parse_xml(read_text_file(p.path));
        collect_project_configurations(root, w.configurations);
        if (w.configurations.empty()) w.configurations.push_back("Debug|AnyCPU");
        for (const auto& c : w.configurations) configure(p, c, c, true);
        w.projects.push_back(p);
    }
    if (w.configurations.empty()) w.configurations.push_back("Debug|Any CPU");
    // Resolve project references to workspace projects.
    for (auto& p : w.projects) {
        std::vector<std::wstring> missing;
        for (const auto& ref : p.unresolved_references) {
            auto it = std::find_if(w.projects.begin(), w.projects.end(), [&](const Project& q) {
                return CompareStringOrdinal(q.path.c_str(), -1, ref.c_str(), -1, TRUE) == CSTR_EQUAL; });
            if (it != w.projects.end()) p.project_references.push_back(it->id); else missing.push_back(ref);
        }
        p.unresolved_references = missing;
    }
    for (const auto& p : w.projects) if (p.executable()) { w.startup_project = p.id; break; }
    w.id = next_++;
    return workspaces_.emplace(w.id, std::move(w)).first->second;
}

Workspace& WorkspaceService::get(std::uint64_t id)
{
    auto it = workspaces_.find(id);
    if (it == workspaces_.end()) throw KernelError(ErrorCode::invalid_argument, "工作区不存在或已关闭。");
    return it->second;
}

void WorkspaceService::close(std::uint64_t id) { workspaces_.erase(id); }

} // namespace openmse::core
