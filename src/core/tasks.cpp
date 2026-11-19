#include "tasks.hpp"

#include "documents.hpp"
#include "json.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <set>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace openmse::core {

namespace {

std::uint64_t now_file_time()
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    return (static_cast<std::uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}
std::uint64_t to_unix_ms(std::uint64_t file_time) { return file_time / 10000 - 11644473600000ull; }

std::uint64_t write_time(const std::wstring& path)
{
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data)) return 0;
    return (static_cast<std::uint64_t>(data.ftLastWriteTime.dwHighDateTime) << 32) | data.ftLastWriteTime.dwLowDateTime;
}

std::string trim(const std::string& s)
{
    std::size_t b = s.find_first_not_of(" \t"), e = s.find_last_not_of(" \t");
    return b == std::string::npos ? std::string() : s.substr(b, e - b + 1);
}

// Parses MSBuild/compiler canonical messages: "origin(line,col): error CODE: text [project]".
bool parse_diagnostic(const std::string& raw, const std::vector<std::wstring>& roots, BuildDiagnostic& d)
{
    std::string line = trim(raw);
    std::size_t at = std::string::npos, keyword = 0;
    for (const char* k : { ": error ", ": warning " }) {
        std::size_t p = line.find(k);
        if (p != std::string::npos && p < at) { at = p; keyword = std::strlen(k); d.is_error = k[2] == 'e'; }
    }
    if (at == std::string::npos) return false;
    std::string origin = trim(line.substr(0, at)), rest = line.substr(at + keyword);
    std::size_t colon = rest.find(": ");
    if (colon != std::string::npos && rest.substr(0, colon).find(' ') == std::string::npos) {
        d.code = rest.substr(0, colon);
        d.message = rest.substr(colon + 2);
    } else d.message = rest;
    if (!d.message.empty() && d.message.back() == ']') {
        std::size_t open = d.message.rfind(" [");
        if (open != std::string::npos) d.message.erase(open);
    }
    d.file = origin;
    if (!origin.empty() && origin.back() == ')') {
        std::size_t open = origin.rfind('(');
        if (open != std::string::npos) {
            std::string numbers = origin.substr(open + 1, origin.size() - open - 2);
            d.file = origin.substr(0, open);
            d.line = static_cast<std::uint32_t>(std::strtoul(numbers.c_str(), nullptr, 10));
            std::size_t comma = numbers.find(',');
            if (comma != std::string::npos) d.column = static_cast<std::uint32_t>(std::strtoul(numbers.c_str() + comma + 1, nullptr, 10));
        }
    }
    if (d.line && !d.file.empty() && !(d.file.size() > 1 && d.file[1] == ':')) {
        for (const auto& root : roots) {
            std::wstring candidate = root + L"\\" + widen(d.file);
            if (GetFileAttributesW(candidate.c_str()) != INVALID_FILE_ATTRIBUTES) { d.file = narrow(candidate); break; }
        }
    }
    return true;
}

void split_configuration(const std::string& c, std::string& configuration, std::string& platform)
{
    std::size_t bar = c.find('|');
    configuration = c.substr(0, bar);
    platform = bar == std::string::npos ? "Any CPU" : c.substr(bar + 1);
}

void collect(const Workspace& w, const std::string& id, std::set<std::string>& out)
{
    if (!out.insert(id).second) return;
    if (const Project* p = w.project(id)) for (const auto& r : p->project_references) collect(w, r, out);
}

} // namespace

const char* task_state_name(TaskState s)
{
    switch (s) {
    case TaskState::running: return "running";
    case TaskState::succeeded: return "succeeded";
    case TaskState::failed: return "failed";
    case TaskState::cancelled: return "cancelled";
    }
    return "failed";
}

std::string TaskManager::json(const TaskInfo& t)
{
    std::string j = "{\"id\":" + std::to_string(t.id) + ",\"kind\":" + json_quote(t.kind) + ",\"state\":" + json_quote(task_state_name(t.state))
        + ",\"exit_code\":" + (t.state == TaskState::running ? std::string("null") : std::to_string(t.exit_code))
        + ",\"workspace\":" + std::to_string(t.workspace) + ",\"project\":" + json_quote(t.project) + ",\"configuration\":" + json_quote(t.configuration)
        + ",\"tool\":" + json_quote(t.tool) + ",\"command\":" + json_quote(narrow(t.command)) + ",\"pid\":" + std::to_string(t.pid)
        + ",\"started_ms\":" + std::to_string(t.started_ms) + ",\"duration_ms\":" + std::to_string(t.duration_ms)
        + ",\"errors\":" + std::to_string(t.errors) + ",\"warnings\":" + std::to_string(t.warnings) + ",\"lines\":" + std::to_string(t.lines) + ",\"diagnostics\":[";
    for (std::size_t i = 0; i < t.diagnostics.size(); ++i) {
        const auto& d = t.diagnostics[i];
        if (i) j += ",";
        j += "{\"file\":" + json_quote(d.file) + ",\"line\":" + std::to_string(d.line) + ",\"column\":" + std::to_string(d.column)
            + ",\"code\":" + json_quote(d.code) + ",\"message\":" + json_quote(d.message) + ",\"is_error\":" + (d.is_error ? "true" : "false") + "}";
    }
    j += "],\"artifacts\":[";
    for (std::size_t i = 0; i < t.artifacts.size(); ++i) j += (i ? "," : "") + json_quote(narrow(t.artifacts[i]));
    j += "],\"tail\":[";
    for (std::size_t i = 0; i < t.tail.size(); ++i) j += (i ? "," : "") + json_quote(t.tail[i]);
    return j + "]}";
}

