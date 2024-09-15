$VCPKG_PATH = 'external\vcpkg'
$env:VCPKG_ROOT = Join-Path $PSScriptRoot, $VCPKG_PATH
$env:PATH += $VCPKG_ROOT

& bootstrap-vcpkg.bat
& vcpkg.exe --vcpkg-root=$env:VCPKG_ROOT install

