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
include_directories(SYSTEM "external/glad/include")
include_directories("external/SOIL2")
include_directories("src")

add_subdirectory(external/spdlog)
add_subdirectory(external/imgui)
add_subdirectory(external/SOIL2)
add_subdirectory(external/corrosion)

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
    include_directories(GL)

endif()
if(APPLE)
    add_definitions(-DRC_PLATFORM_UNIX)
    set(WINDOW
        src/Platform/Mac/MacWindow.cpp
        src/Platform/Mac/MacWindow.h
        )
endif()

add_definitions(-DRC_BUILD_DLL)
add_definitions(-DGLFW_INCLUDE_NONE)

if (CMAKE_BUILD_TYPE EQUAL "Debug")
    add_definitions(-DRC_DEBUG)
endif()
#SOURCE-------------------------------------------------------------

set(SOURCE_FILES
        src/RcEngine/Core/EntryPoint.h src/RcEngine/Core/Log.cpp
        src/RcEngine/Core/Log.h src/RcEngine/Events/Event.h
        src/RcEngine/Events/ApplicationEvent.h src/RcEngine/Events/KeyEvent.h
        src/RcEngine/Events/MouseEvent.h
        src/RcEngine/Core/Window.h ${WINDOW}
        src/RcEngine/Core/LayerStack.cpp src/RcEngine/Core/LayerStack.h
        src/RcEngine/Core/Layer.cpp src/RcEngine/Core/Layer.h
        src/RcEngine/Core/Input.h src/Platform/Mac/MacInput.cpp
        src/Platform/Mac/MacInput.h src/RcEngine/Core/keycodes.h
        src/RcEngine/Core/MouseButtonCodes.h
        )
#GUI---------------------------------------------------------------
set(GUI
        src/RcEngine/ImGui/ImGuiLayer.cpp
        src/RcEngine/ImGui/ImGuiLayer.h
        src/RcEngine/ImGui/ImGuiBuild.cpp
)
#RENDERER----------------------------------------------------------
set(RENDERER
        src/RcEngine/Renderer/Shader.h src/RcEngine/Renderer/Shader.cpp
        src/RcEngine/Renderer/Buffer.h
        src/RcEngine/Renderer/Buffer.cpp src/RcEngine/Renderer/Renderer.h
        src/RcEngine/Renderer/Renderer.cpp
        src/RcEngine/Renderer/VertexArray.h src/RcEngine/Renderer/VertexArray.cpp
        src/RcEngine/Renderer/RenderAPI.h src/RcEngine/Renderer/RenderAPI.cpp
        src/RcEngine/Renderer/RenderCommand.h src/RcEngine/Renderer/RenderCommand.cpp
        src/RcEngine/Renderer/Camera.h src/RcEngine/Renderer/OrthoCamera.cpp
        src/RcEngine/Renderer/OrthoCamera.h
        src/RcEngine/Renderer/GraphicsContext.h
        src/RcEngine/Renderer/GraphicsContext.cpp
        src/RcEngine/Renderer/Texture.cpp src/RcEngine/Renderer/Texture.h
        src/RcEngine/Renderer/OrthoCameraController.h
        src/RcEngine/Renderer/OrthoCameraController.cpp
        src/RcEngine/Renderer/ProjectionCamera.cpp src/RcEngine/Renderer/ProjectionCamera.h
        src/RcEngine/Renderer/Renderer2D.cpp src/RcEngine/Renderer/Renderer2D.h)
#CORE--------------------------------------------------------------
set(CORE
        src/RcEngine.h src/rcpch.cpp src/rcpch.h
        src/RcEngine/Core/Assert.h src/RcEngine/Core/Core.h
        src/RcEngine/Core/Application.h src/RcEngine/Core/Application.cpp
        src/RcEngine/Core/Timestep.h)
#------------------------------------------------------------------
set(OPENGL_CXT
        src/Platform/OpenGL/OpenGLUtils.cpp src/Platform/OpenGL/OpenGLUtils.h
        src/Platform/OpenGL/OpenGLContext.h src/Platform/OpenGL/OpenGLContext.cpp
        src/Platform/OpenGL/OpenGLBuffer.h src/Platform/OpenGL/OpenGLBuffer.cpp
        src/Platform/OpenGL/OpenGLVertexArray.h src/Platform/OpenGL/OpenGLRenderAPI.h
        src/Platform/OpenGL/OpenGLRenderAPI.cpp src/Platform/OpenGL/OpenGLVertexArray.cpp
        src/Platform/OpenGL/OpenGLShader.cpp src/Platform/OpenGL/OpenGLShader.h
        src/Platform/OpenGL/OpenGLTexture.cpp src/Platform/OpenGL/OpenGLTexture.h)

#DEBUG-------------------------------------------------------------
set(DEBUG_FILE
        src/RcEngine/Debug/Instrumentor.h)
#------------------------------------------------------------------
if(APPLE)
    set(NETWORK src/RcEngine/Network/Network.h)
endif()
#------------------------------------------------------------------
add_library(rcengine STATIC
        ${CORE}
        ${RENDERER}
        ${OPENGL_CXT}
        ${SOURCE_FILES}
        ${GUI}
        ${source_headers}
        ${GLOB}
        ${NETWORK}
        )
#------------------------------------------------------------------
if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    message("DEBUG BUILD")
    add_library(rcengine STATIC
            ${DEBUG_FILE}
            )
endif()
#------------------------------------------------------------------
target_precompile_headers(rcengine PRIVATE src/rcpch.h)


#link with libraries-------------------
find_package(OpenGL REQUIRED)
find_package(spdlog)
find_package(imgui)
find_package(soil2)
find_package(GLFW)
find_package(glad)

#Rust library link---------------------
if(APPLE)
    corrosion_import_crate(MANIFEST_PATH external/RcNetwork/Cargo.toml)
    set(SDK_LOC /Volumes/MacStorage/sdk)
    set_property(
            TARGET rc_network
            APPEND
            PROPERTY CORROSION_ENVIRONMENT_VARIABLES
            "STEAM_SDK_LOCATION=${SDK_LOC}"
    )
endif()
#--------------------------------------
find_package(glad)
target_link_libraries(rcengine
        glfw
        glad
        spdlog
        OpenGL::GL
        imgui
        soil2
        )
if(APPLE)
    target_link_libraries(rcengine
            rc_network
            )
endif()
#--------------------------------------


target_include_directories(rcengine PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))
