#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winternl.h>
#include <detours.h>
#include <string>
#include <vector>
#include <cstring>
#include <set>
#include <algorithm>

static std::wstring root, registryRoot;
static bool vs8 = false;
static const std::wstring office = L"C:\\Program Files (x86)\\Common Files\\Microsoft Shared\\OFFICE12";
static const std::wstring debugRoot = L"C:\\Program Files (x86)\\Common Files\\Microsoft Shared\\VS7Debug";
static decltype(&RegOpenKeyExW) realOpenW = RegOpenKeyExW;
static decltype(&RegOpenKeyExA) realOpenA = RegOpenKeyExA;
static decltype(&RegCreateKeyExW) realCreateW = RegCreateKeyExW;
static decltype(&RegCreateKeyExA) realCreateA = RegCreateKeyExA;
static decltype(&CreateFileW) realFileW = CreateFileW;
static decltype(&ExitProcess) realExit = ExitProcess;
using LdrLoad = NTSTATUS (NTAPI*)(PWSTR, PULONG, PUNICODE_STRING, PHANDLE);
static LdrLoad realLdrLoad;

static std::wstring wide(const char* s) {
    if (!s) return {};
    int n = MultiByteToWideChar(CP_ACP, 0, s, -1, nullptr, 0);
    std::wstring out(n, 0);
    MultiByteToWideChar(CP_ACP, 0, s, -1, out.data(), n);
    if (!out.empty()) out.pop_back();
    return out;
}
static std::string narrow(const std::wstring& s) {
    int n = WideCharToMultiByte(CP_ACP, 0, s.data(), (int)s.size(), nullptr, 0, nullptr, nullptr);
    std::string out(n, 0);
    WideCharToMultiByte(CP_ACP, 0, s.data(), (int)s.size(), out.data(), n, nullptr, nullptr);
    return out;
}
static void replacePath(std::wstring& s, const std::wstring& from, const std::wstring& to) {
    for (size_t i = 0; i + from.size() <= s.size();) {
        if (_wcsnicmp(s.data() + i, from.c_str(), from.size()) == 0 &&
            (i + from.size() == s.size() || s[i + from.size()] == L'\\' || s[i + from.size()] == 0)) {
            s.replace(i, from.size(), to); i += to.size();
        } else ++i;
    }
}
static std::wstring relocate(std::wstring s) {
    replacePath(s, office, root + L"\\runtime");
    replacePath(s, debugRoot, root + L"\\runtime");
    if (vs8) replacePath(s, L"C:\\Program Files (x86)\\Microsoft Visual Studio 8", root + L"\\runtime");
    return s;
}
static void logPath(const wchar_t* event, const std::wstring& before, const std::wstring& after, LONG status) {
    DWORD saved = GetLastError();
    std::wstring msg = std::wstring(event) + L" | " + before + L" => " + after + L" | status=" + std::to_wstring(status) + L"\r\n";
    auto text = narrow(msg);
    HANDLE h = realFileW((root + (vs8 ? L"\\logs\\redirect-vs8.log" : L"\\logs\\redirect.log")).c_str(), FILE_APPEND_DATA,
                        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h != INVALID_HANDLE_VALUE) {
        DWORD written; WriteFile(h, text.data(), (DWORD)text.size(), &written, nullptr); CloseHandle(h);
    }
    SetLastError(saved);
}
static bool registryPath(HKEY& key, std::wstring& sub) {
    const std::wstring prefix = vs8 ? L"Software\\Microsoft\\VisualStudio\\8.0" : L"Software\\Microsoft\\Office\\12.0\\MSE";
    if ((key == HKEY_LOCAL_MACHINE || key == HKEY_CURRENT_USER) &&
        sub.size() >= prefix.size() && _wcsnicmp(sub.c_str(), prefix.c_str(), prefix.size()) == 0 &&
        (sub.size() == prefix.size() || sub[prefix.size()] == L'\\')) {
        std::wstring tail = sub.substr(prefix.size());
        // Existing machine licensing stays with the installed VS entitlement.
        if (vs8 && key == HKEY_LOCAL_MACHINE &&
            (_wcsnicmp(tail.c_str(), L"\\Licenses", 9) == 0 || _wcsnicmp(tail.c_str(), L"\\Registration", 13) == 0)) return false;
        sub = registryRoot + (key == HKEY_LOCAL_MACHINE ? (vs8 ? L"\\MachineVS8" : L"\\MachineMSE") :
                                                       (vs8 ? L"\\UserVS8" : L"\\UserMSE")) + tail;
        key = HKEY_CURRENT_USER; return true;
    }
    return false;
}
static LSTATUS WINAPI openW(HKEY k, LPCWSTR sub, DWORD opts, REGSAM access, PHKEY out) {
    std::wstring s = sub ? sub : L"";
    if (registryPath(k, s)) return realOpenW(k, s.c_str(), opts, access, out);
    return realOpenW(k, sub, opts, access, out);
}
static LSTATUS WINAPI openA(HKEY k, LPCSTR sub, DWORD opts, REGSAM access, PHKEY out) {
    auto s = wide(sub);
    if (registryPath(k, s)) return realOpenW(k, s.c_str(), opts, access, out);
    return realOpenA(k, sub, opts, access, out);
}
static LSTATUS WINAPI createW(HKEY k, LPCWSTR sub, DWORD reserved, LPWSTR cls, DWORD opts,
                             REGSAM access, LPSECURITY_ATTRIBUTES sa, PHKEY out, LPDWORD disp) {
    std::wstring s = sub ? sub : L"";
    if (registryPath(k, s)) return realCreateW(k, s.c_str(), reserved, cls, opts, access, sa, out, disp);
    return realCreateW(k, sub, reserved, cls, opts, access, sa, out, disp);
}
static LSTATUS WINAPI createA(HKEY k, LPCSTR sub, DWORD reserved, LPSTR cls, DWORD opts,
                             REGSAM access, LPSECURITY_ATTRIBUTES sa, PHKEY out, LPDWORD disp) {
    auto s = wide(sub); auto cl = wide(cls);
    if (registryPath(k, s)) return realCreateW(k, s.c_str(), reserved, cls ? cl.data() : nullptr, opts, access, sa, out, disp);
    return realCreateA(k, sub, reserved, cls, opts, access, sa, out, disp);
}
static NTSTATUS NTAPI load(PWSTR search, PULONG flags, PUNICODE_STRING name, PHANDLE handle) {
    if (!name || !name->Buffer) return realLdrLoad(search, flags, name, handle);
    std::wstring original(name->Buffer, name->Length / sizeof(wchar_t));
    auto mapped = relocate(original); UNICODE_STRING replacement = *name;
    if (mapped != original) {
        replacement.Buffer = mapped.data(); replacement.Length = (USHORT)(mapped.size() * sizeof(wchar_t));
        replacement.MaximumLength = replacement.Length + sizeof(wchar_t);
    }
    NTSTATUS result = realLdrLoad(search, flags, mapped == original ? name : &replacement, handle);
    logPath(L"LOAD", original, mapped, result);
    return result;
}
static void WINAPI exitProcess(UINT code) { logPath(L"EXIT", L"ExitProcess", L"", (LONG)code); realExit(code); }

