cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 17)
set(LANGUAGES C CXX)

option(RCENGINE_RENDER_METAL "Build RcEngine with Metal on MacOS only" OFF)
option(RCENGINE_RENDER_OPENGL "Build RcEngine with OpenGL" ON)

if(APPLE)
    message(STATUS "Including Objective-C++ for APPLE")
    list(APPEND LANGUAGES OBJC)
    list(APPEND LANGUAGES OBJCXX)
    set(CMAKE_OBJCXX_STANDARD 14)
    set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)
    set(CMAKE_OBJCXX_EXTENSIONS OFF)
endif()

project(RcEngine LANGUAGES ${LANGUAGES})


file(GLOB source_headers
        "external/imgui/*.cpp")

include_directories("external/imgui")
include_directories(SYSTEM "external/glad/include")
include_directories(SYSTEM "external/entt/include")
include_directories("src")

add_subdirectory(external/imguizmo)
add_subdirectory(external/glm)
add_subdirectory(external/spdlog)
add_subdirectory(external/imgui)
add_subdirectory(external/SOIL2)
add_subdirectory(external/yaml-cpp)
add_subdirectory(external/glad)

#GLFW--------------------------------------------
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/GLFW)
#------------------------------------------------


#Pass arguments----------------------------------------------------
if(WIN32)
    add_definitions(-DRC_PLATFORM_WINDOWS)
    set(WINDOW
            src/Platform/Windows/WindowsWindow.cpp
            src/Platform/Windows/WindowsWindow.h
            src/Platform/MacUtils.h
            src/Platform/Windows/processordetection.cpp)

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    include_directories(GL)

endif(WIN32)
if(APPLE)
    add_definitions(-DRC_PLATFORM_MAC)
    add_definitions(-DRC_PLATFORM_UNIX)
    set(WINDOW
            src/Platform/Mac/MacWindow.cpp
            src/Platform/Mac/MacWindow.h
            src/Platform/Mac/MacUtils.mm
            src/Platform/Mac/processordetection.cpp)

    file(GLOB_RECURSE METAL "src/Platform/Metal/*.cpp" "src/Platform/Metal/*.h")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework AppKit -framework Metal -framework Cocoa")

    #Allows Objc++ code to be called from within C++ files
    #without the need for wrappers
#    set_source_files_properties(${METAL} PROPERTIES
#            COMPILE_FLAGS "-x objective-c++ ")
    set(CMAKE_C_FLAGS "-x objective-c++")

    add_subdirectory(external/corrosion)

endif(APPLE)

add_definitions(-DRC_BUILD_DLL)
add_definitions(-DGLFW_INCLUDE_NONE)

add_definitions(-DRC_DEBUG)

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
        src/Platform/processordetection.h)
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
        src/RcEngine/Renderer/Renderer2D.cpp src/RcEngine/Renderer/Renderer2D.h
        src/RcEngine/Renderer/IsoCameraController.cpp src/RcEngine/Renderer/IsoCameraController.h
        src/RcEngine/Renderer/SubTexture2D.h src/RcEngine/Renderer/SubTexture2D.cpp
        src/RcEngine/Renderer/FrameBuffer.cpp src/RcEngine/Renderer/FrameBuffer.h
        src/RcEngine/Renderer/EditorCamera.cpp src/RcEngine/Renderer/EditorCamera.h
        src/RcEngine/Renderer/Renderer3D.cpp src/RcEngine/Renderer/Renderer3D.h
        src/RcEngine/Renderer/UniformBuffer.cpp src/RcEngine/Renderer/UniformBuffer.h)
#CORE--------------------------------------------------------------
set(CORE
        src/RcEngine.h src/rcpch.cpp src/rcpch.h
        src/RcEngine/Core/Assert.h src/RcEngine/Core/Core.h
        src/RcEngine/Core/Application.h src/RcEngine/Core/Application.cpp
        src/RcEngine/Core/Timestep.h src/RcEngine/Core/Window.cpp src/RcEngine/Math/Math.cpp
        src/RcEngine/Math/Math.h)
#OPENGL------------------------------------------------------------
set(OPENGL_CXT
        src/Platform/OpenGL/OpenGLUtils.cpp src/Platform/OpenGL/OpenGLUtils.h
        src/Platform/OpenGL/OpenGLContext.h src/Platform/OpenGL/OpenGLContext.cpp
        src/Platform/OpenGL/OpenGLBuffer.h src/Platform/OpenGL/OpenGLBuffer.cpp
        src/Platform/OpenGL/OpenGLVertexArray.h src/Platform/OpenGL/OpenGLRenderAPI.h
        src/Platform/OpenGL/OpenGLRenderAPI.cpp src/Platform/OpenGL/OpenGLVertexArray.cpp
        src/Platform/OpenGL/OpenGLShader.cpp src/Platform/OpenGL/OpenGLShader.h
        src/Platform/OpenGL/OpenGLTexture.cpp src/Platform/OpenGL/OpenGLTexture.h
        src/Platform/OpenGL/OpenGLFrameBuffer.cpp src/Platform/OpenGL/OpenGLFrameBuffer.h
        src/Platform/OpenGL/OpenGLUniformBuffer.cpp src/Platform/OpenGL/OpenGLUniformBuffer.h)
#VULKAN------------------------------------------------------------
set(VK_CXT
        src/Platform/Vulkan/VKBuffer.cpp src/Platform/Vulkan/VKBuffer.h)
#SCENE-------------------------------------------------------------
set(SCENE
        src/RcEngine/Scene/Scene.h
        src/RcEngine/Scene/Scene.cpp
        src/RcEngine/Scene/Component.h src/RcEngine/Scene/Entity.h
        src/RcEngine/Scene/Entity.cpp src/RcEngine/Scene/SceneCamera.h
        src/RcEngine/Scene/SceneCamera.cpp src/RcEngine/Scene/ScriptableEntity.h
        src/RcEngine/Scene/SceneSerializer.cpp src/RcEngine/Scene/SceneSerializer.h)
#UTILS-------------------------------------------------------------
set(UTILS
        src/RcEngine/Utils/PlatformUtils.h)
#DEBUG-------------------------------------------------------------
set(DEBUG_FILE
        src/RcEngine/Debug/Instrumentor.h)
#------------------------------------------------------------------
if(APPLE)
    set(NETWORK src/RcEngine/Network/Network.h)
endif(APPLE)
#------------------------------------------------------------------
set(PCH
        src/rcpch.h)
#------------------------------------------------------------------
add_library(rcengine STATIC
        #${PCH}
        ${CORE}
        ${RENDERER}
        ${OPENGL_CXT}
        ${VK_CXT}
        ${SOURCE_FILES}
        ${GUI}
        ${source_headers}
        ${GLOB}
        ${METAL}
        ${NETWORK}
        ${SCENE}
        ${UTILS}
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
find_package(Vulkan REQUIRED)

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

target_link_libraries(rcengine
        glfw
        glm::glm
        glad
        spdlog
        OpenGL::GL
        imgui
        soil2
        yaml-cpp
        imguizmo
        Vulkan::Vulkan
        )

if(APPLE)
    target_link_libraries(rcengine
            rc_network
            )
        set_source_files_properties(${METAL} PROPERTIES
                COMPILE_FLAGS "-x objective-c++ ")
    set_property(TARGET rcengine APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")
endif()
#--------------------------------------
target_include_directories(rcengine PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))