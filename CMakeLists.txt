cmake_minimum_required(VERSION 3.20)

set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)

project(Forgotten_Land)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(FL_SharedLib)
add_subdirectory(FL_Client)
add_subdirectory(FL_Server)

foreach(target FL_Client FL_Server FL_SharedLib)
    if(TARGET ${target})
        target_compile_features(${target} PUBLIC cxx_std_20)
        if(MSVC)
            target_compile_options(${target} PRIVATE /utf-8 /std:c++20)
        endif()
    endif()
endforeach()