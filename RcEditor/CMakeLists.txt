cmake_minimum_required(VERSION 3.19)

project(RcEditor)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        src/RcEditorApp.cpp
        src/EditorLayer.h src/EditorLayer.cpp
        src/Panels/ContentBrowserPanel.cpp
        src/Panels/ContentBrowserPanel.h)

set(PANELS src/Panels/SceneHierarchyPanel.cpp
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
else()
    add_executable (RcEditor
            ${SOURCE_FILES}
            ${PANELS})

endif(APPLE)

include_directories("${CMAKE_SOURCE_DIR}/RcEngine/src")

target_link_libraries(RcEditor rcengine)