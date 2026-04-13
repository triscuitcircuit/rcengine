cmake_minimum_required(VERSION 3.19)

#find_package(Vulkan REQUIRED COMPONENTS glslc)
#find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

project(RcEditor)
set(CMAKE_CXX_STANDARD 17)

set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders")
set(SHADER_BINARY_DIR ${PROJECT_BINARY_DIR}/Shaders)

file(GLOB SHADERS
        ${SHADER_SOURCE_DIR}/Vulkan/*.frag
        ${SHADER_SOURCE_DIR}/Vulkan/*.vert
        )

#add_custom_command(
#        COMMAND
#        ${CMAKE_COMMAND} -E make_directory ${SHADERS_BINARY_DIR}
#        OUTPUT ${SHADERS_BINARY_DIR}
#        COMMENT "Creating ${SHADERS_BINARY_DIR}"
#)

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
        src/RcEditorApp.cpp
        src/EditorLayer.h src/EditorLayer.cpp
        src/Panels/ContentBrowserPanel.cpp
        src/Panels/ContentBrowserPanel.h src/ViewPanel.cpp src/ViewPanel.h)

set(PANELS
        src/Panels/SceneHierarchyPanel.cpp
        src/Panels/SceneHierarchyPanel.h)

if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE AppImage.icns )
    set(RCEDITOR_ICON ${CMAKE_CURRENT_SOURCE_DIR}/media/AppImage.icns)
    set(UI_OS_EXTRAS ${RCEDITOR_ICON})

    set(CPACK_BUNDLE_ICON ${RCEDITOR_ICON})
    set(CPACK_PACKAGE_ICON ${RCEDITOR_ICON})

    set(ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
    set_source_files_properties(${RCEDITOR_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    file( COPY ${ASSETS} DESTINATION "RcEditor.app/Contents/Resources/" )

    #CODE SIGN-----------------------------------------------
    set(RCGAME_MACOSX_CODESIGN_CERT "" CACHE STRING "Name of the Apple Developer to sign application")
    MARK_AS_ADVANCED(RCGAME_MACOSX_CODESIGN_CERT)
    set(CPACK_BUNDLE_APPLE_CERT_APP ${RCGAME_MACOSX_CODESIGN_CERT})


    add_executable (RcEditor MACOSX_BUNDLE
            ${RCEDITOR_ICON}
            ${SOURCE_FILES}
            ${PANELS}
            )

    set_target_properties(
            RcEditor PROPERTIES
            BUNDLE True
            MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifier.${CUR_TARGET}
            MACOSX_BUNDLE_BUNDLE_NAME RcEditor
            MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
            )
else()
    add_executable (RcEditor
            ${SOURCE_FILES}
            ${PANELS})

endif(APPLE)

message("test ${SPV_SHADERS}")


add_custom_target(
        shaders ALL DEPENDS ${SPV_SHADERS}
)

include_directories("${CMAKE_SOURCE_DIR}/RcEngine/src")

target_link_libraries(RcEditor RcEngine)
