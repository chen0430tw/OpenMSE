$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$shell = New-Object -ComObject WScript.Shell
$mse = $shell.CreateShortcut("$root\MSE.lnk")
$mse.TargetPath = "$root\OpenMSE.exe"
$mse.Arguments = '--mse'
$mse.WorkingDirectory = $root
$mse.IconLocation = "$root\OpenMSE.exe,0"
$mse.Description = 'OpenMSE script editor'
$mse.Save()
$unified = $shell.CreateShortcut("$root\OpenMSE.lnk")
$unified.TargetPath = "$root\OpenMSE.exe"
$unified.WorkingDirectory = $root
$unified.IconLocation = "$root\OpenMSE.exe,0"
$unified.Description = 'OpenMSE unified workspace'
$unified.Save()
if (Test-Path -LiteralPath "$root\runtime\Common7\IDE\devenv.exe") {
    $vs8 = $shell.CreateShortcut("$root\Visual Studio 2005.lnk")
    $vs8.TargetPath = "$root\OpenMSE.exe"
    $vs8.Arguments = '--vs8'
    $vs8.WorkingDirectory = $root
    $vs8.IconLocation = "$root\runtime\Common7\IDE\devenv.exe,0"
    $vs8.Description = 'OpenMSE integrated Visual Studio 2005 environment'
    $vs8.Save()
}
