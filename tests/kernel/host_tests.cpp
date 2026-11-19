// Host lifecycle tests against the real legacy VS2005 runtime (phase 4). Starts only its own hidden instances.
// Usage: host-tests.exe <OpenMSE root> <empty test directory under any path>
#include <openmse/kernel/abi.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <thread>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>

namespace {

int failures = 0, passes = 0;
void check(bool ok, const char* name, const std::string& detail = std::string())
{
    if (ok) { ++passes; std::printf("PASS %s\n", name); }
    else { ++failures; std::printf("FAIL %s\n", name); if (!detail.empty()) std::printf("  %s\n", detail.substr(0, 2000).c_str()); }
    std::fflush(stdout);
}

struct Call {
    omk_status status = -1;
    std::string json;
    bool ok() const { return status == OMK_OK; }
    bool has(const std::string& s) const { return json.find(s) != std::string::npos; }
    unsigned long long num(const std::string& key) const
    {
        auto p = json.find("\"" + key + "\":");
        return p == std::string::npos ? ~0ull : std::strtoull(json.c_str() + p + key.size() + 3, nullptr, 10);
    }
};
Call take(omk_status s, omk_buffer& b) { Call c{ s, b.data ? std::string(b.data, b.size) : std::string() }; omk_buffer_free(&b); return c; }

omk_kernel* k = nullptr;
std::wstring wide(const std::string& u)
{
    std::wstring w(static_cast<std::size_t>(MultiByteToWideChar(CP_UTF8, 0, u.data(), static_cast<int>(u.size()), nullptr, 0)), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, u.data(), static_cast<int>(u.size()), w.data(), static_cast<int>(w.size()));
    return w;
}
std::string utf8(const std::wstring& w)
{
    std::string s(static_cast<std::size_t>(WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()), nullptr, 0, nullptr, nullptr)), '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()), s.data(), static_cast<int>(s.size()), nullptr, nullptr);
    return s;
}
void write_bytes(const std::string& path, const std::string& data) { std::ofstream f(wide(path), std::ios::binary | std::ios::trunc); f.write(data.data(), static_cast<std::streamsize>(data.size())); }
std::string read_bytes(const std::string& path) { std::ifstream f(wide(path), std::ios::binary); return std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()); }

Call load(const std::string& manifest, const std::string& root) { omk_buffer b{}; return take(omk_hosts_load(k, manifest.data(), manifest.size(), root.data(), root.size(), &b), b); }
Call start(const std::string& engine, const std::string& dir) { omk_buffer b{}; return take(omk_host_start(k, engine.data(), engine.size(), dir.data(), dir.size(), &b), b); }
Call wait(unsigned long long h, uint32_t ms) { omk_buffer b{}; return take(omk_host_wait(k, h, ms, &b), b); }
Call snap(unsigned long long h) { omk_buffer b{}; return take(omk_host_snapshot(k, h, &b), b); }
Call request(unsigned long long h, const std::string& method, const std::string& params, uint32_t ms)
{ omk_buffer b{}; return take(omk_host_request(k, h, method.data(), method.size(), params.data(), params.size(), ms, &b), b); }
Call sync(unsigned long long h, unsigned long long doc, uint32_t ms) { omk_buffer b{}; return take(omk_host_sync_document(k, h, doc, ms, &b), b); }
Call rwait(unsigned long long h, unsigned long long r, uint32_t ms) { omk_buffer b{}; return take(omk_host_request_wait(k, h, r, ms, &b), b); }
Call verify(unsigned long long h, uint32_t ms) { omk_buffer b{}; return take(omk_host_verify(k, h, ms, &b), b); }
Call reconnect(unsigned long long h) { omk_buffer b{}; return take(omk_host_reconnect(k, h, &b), b); }
Call stop(unsigned long long h, const std::string& mode) { omk_buffer b{}; return take(omk_host_stop(k, h, mode.data(), mode.size(), &b), b); }
Call open(const std::string& path) { omk_buffer b{}; return take(omk_document_open(k, path.data(), path.size(), &b), b); }
Call snapshot(unsigned long long id) { omk_buffer b{}; return take(omk_document_snapshot(k, id, &b), b); }
Call replace_all(unsigned long long id, unsigned long long rev, uint32_t line, const std::string& text)
{ omk_buffer b{}; return take(omk_document_replace(k, id, rev, line, 0, line, 0, text.data(), text.size(), 0, &b), b); }

