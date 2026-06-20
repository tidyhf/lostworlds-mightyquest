# host-revert.ps1 - undo host-setup.ps1 (restore the hosts file). Self-elevates.
$ErrorActionPreference = 'Stop'

$principal = New-Object Security.Principal.WindowsPrincipal([Security.Principal.WindowsIdentity]::GetCurrent())
if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    Start-Process powershell "-NoProfile -ExecutionPolicy Bypass -File `"$PSCommandPath`"" -Verb RunAs
    return
}

$h = "$env:windir\System32\drivers\etc\hosts"
if (Test-Path "$h.mqelbak") {
    Copy-Item "$h.mqelbak" $h -Force; Remove-Item "$h.mqelbak" -Force
    Write-Host "hosts restored from backup."
} else {
    $names = 'Gameserver','gs.themightyquest.com'
    $kept = Get-Content $h | Where-Object {
        $line = $_
        ($line -notmatch 'Mighty Quest local server') -and
        -not ($names | Where-Object { $line -match ("\s" + [regex]::Escape($_) + "\s*$") })
    }
    Set-Content -Path $h -Value $kept -Encoding ASCII
    Write-Host "hosts entries removed."
}
