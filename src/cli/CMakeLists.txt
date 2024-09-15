set(EXECUTABLE_NAME "meta_fetcher")
set(APP_SOURCES
        main.cc
)

find_package(cxxopts REQUIRED)

add_executable(${EXECUTABLE_NAME} ${APP_SOURCES})

target_include_directories(${EXECUTABLE_NAME}
        PRIVATE
        "${CMAKE_BINARY_DIR}/include")

target_link_libraries(
        ${EXECUTABLE_NAME}
        PRIVATE "canonical"
        cxxopts::cxxopts
        Boost::system
        Boost::beast
)

install(
        TARGETS ${EXECUTABLE_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
