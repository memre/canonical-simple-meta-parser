#!/bin/bash

VCPKG_PATH='external/vcpkg'
VCPKG_ROOT=$(realpath "$VCPKG_PATH")
export PATH="$VCPKG_ROOT:$PATH"

vcpkg_bootstrap="$VCPKG_ROOT/bootstrap-vcpkg.sh"
if [ -f "$vcpkg_bootstrap" ]; then
    $vcpkg_bootstrap
else
    echo "Error: bootstrap-vcpkg.sh not found in $VCPKG_ROOT"
    exit 1
fi

vcpkg="$VCPKG_ROOT/vcpkg"
if [ -f "$vcpkg" ]; then
    vcpkg install
else
    echo "Error: vcpkg executable not found in $VCPKG_ROOT"
    exit 1
fi

build_dir="build"
if [ ! -d "$build_dir" ]; then
    mkdir "$build_dir"
fi

(
    cd "$build_dir"
    cmake ..
    cmake --build .

    echo "Build success!"
) || {
    echo "Error during build process!"
    exit 1
}
