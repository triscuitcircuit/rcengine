cmake_minimum_required(VERSION 3.19)

project(RcGame)
set(CMAKE_CXX_STANDARD 17)


if(WIN32)
    add_definitions(-DRC_PLATFORM_WINDOWS)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})
endif(WIN32)
if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE AppImage.icns )
    set(RcGame_ICON ${CMAKE_CURRENT_SOURCE_DIR}/media/AppImage.icns)
    set(ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
    set_source_files_properties(${RcGame_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    file( COPY ${ASSETS} DESTINATION "RcGame.app/Contents/Resources/" )
endif()

if(APPLE)
        add_executable (RcGame MACOSX_BUNDLE ${RcGame_ICON} RcApp.cpp)
    else()
        add_executable (RcGame RcApp.cpp)
endif()
include_directories ("${CMAKE_SOURCE_DIR}/RcEngine/src")
include_directories ("${CMAKE_SOURCE_DIR}/RcEngine/external/glm/glm")


target_link_libraries(RcGame rcengine)