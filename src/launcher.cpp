#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <detours.h>
#include <string>
#include <cstdio>
#include <fstream>

static std::wstring quote(const std::wstring& arg) {
    std::wstring result = L"\""; size_t slashes = 0;
    for (wchar_t ch : arg) {
        if (ch == L'\\') { ++slashes; continue; }
        result.append(slashes * (ch == L'"' ? 2 : 1), L'\\'); slashes = 0;
        if (ch == L'"') result += L'\\'; result += ch;
    }
    result.append(slashes * 2, L'\\'); return result + L'"';
}
static std::string ansi(const std::wstring& value) {
    int n = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, value.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string out(n, 0); BOOL lost = FALSE;
    WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, value.c_str(), -1, out.data(), n, nullptr, &lost);
    if (lost) return {}; if (!out.empty()) out.pop_back(); return out;
}
static void logMessage(const std::wstring& root, const std::wstring& message) {
    std::wofstream file((root + L"\\logs\\launcher.log").c_str(), std::ios::app);
    file << message << L"\n";
}
static int fail(const std::wstring& root, const std::wstring& message) {
    logMessage(root, message);
    MessageBoxW(nullptr, message.c_str(), L"OpenMSE", MB_OK | MB_ICONERROR);
    return 1;
}
static int runOpenMSE(int argc, wchar_t** argv) {
    wchar_t module[32768]; GetModuleFileNameW(nullptr, module, 32768);
    std::wstring root = module; root.resize(root.find_last_of(L'\\'));
    bool vs8 = false, hidden = false, wait = false, loadsOnly = false;
    for (int i = 1; i < argc; ++i) {
        if (wcscmp(argv[i], L"--vs8") == 0) vs8 = true;
        if (wcscmp(argv[i], L"--hidden") == 0) hidden = true;
        if (wcscmp(argv[i], L"--wait") == 0) wait = true;
        if (wcscmp(argv[i], L"--loads-only") == 0) loadsOnly = true;
    }
    SetEnvironmentVariableW(L"OPENMSE_ENGINE", vs8 ? L"vs8" : L"mse");
    SetEnvironmentVariableW(L"OPENMSE_LOADS_ONLY", loadsOnly ? L"1" : nullptr);
    wchar_t system[32768]; GetSystemDirectoryW(system, 32768);
    std::wstring ps = std::wstring(system) + L"\\WindowsPowerShell\\v1.0\\powershell.exe";
    std::wstring setup = quote(ps) + L" -NoProfile -NonInteractive -ExecutionPolicy Bypass -File " +
                         quote(root + L"\\scripts\\setup.ps1") + L" -Engine " + (vs8 ? L"vs8" : L"mse");
    STARTUPINFOW si = {}; si.cb = sizeof(si); PROCESS_INFORMATION pi = {};
    if (!CreateProcessW(ps.c_str(), setup.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, root.c_str(), &si, &pi))
        return fail(root, L"OpenMSE configuration process could not start. Error " + std::to_wstring(GetLastError()));
    WaitForSingleObject(pi.hProcess, INFINITE); DWORD result = 1; GetExitCodeProcess(pi.hProcess, &result);
    CloseHandle(pi.hThread); CloseHandle(pi.hProcess);
    if (result) return fail(root, L"OpenMSE configuration failed. Exit code " + std::to_wstring(result));
    std::ifstream input((root + L"\\config\\registry-root.txt").c_str()); std::string reg; std::getline(input, reg);
    if (reg.empty()) return fail(root, L"Missing OpenMSE registry configuration.");
    SetEnvironmentVariableW(L"OPENMSE_ROOT", root.c_str()); SetEnvironmentVariableA("OPENMSE_REGISTRY_ROOT", reg.c_str());
    std::wstring dir = root + (vs8 ? L"\\runtime\\VS8\\Common7\\IDE" : L"\\runtime\\OFFICE12");
    std::wstring exe = dir + (vs8 ? L"\\devenv.exe" : L"\\MSE7.EXE");
    if (GetFileAttributesW(exe.c_str()) == INVALID_FILE_ATTRIBUTES)
        return fail(root, L"The selected IDE has not been imported: " + exe);
    if (vs8) {
        std::wstring install = root + L"\\runtime\\VS8\\";
        SetEnvironmentVariableW(L"VSINSTALLDIR", install.c_str());
        SetEnvironmentVariableW(L"VCINSTALLDIR", (install + L"VC\\").c_str());
        SetEnvironmentVariableW(L"VS80COMNTOOLS", (install + L"Common7\\Tools\\").c_str());
        SetEnvironmentVariableW(L"DevEnvDir", (dir + L"\\").c_str());
        DWORD count = GetEnvironmentVariableW(L"PATH", nullptr, 0);
        std::wstring path(count, 0); if (count) GetEnvironmentVariableW(L"PATH", path.data(), count);
        path = dir + L";" + install + L"VC\\bin;" + install + L"Common7\\Tools;" + path;
        SetEnvironmentVariableW(L"PATH", path.c_str());
    }
    std::wstring cmd = quote(exe);
    for (int i = 1; i < argc; ++i) {
        if (wcscmp(argv[i], L"--mse") == 0 || wcscmp(argv[i], L"--vs8") == 0 || wcscmp(argv[i], L"--hidden") == 0 ||
            wcscmp(argv[i], L"--wait") == 0 || wcscmp(argv[i], L"--loads-only") == 0) continue;
        // VS2005 treats a quoted /switch as a document name.
        bool isSwitch = vs8 && (argv[i][0] == L'/' || argv[i][0] == L'-') &&
                        wcspbrk(argv[i], L" \t\"") == nullptr;
        cmd += L" " + (isSwitch ? std::wstring(argv[i]) : quote(argv[i]));
    }
    si = {}; si.cb = sizeof(si); si.dwFlags = STARTF_USESHOWWINDOW; si.wShowWindow = hidden ? SW_HIDE : SW_SHOWNORMAL;
    std::string dll = ansi(root + L"\\OpenMSEHook.dll");
    if (dll.empty()) return fail(root, L"The project path cannot be represented by the active Windows ANSI code page.");
    if (!DetourCreateProcessWithDllExW(exe.c_str(), cmd.data(), nullptr, nullptr, FALSE, 0, nullptr,
                                     dir.c_str(), &si, &pi, dll.c_str(), nullptr))
        return fail(root, L"OpenMSE could not start the selected IDE. Error " + std::to_wstring(GetLastError()));
    std::wstring stateFile = root + (vs8 ? L"\\logs\\last-process-vs8.txt" : L"\\logs\\last-process.txt");
    std::ofstream state(stateFile.c_str()); state << pi.dwProcessId << "\n" << std::flush;
    logMessage(root, std::wstring(vs8 ? L"VS8 PID " : L"MSE PID ") + std::to_wstring(pi.dwProcessId));
    if (wait) { WaitForSingleObject(pi.hProcess, INFINITE); DWORD code; GetExitCodeProcess(pi.hProcess, &code);
        logMessage(root, std::wstring(vs8 ? L"VS8 exit code " : L"MSE exit code ") + std::to_wstring(code)); }
    CloseHandle(pi.hThread); CloseHandle(pi.hProcess); return 0;
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    int argc = 0;
    wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!argv) return 1;
    bool legacy = false;
    for (int i = 1; i < argc; ++i)
        if (wcscmp(argv[i], L"--mse") == 0 || wcscmp(argv[i], L"--vs8") == 0 ||
            wcscmp(argv[i], L"--hidden") == 0 || wcscmp(argv[i], L"--wait") == 0 ||
            wcscmp(argv[i], L"--loads-only") == 0) legacy = true;
    int result = 0;
    if (legacy) result = runOpenMSE(argc, argv);
    else {
        wchar_t module[32768]; GetModuleFileNameW(nullptr, module, 32768);
        std::wstring root = module; root.resize(root.find_last_of(L'\\'));
        std::wstring shell = root + L"\\bin\\OpenMSE.Shell.exe";
        std::wstring command = quote(shell);
        for (int i = 1; i < argc; ++i) if (wcscmp(argv[i], L"--gui") != 0) command += L" " + quote(argv[i]);
        STARTUPINFOW si = {}; si.cb = sizeof(si); PROCESS_INFORMATION pi = {};
        if (!CreateProcessW(shell.c_str(), command.data(), nullptr, nullptr, FALSE, 0, nullptr, root.c_str(), &si, &pi))
            result = fail(root, L"OpenMSE GUI could not start. Run scripts/build-shell.ps1. Error " + std::to_wstring(GetLastError()));
        else { CloseHandle(pi.hThread); CloseHandle(pi.hProcess); }
    }
    LocalFree(argv);
    return result;
}
