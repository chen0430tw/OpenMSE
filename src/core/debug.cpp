#include "debug.hpp"

#include "documents.hpp"
#include "json.hpp"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace openmse::core {

namespace {

std::wstring module_directory()
{
    HMODULE self = nullptr;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCWSTR>(&module_directory), &self);
    wchar_t path[MAX_PATH * 2];
    DWORD n = GetModuleFileNameW(self, path, MAX_PATH * 2);
    std::wstring p(path, n);
    return p.substr(0, p.find_last_of(L'\\'));
}

std::string str(const JsonValue* v) { return v && v->type == JsonValue::Type::string ? v->text : std::string(); }

ErrorCode map_error(const std::string& code)
{
    if (code == "invalid_state") return ErrorCode::invalid_state;
    if (code == "invalid_argument" || code == "not_found" || code == "null_reference") return ErrorCode::invalid_argument;
    if (code == "unsupported" || code == "unsupported_expression" || code == "method_not_supported") return ErrorCode::unsupported;
    if (code == "dependency_missing") return ErrorCode::dependency_missing;
    return ErrorCode::backend_error;
}

} // namespace

std::string pe_debug_bitness(const std::wstring& exe)
{
    std::ifstream f(exe, std::ios::binary);
    auto read = [&](std::streamoff at, void* out, std::size_t n) { f.seekg(at); f.read(static_cast<char*>(out), static_cast<std::streamsize>(n)); return static_cast<bool>(f); };
    IMAGE_DOS_HEADER dos{};
    DWORD signature = 0;
    IMAGE_FILE_HEADER file{};
    WORD magic = 0;
    if (!read(0, &dos, sizeof(dos)) || dos.e_magic != IMAGE_DOS_SIGNATURE || !read(dos.e_lfanew, &signature, 4) || signature != IMAGE_NT_SIGNATURE ||
        !read(dos.e_lfanew + 4, &file, sizeof(file)) || !read(dos.e_lfanew + 4 + sizeof(file), &magic, 2))
        throw KernelError(ErrorCode::unsupported, "不是有效的 Windows 可执行文件：" + narrow(exe));
    std::streamoff optional = dos.e_lfanew + 4 + static_cast<std::streamoff>(sizeof(file));
    IMAGE_DATA_DIRECTORY clr{};
    if (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) read(optional + offsetof(IMAGE_OPTIONAL_HEADER64, DataDirectory) + IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR * sizeof(IMAGE_DATA_DIRECTORY), &clr, sizeof(clr));
    else read(optional + offsetof(IMAGE_OPTIONAL_HEADER32, DataDirectory) + IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR * sizeof(IMAGE_DATA_DIRECTORY), &clr, sizeof(clr));
    if (!clr.VirtualAddress) throw KernelError(ErrorCode::unsupported, "目标不是 .NET 程序，托管调试器无法调试：" + narrow(exe));
    if (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) return "x64";
    // Map the CLR header's RVA to a file offset to read its flags.
    std::streamoff sections = optional + file.SizeOfOptionalHeader;
    for (WORD i = 0; i < file.NumberOfSections; ++i) {
        IMAGE_SECTION_HEADER s{};
        read(sections + i * static_cast<std::streamoff>(sizeof(s)), &s, sizeof(s));
        if (clr.VirtualAddress >= s.VirtualAddress && clr.VirtualAddress < s.VirtualAddress + std::max(s.Misc.VirtualSize, s.SizeOfRawData)) {
            IMAGE_COR20_HEADER header{};
            read(s.PointerToRawData + (clr.VirtualAddress - s.VirtualAddress), &header, sizeof(header));
            bool il_only = (header.Flags & COMIMAGE_FLAGS_ILONLY) != 0, x86_only = (header.Flags & COMIMAGE_FLAGS_32BITREQUIRED) != 0;
            // An IL-only image without the 32-bit flag ("AnyCPU") runs as a 64-bit process on this 64-bit system.
            return il_only && !x86_only ? "x64" : "x86";
        }
    }
    return "x86";
}

struct DebugManager::Session {
    std::uint64_t id = 0;
    std::string state = "starting", reason, adapter, program, last_error, frames = "[]";
    long long exit_code = 0;
    bool has_exit = false, ready = false, adapter_exited = false;
    std::uint32_t pid = 0;
    std::map<int, std::string> breakpoints;     // id -> JSON
    std::vector<std::string> output;
    std::uint64_t modules = 0;
    std::unique_ptr<ChildProcess> process;
    std::uint64_t next_request = 1;
    std::map<std::string, std::string> responses; // request id -> full response line
};

