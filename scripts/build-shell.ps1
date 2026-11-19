param([string]$OutputDirectory, [switch]$SkipCore)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$compiler = "$env:WINDIR\Microsoft.NET\Framework64\v4.0.30319\csc.exe"
if (-not $OutputDirectory) { $OutputDirectory = "$root\bin" }
New-Item -ItemType Directory -Force -Path $OutputDirectory | Out-Null
# The GUI loads the x64 kernel DLL from its own directory.
if (-not $SkipCore) {
    & cmd.exe /c "`"$root\build-core.cmd`""
    if ($LASTEXITCODE -ne 0) { throw 'OpenMSE.Core compilation failed' }
    # Managed debug adapters (x86 and x64) live next to the kernel DLL.
    & cmd.exe /c "`"$root\build-debughost.cmd`""
    if ($LASTEXITCODE -ne 0) { throw 'OpenMSE.DebugHost compilation failed' }
}
if ((Resolve-Path $OutputDirectory).Path -ne (Resolve-Path "$root\bin").Path) {
    Copy-Item -Force "$root\bin\OpenMSE.Core.dll", "$root\bin\OpenMSE.DebugHost.x86.exe", "$root\bin\OpenMSE.DebugHost.x64.exe" $OutputDirectory
}
$json = "$env:WINDIR\Microsoft.NET\assembly\GAC_MSIL\System.Web.Extensions\v4.0_4.0.0.0__31bf3856ad364e35\System.Web.Extensions.dll"
& $compiler /nologo /noconfig /target:winexe /platform:x64 /optimize+ /utf8output /reference:System.dll /reference:System.Core.dll /reference:System.Drawing.dll /reference:System.Windows.Forms.dll /reference:System.Xml.dll /reference:System.Web.dll "/reference:$json" "/win32icon:$root\assets\mse.ico" "/win32manifest:$root\src\shell\app.manifest" "/out:$OutputDirectory\OpenMSE.Shell.exe" "$root\src\shell\Program.cs" "$root\src\shell\AssemblyInfo.cs" "$root\src\shell\KernelClient.cs" "$root\src\shell\KernelWorkspace.cs" "$root\src\shell\Workspace.cs" "$root\src\shell\MainForm.cs" "$root\src\shell\MainForm.Debug.cs" "$root\tests\gui\GuiSmoke.cs" "$root\src\shell\DesignerModel.cs" "$root\src\shell\DesignerView.cs" "$root\src\shell\ToolboxPanel.cs" "$root\src\shell\ToolboxIcons.cs" "$root\tests\gui\DesignerSmoke.cs"
if ($LASTEXITCODE -ne 0) { throw 'Unified GUI compilation failed' }
