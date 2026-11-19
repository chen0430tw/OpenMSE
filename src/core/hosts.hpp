#pragma once
// HostManager: lifecycle of legacy hosts (VS2005 / MSE) described by config/hosts.json.
// Only instances started by this kernel are ever bound (PID + creation time, process handle held).
// Requests are asynchronous with deadlines; an unanswered request leaves the host "needs_verification",
// which refuses writes until identity is re-confirmed, so nothing is replayed blindly.
#include "process.hpp"

#include <condition_variable>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace openmse::core {

struct HostSpec {
    std::string engine, name, file_version, sha256, capabilities_json;
    std::wstring executable, launcher, process_file, working_directory, bridge;
    std::vector<std::wstring> launcher_arguments;
    std::uint32_t start_timeout_ms = 60000;
    bool verified = false;
    std::string actual_version, actual_sha256, verify_error;
};

struct HostRequest {
    std::uint64_t id = 0;
    std::string method;
    bool done = false;
    std::string outcome;         // ok | error | timeout | not_executed | disconnected | host_exited
    int executed = -1;           // -1 unknown, 0 not executed, 1 executed
    std::string result_json = "null", error_code, error_message;
    bool late = false;           // an answer arrived after the deadline
    std::string late_json;
    bool verification = false;
    std::uint64_t deadline_ms = 0;
    std::uint64_t document = 0;  // for document sync bookkeeping
    std::string synced_text;
};

class HostManager {
public:
    using Publisher = std::function<void(const char* topic, const std::string& payload_json)>;
    explicit HostManager(Publisher publish);
    ~HostManager();

    std::string load(const std::string& manifest_utf8_path, const std::string& root_utf8_path); // returns specs JSON
    std::uint64_t start(const std::string& engine, const std::string& workspace_utf8);
    std::string snapshot(std::uint64_t host);
    std::string list();
    std::string wait(std::uint64_t host, std::uint32_t timeout_ms, bool& settled);
    std::uint64_t request(std::uint64_t host, const std::string& method, const std::string& params_json, std::uint32_t timeout_ms);
    std::uint64_t sync_document(std::uint64_t host, std::uint64_t document, const std::string& utf8_path, const std::string& text, std::uint32_t timeout_ms);
    std::string wait_request(std::uint64_t host, std::uint64_t request, std::uint32_t timeout_ms, bool& finished);
    std::uint64_t verify(std::uint64_t host, std::uint32_t timeout_ms);
    void reconnect(std::uint64_t host);
    std::uint64_t stop(std::uint64_t host, const std::string& mode); // detach | shutdown | terminate; shutdown returns its request id

private:
    struct Host;
    std::mutex lock_;
    std::condition_variable changed_;
    std::map<std::uint64_t, std::shared_ptr<Host>> hosts_;
    std::vector<HostSpec> specs_;
    std::wstring root_;
    std::uint64_t next_ = 1;
    bool closing_ = false;
    Publisher publish_;
    std::thread watchdog_;
    std::vector<std::thread> workers_;

    std::shared_ptr<Host> find(std::uint64_t id);
    std::string host_json(const Host&);
    static std::string request_json(const HostRequest&);
    void set_state(Host&, const char* state, const std::string& error = std::string());
    void run_start(std::shared_ptr<Host>, bool launch);
    bool connect_bridge(const std::shared_ptr<Host>&, std::uint64_t deadline_ms);
    void on_bridge_line(const std::shared_ptr<Host>&, std::uint64_t generation, const std::string& line);
    void on_bridge_exit(const std::shared_ptr<Host>&, std::uint64_t generation, unsigned long exit_code);
    void on_process_exit(const std::shared_ptr<Host>&);
    std::uint64_t send(const std::shared_ptr<Host>&, const std::string& method, const std::string& params_json, std::uint32_t timeout_ms, bool verification, std::uint64_t document, const std::string& synced);
    void finish_locked(Host&, HostRequest&, const std::string& outcome, std::vector<std::string>& events);
    void watchdog();
};

std::string sha256_hex(const std::wstring& path); // upper-case hex, "" when unreadable

} // namespace openmse::core
