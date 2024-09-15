cmake_minimum_required(VERSION 3.22)

project(
    "canonical-simple-meta-parser"
    VERSION 1.0.0
    LANGUAGES C CXX)

# Global CMake variables are set here
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT>
        $<$<CONFIG:Debug>:/MTd>
        $<$<CONFIG:Release>:/MT>
    )
    add_compile_options(/utf-8)
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++ -fexperimental-library")
    endif ()
endif ()

include(${CMAKE_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake)

if (WIN32)
    include(${CMAKE_SOURCE_DIR}/external/vcpkg/scripts/cmake/vcpkg_fixup_pkgconfig.cmake)
endif()

find_package(Boost REQUIRED COMPONENTS system beast)
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)
find_package(range-v3 REQUIRED)

include_directories(include)
add_subdirectory(src)
