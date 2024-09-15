$ErrorActionPreference = 'Stop'

$VCPKG_PATH = 'external\vcpkg'
$env:VCPKG_ROOT = (Join-Path $PSScriptRoot $VCPKG_PATH)
$env:PATH += ";$VCPKG_ROOT"

# Check if the vcpkg bootstrap script exists and run it
$bootstrapScript = Join-Path $env:VCPKG_ROOT 'bootstrap-vcpkg.bat'
if (Test-Path $bootstrapScript) {
    & $bootstrapScript
}
else {
    Write-Error "bootstrap-vcpkg.bat not found in $env:VCPKG_ROOT"
    exit 1
}

$vcpkgExe = (Join-Path $env:VCPKG_ROOT 'vcpkg.exe')
if (Test-Path $vcpkgExe) {
    & $vcpkgExe install
}
else {
    Write-Error "vcpkg.exe not found in $env:VCPKG_ROOT"
    exit 1
}

# Create build directory if not exists
$buildDir = Join-Path $PSScriptRoot 'build'
if (-Not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir
}

try {
    Push-Location $buildDir
    cmake ..
    cmake --build .

    Write-Host "Build success!"
}
catch {
    Write-Error "Error!"
}
finally {
    Pop-Location
}
