#pragma once
// DebugService: managed (CLR v2) debugging through the OpenMSE.DebugHost adapter processes.
// States: starting, running, stopped, terminating, exited, failed. Stepping, variables and evaluation are
// only accepted in "stopped"; every state change and all program output are published as kernel events.
#include "process.hpp"
#include "workspace.hpp"

#include <condition_variable>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace openmse::core {

std::string pe_debug_bitness(const std::wstring& exe); // "x86" | "x64"; throws KernelError for non-.NET files

class DebugManager {
public:
    using Publisher = std::function<void(const char* topic, const std::string& payload_json)>;
    explicit DebugManager(Publisher publish) : publish_(std::move(publish)) {}
    ~DebugManager();
    std::uint64_t start(const Workspace&, const std::string& configuration, const std::string& project, const std::string& breakpoints_json);
    std::string snapshot(std::uint64_t session);
    std::string wait(std::uint64_t session, std::uint32_t timeout_ms, bool& settled);
    std::string control(std::uint64_t session, const std::string& action);
    std::string add_breakpoint(std::uint64_t session, const std::string& file, std::uint32_t line);
    std::string remove_breakpoint(std::uint64_t session, std::uint32_t id);
    std::string variables(std::uint64_t session, std::uint32_t frame);
    std::string evaluate(std::uint64_t session, std::uint32_t frame, const std::string& expression);

private:
    struct Session;
    std::mutex lock_;
    std::condition_variable changed_;
    std::map<std::uint64_t, std::shared_ptr<Session>> sessions_;
    std::uint64_t next_ = 1;
    Publisher publish_;

    std::shared_ptr<Session> find(std::uint64_t id);
    std::string json(const Session&);
    void set_state(Session&, const char* state);
    void on_line(const std::shared_ptr<Session>&, const std::string& line);
    void on_exit(const std::shared_ptr<Session>&, unsigned long code);
    std::string call(const std::shared_ptr<Session>&, const std::string& method, const std::string& params, std::uint32_t timeout_ms,
        std::unique_lock<std::mutex>& held);
    void require(const Session&, const char* state, const char* action);
};

} // namespace openmse::core
