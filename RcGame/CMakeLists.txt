cmake_minimum_required(VERSION 3.19)

#project(rcengine_clion)
project(RcGame)
set(CMAKE_CXX_STANDARD 17)


add_executable (RcGame RcApp.cpp)

include_directories ("${CMAKE_SOURCE_DIR}/RcEngine/src")

target_link_libraries(RcGame rcengine)