# Mighty Quest Server

Private server for **The Mighty Quest for Epic Loot**, whose PC servers shut down on 25 October 2016. Runs the original Steam client against a server you control.

Part of the Lost Worlds project.

## What you need

- Windows 10/11.
- The client: enter `steam://install/239220` in your browser to install it from Steam. It is free-to-play — you do not need to own it first.
- Steam running (the client's DRM unpacks through Steam).

Prebuilt binaries are in `bin/`; no build tools are required.

## Setup (single machine)

1. Redirect the game's hostname to the server:
   ```powershell
   .\scripts\host-setup.ps1
   ```
2. Start the server and leave it running:
   ```powershell
   .\bin\mqelserver.exe
   ```
3. Launch the game:
   ```powershell
   .\scripts\play.ps1
   ```

Revert the hosts change with `.\scripts\host-revert.ps1`.

## Hosting for others

Run `bin\mqelserver.exe` on the host and allow inbound TCP 443 (port-forward for internet play). Each player runs:

```powershell
.\scripts\host-setup.ps1 -ServerIP <host-ip>
.\scripts\play.ps1
```

## What works

Client boot, Steam DRM, TLS, and 14 `.hqs` endpoints: account info, hero creation, castle and attack selection, start/end attack, rate castle, resurrect, seasonal competition, and the command/telemetry sink. Enough to load into the game.

## What's missing

Most responses are hardcoded or stubbed, there is no inventory persistence, and progression is incomplete. The server logs unhandled calls to `bin\Plugins\Logs\MQELOffline.log` (`NO HANDLER for /X.hqs/Y`). Handlers live in `src/mqeloffline/Source/Frontend/Services/`.

## How it works

- The retail executable is SteamStub-packed; launching it with `SteamAppId=239220` while Steam is running unpacks it.
- The game resolves `Gameserver`; the hosts entry routes that to the server, which terminates TLS with a self-signed certificate.
- The client's libCURL validates against a CA compiled into the executable and ignores the Windows certificate store, so it rejects external certificates. `certpatch.dll`, injected at launch by `play.ps1`, patches verification to succeed. Validated against client build 280139.
- `mqelserver.exe` is MQELOffline's `Gameserver` driven over a standard socket instead of the original in-process hook.

See [CREDITS.md](CREDITS.md).

## Building

Requires Visual Studio 2022 (Desktop C++ workload) and Git:

```powershell
.\build.ps1
```

This fetches OpenSSL via vcpkg, builds the server and cert-shim, and stages `bin/`.

## Legal

Non-commercial preservation project. It ships no game assets or game code; you supply your own client from Steam. The Mighty Quest for Epic Loot is © Ubisoft and this project is unaffiliated with and unendorsed by Ubisoft. MIT licensed.
