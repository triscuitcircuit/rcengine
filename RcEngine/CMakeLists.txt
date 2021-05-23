cmake_minimum_required(VERSION 3.19)

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
        "external/spdlog/*.h"
        )

include_directories("external/glm/glm")
include_directories("external/imgui")
include_directories("src")

add_subdirectory(external/spdlog)

#GLFW--------------------------------------------
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/GLFW)
#------------------------------------------------

find_package(spdlog)
find_package(GLFW)


#Pass arguments----------------------------------------------------
if(WIN32)
    add_definitions(-DRC_PLATFORM_WINDOWS)
endif(WIN32)
add_definitions(-DRC_BUILD_DLL)
if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    add_definitions(-DRC_ENABLE_ASSERTS)
endif()
#------------------------------------------------------------------

set(SOURCE_FILES src/RcEngine.h src/rcpch.cpp src/rcpch.h
        src/RcEngine/Core/Assert.h src/RcEngine/Core/Core.h
        src/RcEngine/Core/Application.h src/RcEngine/Core/Application.cpp
        src/RcEngine/EntryPoint.h src/RcEngine/Log.cpp
        src/RcEngine/Log.h src/RcEngine/Events/Event.h
        src/RcEngine/Events/ApplicationEvent.h src/RcEngine/Events/KeyEvent.h src/RcEngine/Events/MouseEvent.h
        src/RcEngine/Window.h src/Platform/Mac/MacWindow.cpp src/Platform/Mac/MacWindow.h)

add_library(rcengine ${SOURCE_FILES} ${source_headers})

target_precompile_headers(rcengine PRIVATE src/rcpch.h)

find_package(OpenGL REQUIRED)

#link with libraries-------------------
target_link_libraries(rcengine spdlog)
target_link_libraries(rcengine glfw)
target_link_libraries(rcengine OpenGL::GL)
#--------------------------------------

target_include_directories(rcengine PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))