// Request and wait for completion; returns the request_wait result.
Call call(unsigned long long h, const std::string& method, const std::string& params, uint32_t ms = 30000)
{
    Call r = request(h, method, params, ms);
    if (!r.ok()) return r;
    return rwait(h, r.num("request"), ms + 5000);
}
bool state_is(unsigned long long h, const std::string& s) { return snap(h).has("\"state\":\"" + s + "\""); }
bool process_alive(unsigned long pid)
{
    HANDLE p = OpenProcess(SYNCHRONIZE, FALSE, pid);
    if (!p) return false;
    bool running = WaitForSingleObject(p, 0) == WAIT_TIMEOUT;
    CloseHandle(p);
    return running;
}
std::string normalize(std::string s) { std::string o; for (char c : s) if (c != '\r') o.push_back(c); return o; }

} // namespace

int main()
{
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argc != 3) { std::printf("usage: host-tests <OpenMSE root> <test directory>\n"); return 2; }
    std::string root = utf8(argv[1]), dir = utf8(argv[2]);
    LocalFree(argv);
    CreateDirectoryW(wide(dir).c_str(), nullptr);
    if (dir.back() != '\\') dir += '\\';
    check(omk_create(OMK_ABI_VERSION, &k) == OMK_OK, "kernel created");

    // Manifest: pinned versions/hashes; a tampered entry is refused before anything starts.
    std::string manifest = root + "\\config\\hosts.json";
    std::string text = read_bytes(manifest);
    std::string tampered = text;
    tampered.replace(tampered.find("8.0.50727.867"), 13, "8.0.50727.999");
    write_bytes(dir + "tampered.json", tampered);
    Call bad = load(dir + "tampered.json", root);
    check(bad.ok() && bad.has("\"engine\":\"vs8\"") && bad.has("\"verified\":false") && bad.has(u8"与清单 8.0.50727.999 不一致"), "tampered manifest entry not verified", bad.json);
    check(start("vs8", dir).status == OMK_E_DEPENDENCY_MISSING, "unverified host refused");
    Call good = load(manifest, root);
    check(good.ok() && good.has("\"actual_version\":\"8.0.50727.867\",\"verified\":true"), "real manifest verified", good.json);

    // Start an owned hidden VS2005 and bind to it.
    auto t0 = std::chrono::steady_clock::now();
    Call s = start("vs8", dir);
    auto started_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count();
    unsigned long long h = s.num("id");
    check(s.ok() && (s.has("\"state\":\"starting\"") || s.has("\"state\":\"connecting\"")) && started_ms < 1000, "start returns immediately (asynchronous)", s.json);
    Call w = wait(h, 120000);
    unsigned long pid1 = static_cast<unsigned long>(w.num("pid"));
    check(w.has("\"settled\":true") && w.has("\"state\":\"ready\"") && w.has("\"protocol_version\":2") && pid1 > 0 && process_alive(pid1), "owned VS2005 host ready over bridge protocol v2", w.json);
    if (!w.has("\"state\":\"ready\"")) { omk_destroy(k); std::printf("%d passed, %d failed\n", passes, failures); return 1; }
    Call ping = call(h, "host.ping", "");
    check(ping.has("\"outcome\":\"ok\"") && ping.has("\"executed\":true") && ping.has("\"pid\":" + std::to_string(pid1)), "ping", ping.json);

    // Kernel document -> host buffer, idempotent by content.
    std::string file = dir + u8"宿主文档.txt";
    write_bytes(file, "\xEF\xBB\xBFline1\r\n");
    Call d = open(file);
    unsigned long long doc = d.num("id");
    Call e = replace_all(doc, d.num("revision"), 1, u8"中文\n");
    Call sy = sync(h, doc, 30000);
    Call syw = rwait(h, sy.num("request"), 35000);
    Call rd = call(h, "documents.read", "{\"path\":\"" + std::string() + "\"}");
    std::string path_json;
    for (char c : file) { if (c == '\\') path_json += "\\\\"; else path_json.push_back(c); }
    rd = call(h, "documents.read", "{\"path\":\"" + path_json + "\"}");
    check(syw.has("\"outcome\":\"ok\"") && syw.has("\"applied\":true") && rd.has(u8"line1\\r\\n中文\\r\\n"), "document synced into the host buffer", syw.json + rd.json);
    Call again = rwait(h, sync(h, doc, 30000).num("request"), 35000);
    check(again.has("\"applied\":false") && again.has("\"already\":true"), "re-sending the same document does not write again", again.json);

    // Timeout: the outcome is unknown, writes are refused, nothing is replayed; late answers are only recorded.
    auto t1 = std::chrono::steady_clock::now();
    Call slow = request(h, "host.sleep", "{\"ms\":4000}", 500);
    Call queued = request(h, "documents.read", "{\"path\":\"" + path_json + "\"}", 800);
    auto issue_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count();
    check(slow.ok() && queued.ok() && issue_ms < 200, "requests are issued without blocking the caller");
    Call sw = rwait(h, slow.num("request"), 3000);
    check(sw.has("\"outcome\":\"timeout\"") && sw.has("\"executed\":null") && state_is(h, "needs_verification"), "timeout reports an unknown outcome and requires verification", sw.json + snap(h).json);
    Call refused = sync(h, doc, 5000);
    check(refused.status == OMK_E_INVALID_STATE, "writes refused until verified", refused.json);
    std::this_thread::sleep_for(std::chrono::milliseconds(4500));
    Call late_slow = rwait(h, slow.num("request"), 1000), late_queued = rwait(h, queued.num("request"), 1000);
    check(late_slow.has("\"late\":{") && late_slow.has("\\\"slept\\\":4000") || late_slow.has("\"slept\":4000"), "late answer recorded, not applied", late_slow.json);
    check(late_queued.has("\"outcome\":\"timeout\"") && late_queued.has("deadline_exceeded") && late_queued.has("\"executed\":false"), "request that expired in the bridge queue was not executed", late_queued.json);
    Call v = rwait(h, verify(h, 10000).num("request"), 15000);
    check(v.has("\"outcome\":\"ok\"") && state_is(h, "ready"), "verification restores the ready state", v.json + snap(h).json);

    // Bridge disconnect: the IDE keeps running; reconnect re-checks identity; documents are not lost or written twice.
    Call gone = call(h, "host.exit_bridge", "", 5000);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    check(gone.has("\"outcome\":\"disconnected\"") && state_is(h, "disconnected") && process_alive(pid1), "bridge loss leaves the host disconnected, IDE alive", gone.json + snap(h).json);
    check(snapshot(doc).has(u8"line1\\n中文\\n"), "kernel document unaffected by the disconnect");
    check(sync(h, doc, 5000).status != OMK_OK, "no requests while disconnected");
    Call rc = reconnect(h);
    Call rw = wait(h, 60000);
    check(rc.ok() && rw.has("\"state\":\"ready\"") && rw.num("pid") == pid1 && rw.num("bridge_generation") >= 2, "reconnected to the same verified instance", rw.json);
    again = rwait(h, sync(h, doc, 30000).num("request"), 35000);
    check(again.has("\"already\":true"), "after reconnect the same content is not written again", again.json);
    e = replace_all(doc, snapshot(doc).num("revision"), 2, "line3\n");
    Call more = rwait(h, sync(h, doc, 30000).num("request"), 35000);
    rd = call(h, "documents.read", "{\"path\":\"" + path_json + "\"}");
    check(more.has("\"applied\":true") && rd.has(u8"line1\\r\\n中文\\r\\nline3\\r\\n"), "new kernel edits sync after reconnect", more.json + rd.json);

    // Host process dies: pending work ends as host_exited; the kernel document survives; a new instance is started.
    Call pending = request(h, "host.sleep", "{\"ms\":5000}", 20000);
    Call term = stop(h, "terminate");
    Call ex = wait(h, 20000);
    Call pw = rwait(h, pending.num("request"), 5000);
    check(term.ok() && ex.has("\"state\":\"exited\"") && pw.has("\"outcome\":\"host_exited\"") && !process_alive(pid1), "terminated host: exited state, pending request host_exited", ex.json + pw.json);
    check(request(h, "host.ping", "", 1000).status == OMK_E_HOST_EXITED, "requests to an exited host refused");
    check(snapshot(doc).has(u8"line1\\n中文\\nline3\\n") && snapshot(doc).has("\"dirty\":true"), "kernel document intact after the host died");
    Call s2 = start("vs8", dir);
    unsigned long long h2 = s2.num("id");
    Call w2 = wait(h2, 120000);
    unsigned long pid2 = static_cast<unsigned long>(w2.num("pid"));
    check(w2.has("\"state\":\"ready\"") && pid2 != pid1, "restarted as a new owned instance", w2.json);
    more = rwait(h2, sync(h2, doc, 30000).num("request"), 35000);
    rd = call(h2, "documents.read", "{\"path\":\"" + path_json + "\"}");
    check(more.has("\"applied\":true") && rd.has(u8"line1\\r\\n中文\\r\\nline3\\r\\n"), "document restored into the new host from the kernel", more.json + rd.json);
    again = rwait(h2, sync(h2, doc, 30000).num("request"), 35000);
    check(again.has("\"already\":true"), "no duplicate write into the new host", again.json);

    // Graceful shutdown requires saved host buffers.
    Call saved = call(h2, "documents.save", "{\"path\":\"" + path_json + "\"}");
    Call down = stop(h2, "shutdown");
    Call downw = rwait(h2, down.num("request"), 25000);
    bool exited = false;
    for (int i = 0; i < 300 && !exited; ++i) { exited = state_is(h2, "exited"); if (!exited) std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
    check(saved.has("\"outcome\":\"ok\"") && down.ok() && exited && !process_alive(pid2), "graceful shutdown ends the owned host", saved.json + downw.json + snap(h2).json);
    check(normalize(read_bytes(file)) == "\xEF\xBB\xBF" + std::string(u8"line1\n中文\nline3\n"), "host saved the synced text to disk");

    // MSE is accepted separately: its automation has never been reachable. The kernel must report that as a
    // failed start with the reason, and must not leave the hidden instance it launched behind.
    Call m = start("mse", dir);
    Call mw = wait(m.num("id"), 90000);
    unsigned long mse_pid = static_cast<unsigned long>(mw.num("pid"));
    for (int i = 0; i < 50 && mse_pid && process_alive(mse_pid); ++i) std::this_thread::sleep_for(std::chrono::milliseconds(100)); // termination is asynchronous
    check(mw.has("\"state\":\"failed\"") && mw.has("automation_not_registered") && mse_pid > 0 && !process_alive(mse_pid), "MSE: start fails with the automation reason; owned instance ended", mw.json);
    std::printf("MSE EVIDENCE %s\n", mw.json.c_str());

    omk_destroy(k);
    std::printf("%d passed, %d failed\n", passes, failures);
    return failures == 0 ? 0 : 1;
}
