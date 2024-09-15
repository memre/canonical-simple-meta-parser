set(LIBRARY_NAME "canonical")

set(LIBRARY_SOURCES
        src/meta_info_parser.cc
        src/http_client.cc
        src/cloud_images_fetcher.cc
)
set(LIBRARY_HEADERS
        include/
)
set(LIBRARY_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${LIBRARY_NAME}
        PUBLIC ${LIBRARY_INCLUDES}
        PRIVATE .
)

target_link_libraries(
        ${LIBRARY_NAME}
        PRIVATE
        nlohmann_json::nlohmann_json
        fmt::fmt
        Boost::system
        range-v3::range-v3
)

# INSTALL TARGETS
install(
        TARGETS ${LIBRARY_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