TaskManager::~TaskManager()
{
    std::vector<std::unique_ptr<ChildProcess>> processes;
    {
        std::lock_guard<std::mutex> hold(lock_);
        for (auto& [id, e] : tasks_) {
            if (e->info.state == TaskState::running) { e->cancel_requested = true; if (e->process) e->process->terminate(1); }
            if (e->process) processes.push_back(std::move(e->process));
        }
    }
    processes.clear(); // joins readers outside the lock; their exit callbacks still take lock_
}

void TaskManager::reap()
{
    std::vector<std::unique_ptr<ChildProcess>> done;
    {
        std::lock_guard<std::mutex> hold(lock_);
        for (auto& [id, e] : tasks_) if (e->info.state != TaskState::running && e->process) done.push_back(std::move(e->process));
        while (tasks_.size() > 100) {
            auto it = std::find_if(tasks_.begin(), tasks_.end(), [](const auto& t) { return t.second->info.state != TaskState::running; });
            if (it == tasks_.end()) break;
            tasks_.erase(it);
        }
    }
}

std::uint64_t TaskManager::launch(std::shared_ptr<Entry> e, ProcessOptions options)
{
    reap();
    std::uint64_t id;
    {
        std::lock_guard<std::mutex> hold(lock_);
        id = e->info.id = next_++;
        e->start_file_time = now_file_time();
        e->info.started_ms = to_unix_ms(e->start_file_time);
        e->info.command = ChildProcess::quote(options.executable);
        for (const auto& a : options.arguments) e->info.command += L" " + ChildProcess::quote(a);
        tasks_[id] = e;
    }
    publish_("task.started", json(e->info));
    std::unique_ptr<ChildProcess> process;
    try {
        process = std::make_unique<ChildProcess>(options,
            [this, e](const std::string& line) { on_line(e, line); },
            [this, e](unsigned long code) { on_exit(e, code); });
    } catch (...) {
        std::lock_guard<std::mutex> hold(lock_);
        tasks_.erase(id);
        throw;
    }
    std::lock_guard<std::mutex> hold(lock_);
    e->info.pid = process->pid();
    e->process = std::move(process);
    return id;
}

std::uint64_t TaskManager::start_build(const Workspace& w, const std::vector<Tool>& tools, const std::string& configuration, const std::string& project)
{
    if (std::find(w.configurations.begin(), w.configurations.end(), configuration) == w.configurations.end())
        throw KernelError(ErrorCode::invalid_argument, "工作区没有配置：" + configuration);
    const Tool* tool = choose_msbuild(tools, w);
    if (!tool) throw KernelError(ErrorCode::dependency_missing, "未找到可用的 MSBuild（需要 .NET Framework 2.0 / 3.5 / 4.x）。");
    auto e = std::make_shared<Entry>();
    e->info.kind = "build";
    e->info.workspace = w.id;
    e->info.project = project;
    e->info.configuration = configuration;
    e->info.tool = tool->id;
    ProcessOptions o;
    o.executable = tool->path;
    o.directory = w.root;
    std::set<std::string> included;
    std::string config, platform;
    if (project.empty()) {
        split_configuration(configuration, config, platform);
        o.arguments.push_back(w.path);
        for (const auto& p : w.projects) {
            const ProjectConfiguration* pc = p.configuration(configuration);
            if (pc && pc->build) included.insert(p.id);
        }
    } else {
        const Project* p = w.project(project);
        if (!p) throw KernelError(ErrorCode::invalid_argument, "项目不存在：" + project);
        const ProjectConfiguration* pc = p->configuration(configuration);
        split_configuration(pc ? pc->project_configuration : configuration, config, platform);
        o.arguments.push_back(p->path);
        collect(w, project, included);
    }
    o.arguments.insert(o.arguments.end(), { L"/t:Build", L"/p:Configuration=" + widen(config), L"/p:Platform=" + widen(platform), L"/nologo" });
    if (tool->id == "msbuild-4.0") o.arguments.push_back(L"/nr:false");
    for (const auto& p : w.projects) {
        e->search_roots.push_back(p.directory);
        const ProjectConfiguration* pc = p.configuration(configuration);
        if (included.count(p.id) && pc && !pc->artifact.empty()) e->expected_artifacts.push_back(pc->artifact);
    }
    e->search_roots.push_back(w.root);
    return launch(e, o);
}

