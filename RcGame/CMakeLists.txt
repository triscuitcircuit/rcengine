cmake_minimum_required(VERSION 3.19)

project(RcGame)
set(CMAKE_CXX_STANDARD 17)


set(SOURCE_FILES
        RcApp.cpp RcGame2D.cpp RcGame2D.h ParticleSystem.h ParticleSystem.cpp)

#Windows App configuration---------------------------------
if(WIN32)
    add_definitions(-DRC_PLATFORM_WINDOWS)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})

    add_executable (RcGame ${SOURCE_FILES})
endif(WIN32)
#----------------------------------------------------------

#Apple App Icon configuration
if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE AppImage.icns )
    set(RCGAME_ICON ${CMAKE_CURRENT_SOURCE_DIR}/media/AppImage.icns)
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

include_directories ("${CMAKE_SOURCE_DIR}/RcEngine/src")
include_directories ("${CMAKE_SOURCE_DIR}/RcEngine/external/glm/glm")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/RcEngine/entt/include")


target_link_libraries(RcGame rcengine)