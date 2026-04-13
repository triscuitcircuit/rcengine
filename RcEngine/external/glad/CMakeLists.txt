cmake_minimum_required(VERSION 3.19)
project(glad)

add_library(glad include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))