std::uint64_t TaskManager::start_run(const Workspace& w, const std::string& configuration, const std::string& project)
{
    const Project* p = w.project(project.empty() ? w.startup_project : project);
    if (!p) throw KernelError(ErrorCode::invalid_state, "工作区没有可运行的启动项目。");
    if (!p->executable()) throw KernelError(ErrorCode::invalid_state, "项目“" + p->name + "”不是可执行程序。");
    const ProjectConfiguration* pc = p->configuration(configuration);
    if (!pc || pc->artifact.empty()) throw KernelError(ErrorCode::invalid_argument, "项目没有配置：" + configuration);
    if (GetFileAttributesW(pc->artifact.c_str()) == INVALID_FILE_ATTRIBUTES)
        throw KernelError(ErrorCode::invalid_state, "尚未生成 " + narrow(pc->artifact) + "，请先生成。");
    auto e = std::make_shared<Entry>();
    e->info.kind = "run";
    e->info.workspace = w.id;
    e->info.project = p->id;
    e->info.configuration = configuration;
    e->info.tool = "artifact";
    ProcessOptions o;
    o.executable = pc->artifact;
    o.directory = pc->artifact.substr(0, pc->artifact.find_last_of(L'\\'));
    return launch(e, o);
}

void TaskManager::on_line(const std::shared_ptr<Entry>& e, const std::string& line)
{
    std::string payload;
    {
        std::lock_guard<std::mutex> hold(lock_);
        ++e->info.lines;
        e->info.tail.push_back(line);
        if (e->info.tail.size() > 200) e->info.tail.erase(e->info.tail.begin());
        BuildDiagnostic d;
        if (e->info.kind == "build" && parse_diagnostic(line, e->search_roots, d) && !e->seen[trim(line)]) {
            e->seen[trim(line)] = true;
            (d.is_error ? e->info.errors : e->info.warnings)++;
            if (e->info.diagnostics.size() < 1000) e->info.diagnostics.push_back(d);
        }
        payload = "{\"task\":" + std::to_string(e->info.id) + ",\"line\":" + json_quote(line) + "}";
    }
    publish_("task.output", payload);
}

void TaskManager::on_exit(const std::shared_ptr<Entry>& e, unsigned long code)
{
    std::string payload;
    {
        std::lock_guard<std::mutex> hold(lock_);
        e->info.exit_code = static_cast<long long>(code);
        e->info.duration_ms = to_unix_ms(now_file_time()) - e->info.started_ms;
        e->info.state = e->cancel_requested ? TaskState::cancelled : code == 0 ? TaskState::succeeded : TaskState::failed;
        for (const auto& a : e->expected_artifacts) {
            std::uint64_t t = write_time(a);
            if (t && t >= e->start_file_time - 20000000ull) e->info.artifacts.push_back(a); // 2 s tolerance for file-system granularity
        }
        payload = json(e->info);
    }
    changed_.notify_all();
    publish_("task.finished", payload);
}

void TaskManager::cancel(std::uint64_t id)
{
    std::lock_guard<std::mutex> hold(lock_);
    auto it = tasks_.find(id);
    if (it == tasks_.end()) throw KernelError(ErrorCode::invalid_argument, "任务不存在。");
    if (it->second->info.state != TaskState::running) throw KernelError(ErrorCode::invalid_state, "任务已结束。");
    it->second->cancel_requested = true;
    if (it->second->process) it->second->process->terminate(1);
}

TaskInfo TaskManager::snapshot(std::uint64_t id)
{
    std::lock_guard<std::mutex> hold(lock_);
    auto it = tasks_.find(id);
    if (it == tasks_.end()) throw KernelError(ErrorCode::invalid_argument, "任务不存在。");
    return it->second->info;
}

TaskInfo TaskManager::wait(std::uint64_t id, std::uint32_t timeout_ms, bool& finished)
{
    std::unique_lock<std::mutex> hold(lock_);
    auto it = tasks_.find(id);
    if (it == tasks_.end()) throw KernelError(ErrorCode::invalid_argument, "任务不存在。");
    auto entry = it->second;
    finished = changed_.wait_for(hold, std::chrono::milliseconds(timeout_ms), [&] { return entry->info.state != TaskState::running; });
    return entry->info;
}

std::vector<TaskInfo> TaskManager::list()
{
    std::lock_guard<std::mutex> hold(lock_);
    std::vector<TaskInfo> out;
    for (const auto& [id, e] : tasks_) out.push_back(e->info);
    return out;
}

} // namespace openmse::core
