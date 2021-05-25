cmake_minimum_required(VERSION 3.19)

project(RcEngine)


set(CMAKE_CXX_STANDARD 17)
file(GLOB source_headers
        "*.h"
        "*.hpp"
        "external/glm/glm/*.cpp"
        "external/glm/glm/*.hpp"
        "external/glm/glm/*.inl"
        "external/GLFW/include/GLFW/*.h"
        "external/imgui/*.h"
        "external/imgui/*.cpp"
        "external/spdlog/*.h"
        )

include_directories("external/glm/glm")
include_directories("external/imgui")
include_directories("src")

add_subdirectory(external/spdlog)
add_subdirectory(external/imgui)

#GLFW--------------------------------------------
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/GLFW/)
add_subdirectory(external/glad/)
#------------------------------------------------


#Pass arguments----------------------------------------------------
if(WIN32)
    add_definitions(-DRC_PLATFORM_WINDOWS)
    set(WINDOW
            src/Platform/Windows/WindowsWindow.cpp
            src/Platform/Windows/WindowsWindow.h
            )
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
endif(WIN32)
if(APPLE)
    set(WINDOW
            src/Platform/Mac/MacWindow.cpp
            src/Platform/Mac/MacWindow.h
            )

endif(APPLE)
add_definitions(-DRC_BUILD_DLL)
add_definitions(-DGLFW_INCLUDE_NONE)

if (CMAKE_BUILD_TYPE EQUAL "Debug")
    add_definitions(-DRC_ENABLE_ASSERTS)
endif()
#------------------------------------------------------------------

set(SOURCE_FILES src/RcEngine.h src/rcpch.cpp src/rcpch.h
        src/RcEngine/Core/Assert.h src/RcEngine/Core/Core.h
        src/RcEngine/Core/Application.h src/RcEngine/Core/Application.cpp
        src/RcEngine/EntryPoint.h src/RcEngine/Log.cpp
        src/RcEngine/Log.h src/RcEngine/Events/Event.h
        src/RcEngine/Events/ApplicationEvent.h src/RcEngine/Events/KeyEvent.h src/RcEngine/Events/MouseEvent.h
        src/RcEngine/Window.h ${WINDOW}
        src/RcEngine/LayerStack.cpp src/RcEngine/LayerStack.h
        src/RcEngine/Layer.cpp src/RcEngine/Layer.h src/RcEngine/ImGui/ImGuiLayer.cpp
        src/RcEngine/ImGui/ImGuiLayer.h
        src/Platform/OpenGL/imgui_impl_glfw.cpp
        src/Platform/OpenGL/imgui_impl_opengl3.cpp
        src/RcEngine/Input.h src/Platform/Mac/MacInput.cpp
        src/Platform/Mac/MacInput.h src/RcEngine/keycodes.h
        src/RcEngine/MouseButtonCodes.h)

add_library(rcengine ${SOURCE_FILES} ${source_headers} ${GLOB})


target_precompile_headers(rcengine PRIVATE src/rcpch.h)


#link with libraries-------------------
find_package(OpenGL REQUIRED)
find_package(spdlog)
find_package(imgui)
find_package(GLFW)

find_package(glad)
target_link_libraries(rcengine
        glfw
        glad
        spdlog
        OpenGL::GL
        imgui
        )
#--------------------------------------


target_include_directories(rcengine PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))
