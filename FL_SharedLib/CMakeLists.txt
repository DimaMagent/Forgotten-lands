find_package(asio CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(SFML COMPONENTS System REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "source/*.cpp")

add_library(FL_SharedLib STATIC ${SOURCES})

target_precompile_headers(FL_SharedLib PRIVATE include/pch.h)

set_target_properties(FL_SharedLib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${TARGET_ARCH}/Debug"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${TARGET_ARCH}/Release"
)

target_include_directories(FL_SharedLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(FL_SharedLib PUBLIC
    SFML::System
    asio::asio
    nlohmann_json::nlohmann_json
)