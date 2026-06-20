# host-setup.ps1 - point the game's server hostname at your MQEL server.
# Run once on each machine that will PLAY. Single PC (server + game together)? Use the
# default 127.0.0.1. Remote/LAN host? Pass the server's IP:  .\host-setup.ps1 -ServerIP 192.168.1.50
# Self-elevates because editing the hosts file requires administrator rights.
param([string]$ServerIP = '127.0.0.1')
$ErrorActionPreference = 'Stop'

$principal = New-Object Security.Principal.WindowsPrincipal([Security.Principal.WindowsIdentity]::GetCurrent())
if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    Start-Process powershell "-NoProfile -ExecutionPolicy Bypass -File `"$PSCommandPath`" -ServerIP $ServerIP" -Verb RunAs
    return
}

$h = "$env:windir\System32\drivers\etc\hosts"
if (-not (Test-Path "$h.mqelbak")) { Copy-Item $h "$h.mqelbak" -Force }

$names = 'Gameserver','gs.themightyquest.com'
$kept = Get-Content $h | Where-Object {
    $line = $_
    ($line -notmatch 'Mighty Quest local server') -and
    -not ($names | Where-Object { $line -match ("\s" + [regex]::Escape($_) + "\s*$") })
}
$kept += "# Mighty Quest local server (Lost Worlds) - remove to revert"
foreach ($n in $names) { $kept += "$ServerIP`t$n" }
Set-Content -Path $h -Value $kept -Encoding ASCII
Write-Host "hosts updated: Gameserver + gs.themightyquest.com -> $ServerIP"
Write-Host "Revert any time with host-revert.ps1"
