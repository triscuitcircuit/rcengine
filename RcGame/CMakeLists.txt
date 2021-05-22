cmake_minimum_required(VERSION 3.19)

project(RcGame)
set(CMAKE_CXX_STANDARD 17)


if(WIN32)
    add_definitions(-DRC_PLATFORM_WINDOWS)
endif(WIN32)

add_executable (RcGame RcApp.cpp)

include_directories ("${CMAKE_SOURCE_DIR}/RcEngine/src")

target_link_libraries(RcGame rcengine)