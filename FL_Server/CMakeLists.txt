find_package(SFML COMPONENTS System REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "source/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "source/*.hpp")

add_executable(FL_Server ${SOURCES} ${HEADERS})

target_precompile_headers(FL_Server PRIVATE source/pch.hpp)

set_target_properties(FL_Server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${TARGET_ARCH}/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${TARGET_ARCH}/Release"
)

target_link_libraries(FL_Server PRIVATE
    FL_SharedLib
    SFML::System
    asio::asio
    spdlog::spdlog
    libpqxx::pqxx
    OpenSSL::SSL
    OpenSSL::Crypto
)

add_custom_command(TARGET FL_Server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:FL_Server>
        $<TARGET_FILE_DIR:FL_Server>
    COMMAND_EXPAND_LISTS
)

add_custom_command(
    TARGET FL_Server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/FL_Server/resources"
            "$<TARGET_FILE_DIR:FL_Server>"
)