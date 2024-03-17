cmake_minimum_required(VERSION 3.19)

project(RcGame)
set(CMAKE_CXX_STANDARD 17)

set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders")
set(SHADER_BINARY_DIR ${PROJECT_BINARY_DIR}/Shaders)

file(GLOB SHADERS
        ${SHADER_SOURCE_DIR}/Vulkan/*.frag
        ${SHADER_SOURCE_DIR}/Vulkan/*.vert
)

foreach(GLSL IN LISTS SHADERS)
    get_filename_component(FILE_NAME ${GLSL} NAME)
    message(${FILE_NAME})
    message(debug ${SHADER_BINARY_DIR})
    add_custom_command(
            COMMAND
            ${glslc_executable}
            -o ${SHADER_BINARY_DIR}/${FILE_NAME}.spv
            ${GLSL}
            OUTPUT ${SHADER_BINARY_DIR}/${FILE_NAME}.spv
            DEPENDS ${GLSL} ${SHADER_BINARY_DIR}
            COMMENT "COMPILING ${FILE_NAME}"
    )
    list(APPEND SPV_SHADERS ${SHADER_BINARY_DIR}/${FILE_NAME}.spv)
endforeach(GLSL)


set(SOURCE_FILES
        src/Panels/SpikeGraphPanel.cpp
        src/Panels/SpikeGraphPanel.h

        src/EditorLayer.cpp
        src/EditorLayer.h
        src/RcEditorApp.cpp

)

#Windows App configuration---------------------------------
if(WIN32)
    add_definitions(-DRC_PLATFORM_WINDOWS)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})

    add_executable (RcGame ${SOURCE_FILES})
elseif(UNIX AND NOT APPLE)
    add_definitions(-DRC_PLATFORM_UNIX)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})

    add_executable (RcGame ${SOURCE_FILES})
endif(WIN32)
#----------------------------------------------------------

#Apple App Icon configuration
if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE AppImage.icns )
    set(RCGAME_ICON ${CMAKE_CURRENT_SOURCE_DIR}/media/AppImage.icns
            src/RcEditorApp.cpp)
    set(UI_OS_EXTRAS ${RCGAME_ICON})

    set(CPACK_BUNDLE_ICON ${RCGAME_ICON})
    set(CPACK_PACKAGE_ICON ${RCGAME_ICON})

    set(ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
    set_source_files_properties(${RcGame_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    file( COPY ${ASSETS} DESTINATION "RcGame.app/Contents/Resources/" )

    # code signing---------------------------------------------
    set(RCGAME_MACOSX_CODESIGN_CERT "" CACHE STRING "Name of the Apple Developer to sign application")
    MARK_AS_ADVANCED(RCGAME_MACOSX_CODESIGN_CERT)
    set(CPACK_BUNDLE_APPLE_CERT_APP ${RCGAME_MACOSX_CODESIGN_CERT})
    #----------------------------------------------------------

    add_executable (RcGame MACOSX_BUNDLE ${RCGAME_ICON} ${SOURCE_FILES})

endif(APPLE)
#----------------------------------------------------------

include_directories("${CMAKE_SOURCE_DIR}/RcEngine/src")

target_link_libraries(RcGame RcEngine)
#find_package(implot CONFIG REQUIRED)
#
#target_link_libraries(RcGame
#        RcEngine implot::implot)