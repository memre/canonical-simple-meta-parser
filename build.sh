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
if [ -f "$VCPKG_EXE" ]; then
    vcpkg install
else
    echo "Error: vcpkg executable not found in $VCPKG_ROOT"
    exit 1
fi

BUILD_DIR="build"
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

(
    cd "$BUILD_DIR"
    cmake ..
    cmake --build .

    echo "Build success!"
) || {
    echo "Error during build process!"
    exit 1
}
