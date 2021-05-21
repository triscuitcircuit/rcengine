cmake_minimum_required(VERSION 3.19)

#project(rcengine_clion)
project(RcEngine)
set(CMAKE_CXX_STANDARD 17)
file(GLOB source_headers
        "*.h"
        "*.hpp"
        "external/glm/glm/*.cpp"
        "external/glm/glm/*.hpp"
        "external/GLFW/include/GLFW/*.h"
        "external/imgui/*.h"
        "external/imgui/*.cpp"
        "external/spdlog/include/spdlog/*.h"
        )

include_directories("external/glm/glm")
include_directories("external/GLFW/include/GLFW")
include_directories("external/imgui")
include_directories("external/spdlog/include/spdlog")
include_directories("src")

if(WIN32)
    add_definitions(-DRC_PLATFORM_WINDOWS)
endif(WIN32)
add_definitions(-DRC_BUILD_DLL)
set(GLFW external/GLFW/include/GLFW/glfw3.h external/GLFW/include/GLFW/glfw3native.h)
set(SOURCE_FILES src/RcEngine.h src/rcpch.cpp src/rcpch.h src/RcEngine/Core/Assert.h src/RcEngine/Core/Core.h src/RcEngine/Core/Application.h src/RcEngine/Core/Application.cpp src/RcEngine/EntryPoint.h)

add_library(rcengine ${SOURCE_FILES} ${source_headers} ${GLFW})

target_include_directories(rcengine PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))
