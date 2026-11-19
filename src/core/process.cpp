#include "process.hpp"

#include "documents.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#pragma comment(lib, "version.lib")

namespace openmse::core {

namespace {
std::string last_error(const char* action)
{
    DWORD code = GetLastError();
    return std::string(action) + "（Windows 错误 " + std::to_string(code) + "）";
}
} // namespace

std::wstring ChildProcess::quote(const std::wstring& a)
{
    if (!a.empty() && a.find_first_of(L" \t\n\v\"") == std::wstring::npos) return a;
    std::wstring out = L"\"";
    std::size_t slashes = 0;
    for (wchar_t c : a) {
        if (c == L'\\') { ++slashes; continue; }
        out.append(c == L'"' ? slashes * 2 + 1 : slashes, L'\\');
        slashes = 0;
        out.push_back(c);
    }
    out.append(slashes * 2, L'\\');
    out.push_back(L'"');
    return out;
}

std::string decode_codepage_line(const std::string& bytes, unsigned codepage)
{
    if (bytes.empty()) return {};
    if (codepage == CP_UTF8 && valid_utf8(bytes)) return bytes;
    int n = MultiByteToWideChar(codepage, 0, bytes.data(), static_cast<int>(bytes.size()), nullptr, 0);
    if (n <= 0) return std::string(bytes.size(), '?');
    std::wstring wide(static_cast<std::size_t>(n), L'\0');
    MultiByteToWideChar(codepage, 0, bytes.data(), static_cast<int>(bytes.size()), wide.data(), n);
    for (auto& c : wide) if (c >= 0xD800 && c <= 0xDFFF) c = 0xFFFD; // stray surrogates from broken output
    return narrow(wide);
}

std::wstring windows_directory()
{
    wchar_t buffer[MAX_PATH];
    UINT n = GetWindowsDirectoryW(buffer, MAX_PATH);
    return std::wstring(buffer, n);
}

std::string file_version(const std::wstring& path)
{
    DWORD ignored = 0, size = GetFileVersionInfoSizeW(path.c_str(), &ignored);
    if (!size) return {};
    std::vector<unsigned char> data(size);
    if (!GetFileVersionInfoW(path.c_str(), 0, size, data.data())) return {};
    VS_FIXEDFILEINFO* info = nullptr;
    UINT length = 0;
    if (!VerQueryValueW(data.data(), L"\\", reinterpret_cast<void**>(&info), &length) || !info) return {};
    return std::to_string(HIWORD(info->dwFileVersionMS)) + "." + std::to_string(LOWORD(info->dwFileVersionMS)) + "." +
           std::to_string(HIWORD(info->dwFileVersionLS)) + "." + std::to_string(LOWORD(info->dwFileVersionLS));
}

ChildProcess::ChildProcess(const ProcessOptions& o, LineHandler on_line, ExitHandler on_exit)
{
    if (GetFileAttributesW(o.executable.c_str()) == INVALID_FILE_ATTRIBUTES)
        throw KernelError(ErrorCode::dependency_missing, "找不到程序：" + narrow(o.executable));
    SECURITY_ATTRIBUTES inherit{ sizeof(inherit), nullptr, TRUE };
    HANDLE out_read = nullptr, out_write = nullptr, in_read = nullptr, in_write = nullptr;
    if (!CreatePipe(&out_read, &out_write, &inherit, 0)) throw KernelError(ErrorCode::io_error, last_error("无法创建输出管道"));
    SetHandleInformation(out_read, HANDLE_FLAG_INHERIT, 0);
    if (o.stdin_pipe) {
        if (!CreatePipe(&in_read, &in_write, &inherit, 0)) { CloseHandle(out_read); CloseHandle(out_write); throw KernelError(ErrorCode::io_error, last_error("无法创建输入管道")); }
        SetHandleInformation(in_write, HANDLE_FLAG_INHERIT, 0);
    } else {
        in_read = CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &inherit, OPEN_EXISTING, 0, nullptr);
    }
    auto cleanup = [&] {
        if (out_read) CloseHandle(out_read);
        if (out_write) CloseHandle(out_write);
        if (in_read && in_read != INVALID_HANDLE_VALUE) CloseHandle(in_read);
        if (in_write) CloseHandle(in_write);
    };

    // Only the two pipe ends are inherited, even if other threads start processes concurrently.
    HANDLE inherited[2] = { out_write, in_read };
    SIZE_T attribute_size = 0;
    InitializeProcThreadAttributeList(nullptr, 1, 0, &attribute_size);
    std::vector<unsigned char> attribute_buffer(attribute_size);
    auto attributes = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(attribute_buffer.data());
    if (!InitializeProcThreadAttributeList(attributes, 1, 0, &attribute_size) ||
        !UpdateProcThreadAttribute(attributes, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST, inherited, sizeof(inherited), nullptr, nullptr)) {
        cleanup();
        throw KernelError(ErrorCode::io_error, last_error("无法设置句柄继承"));
    }
    STARTUPINFOEXW si{};
    si.StartupInfo.cb = sizeof(si);
    si.StartupInfo.dwFlags = STARTF_USESTDHANDLES;
    si.StartupInfo.hStdInput = in_read;
    si.StartupInfo.hStdOutput = out_write;
    si.StartupInfo.hStdError = out_write;
    si.lpAttributeList = attributes;