DebugManager::~DebugManager()
{
    std::vector<std::unique_ptr<ChildProcess>> adapters;
    {
        std::lock_guard<std::mutex> hold(lock_);
        for (auto& [id, s] : sessions_) if (s->process) adapters.push_back(std::move(s->process));
    }
    adapters.clear(); // closing stdin makes the adapter terminate its debuggee; the job ends anything left
}

std::shared_ptr<DebugManager::Session> DebugManager::find(std::uint64_t id)
{
    auto it = sessions_.find(id);
    if (it == sessions_.end()) throw KernelError(ErrorCode::invalid_argument, "调试会话不存在。");
    return it->second;
}

std::string DebugManager::json(const Session& s)
{
    std::string j = "{\"id\":" + std::to_string(s.id) + ",\"state\":" + json_quote(s.state) + ",\"reason\":" + json_quote(s.reason)
        + ",\"exit_code\":" + (s.has_exit ? std::to_string(s.exit_code) : std::string("null")) + ",\"pid\":" + std::to_string(s.pid)
        + ",\"adapter\":" + json_quote(s.adapter) + ",\"program\":" + json_quote(s.program) + ",\"modules\":" + std::to_string(s.modules)
        + ",\"last_error\":" + json_quote(s.last_error) + ",\"frames\":" + s.frames + ",\"breakpoints\":[";
    bool first = true;
    for (const auto& [id, b] : s.breakpoints) { j += (first ? "" : ",") + b; first = false; }
    j += "],\"output\":[";
    for (std::size_t i = 0; i < s.output.size(); ++i) j += (i ? "," : "") + json_quote(s.output[i]);
    return j + "]}";
}

void DebugManager::set_state(Session& s, const char* state)
{
    s.state = state;
    changed_.notify_all();
    publish_("debug.state", json(s));
}

void DebugManager::require(const Session& s, const char* state, const char* action)
{
    if (s.state != state)
        throw KernelError(ErrorCode::invalid_state, std::string(action) + "要求调试会话处于 " + state + " 状态（当前 " + s.state + "）。");
}

std::uint64_t DebugManager::start(const Workspace& w, const std::string& configuration, const std::string& project, const std::string& breakpoints_json)
{
    const Project* p = w.project(project.empty() ? w.startup_project : project);
    if (!p) throw KernelError(ErrorCode::invalid_state, "工作区没有可调试的启动项目。");
    if (!p->executable()) throw KernelError(ErrorCode::invalid_state, "项目“" + p->name + "”不是可执行程序。");
    const ProjectConfiguration* pc = p->configuration(configuration);
    if (!pc || pc->artifact.empty()) throw KernelError(ErrorCode::invalid_argument, "项目没有配置：" + configuration);
    if (GetFileAttributesW(pc->artifact.c_str()) == INVALID_FILE_ATTRIBUTES) throw KernelError(ErrorCode::invalid_state, "尚未生成 " + narrow(pc->artifact) + "，请先生成。");
    if (!breakpoints_json.empty()) {
        JsonValue list;
        try { list = parse_json(breakpoints_json); } catch (const JsonError&) { throw KernelError(ErrorCode::invalid_argument, "断点必须是 JSON 数组。"); }
        if (list.type != JsonValue::Type::array) throw KernelError(ErrorCode::invalid_argument, "断点必须是 JSON 数组。");
        for (const auto& b : list.items)
            if (b.type != JsonValue::Type::object || str(b.find("file")).empty() || !b.find("line") || !b.find("line")->integer || b.find("line")->int_value < 1)
                throw KernelError(ErrorCode::invalid_argument, "每个断点需要 file 和从 1 开始的 line。");
    }
    std::string bitness = pe_debug_bitness(pc->artifact);
    std::wstring adapter = module_directory() + L"\\OpenMSE.DebugHost." + widen(bitness) + L".exe";
    if (GetFileAttributesW(adapter.c_str()) == INVALID_FILE_ATTRIBUTES)
        throw KernelError(ErrorCode::dependency_missing, "找不到调试适配器 " + narrow(adapter) + "（运行 build-debughost.cmd）。");

    auto s = std::make_shared<Session>();
    std::unique_lock<std::mutex> hold(lock_);
    s->id = next_++;
    s->adapter = bitness;
    s->program = narrow(pc->artifact);
    sessions_[s->id] = s;
    hold.unlock();
    ProcessOptions o;
    o.executable = adapter;
    o.directory = module_directory();
    o.stdin_pipe = true;
    o.output_codepage = CP_UTF8;
    auto process = std::make_unique<ChildProcess>(o, [this, s](const std::string& line) { on_line(s, line); },
        [this, s](unsigned long code) { on_exit(s, code); });
    hold.lock();
    s->process = std::move(process);
    if (!changed_.wait_for(hold, std::chrono::seconds(10), [&] { return s->ready || s->adapter_exited; }) || !s->ready) {
        set_state(*s, "failed");
        throw KernelError(ErrorCode::host_unavailable, "调试适配器没有启动。");
    }
    std::wstring dir = pc->artifact.substr(0, pc->artifact.find_last_of(L'\\'));
    std::string params = "{\"program\":" + json_quote(narrow(pc->artifact)) + ",\"cwd\":" + json_quote(narrow(dir))
        + ",\"breakpoints\":" + (breakpoints_json.empty() ? "[]" : breakpoints_json) + "}";
    publish_("debug.state", json(*s));
    try {
        std::string result = call(s, "launch", params, 30000, hold);
        JsonValue r = parse_json(result);
        s->pid = r.find("pid") && r.find("pid")->integer ? static_cast<std::uint32_t>(r.find("pid")->int_value) : 0;
        if (s->state == "starting") set_state(*s, "running");
    } catch (const KernelError& e) {
        s->last_error = e.what();
        set_state(*s, "failed");
        throw;
    }
    return s->id;
}

