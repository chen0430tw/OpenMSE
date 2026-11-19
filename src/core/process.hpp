#pragma once
// Child processes owned by the kernel: no console window, restricted handle inheritance, a job object
// so cancellation ends the whole process tree, and a reader thread that delivers decoded output lines.
#include <atomic>
#include <cstdint>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

namespace openmse::core {

struct ProcessOptions {
    std::wstring executable;
    std::vector<std::wstring> arguments;
    std::wstring directory;
    bool stdin_pipe = false;     // keep a writable stdin (bridge protocols); otherwise stdin is NUL
    unsigned output_codepage = 0; // 0 = OEM code page (console default); CP_UTF8 for UTF-8 protocols
    bool own_process_tree = true; // job object: terminate()/destruction ends all descendants. False for launchers
                                  // whose children must outlive them (e.g. the legacy IDE started by OpenMSE.exe).
};

class ChildProcess {
public:
    using LineHandler = std::function<void(const std::string& utf8_line)>;
    using ExitHandler = std::function<void(unsigned long exit_code)>;
    // Starts the process; throws KernelError(io_error / dependency_missing) on failure.
    ChildProcess(const ProcessOptions&, LineHandler on_line, ExitHandler on_exit);
    ~ChildProcess(); // terminates the job if still running, joins the reader
    ChildProcess(const ChildProcess&) = delete;
    ChildProcess& operator=(const ChildProcess&) = delete;

    std::uint32_t pid() const { return pid_; }
    std::uint64_t creation_time() const { return created_; }
    bool write_line(const std::string& utf8_line); // false if stdin is closed or broken
    void close_stdin();
    void terminate(unsigned exit_code = 1);        // ends the whole job (process tree)
    bool running() const { return !finished_; }

    static std::wstring quote(const std::wstring& argument);

private:
    void* process_ = nullptr;
    void* job_ = nullptr;
    void* stdout_read_ = nullptr;
    void* stdin_write_ = nullptr;
    std::uint32_t pid_ = 0;
    std::uint64_t created_ = 0;
    std::mutex stdin_lock_;
    std::atomic<bool> finished_{false};
    std::thread reader_;
};

std::string decode_codepage_line(const std::string& bytes, unsigned codepage);
std::wstring windows_directory();
std::string file_version(const std::wstring& path); // "" when unavailable

} // namespace openmse::core