    std::wstring command = quote(o.executable);
    for (const auto& a : o.arguments) command += L" " + quote(a);
    PROCESS_INFORMATION pi{};
    BOOL ok = CreateProcessW(o.executable.c_str(), command.data(), nullptr, nullptr, TRUE,
        CREATE_NO_WINDOW | CREATE_SUSPENDED | EXTENDED_STARTUPINFO_PRESENT | CREATE_UNICODE_ENVIRONMENT, nullptr,
        o.directory.empty() ? nullptr : o.directory.c_str(), &si.StartupInfo, &pi);
    DeleteProcThreadAttributeList(attributes);
    if (!ok) { std::string message = last_error(("无法启动 " + narrow(o.executable)).c_str()); cleanup(); throw KernelError(ErrorCode::io_error, message); }

    if (o.own_process_tree) {
        job_ = CreateJobObjectW(nullptr, nullptr);
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits{};
        limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        if (job_) SetInformationJobObject(job_, JobObjectExtendedLimitInformation, &limits, sizeof(limits));
    }
    if (o.own_process_tree && (!job_ || !AssignProcessToJobObject(job_, pi.hProcess))) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hThread); CloseHandle(pi.hProcess);
        if (job_) CloseHandle(job_);
        job_ = nullptr;
        cleanup();
        throw KernelError(ErrorCode::io_error, last_error("无法把进程加入作业对象"));
    }
    ResumeThread(pi.hThread);
    CloseHandle(pi.hThread);
    CloseHandle(out_write); out_write = nullptr;
    if (in_read && in_read != INVALID_HANDLE_VALUE) CloseHandle(in_read);
    in_read = nullptr;
    process_ = pi.hProcess;
    pid_ = pi.dwProcessId;
    FILETIME created{}, exited{}, kernel_time{}, user_time{};
    if (GetProcessTimes(pi.hProcess, &created, &exited, &kernel_time, &user_time))
        created_ = (static_cast<std::uint64_t>(created.dwHighDateTime) << 32) | created.dwLowDateTime;
    stdout_read_ = out_read;
    stdin_write_ = in_write;

    unsigned codepage = o.output_codepage ? o.output_codepage : GetOEMCP();
    reader_ = std::thread([this, codepage, on_line = std::move(on_line), on_exit = std::move(on_exit)] {
        std::string pending;
        char buffer[4096];
        DWORD got = 0;
        while (ReadFile(stdout_read_, buffer, sizeof(buffer), &got, nullptr) && got > 0) {
            pending.append(buffer, got);
            std::size_t nl;
            while ((nl = pending.find('\n')) != std::string::npos) {
                std::string line = pending.substr(0, nl);
                if (!line.empty() && line.back() == '\r') line.pop_back();
                pending.erase(0, nl + 1);
                if (on_line) on_line(decode_codepage_line(line, codepage));
            }
            if (pending.size() > (1u << 20)) { if (on_line) on_line(decode_codepage_line(pending, codepage)); pending.clear(); }
        }
        if (!pending.empty() && on_line) on_line(decode_codepage_line(pending, codepage));
        WaitForSingleObject(process_, INFINITE);
        DWORD code = 1;
        GetExitCodeProcess(process_, &code);
        finished_ = true;
        if (on_exit) on_exit(code);
    });
}

bool ChildProcess::write_line(const std::string& line)
{
    std::lock_guard<std::mutex> hold(stdin_lock_);
    if (!stdin_write_) return false;
    std::string data = line + "\n";
    DWORD wrote = 0;
    std::size_t done = 0;
    while (done < data.size()) {
        if (!WriteFile(stdin_write_, data.data() + done, static_cast<DWORD>(data.size() - done), &wrote, nullptr)) return false;
        done += wrote;
    }
    return true;
}

void ChildProcess::close_stdin()
{
    std::lock_guard<std::mutex> hold(stdin_lock_);
    if (stdin_write_) { CloseHandle(stdin_write_); stdin_write_ = nullptr; }
}

void ChildProcess::terminate(unsigned exit_code)
{
    if (job_) TerminateJobObject(job_, exit_code);
    else if (process_) TerminateProcess(process_, exit_code);
}

ChildProcess::~ChildProcess()
{
    close_stdin();
    if (!finished_) terminate(1);
    if (reader_.joinable()) {
        if (reader_.get_id() == std::this_thread::get_id()) reader_.detach(); // destroyed from its own exit callback
        else reader_.join();
    }
    if (stdout_read_) CloseHandle(stdout_read_);
    if (process_) CloseHandle(process_);
    if (job_) CloseHandle(job_);
}

} // namespace openmse::core
