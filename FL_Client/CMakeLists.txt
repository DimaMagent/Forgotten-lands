
find_package(SFML COMPONENTS Graphics Window Audio System REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "source/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "source/*.hpp")

add_executable(FL_Client ${SOURCES} ${HEADERS})

target_precompile_headers(FL_Client PRIVATE source/pch.hpp)

set_target_properties(FL_Client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${TARGET_ARCH}/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${TARGET_ARCH}/Release"
)

target_link_libraries(FL_Client PRIVATE
    FL_SharedLib
    SFML::Window
    SFML::Graphics
    SFML::Audio
    SFML::System
    asio::asio
    OpenSSL::SSL
    OpenSSL::Crypto
    spdlog::spdlog
)

add_custom_command(TARGET FL_Client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:FL_Client>
        $<TARGET_FILE_DIR:FL_Client>
    COMMAND_EXPAND_LISTS
)

add_custom_command(
    TARGET FL_Client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/FL_Client/resources"
            "$<TARGET_FILE_DIR:FL_Client>"
)
add_custom_command(
    TARGET FL_Client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/sharedData/resources"
            "$<TARGET_FILE_DIR:FL_Client>"
)