cmake_minimum_required(VERSION 3.19)

project(RCEngine)
set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS}  -Wall -pedantic -Wextra -m64  -mfma -ffast-math")


add_subdirectory(RcEngine)
add_subdirectory(RcGame)
add_subdirectory(RcEditor)