std::string DebugManager::call(const std::shared_ptr<Session>& s, const std::string& method, const std::string& params, std::uint32_t timeout_ms,
    std::unique_lock<std::mutex>& held)
{
    if (!s->process || s->adapter_exited) throw KernelError(ErrorCode::host_exited, "调试适配器已退出。");
    std::string id = std::to_string(s->next_request++);
    if (!s->process->write_line("{\"id\":\"" + id + "\",\"method\":" + json_quote(method) + ",\"params\":" + params + "}"))
        throw KernelError(ErrorCode::host_exited, "无法向调试适配器发送请求。");
    if (!changed_.wait_for(held, std::chrono::milliseconds(timeout_ms), [&] { return s->responses.count(id) || s->adapter_exited; }))
        throw KernelError(ErrorCode::timeout, "调试适配器没有在限定时间内响应 " + method + "。");
    if (!s->responses.count(id)) throw KernelError(ErrorCode::host_exited, "调试适配器已退出。");
    std::string line = s->responses[id];
    s->responses.erase(id);
    JsonValue v = parse_json(line);
    if (const JsonValue* ok = v.find("ok"); ok && ok->boolean) {
        const JsonValue* r = v.find("result");
        return r ? line.substr(r->begin, r->end - r->begin) : "{}";
    }
    const JsonValue* e = v.find("error");
    throw KernelError(map_error(e ? str(e->find("code")) : ""), e ? str(e->find("message")) : "调试适配器返回了无效响应。");
}

void DebugManager::on_line(const std::shared_ptr<Session>& s, const std::string& line)
{
    JsonValue v;
    try { v = parse_json(line); } catch (const JsonError&) { return; }
    std::lock_guard<std::mutex> hold(lock_);
    std::string type = str(v.find("type"));
    if (type == "ready") { s->ready = true; changed_.notify_all(); return; }
    if (type != "event") {
        if (const JsonValue* id = v.find("id"); id && id->type == JsonValue::Type::string) { s->responses[id->text] = line; changed_.notify_all(); }
        return;
    }
    std::string name = str(v.find("event"));
    const JsonValue* body = v.find("body");
    std::string body_json = body ? line.substr(body->begin, body->end - body->begin) : "{}";
    std::string payload = "{\"session\":" + std::to_string(s->id) + ",\"body\":" + body_json + "}";
    if (name == "stopped" && body) {
        s->reason = str(body->find("reason"));
        const JsonValue* frames = body->find("frames");
        s->frames = frames ? line.substr(frames->begin, frames->end - frames->begin) : "[]";
        set_state(*s, "stopped");
        publish_("debug.stopped", payload);
    } else if (name == "exited" && body) {
        const JsonValue* code = body->find("exit_code");
        s->exit_code = code && code->integer ? code->int_value : 0;
        s->has_exit = true;
        s->frames = "[]";
        s->reason = s->state == "terminating" ? "terminated" : "exited";
        set_state(*s, "exited");
        publish_("debug.exited", payload);
        // The session is over: let the adapter exit so nothing keeps the program's files open.
        if (s->process) s->process->close_stdin();
    } else if (name == "output" && body) {
        s->output.push_back(str(body->find("text")));
        if (s->output.size() > 500) s->output.erase(s->output.begin());
        publish_("debug.output", payload);
    } else if (name == "breakpoint" && body) {
        const JsonValue* id = body->find("id");
        if (id && id->integer) s->breakpoints[static_cast<int>(id->int_value)] = body_json;
        publish_("debug.breakpoint", payload);
    } else if (name == "module") {
        ++s->modules;
    } else if (name == "error" && body) {
        s->last_error = "调试器错误 " + str(body->find("hresult"));
    }
}

