cmake_minimum_required(VERSION 3.20)
project(Forgotten_Land)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(FL_SharedLib)
add_subdirectory(FL_Client)
add_subdirectory(FL_Server)
