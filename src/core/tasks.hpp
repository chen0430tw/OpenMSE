#pragma once
// Long-running work with its own identity: builds and program runs. Each task owns a child process,
// publishes ordered output/state events, records its real exit code, and can be cancelled.
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

struct BuildDiagnostic {
    std::string file, code, message;
    std::uint32_t line = 0, column = 0;
    bool is_error = true;
};

enum class TaskState { running, succeeded, failed, cancelled };
const char* task_state_name(TaskState);

struct TaskInfo {
    std::uint64_t id = 0, workspace = 0;
    std::string kind, project, configuration, tool;
    std::wstring command;
    TaskState state = TaskState::running;
    long long exit_code = -1;                 // -1 until finished
    std::uint64_t started_ms = 0, duration_ms = 0;
    std::uint32_t errors = 0, warnings = 0;
    std::vector<BuildDiagnostic> diagnostics;
    std::vector<std::wstring> artifacts;      // produced (existing, written after the task started)
    std::vector<std::string> tail;            // last output lines
    std::uint64_t lines = 0;
    std::uint32_t pid = 0;
};

class TaskManager {
public:
    using Publisher = std::function<void(const char* topic, const std::string& payload_json)>;
    explicit TaskManager(Publisher publish) : publish_(std::move(publish)) {}
    ~TaskManager();
    std::uint64_t start_build(const Workspace&, const std::vector<Tool>&, const std::string& configuration, const std::string& project);
    std::uint64_t start_run(const Workspace&, const std::string& configuration, const std::string& project);
    void cancel(std::uint64_t id);
    TaskInfo snapshot(std::uint64_t id);
    TaskInfo wait(std::uint64_t id, std::uint32_t timeout_ms, bool& finished);
    std::vector<TaskInfo> list();
    static std::string json(const TaskInfo&);

private:
    struct Entry {
        TaskInfo info;
        bool cancel_requested = false;
        std::vector<std::wstring> expected_artifacts;
        std::vector<std::wstring> search_roots;     // for relative diagnostic paths
        std::uint64_t start_file_time = 0;
        std::map<std::string, bool> seen;          // de-duplicate MSBuild's repeated diagnostics
        std::unique_ptr<ChildProcess> process;
    };
    std::mutex lock_;
    std::condition_variable changed_;
    std::map<std::uint64_t, std::shared_ptr<Entry>> tasks_;
    std::uint64_t next_ = 1;
    Publisher publish_;

    std::uint64_t launch(std::shared_ptr<Entry>, ProcessOptions);
    void on_line(const std::shared_ptr<Entry>&, const std::string& line);
    void on_exit(const std::shared_ptr<Entry>&, unsigned long code);
    void reap();
};

} // namespace openmse::core
