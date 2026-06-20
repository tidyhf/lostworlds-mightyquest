# build.ps1 - build mqelserver + the cert-shim from source.
# Requires: Visual Studio 2022 (Desktop C++ workload) and Git. Builds 32-bit (x86).
# You do NOT need this if you just want to run the prebuilt binaries in bin/.
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot

# vcpkg lives at a space-free path (OpenSSL's build dislikes spaces).
$vcpkg = Join-Path $env:USERPROFILE 'vcpkg'
if (-not (Test-Path (Join-Path $vcpkg 'vcpkg.exe'))) {
    if (-not (Test-Path (Join-Path $vcpkg '.git'))) { git clone --depth 1 https://github.com/microsoft/vcpkg $vcpkg }
    & (Join-Path $vcpkg 'bootstrap-vcpkg.bat') -disableMetrics
}
Write-Host "Installing OpenSSL (x86) via vcpkg (first time takes ~10 min)..."
& (Join-Path $vcpkg 'vcpkg.exe') install openssl:x86-windows
$toolchain = Join-Path $vcpkg 'scripts\buildsystems\vcpkg.cmake'

Write-Host "Building mqelserver..."
cmake -S "$root\src\mqelserver" -B "$root\build\server" -G "Visual Studio 17 2022" -A Win32 `
    -DCMAKE_TOOLCHAIN_FILE="$toolchain" -DVCPKG_TARGET_TRIPLET=x86-windows -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build "$root\build\server" --config Release

Write-Host "Building cert-shim (certpatch.dll + inject.exe)..."
cmake -S "$root\src\certshim" -B "$root\build\certshim" -G "Visual Studio 17 2022" -A Win32 -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build "$root\build\certshim" --config Release

Write-Host "Staging binaries into bin\ ..."
foreach ($name in 'mqelserver.exe','certpatch.dll','inject.exe') {
    $f = Get-ChildItem "$root\build" -Recurse -Filter $name -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($f) { Copy-Item $f.FullName "$root\bin\" -Force }
}
foreach ($dll in 'libssl-3.dll','libcrypto-3.dll','legacy.dll') {
    $src = Join-Path $vcpkg "installed\x86-windows\bin\$dll"
    if (Test-Path $src) { Copy-Item $src "$root\bin\" -Force }
}
Write-Host "Done. Binaries are in bin\."
