configure_file(
    "config.hpp.in" "${CMAKE_BINARY_DIR}/include/config.hpp"
    ESCAPE_QUOTES)

add_subdirectory(cli)
add_subdirectory(canonical)