$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$compiler = "$env:WINDIR\Microsoft.NET\Framework\v4.0.30319\csc.exe"
$dte = "$env:WINDIR\assembly\GAC\EnvDTE\8.0.0.0__b03f5f7f11d50a3a\EnvDTE.dll"
$json = "$env:WINDIR\Microsoft.NET\assembly\GAC_MSIL\System.Web.Extensions\v4.0_4.0.0.0__31bf3856ad364e35\System.Web.Extensions.dll"
foreach ($file in @($compiler, $dte, $json)) { if (-not (Test-Path -LiteralPath $file)) { throw "Missing bridge build dependency: $file" } }
New-Item -ItemType Directory -Force -Path "$root\build" | Out-Null
& $compiler /nologo /noconfig /reference:System.dll /reference:System.Core.dll /target:exe /platform:x86 /optimize+ "/reference:$dte" "/reference:$json" "/out:$root\build\HostBridge.exe" "$root\src\bridge\HostBridge.cs"
if ($LASTEXITCODE -ne 0) { throw 'Bridge compilation failed' }