// ---- Optional file-access trace (OPENMSE_TRACE_FILES=<file>) used to measure package dependency closures.
// Every file or directory this process opens successfully is written once, as a DOS path. Opens relative to
// another handle are not resolved. Tracing is off unless the variable is set; nothing is redirected by it.
using NtCreate = NTSTATUS (NTAPI*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, PLARGE_INTEGER, ULONG, ULONG, ULONG, ULONG, PVOID, ULONG);
using NtOpen = NTSTATUS (NTAPI*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, ULONG, ULONG);
static NtCreate realNtCreate;
static NtOpen realNtOpen;
static std::wstring tracePath;
static CRITICAL_SECTION traceLock;
static std::set<std::wstring>* traced;
static thread_local bool tracing = false;
static void trace(POBJECT_ATTRIBUTES attributes, NTSTATUS status) {
    if (status < 0 || tracing || !attributes || attributes->RootDirectory || !attributes->ObjectName || !attributes->ObjectName->Buffer) return;
    std::wstring path(attributes->ObjectName->Buffer, attributes->ObjectName->Length / sizeof(wchar_t));
    if (path.compare(0, 4, L"\\??\\") != 0) return;          // only DOS-device paths: drive letters and UNC
    path.erase(0, 4);
    if (path.compare(0, 4, L"UNC\\") == 0) path = L"\\" + path.substr(3);
    if (path.size() < 3 || path[1] != L':') return;
    std::wstring key = path;
    std::transform(key.begin(), key.end(), key.begin(), ::towlower);
    tracing = true;
    DWORD saved = GetLastError();
    EnterCriticalSection(&traceLock);
    if (traced->insert(key).second) {
        int n = WideCharToMultiByte(CP_UTF8, 0, path.data(), (int)path.size(), nullptr, 0, nullptr, nullptr);
        std::string line(n, 0);
        WideCharToMultiByte(CP_UTF8, 0, path.data(), (int)path.size(), line.data(), n, nullptr, nullptr);
        line += "\n";
        HANDLE h = realFileW(tracePath.c_str(), FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (h != INVALID_HANDLE_VALUE) { DWORD written; WriteFile(h, line.data(), (DWORD)line.size(), &written, nullptr); CloseHandle(h); }
    }
    LeaveCriticalSection(&traceLock);
    SetLastError(saved);
    tracing = false;
}
static NTSTATUS NTAPI ntCreate(PHANDLE h, ACCESS_MASK a, POBJECT_ATTRIBUTES o, PIO_STATUS_BLOCK io, PLARGE_INTEGER size, ULONG attr, ULONG share, ULONG disposition, ULONG options, PVOID ea, ULONG eaLength) {
    NTSTATUS status = realNtCreate(h, a, o, io, size, attr, share, disposition, options, ea, eaLength);
    trace(o, status);
    return status;
}
static NTSTATUS NTAPI ntOpen(PHANDLE h, ACCESS_MASK a, POBJECT_ATTRIBUTES o, PIO_STATUS_BLOCK io, ULONG share, ULONG options) {
    NTSTATUS status = realNtOpen(h, a, o, io, share, options);
    trace(o, status);
    return status;
}
extern "C" void __cdecl OpenMSEHookEntry() {}
BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID) {
    if (DetourIsHelperProcess()) return TRUE;
    if (reason == DLL_PROCESS_ATTACH) {
        DetourRestoreAfterWith();
        wchar_t text[32768];
        if (!GetEnvironmentVariableW(L"OPENMSE_ROOT", text, 32768)) return FALSE; root = text;
        if (!GetEnvironmentVariableW(L"OPENMSE_REGISTRY_ROOT", text, 32768)) return FALSE; registryRoot = text;
        vs8 = GetEnvironmentVariableW(L"OPENMSE_ENGINE", text, 32768) && wcscmp(text, L"vs8") == 0;
        SetDllDirectoryW((root + (vs8 ? L"\\runtime\\Common7\\IDE" : L"\\runtime\\VS Runtime")).c_str());
        realLdrLoad = (LdrLoad)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "LdrLoadDll");
        if (!realLdrLoad) return FALSE;
        DetourTransactionBegin(); DetourUpdateThread(GetCurrentThread());
#define ATTACH(real, hook) DetourAttach(&(PVOID&)real, hook)
        ATTACH(realLdrLoad, load);
        ATTACH(realExit, exitProcess);
        if (GetEnvironmentVariableW(L"OPENMSE_TRACE_FILES", text, 32768)) {
            tracePath = text;
            size_t token = tracePath.find(L"{engine}");
            if (token != std::wstring::npos) tracePath.replace(token, 8, vs8 ? L"vs8" : L"mse");
            InitializeCriticalSection(&traceLock);
            traced = new std::set<std::wstring>();
            HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
            realNtCreate = (NtCreate)GetProcAddress(ntdll, "NtCreateFile");
            realNtOpen = (NtOpen)GetProcAddress(ntdll, "NtOpenFile");
            if (realNtCreate && realNtOpen) { ATTACH(realNtCreate, ntCreate); ATTACH(realNtOpen, ntOpen); }
        }
        bool loadsOnly = GetEnvironmentVariableW(L"OPENMSE_LOADS_ONLY", text, 32768) != 0;
        if (!loadsOnly) {
        ATTACH(realOpenW, openW); ATTACH(realOpenA, openA);
        ATTACH(realCreateW, createW); ATTACH(realCreateA, createA);
        }
        LONG result = DetourTransactionCommit();
        logPath(L"INIT", L"OpenMSE", root, result);
        return result == NO_ERROR;
    }
    return TRUE;
}
