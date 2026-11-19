param([int] $ProcessId = 0, [ValidateSet('mse','vs8')][string] $Engine = 'mse')
$ErrorActionPreference = 'Stop'
# Windows PowerShell/.NET Framework x64 does not enumerate this WOW64 process reliably.
if ([Environment]::Is64BitProcess) {
    & "$env:WINDIR\SysWOW64\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -File $PSCommandPath -ProcessId $ProcessId -Engine $Engine
    exit $LASTEXITCODE
}
$root = Split-Path -Parent $PSScriptRoot
$stateName = if ($Engine -eq 'vs8') { 'last-process-vs8.txt' } else { 'last-process.txt' }
if ($ProcessId -le 0) { $ProcessId = [int](Get-Content -LiteralPath "$root\logs\$stateName") }
if ($ProcessId -le 0) { throw 'No valid project process ID.' }
$process = Get-Process -Id $ProcessId
$expected = if ($Engine -eq 'vs8') { "$root\runtime\Common7\IDE\devenv.exe" } else { "$root\runtime\MSE7.EXE" }
if ($process.Path -ne $expected) { throw 'Process does not belong to this project.' }
Add-Type -TypeDefinition @'
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
public static class OpenMSEWindowProbe {
    public class Window { public string Handle; public string Class; public string Title; }
    private delegate bool Callback(IntPtr hwnd, IntPtr param);
    [DllImport("user32.dll")] private static extern bool EnumWindows(Callback callback, IntPtr param);
    [DllImport("user32.dll")] private static extern uint GetWindowThreadProcessId(IntPtr hwnd, out uint processId);
    [DllImport("user32.dll", CharSet = CharSet.Unicode)] private static extern int GetWindowText(IntPtr hwnd, StringBuilder text, int max);
    [DllImport("user32.dll", CharSet = CharSet.Unicode)] private static extern int GetClassName(IntPtr hwnd, StringBuilder text, int max);
    public static Window[] Read(uint processId) {
        var result = new List<Window>();
        EnumWindows((hwnd, param) => {
            uint owner; GetWindowThreadProcessId(hwnd, out owner);
            if (owner == processId) {
                var title = new StringBuilder(4096); var cls = new StringBuilder(256);
                GetWindowText(hwnd, title, title.Capacity); GetClassName(hwnd, cls, cls.Capacity);
                result.Add(new Window {Handle = "0x" + hwnd.ToInt64().ToString("X"), Class = cls.ToString(), Title = title.ToString()});
            }
            return true;
        }, IntPtr.Zero);
        return result.ToArray();
    }
}
'@
$modules = @($process.Modules | Select-Object ModuleName, FileName, @{n='Version';e={$_.FileVersionInfo.FileVersion}})
$windows = @([OpenMSEWindowProbe]::Read([uint32]$ProcessId))
$required = if ($Engine -eq 'vs8') { @('devenv.exe', 'OpenMSEHook.dll', 'MSENV.DLL') } else { @('MSE7.EXE', 'OpenMSEHook.dll', 'MSO.DLL', 'MSENV.DLL', 'MSOMSE.DLL') }
$missing = @($required | Where-Object { $_ -notin $modules.ModuleName })
$original = @($modules | Where-Object { $_.FileName -match '^C:\\Program Files.*\\Microsoft Shared\\(OFFICE12|VS7Debug)' -or $_.FileName -match '^C:\\Program Files.*\\Microsoft Visual Studio 8\\' })
$dialogs = @($windows | Where-Object Class -EQ '#32770')
$design = @($windows | Where-Object Class -EQ 'wndclass_desked_gsk')
$result = [ordered]@{
    Engine = $Engine
    ProcessId = $ProcessId
    Executable = $process.Path
    Responding = $process.Responding
    ModuleCount = $modules.Count
    DesignWindows = $design
    Dialogs = $dialogs
    MissingRequiredModules = $missing
    OriginalPrivateModules = $original
    Passed = ($process.Responding -and $missing.Count -eq 0 -and $original.Count -eq 0 -and $dialogs.Count -eq 0 -and $design.Count -gt 0)
    Modules = $modules
}
$reportName = if ($Engine -eq 'vs8') { 'validation-vs8.json' } else { 'validation.json' }
$result | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath "$root\logs\$reportName" -Encoding UTF8
Write-Output "PID=$ProcessId modules=$($modules.Count) passed=$($result.Passed)"
$design | Format-Table Title -AutoSize
if (-not $result.Passed) { exit 1 }
