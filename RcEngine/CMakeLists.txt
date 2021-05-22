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
include_directories("external/GLFW/include/GLFW")
include_directories("external/imgui")
include_directories("src")

add_subdirectory(external/spdlog)
find_package(spdlog)



#Pass arguments---------------------------------------------------
if(WIN32)
    add_definitions(-DRC_PLATFORM_WINDOWS)
endif(WIN32)
add_definitions(-DRC_BUILD_DLL)
#------------------------------------------------------------------

set(GLFW external/GLFW/include/GLFW/glfw3.h
        external/GLFW/include/GLFW/glfw3native.h)

set(SOURCE_FILES src/RcEngine.h src/rcpch.cpp src/rcpch.h
        src/RcEngine/Core/Assert.h src/RcEngine/Core/Core.h
        src/RcEngine/Core/Application.h src/RcEngine/Core/Application.cpp
        src/RcEngine/EntryPoint.h src/RcEngine/Log.cpp
        src/RcEngine/Log.h src/RcEngine/Events/Event.h src/RcEngine/Events/ApplicationEvent.h src/RcEngine/Events/KeyEvent.h src/RcEngine/Events/MouseEvent.h)

add_library(rcengine ${SOURCE_FILES} ${source_headers} ${GLFW})

#link with libraries-------------------
target_link_libraries(rcengine spdlog)
#--------------------------------------

target_include_directories(rcengine PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))
