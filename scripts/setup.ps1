param([ValidateSet('mse','vs8')][string] $Engine = 'mse')
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$digest = [Security.Cryptography.SHA256]::Create().ComputeHash([Text.Encoding]::UTF8.GetBytes($root.ToLowerInvariant()))
$instance = ([BitConverter]::ToString($digest).Replace('-', '')).Substring(0, 16)
$registryRoot = "Software\OpenMSE\Instances\$instance"
$templateName = if ($Engine -eq 'vs8') { 'vs8-registry-template.json' } else { 'registry-template.json' }
$template = Get-Content -LiteralPath "$root\config\$templateName" -Raw -Encoding UTF8 | ConvertFrom-Json
function Relocate([string] $value) {
    foreach ($mapping in $template.Mappings) {
        $target = Join-Path $root $mapping.Target
        $value = [regex]::Replace($value, [regex]::Escape($mapping.Source), [System.Text.RegularExpressions.MatchEvaluator]{ param($m) $target }, [Text.RegularExpressions.RegexOptions]::IgnoreCase)
    }
    return $value
}
$base = [Microsoft.Win32.RegistryKey]::OpenBaseKey([Microsoft.Win32.RegistryHive]::CurrentUser, [Microsoft.Win32.RegistryView]::Registry32)
try {
    $userStore = if ($Engine -eq 'vs8') { 'UserVS8' } else { 'UserMSE' }
    $existing = $base.OpenSubKey("$registryRoot\$userStore")
    $initializeUser = ($null -eq $existing)
    if ($null -ne $existing) { $initializeUser = ($existing.GetValue('OpenMSEInitialized', 0) -ne 1); $existing.Dispose() }
    foreach ($node in $template.Nodes) {
        $store = if ($node.Store) { [string]$node.Store } else { 'MachineMSE' }
        if ($store -notin @('MachineMSE','UserMSE','MachineVS8','UserVS8')) { throw 'Invalid template store' }
        if ($store.StartsWith('User') -and -not $initializeUser) { continue }
        $path = "$registryRoot\$store"
        if ($node.Path) { $path += "\$($node.Path)" }
        $key = $base.CreateSubKey($path)
        try {
            foreach ($item in $node.Values) {
                $kind = [Microsoft.Win32.RegistryValueKind][int]$item.Type
                $value = $item.Data
                switch ([int]$item.Type) {
                    1 { $value = Relocate ([string]$value) }
                    2 { $value = Relocate ([string]$value) }
                    3 { $value = [Convert]::FromBase64String([string]$value) }
                    4 { $value = [BitConverter]::ToInt32([BitConverter]::GetBytes([uint32]$value), 0) }
                    7 { $value = [string[]]@($value | ForEach-Object { Relocate ([string]$_) }) }
                    11 { $value = [long]$value }
                }
                $key.SetValue([string]$item.Name, $value, $kind)
            }
        } finally { $key.Dispose() }
    }
    $key = $base.CreateSubKey("$registryRoot\$userStore")
    $key.SetValue('OpenMSEInitialized', 1, [Microsoft.Win32.RegistryValueKind]::DWord); $key.Dispose()
} finally { $base.Dispose() }
$rootFile = "$root\config\registry-root.txt"
if (-not (Test-Path -LiteralPath $rootFile) -or [IO.File]::ReadAllText($rootFile) -ne $registryRoot) {
    [IO.File]::WriteAllText($rootFile, $registryRoot, [Text.Encoding]::ASCII)
}
