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
function MigrateRuntimeValue([string] $value) {
    foreach ($oldName in @('OFFICE12','VS7Debug','VS8')) {
        $oldRoot = "$root\runtime\$oldName"
        $value = [regex]::Replace($value, ([regex]::Escape($oldRoot) + '(?=\\|$)'), [System.Text.RegularExpressions.MatchEvaluator]{ param($m) "$root\runtime" }, [Text.RegularExpressions.RegexOptions]::IgnoreCase)
    }
    return $value
}
function MigratePrivateRuntimePaths([Microsoft.Win32.RegistryKey] $key) {
    foreach ($name in $key.GetValueNames()) {
        $kind = $key.GetValueKind($name)
        $value = $key.GetValue($name, $null, [Microsoft.Win32.RegistryValueOptions]::DoNotExpandEnvironmentNames)
        if ($kind -in @([Microsoft.Win32.RegistryValueKind]::String,[Microsoft.Win32.RegistryValueKind]::ExpandString)) {
            $updated = MigrateRuntimeValue ([string]$value)
            if ($updated -ne $value) { $key.SetValue($name,$updated,$kind) }
        } elseif ($kind -eq [Microsoft.Win32.RegistryValueKind]::MultiString) {
            $updated = [string[]]@($value | ForEach-Object { MigrateRuntimeValue ([string]$_) })
            if (($updated -join [char]0) -ne ($value -join [char]0)) { $key.SetValue($name,$updated,$kind) }
        }
    }
    foreach ($child in $key.GetSubKeyNames()) {
        $subkey = $key.OpenSubKey($child,$true)
        if ($subkey) { try { MigratePrivateRuntimePaths $subkey } finally { $subkey.Dispose() } }
    }
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
    $privateRoot = $base.OpenSubKey($registryRoot,$true)
    if ($privateRoot) { try { MigratePrivateRuntimePaths $privateRoot } finally { $privateRoot.Dispose() } }
    $key = $base.CreateSubKey("$registryRoot\$userStore")
    $key.SetValue('OpenMSEInitialized', 1, [Microsoft.Win32.RegistryValueKind]::DWord)
    # VS2005 SP1 shows a modal "run as administrator" notice at every start until it is dismissed. A hidden
    # back-end instance can never dismiss it, so it is turned off in this project's private VS8 settings only.
    if ($Engine -eq 'vs8' -and $null -eq $key.GetValue('ShowWindowsAdminDlg')) { $key.SetValue('ShowWindowsAdminDlg', 0, [Microsoft.Win32.RegistryValueKind]::DWord) }
    $key.Dispose()
} finally { $base.Dispose() }
$rootFile = "$root\config\registry-root.txt"
if (-not (Test-Path -LiteralPath $rootFile) -or [IO.File]::ReadAllText($rootFile) -ne $registryRoot) {
    [IO.File]::WriteAllText($rootFile, $registryRoot, [Text.Encoding]::ASCII)
}
