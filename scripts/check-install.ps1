<#
OpenMSE install check (integration plan phase 6).
Checks system prerequisites, the integrity of the installed packages, and the external dependencies measured for
each optional package (config/packages/<name>.json). Writes logs/install-check.json.
  powershell -NoProfile -ExecutionPolicy Bypass -File scripts\check-install.ps1 [-Full]
-Full also verifies SHA-256 of every packaged file (slower). Exit code 0 when the core package is usable.
#>
param([switch] $Full)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$windows = $env:WINDIR
$results = New-Object System.Collections.Generic.List[object]
function Add([string] $package, [string] $check, [string] $state, [string] $detail) {
    $results.Add([ordered]@{ package = $package; check = $check; state = $state; detail = $detail })
}
function FileHash([string] $path) { (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash }

# ---- system
$os64 = [Environment]::Is64BitOperatingSystem
Add 'system' 'Windows x64' ($(if ($os64) { 'ok' } else { 'missing' })) ([Environment]::OSVersion.VersionString)
$ndp4 = Get-ItemProperty 'HKLM:\SOFTWARE\Microsoft\NET Framework Setup\NDP\v4\Full' -ErrorAction SilentlyContinue
Add 'system' '.NET Framework 4.x' ($(if ($ndp4 -and $ndp4.Release -ge 378389) { 'ok' } else { 'missing' })) ($(if ($ndp4) { "Release $($ndp4.Release)" } else { 'not installed' }))
$fx2 = "$windows\Microsoft.NET\Framework\v2.0.50727"
$fx2x64 = "$windows\Microsoft.NET\Framework64\v2.0.50727"
foreach ($item in @(
    @('MSBuild 2.0', "$fx2\MSBuild.exe", 'build and run'),
    @('CLR 2.0 debugging (x86)', "$fx2\mscordbi.dll", 'x86 debugging'),
    @('CLR 2.0 debugging (x64)', "$fx2x64\mscordbi.dll", 'AnyCPU/x64 debugging'),
    @('CLR 2.0 symbol reader', "$fx2\diasymreader.dll", 'source-level debugging'))) {
    Add 'system' $item[0] ($(if (Test-Path -LiteralPath $item[1]) { 'ok' } else { 'missing' })) "$($item[1]) - $($item[2])"
}

# ---- installed packages and their integrity
$manifestPath = Join-Path $root 'package-manifest.json'
$installed = @()
if (Test-Path -LiteralPath $manifestPath) {
    $manifest = Get-Content -LiteralPath $manifestPath -Raw -Encoding UTF8 | ConvertFrom-Json
    $installed = @($manifest.packages)
    $bad = 0; $missing = 0; $count = 0
    foreach ($property in $manifest.files.PSObject.Properties) {
        $count++
        $path = Join-Path $root ($property.Name -replace '/', '\')
        if (-not (Test-Path -LiteralPath $path)) { $missing++; continue }
        if ((Get-Item -LiteralPath $path -Force).Length -ne $property.Value.size) { $bad++; continue }
        if ($Full -and (FileHash $path) -ne $property.Value.sha256) { $bad++ }
    }
    $state = if ($missing -eq 0 -and $bad -eq 0) { 'ok' } else { 'damaged' }
    Add 'packages' ('installed: ' + ($installed -join ', ')) $state "$count files; missing $missing; changed $bad$(if (-not $Full) { ' (sizes only; use -Full for SHA-256)' })"
} else {
    # A source checkout: infer packages from what is present.
    $installed = @('core')
    if (Test-Path "$root\runtime\Common7\IDE\devenv.exe") { $installed += 'vs8-backend' }
    if (Test-Path "$root\runtime\MSE7.EXE") { $installed += 'mse-window' }
    Add 'packages' ('source checkout: ' + ($installed -join ', ')) 'ok' 'no package-manifest.json; integrity not checked'
}

# ---- core files
foreach ($rel in @('OpenMSE.exe', 'bin\OpenMSE.Shell.exe', 'bin\OpenMSE.Core.dll', 'bin\OpenMSE.DebugHost.x86.exe', 'bin\OpenMSE.DebugHost.x64.exe', 'config\toolbox.json')) {
    Add 'core' $rel ($(if (Test-Path -LiteralPath (Join-Path $root $rel)) { 'ok' } else { 'missing' })) ''
}

# ---- optional packages: entry versions/hashes from hosts.json and measured external dependencies
$hosts = $null
if (Test-Path "$root\config\hosts.json") { $hosts = Get-Content "$root\config\hosts.json" -Raw -Encoding UTF8 | ConvertFrom-Json }
foreach ($package in @(@{ name = 'vs8-backend'; engine = 'vs8' }, @{ name = 'mse-window'; engine = 'mse' })) {
    $name = $package.name
    $present = ($installed -contains $name) -or ($installed -contains 'legacy-full')
    if (-not $present) { Add $name 'package' 'not-installed' 'optional'; continue }
    $spec = if ($hosts) { $hosts.hosts | Where-Object engine -EQ $package.engine } else { $null }
    if (-not $spec) { Add $name 'hosts.json entry' 'missing' 'config\hosts.json' }
    else {
        $exe = Join-Path $root ($spec.executable -replace '/', '\')
        if (-not (Test-Path -LiteralPath $exe)) { Add $name $spec.executable 'missing' '' }
        else {
            $version = (Get-Item -LiteralPath $exe).VersionInfo.FileVersion -replace ' .*$', ''
            $hashOk = (FileHash $exe) -eq $spec.sha256
            Add $name $spec.executable ($(if ($version -eq $spec.fileVersion -and $hashOk) { 'ok' } else { 'mismatch' })) "version $version; sha256 $(if ($hashOk) { 'matches' } else { 'differs' })"
        }
        foreach ($rel in @($spec.launcher, $spec.bridge, 'OpenMSEHook.dll', 'scripts/setup.ps1')) {
            Add $name $rel ($(if (Test-Path -LiteralPath (Join-Path $root ($rel -replace '/', '\'))) { 'ok' } else { 'missing' })) ''
        }
    }
    if ($package.engine -eq 'vs8') {
        $dte = "$windows\assembly\GAC\EnvDTE\8.0.0.0__b03f5f7f11d50a3a\EnvDTE.dll"
        Add $name 'EnvDTE 8.0 (GAC)' ($(if (Test-Path -LiteralPath $dte) { 'ok' } else { 'missing' })) 'required by HostBridge'
    }
    $measuredPath = Join-Path $root "config\packages\$name.json"
    if (-not (Test-Path -LiteralPath $measuredPath)) { Add $name 'measured dependencies' 'unknown' "config\packages\$name.json is missing"; continue }
    $measured = Get-Content -LiteralPath $measuredPath -Raw -Encoding UTF8 | ConvertFrom-Json
    foreach ($kind in @('gac', 'framework', 'originalInstall')) {
        $list = @($measured.externalDependencies.$kind)
        $absent = @($list | Where-Object { -not (Test-Path -LiteralPath $_) })
        # Files read from the original VS2005/Office installation are a dependency on that installation.
        $state = if ($absent.Count -eq 0) { 'ok' } elseif ($kind -eq 'originalInstall') { 'missing' } else { 'missing' }
        $detail = "$($list.Count) measured; $($absent.Count) absent$(if ($absent.Count) { ': ' + (($absent | Select-Object -First 3) -join '; ') })"
        Add $name "external: $kind" $state $detail
    }
}

$coreOk = -not ($results | Where-Object { ($_.package -eq 'system' -or $_.package -eq 'core') -and $_.state -ne 'ok' -and $_.check -notmatch 'x64\)' })
$report = [ordered]@{ checked = (Get-Date).ToString('s'); root = $root; full = [bool]$Full; coreUsable = [bool]$coreOk; results = $results }
New-Item -ItemType Directory -Force -Path "$root\logs" | Out-Null
$report | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath "$root\logs\install-check.json" -Encoding UTF8
foreach ($r in $results) { '{0,-14} {1,-12} {2} {3}' -f $r.package, "[$($r.state)]", $r.check, $(if ($r.detail) { "- $($r.detail)" } else { '' }) }
"核心包可用：$coreOk  报告：$root\logs\install-check.json"
if ($coreOk) { exit 0 } else { exit 1 }
