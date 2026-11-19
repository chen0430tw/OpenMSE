#include "hosts.hpp"

#include "documents.hpp"
#include "json.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iterator>
#include <set>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <bcrypt.h>

#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "user32.lib")

namespace openmse::core {

namespace {

std::uint64_t tick() { return GetTickCount64(); }
std::uint64_t unix_ms()
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    return ((static_cast<std::uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime) / 10000 - 11644473600000ull;
}

std::wstring full_path(const std::wstring& p)
{
    DWORD n = GetFullPathNameW(p.c_str(), 0, nullptr, nullptr);
    std::wstring out(n, L'\0');
    out.resize(GetFullPathNameW(p.c_str(), n, out.data(), nullptr));
    return out;
}

std::wstring under(const std::wstring& root, const std::string& relative)
{
    std::wstring r = widen(relative);
    std::replace(r.begin(), r.end(), L'/', L'\\');
    return full_path(root + L"\\" + r);
}

bool same_path(const std::wstring& a, const std::wstring& b)
{
    return CompareStringOrdinal(a.c_str(), -1, b.c_str(), -1, TRUE) == CSTR_EQUAL;
}

std::uint64_t creation_of(HANDLE process)
{
    FILETIME c{}, e{}, k{}, u{};
    if (!GetProcessTimes(process, &c, &e, &k, &u)) return 0;
    return (static_cast<std::uint64_t>(c.dwHighDateTime) << 32) | c.dwLowDateTime;
}

bool alive(HANDLE process) { return process && WaitForSingleObject(process, 0) == WAIT_TIMEOUT; }

const std::set<std::string>& read_methods()
{
    static const std::set<std::string> s = { "host.identity", "host.ping", "host.inspect", "documents.read", "debug.inspect", "host.sleep" };
    return s;
}

std::string str(const JsonValue* v) { return v && v->type == JsonValue::Type::string ? v->text : std::string(); }

// Text of visible dialog boxes owned by a process: a hidden host blocked on a modal prompt never finishes
// starting, and its prompt is the only explanation available.
std::string dialogs_of(std::uint32_t pid)
{
    struct Search { std::uint32_t pid; std::wstring text; } search{ pid, {} };
    EnumWindows([](HWND window, LPARAM param) -> BOOL {
        auto* s = reinterpret_cast<Search*>(param);
        DWORD owner = 0;
        GetWindowThreadProcessId(window, &owner);
        wchar_t cls[64];
        if (owner != s->pid || !IsWindowVisible(window) || !GetClassNameW(window, cls, 64) || wcscmp(cls, L"#32770") != 0) return TRUE;
        EnumChildWindows(window, [](HWND child, LPARAM p) -> BOOL {
            wchar_t text[1024], kind[64];
            if (GetClassNameW(child, kind, 64) && wcscmp(kind, L"Static") == 0 && GetWindowTextW(child, text, 1024) > 0) {
                auto* t = reinterpret_cast<std::wstring*>(p);
                if (!t->empty()) *t += L" ";
                *t += text;
            }
            return TRUE;
        }, reinterpret_cast<LPARAM>(&s->text));
        return TRUE;
    }, reinterpret_cast<LPARAM>(&search));
    try { return narrow(search.text.substr(0, 400)); } catch (...) { return {}; }
}

} // namespace

std::string sha256_hex(const std::wstring& path)
{
    std::ifstream f(path, std::ios::binary);
    if (!f) return {};
    BCRYPT_ALG_HANDLE alg = nullptr;
    BCRYPT_HASH_HANDLE hash = nullptr;
    if (BCryptOpenAlgorithmProvider(&alg, BCRYPT_SHA256_ALGORITHM, nullptr, 0) != 0) return {};
    std::string out;
    if (BCryptCreateHash(alg, &hash, nullptr, 0, nullptr, 0, 0) == 0) {
        std::vector<char> buffer(1 << 16);
        while (f) {
            f.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
            if (f.gcount() > 0) BCryptHashData(hash, reinterpret_cast<PUCHAR>(buffer.data()), static_cast<ULONG>(f.gcount()), 0);
        }
        unsigned char digest[32];
        if (BCryptFinishHash(hash, digest, sizeof(digest), 0) == 0) {
            static const char hex[] = "0123456789ABCDEF";
            for (unsigned char c : digest) { out.push_back(hex[c >> 4]); out.push_back(hex[c & 15]); }
        }
        BCryptDestroyHash(hash);
    }
    BCryptCloseAlgorithmProvider(alg, 0);
    return out;
}

struct HostManager::Host {
    std::uint64_t id = 0;
    HostSpec spec;
    std::wstring workspace;
    std::string state = "starting", last_error;
    std::uint32_t pid = 0, bridge_pid = 0;
    std::uint64_t created = 0;
    HANDLE process = nullptr, stop_event = nullptr;
    std::unique_ptr<ChildProcess> bridge;
    std::uint64_t generation = 0;
    bool bridge_ready = false, bridge_exited = false;
    int protocol = 0;
    std::map<std::uint64_t, HostRequest> requests;
    std::uint64_t next_request = 1;
    std::map<std::uint64_t, std::string> synced; // document id -> text last confirmed in the host buffer
    std::thread watcher;
};

HostManager::HostManager(Publisher publish) : publish_(std::move(publish))
{
    watchdog_ = std::thread([this] { watchdog(); });
}

HostManager::~HostManager()
{
    std::vector<std::unique_ptr<ChildProcess>> bridges;
    std::vector<std::shared_ptr<Host>> hosts;
    {
        std::lock_guard<std::mutex> hold(lock_);
        closing_ = true;
        for (auto& [id, h] : hosts_) {
            hosts.push_back(h);
            if (h->stop_event) SetEvent(h->stop_event);
            // Owned instances are hidden test/back-end hosts; the kernel is the document authority, so they are ended.
            if (h->state != "stopped" && alive(h->process)) TerminateProcess(h->process, 1);
            if (h->bridge) bridges.push_back(std::move(h->bridge));
        }
    }
    changed_.notify_all();
    bridges.clear();
    if (watchdog_.joinable()) watchdog_.join();
    for (auto& w : workers_) if (w.joinable()) w.join();
    for (auto& h : hosts) {
        if (h->watcher.joinable()) h->watcher.join();
        if (h->process) CloseHandle(h->process);
        if (h->stop_event) CloseHandle(h->stop_event);
    }
}

std::string HostManager::load(const std::string& manifest, const std::string& root)
{
    std::wstring manifest_path = full_path(widen(manifest));
    std::ifstream f(manifest_path, std::ios::binary);
    if (!f) throw KernelError(ErrorCode::io_error, "无法读取宿主清单：" + manifest);
    std::string text((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    if (text.compare(0, 3, "\xEF\xBB\xBF") == 0) text.erase(0, 3);
    JsonValue doc;
    try { doc = parse_json(text); } catch (const JsonError& e) { throw KernelError(ErrorCode::invalid_argument, std::string("宿主清单 JSON 无效：") + e.what()); }
    const JsonValue* list = doc.find("hosts");
    if (!list || list->type != JsonValue::Type::array) throw KernelError(ErrorCode::invalid_argument, "宿主清单缺少 hosts 数组。");
    std::wstring root_path = full_path(widen(root));
    std::vector<HostSpec> specs;
    for (const auto& h : list->items) {
        HostSpec s;
        s.engine = str(h.find("engine"));
        s.name = str(h.find("name"));
        s.file_version = str(h.find("fileVersion"));
        s.sha256 = str(h.find("sha256"));
        if (s.engine.empty() || str(h.find("executable")).empty() || str(h.find("launcher")).empty() || str(h.find("bridge")).empty())
            throw KernelError(ErrorCode::invalid_argument, "宿主清单条目缺少 engine / executable / launcher / bridge。");
        s.executable = under(root_path, str(h.find("executable")));
        s.launcher = under(root_path, str(h.find("launcher")));
        s.process_file = under(root_path, str(h.find("processFile")));
        s.working_directory = under(root_path, str(h.find("workingDirectory")).empty() ? "." : str(h.find("workingDirectory")));
        s.bridge = under(root_path, str(h.find("bridge")));
        if (const JsonValue* a = h.find("launcherArguments")) for (const auto& v : a->items) s.launcher_arguments.push_back(widen(v.text));
        if (const JsonValue* t = h.find("startTimeoutMs"); t && t->integer) s.start_timeout_ms = static_cast<std::uint32_t>(t->int_value);
        if (const JsonValue* c = h.find("capabilities")) s.capabilities_json = text.substr(c->begin, c->end - c->begin); else s.capabilities_json = "{}";
        // The manifest pins the exact legacy binaries; anything else is refused before it is started.
        if (GetFileAttributesW(s.executable.c_str()) == INVALID_FILE_ATTRIBUTES) s.verify_error = "找不到宿主程序：" + narrow(s.executable);
        else {
            s.actual_version = file_version(s.executable);
            s.actual_sha256 = sha256_hex(s.executable);
            if (!s.file_version.empty() && s.actual_version != s.file_version) s.verify_error = "文件版本 " + s.actual_version + " 与清单 " + s.file_version + " 不一致。";
            else if (!s.sha256.empty() && _stricmp(s.actual_sha256.c_str(), s.sha256.c_str()) != 0) s.verify_error = "SHA-256 与清单不一致。";
            else if (GetFileAttributesW(s.launcher.c_str()) == INVALID_FILE_ATTRIBUTES) s.verify_error = "找不到启动器：" + narrow(s.launcher);
            else if (GetFileAttributesW(s.bridge.c_str()) == INVALID_FILE_ATTRIBUTES) s.verify_error = "找不到桥接程序：" + narrow(s.bridge) + "（运行 scripts/build-bridge.ps1）";
        }
        s.verified = s.verify_error.empty();
        specs.push_back(std::move(s));
    }
    std::string out = "{\"hosts\":[";
    {
        std::lock_guard<std::mutex> hold(lock_);
        specs_ = specs;
        root_ = root_path;
    }
    for (std::size_t i = 0; i < specs.size(); ++i) {
        const auto& s = specs[i];
        out += std::string(i ? "," : "") + "{\"engine\":" + json_quote(s.engine) + ",\"name\":" + json_quote(s.name) + ",\"executable\":" + json_quote(narrow(s.executable))
            + ",\"file_version\":" + json_quote(s.file_version) + ",\"actual_version\":" + json_quote(s.actual_version) + ",\"verified\":" + (s.verified ? "true" : "false")
            + ",\"verify_error\":" + json_quote(s.verify_error) + ",\"capabilities\":" + s.capabilities_json + "}";
    }
    return out + "]}";
}

std::shared_ptr<HostManager::Host> HostManager::find(std::uint64_t id)
{
    auto it = hosts_.find(id);
    if (it == hosts_.end()) throw KernelError(ErrorCode::invalid_argument, "宿主不存在。");
    return it->second;
}

std::string HostManager::request_json(const HostRequest& r)
{
    std::string j = "{\"id\":" + std::to_string(r.id) + ",\"method\":" + json_quote(r.method) + ",\"done\":" + (r.done ? "true" : "false")
        + ",\"outcome\":" + json_quote(r.outcome) + ",\"executed\":" + (r.executed < 0 ? "null" : r.executed ? "true" : "false")
        + ",\"result\":" + r.result_json + ",\"error\":";
    j += r.error_code.empty() ? "null" : "{\"code\":" + json_quote(r.error_code) + ",\"message\":" + json_quote(r.error_message) + "}";
    j += ",\"late\":" + std::string(r.late ? r.late_json : "null") + "}";
    return j;
}

std::string HostManager::host_json(const Host& h)
{
    std::size_t pending = 0;
    for (const auto& [id, r] : h.requests) pending += !r.done;
    return "{\"id\":" + std::to_string(h.id) + ",\"engine\":" + json_quote(h.spec.engine) + ",\"state\":" + json_quote(h.state)
        + ",\"pid\":" + std::to_string(h.pid) + ",\"created_file_time\":" + json_quote(std::to_string(h.created))
        + ",\"bridge_pid\":" + std::to_string(h.bridge_pid) + ",\"bridge_generation\":" + std::to_string(h.generation)
        + ",\"protocol_version\":" + std::to_string(h.protocol) + ",\"pending\":" + std::to_string(pending)
        + ",\"workspace\":" + json_quote(narrow(h.workspace)) + ",\"last_error\":" + json_quote(h.last_error) + "}";
}

void HostManager::set_state(Host& h, const char* state, const std::string& error)
{
    h.state = state;
    if (!error.empty()) h.last_error = error;
    changed_.notify_all();
    publish_("host.state", host_json(h));
}

std::uint64_t HostManager::start(const std::string& engine, const std::string& workspace)
{
    std::lock_guard<std::mutex> hold(lock_);
    auto spec = std::find_if(specs_.begin(), specs_.end(), [&](const HostSpec& s) { return s.engine == engine; });
    if (spec == specs_.end()) throw KernelError(ErrorCode::invalid_argument, "宿主清单中没有引擎：" + engine);
    if (!spec->verified) throw KernelError(ErrorCode::dependency_missing, spec->verify_error);
    for (const auto& [id, h] : hosts_)
        if (h->spec.engine == engine && (h->state == "starting" || h->state == "connecting"))
            throw KernelError(ErrorCode::host_busy, "该引擎的宿主正在启动。");
    auto h = std::make_shared<Host>();
    h->id = next_++;
    h->spec = *spec;
    h->workspace = full_path(widen(workspace));
    if (GetFileAttributesW(h->workspace.c_str()) == INVALID_FILE_ATTRIBUTES) throw KernelError(ErrorCode::invalid_argument, "宿主工作目录不存在：" + workspace);
    hosts_[h->id] = h;
    publish_("host.state", host_json(*h));
    workers_.emplace_back([this, h] { run_start(h, true); });
    return h->id;
}

void HostManager::run_start(std::shared_ptr<Host> h, bool launch)
{
    try {
        if (launch) {
            // The launcher writes one shared PID file per engine, so launches are serialised by start().
            FILETIME before_ft;
            GetSystemTimeAsFileTime(&before_ft);
            std::uint64_t before = (static_cast<std::uint64_t>(before_ft.dwHighDateTime) << 32) | before_ft.dwLowDateTime;
            bool exited = false;
            unsigned long code = 1;
            {
                ProcessOptions o;
                o.executable = h->spec.launcher;
                o.arguments = h->spec.launcher_arguments;
                o.directory = h->spec.working_directory;
                o.own_process_tree = false; // the IDE it starts must outlive the launcher
                ChildProcess launcher(o, nullptr, [&](unsigned long c) { std::lock_guard<std::mutex> hold(lock_); code = c; exited = true; changed_.notify_all(); });
                std::unique_lock<std::mutex> hold(lock_);
                if (!changed_.wait_for(hold, std::chrono::milliseconds(h->spec.start_timeout_ms), [&] { return exited || closing_; }))
                    throw KernelError(ErrorCode::timeout, "启动器在限定时间内没有返回。");
                if (closing_) return;
            }
            if (code != 0) throw KernelError(ErrorCode::backend_error, "启动器退出码 " + std::to_string(code) + "。");
            std::ifstream pid_file(h->spec.process_file);
            unsigned long pid = 0;
            if (!(pid_file >> pid) || pid == 0) throw KernelError(ErrorCode::backend_error, "启动器没有写出进程 ID：" + narrow(h->spec.process_file));
            HANDLE process = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_TERMINATE, FALSE, pid);
            if (!process) throw KernelError(ErrorCode::host_exited, "宿主进程 " + std::to_string(pid) + " 已不存在。");
            wchar_t image[MAX_PATH * 2];
            DWORD size = MAX_PATH * 2;
            std::uint64_t created = creation_of(process);
            bool ours = QueryFullProcessImageNameW(process, 0, image, &size) && same_path(image, h->spec.executable) && created + 20000000ull >= before;
            if (!ours) { CloseHandle(process); throw KernelError(ErrorCode::protocol_error, "进程 " + std::to_string(pid) + " 不是本次启动的宿主实例，拒绝绑定。"); }
            CreateDirectoryW((root_ + L"\\logs").c_str(), nullptr);
            CreateDirectoryW((root_ + L"\\logs\\hosts").c_str(), nullptr);
            std::wstring marker = root_ + L"\\logs\\hosts\\" + widen(h->spec.engine) + L"-" + std::to_wstring(pid) + L".json";
            {
                std::ofstream m(marker, std::ios::binary | std::ios::trunc);
                m << "{\"engine\":" << json_quote(h->spec.engine) << ",\"pid\":" << pid << ",\"createdFileTime\":\"" << created << "\"}";
            }
            {
                std::lock_guard<std::mutex> hold(lock_);
                h->pid = pid;
                h->created = created;
                h->process = process;
                h->stop_event = CreateEventW(nullptr, TRUE, FALSE, nullptr);
            }
            h->watcher = std::thread([this, h] {
                HANDLE handles[2] = { h->process, h->stop_event };
                if (WaitForMultipleObjects(2, handles, FALSE, INFINITE) == WAIT_OBJECT_0) on_process_exit(h);
            });
        }
        {
            std::lock_guard<std::mutex> hold(lock_);
            set_state(*h, "connecting");
        }
        if (!connect_bridge(h, tick() + h->spec.start_timeout_ms)) {
            std::lock_guard<std::mutex> hold(lock_);
            if (closing_ || h->state == "exited") return;
            std::string dialog = dialogs_of(h->pid);
            throw KernelError(ErrorCode::host_unavailable, "无法通过桥接连接宿主：" + (h->last_error.empty() ? std::string("超时") : h->last_error)
                + (dialog.empty() ? std::string() : "；宿主正显示对话框：" + dialog));
        }
    } catch (const std::exception& e) {
        std::vector<std::unique_ptr<ChildProcess>> dead;
        {
            std::lock_guard<std::mutex> hold(lock_);
            if (h->bridge) dead.push_back(std::move(h->bridge));
            // A hidden instance we launched but could not bind would otherwise leak.
            if (launch && alive(h->process)) TerminateProcess(h->process, 1);
            set_state(*h, "failed", e.what());
        }
    }
}

bool HostManager::connect_bridge(const std::shared_ptr<Host>& h, std::uint64_t deadline)
{
    while (true) {
        std::unique_ptr<ChildProcess> old;
        std::uint64_t generation;
        {
            std::lock_guard<std::mutex> hold(lock_);
            if (closing_ || h->state == "exited") return false;
            generation = ++h->generation;
            h->bridge_ready = h->bridge_exited = false;
            old = std::move(h->bridge);
        }
        old.reset();
        std::wstring marker = root_ + L"\\logs\\hosts\\" + widen(h->spec.engine) + L"-" + std::to_wstring(h->pid) + L".json";
        ProcessOptions o;
        o.executable = h->spec.bridge;
        o.arguments = { std::to_wstring(h->pid), widen(h->spec.engine), root_, h->workspace, marker, L"--stdio" };
        o.directory = root_;
        o.stdin_pipe = true;
        o.output_codepage = CP_UTF8;
        auto bridge = std::make_unique<ChildProcess>(o,
            [this, h, generation](const std::string& line) { on_bridge_line(h, generation, line); },
            [this, h, generation](unsigned long code) { on_bridge_exit(h, generation, code); });
        std::unique_lock<std::mutex> hold(lock_);
        h->bridge = std::move(bridge);
        std::uint64_t now = tick();
        changed_.wait_for(hold, std::chrono::milliseconds(deadline > now ? deadline - now : 0), [&] {
            return closing_ || h->generation != generation || h->bridge_ready || h->bridge_exited || h->state == "exited"; });
        if (h->bridge_ready && h->generation == generation) return true;
        if (closing_ || h->state == "exited" || tick() >= deadline) return false;
        // The IDE registers its automation object some time after its window appears: retry.
        changed_.wait_for(hold, std::chrono::milliseconds(1000), [&] { return closing_; });
        if (closing_ || tick() >= deadline) return false;
    }
}

void HostManager::on_bridge_line(const std::shared_ptr<Host>& h, std::uint64_t generation, const std::string& line)
{
    JsonValue v;
    bool parsed = true;
    try { v = parse_json(line); } catch (const JsonError&) { parsed = false; }
    std::lock_guard<std::mutex> hold(lock_);
    if (generation != h->generation) return; // answer from a bridge that was replaced
    if (!parsed || v.type != JsonValue::Type::object) {
        // Unhandled-exception text or other diagnostics on the bridge's stderr: keep it for the error report.
        if (!line.empty()) h->last_error = "桥接输出：" + line.substr(0, 500);
        return;
    }
    if (str(v.find("type")) == "ready") {
        const JsonValue* version = v.find("protocolVersion");
        const JsonValue* pid = v.find("pid");
        h->protocol = version && version->integer ? static_cast<int>(version->int_value) : 0;
        h->bridge_pid = v.find("bridgePid") && v.find("bridgePid")->integer ? static_cast<std::uint32_t>(v.find("bridgePid")->int_value) : 0;
        if (h->protocol < 2) { h->last_error = "桥接协议版本 " + std::to_string(h->protocol) + " 过旧，需要 2。"; return; }
        if (!pid || !pid->integer || static_cast<std::uint32_t>(pid->int_value) != h->pid || str(v.find("createdFileTime")) != std::to_string(h->created)) {
            set_state(*h, "failed", "桥接报告的宿主身份与本次启动的实例不一致。");
            return;
        }
        h->bridge_ready = true;
        set_state(*h, "ready");
        return;
    }
    const JsonValue* idv = v.find("id");
    const JsonValue* error = v.find("error");
    if (!idv || idv->type != JsonValue::Type::string) {
        if (error) h->last_error = str(error->find("code")) + ": " + str(error->find("message"));
        return;
    }
    std::uint64_t id = std::strtoull(idv->text.c_str(), nullptr, 10);
    auto it = h->requests.find(id);
    if (it == h->requests.end()) return;
    HostRequest& r = it->second;
    const JsonValue* executed = v.find("executed");
    if (r.done) {
        // Too late: the caller was already told the outcome is unknown. Record it, never apply it.
        r.late = true;
        r.late_json = line;
        publish_("host.late_response", "{\"host\":" + std::to_string(h->id) + ",\"request\":" + request_json(r) + "}");
        return;
    }
    r.executed = executed && executed->type == JsonValue::Type::boolean ? executed->boolean : -1;
    std::vector<std::string> events;
    if (const JsonValue* ok = v.find("ok"); ok && ok->boolean) {
        const JsonValue* result = v.find("result");
        r.result_json = result ? line.substr(result->begin, result->end - result->begin) : "null";
        if (r.verification) {
            bool same = result && result->find("pid") && result->find("pid")->int_value == h->pid && str(result->find("createdFileTime")) == std::to_string(h->created);
            if (same) { if (h->state == "needs_verification") set_state(*h, "ready"); }
            else set_state(*h, "failed", "核对失败：宿主身份已改变。");
        }
        if (r.document) h->synced[r.document] = r.synced_text;
        finish_locked(*h, r, "ok", events);
    } else {
        r.error_code = error ? str(error->find("code")) : "protocol_error";
        r.error_message = error ? str(error->find("message")) : "无效响应";
        finish_locked(*h, r, r.error_code == "deadline_exceeded" ? "not_executed" : "error", events);
    }
}

void HostManager::finish_locked(Host& h, HostRequest& r, const std::string& outcome, std::vector<std::string>&)
{
    r.done = true;
    r.outcome = outcome;
    if (outcome == "not_executed") r.executed = 0;
    changed_.notify_all();
    publish_("host.response", "{\"host\":" + std::to_string(h.id) + ",\"request\":" + request_json(r) + "}");
}

void HostManager::on_bridge_exit(const std::shared_ptr<Host>& h, std::uint64_t generation, unsigned long code)
{
    std::lock_guard<std::mutex> hold(lock_);
    if (generation != h->generation) return;
    h->bridge_exited = true;
    h->bridge_ready = false;
    std::vector<std::string> events;
    for (auto& [id, r] : h->requests) if (!r.done) finish_locked(*h, r, "disconnected", events); // outcome unknown
    if (h->state == "ready" || h->state == "needs_verification")
        set_state(*h, "disconnected", "桥接进程已退出（退出码 " + std::to_string(code) + "）。" + (h->last_error.rfind("桥接输出", 0) == 0 ? h->last_error : std::string()));
    changed_.notify_all();
}

void HostManager::on_process_exit(const std::shared_ptr<Host>& h)
{
    std::unique_ptr<ChildProcess> bridge;
    {
        std::lock_guard<std::mutex> hold(lock_);
        std::vector<std::string> events;
        ++h->generation; // ignore whatever the old bridge still reports
        for (auto& [id, r] : h->requests) if (!r.done) finish_locked(*h, r, "host_exited", events);
        bridge = std::move(h->bridge);
        h->bridge_ready = false;
        set_state(*h, "exited");
    }
}

std::uint64_t HostManager::send(const std::shared_ptr<Host>& h, const std::string& method, const std::string& params, std::uint32_t timeout_ms,
    bool verification, std::uint64_t document, const std::string& synced)
{
    // Called with lock_ held.
    bool read = read_methods().count(method) != 0 || verification;
    if (h->state == "needs_verification" && !read)
        throw KernelError(ErrorCode::invalid_state, "宿主有结果未知的请求，请先核对（verify）后再写入；不会自动重放。");
    if (h->state != "ready" && h->state != "needs_verification")
        throw KernelError(h->state == "exited" ? ErrorCode::host_exited : ErrorCode::host_unavailable, "宿主当前状态为 " + h->state + "。");
    if (!h->bridge) throw KernelError(ErrorCode::host_unavailable, "桥接未连接。");
    HostRequest r;
    r.id = h->next_request++;
    r.method = method;
    r.verification = verification;
    r.document = document;
    r.synced_text = synced;
    r.deadline_ms = tick() + timeout_ms;
    std::uint64_t id = r.id;
    h->requests[id] = r;
    std::string line = "{\"id\":\"" + std::to_string(id) + "\",\"method\":" + json_quote(method) + ",\"params\":" + (params.empty() ? "{}" : params)
        + ",\"deadlineUnixMs\":" + std::to_string(unix_ms() + timeout_ms) + "}";
    if (line.size() > 1024 * 1024) { h->requests.erase(id); throw KernelError(ErrorCode::invalid_argument, "请求超过 1 MiB 上限。"); }
    if (!h->bridge->write_line(line)) {
        std::vector<std::string> events;
        h->requests[id].executed = 0;
        finish_locked(*h, h->requests[id], "disconnected", events);
    }
    while (h->requests.size() > 500) {
        auto done = std::find_if(h->requests.begin(), h->requests.end(), [](const auto& p) { return p.second.done; });
        if (done == h->requests.end()) break;
        h->requests.erase(done);
    }
    return id;
}

std::uint64_t HostManager::request(std::uint64_t host, const std::string& method, const std::string& params, std::uint32_t timeout_ms)
{
    if (!params.empty()) {
        try { if (parse_json(params).type != JsonValue::Type::object) throw JsonError("params"); }
        catch (const JsonError&) { throw KernelError(ErrorCode::invalid_argument, "params 必须是 JSON 对象。"); }
    }
    if (method == "host.shutdown") throw KernelError(ErrorCode::invalid_argument, "请使用 stop(\"shutdown\") 关闭宿主。");
    std::lock_guard<std::mutex> hold(lock_);
    return send(find(host), method, params, timeout_ms, false, 0, std::string());
}

std::uint64_t HostManager::sync_document(std::uint64_t host, std::uint64_t document, const std::string& path, const std::string& text, std::uint32_t timeout_ms)
{
    std::lock_guard<std::mutex> hold(lock_);
    auto h = find(host);
    std::string params = "{\"path\":" + json_quote(path) + ",\"text\":" + json_quote(text);
    auto base = h->synced.find(document);
    if (base != h->synced.end()) params += ",\"baseText\":" + json_quote(base->second);
    return send(h, "documents.sync", params + "}", timeout_ms, false, document, text);
}

std::string HostManager::wait_request(std::uint64_t host, std::uint64_t request, std::uint32_t timeout_ms, bool& finished)
{
    std::unique_lock<std::mutex> hold(lock_);
    auto h = find(host);
    auto it = h->requests.find(request);
    if (it == h->requests.end()) throw KernelError(ErrorCode::invalid_argument, "请求不存在。");
    finished = changed_.wait_for(hold, std::chrono::milliseconds(timeout_ms), [&] {
        auto r = h->requests.find(request);
        return r == h->requests.end() || r->second.done; });
    it = h->requests.find(request);
    return it == h->requests.end() ? std::string("null") : request_json(it->second);
}

std::uint64_t HostManager::verify(std::uint64_t host, std::uint32_t timeout_ms)
{
    std::lock_guard<std::mutex> hold(lock_);
    auto h = find(host);
    if (!alive(h->process) || creation_of(h->process) != h->created) throw KernelError(ErrorCode::host_exited, "宿主进程已结束，无法核对。");
    return send(h, "host.identity", std::string(), timeout_ms, true, 0, std::string());
}

void HostManager::reconnect(std::uint64_t host)
{
    std::lock_guard<std::mutex> hold(lock_);
    auto h = find(host);
    if (h->state != "disconnected") throw KernelError(ErrorCode::invalid_state, "只有断开连接的宿主可以重新连接（当前 " + h->state + "）。");
    if (!alive(h->process) || creation_of(h->process) != h->created) throw KernelError(ErrorCode::host_exited, "宿主进程已结束。");
    // Synced texts are kept: the bridge compares them with the live buffer, so edits made in the host while
    // disconnected still surface as a conflict instead of being overwritten.
    set_state(*h, "connecting");
    workers_.emplace_back([this, h] { run_start(h, false); });
}

std::uint64_t HostManager::stop(std::uint64_t host, const std::string& mode)
{
    std::unique_ptr<ChildProcess> bridge;
    std::lock_guard<std::mutex> hold(lock_);
    auto h = find(host);
    if (mode == "terminate") {
        if (!alive(h->process)) throw KernelError(ErrorCode::host_exited, "宿主进程已结束。");
        // Only ever the instance this kernel launched: the handle has been held since its identity was checked.
        TerminateProcess(h->process, 1);
        set_state(*h, "stopping");
    } else if (mode == "detach") {
        ++h->generation;
        bridge = std::move(h->bridge);
        if (h->stop_event) SetEvent(h->stop_event);
        set_state(*h, "stopped");
    } else if (mode == "shutdown") {
        // The host may refuse (unsaved buffers, active debugging); only the process exit changes the state.
        return send(h, "host.shutdown", std::string(), 20000, false, 0, std::string());
    } else throw KernelError(ErrorCode::invalid_argument, "mode 必须是 detach、shutdown 或 terminate。");
    return 0;
}

std::string HostManager::snapshot(std::uint64_t host)
{
    std::lock_guard<std::mutex> hold(lock_);
    return host_json(*find(host));
}

std::string HostManager::list()
{
    std::lock_guard<std::mutex> hold(lock_);
    std::string out = "{\"hosts\":[";
    bool first = true;
    for (const auto& [id, h] : hosts_) { out += (first ? "" : ",") + host_json(*h); first = false; }
    return out + "]}";
}

std::string HostManager::wait(std::uint64_t host, std::uint32_t timeout_ms, bool& settled)
{
    std::unique_lock<std::mutex> hold(lock_);
    auto h = find(host);
    settled = changed_.wait_for(hold, std::chrono::milliseconds(timeout_ms), [&] {
        return h->state != "starting" && h->state != "connecting" && h->state != "stopping"; });
    return host_json(*h);
}

void HostManager::watchdog()
{
    std::unique_lock<std::mutex> hold(lock_);
    while (!closing_) {
        changed_.wait_for(hold, std::chrono::milliseconds(50));
        std::uint64_t now = tick();
        std::vector<std::string> events;
        for (auto& [hid, h] : hosts_) {
            bool expired = false;
            for (auto& [rid, r] : h->requests) {
                if (r.done || now < r.deadline_ms) continue;
                // The bridge may still be executing it: the outcome is unknown, not "failed".
                finish_locked(*h, r, "timeout", events);
                expired = true;
            }
            if (expired && h->state == "ready") set_state(*h, "needs_verification", "请求超时，结果未知。");
        }
    }
}

} // namespace openmse::core
