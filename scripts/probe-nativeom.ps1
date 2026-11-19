param([Parameter(Mandatory=$true)][int]$ProcessId)
$ErrorActionPreference = 'Stop'
if ([IntPtr]::Size -ne 4) {
    & "$env:WINDIR\SysWOW64\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -File $PSCommandPath -ProcessId $ProcessId
    exit $LASTEXITCODE
}
Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
public static class NativeObjectProbe {
    public delegate bool EnumProc(IntPtr hwnd, IntPtr data);
    [DllImport("user32.dll")] static extern bool EnumWindows(EnumProc proc, IntPtr data);
    [DllImport("user32.dll")] static extern uint GetWindowThreadProcessId(IntPtr hwnd, out uint pid);
    [DllImport("oleacc.dll")] static extern int AccessibleObjectFromWindow(IntPtr hwnd, uint id, ref Guid iid, [MarshalAs(UnmanagedType.Interface)] out object value);
    public static string[] Inspect(int target) {
        var result = new List<string>();
        EnumWindows(delegate(IntPtr hwnd, IntPtr data) {
            uint pid; GetWindowThreadProcessId(hwnd, out pid);
            if (pid != target) return true;
            Guid iid = new Guid("00020400-0000-0000-C000-000000000046");
            object value; int hr = AccessibleObjectFromWindow(hwnd, unchecked((uint)-16), ref iid, out value);
            result.Add("HWND=0x" + hwnd.ToInt32().ToString("X") + " nativeOM HRESULT=0x" + hr.ToString("X8") + " object=" + (value != null));
            if (value != null && Marshal.IsComObject(value)) Marshal.ReleaseComObject(value);
            return true;
        }, IntPtr.Zero);
        return result.ToArray();
    }
}
'@
$process = Get-Process -Id $ProcessId
Write-Output "PID=$ProcessId Path=$($process.Path)"
[NativeObjectProbe]::Inspect($ProcessId)
