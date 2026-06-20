# play.ps1 - launch The Mighty Quest for Epic Loot and inject the cert-patch so the
# client talks to your local/private server. Steam must be running and own app 239220.
# Run host-setup.ps1 first (once) to point the game's hostname at your server.
param([int]$WaitSeconds = 12)
$ErrorActionPreference = 'Stop'
$bin = Join-Path (Split-Path $PSScriptRoot -Parent) 'bin'

function Find-Game {
    $steam = (Get-ItemProperty 'HKCU:\Software\Valve\Steam' -ErrorAction SilentlyContinue).SteamPath
    if (-not $steam) { $steam = (Get-ItemProperty 'HKLM:\SOFTWARE\WOW6432Node\Valve\Steam' -ErrorAction SilentlyContinue).InstallPath }
    $libs = @()
    if ($steam) { $libs += $steam }
    $vdf = Join-Path $steam 'steamapps\libraryfolders.vdf'
    if (Test-Path $vdf) {
        foreach ($line in Get-Content $vdf) { if ($line -match '"path"\s+"([^"]+)"') { $libs += ($Matches[1] -replace '\\\\','\') } }
    }
    foreach ($l in $libs) {
        $exe = Join-Path $l 'steamapps\common\The Mighty Quest For Epic Loot\GameData\Bin\MightyQuest.exe'
        if (Test-Path $exe) { return $exe }
    }
    return $null
}

if (-not (Get-Process steam -ErrorAction SilentlyContinue)) {
    Write-Warning "Steam does not appear to be running. The game's DRM needs Steam running (and the game owned). Start Steam, then re-run."
}

$exe = Find-Game
if (-not $exe) { Write-Error "MightyQuest.exe not found. Install the game from Steam (app 239220) and try again."; exit 1 }
$gamebin = Split-Path $exe -Parent
Write-Host "Game found: $exe"

$env:SteamAppId  = '239220'
$env:SteamGameId = '239220'
$argline = '-server_url https://Gameserver -environmentName mqel-live -branchName mqel -steamid 76561201696194782 -steamticket "" -token ""'
$p = Start-Process -FilePath $exe -WorkingDirectory $gamebin -ArgumentList $argline -PassThru
Write-Host ("Launched MightyQuest.exe (PID {0}). Waiting {1}s for the client to start..." -f $p.Id, $WaitSeconds)
Start-Sleep -Seconds $WaitSeconds

if (-not (Get-Process MightyQuest -ErrorAction SilentlyContinue)) { Write-Error "The game exited early (DRM/Steam?). Make sure Steam is running and you own the game."; exit 1 }
& (Join-Path $bin 'inject.exe') 'MightyQuest.exe' (Join-Path $bin 'certpatch.dll')
Write-Host "Cert-patch injected. The client now accepts your server's certificate - enjoy!"
Write-Host "(Patch log: %TEMP%\mqel-certpatch.log)"