void DebugManager::on_exit(const std::shared_ptr<Session>& s, unsigned long code)
{
    std::lock_guard<std::mutex> hold(lock_);
    s->adapter_exited = true;
    if (s->state != "exited") {
        s->last_error = "调试适配器意外退出（退出码 " + std::to_string(code) + "）。";
        set_state(*s, "failed");
    }
    changed_.notify_all();
}

std::string DebugManager::snapshot(std::uint64_t session)
{
    std::lock_guard<std::mutex> hold(lock_);
    return json(*find(session));
}

std::string DebugManager::wait(std::uint64_t session, std::uint32_t timeout_ms, bool& settled)
{
    std::unique_lock<std::mutex> hold(lock_);
    auto s = find(session);
    settled = changed_.wait_for(hold, std::chrono::milliseconds(timeout_ms), [&] {
        return s->state == "stopped" || s->state == "exited" || s->state == "failed"; });
    return json(*s);
}

std::string DebugManager::control(std::uint64_t session, const std::string& action)
{
    std::unique_lock<std::mutex> hold(lock_);
    auto s = find(session);
    if (action == "continue" || action == "step_over" || action == "step_into" || action == "step_out") {
        require(*s, "stopped", action == "continue" ? "继续" : "单步");
        // Running before the request: the adapter may report the next stop before its answer arrives.
        s->frames = "[]";
        s->reason.clear();
        set_state(*s, "running");
        try {
            if (action == "continue") call(s, "continue", "{}", 10000, hold);
            else call(s, "step", std::string("{\"kind\":\"") + (action == "step_over" ? "over" : action == "step_into" ? "into" : "out") + "\"}", 10000, hold);
        } catch (...) { if (s->state == "running") set_state(*s, "stopped"); throw; }
    } else if (action == "pause") {
        require(*s, "running", "暂停");
        call(s, "pause", "{}", 10000, hold);
    } else if (action == "stop") {
        if (s->state == "exited" || s->state == "failed") throw KernelError(ErrorCode::invalid_state, "调试会话已结束。");
        set_state(*s, "terminating");
        call(s, "terminate", "{}", 15000, hold);
    } else throw KernelError(ErrorCode::invalid_argument, "未知操作：" + action);
    return json(*s);
}

std::string DebugManager::add_breakpoint(std::uint64_t session, const std::string& file, std::uint32_t line)
{
    std::unique_lock<std::mutex> hold(lock_);
    auto s = find(session);
    if (s->state == "exited" || s->state == "failed") throw KernelError(ErrorCode::invalid_state, "调试会话已结束。");
    if (file.empty() || line == 0) throw KernelError(ErrorCode::invalid_argument, "断点需要文件和从 1 开始的行号。");
    std::string result = call(s, "breakpoint.add", "{\"file\":" + json_quote(file) + ",\"line\":" + std::to_string(line) + "}", 15000, hold);
    JsonValue r = parse_json(result);
    if (const JsonValue* id = r.find("id"); id && id->integer) s->breakpoints[static_cast<int>(id->int_value)] = result;
    return result;
}

std::string DebugManager::remove_breakpoint(std::uint64_t session, std::uint32_t id)
{
    std::unique_lock<std::mutex> hold(lock_);
    auto s = find(session);
    std::string result = call(s, "breakpoint.remove", "{\"id\":" + std::to_string(id) + "}", 10000, hold);
    s->breakpoints.erase(static_cast<int>(id));
    return result;
}

std::string DebugManager::variables(std::uint64_t session, std::uint32_t frame)
{
    std::unique_lock<std::mutex> hold(lock_);
    auto s = find(session);
    require(*s, "stopped", "读取变量");
    return call(s, "variables", "{\"frame\":" + std::to_string(frame) + "}", 10000, hold);
}

std::string DebugManager::evaluate(std::uint64_t session, std::uint32_t frame, const std::string& expression)
{
    std::unique_lock<std::mutex> hold(lock_);
    auto s = find(session);
    require(*s, "stopped", "求值");
    return call(s, "evaluate", "{\"frame\":" + std::to_string(frame) + ",\"expression\":" + json_quote(expression) + "}", 10000, hold);
}

} // namespace openmse::core
