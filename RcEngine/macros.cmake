# finds and adds sources files in a folder
macro(addPath dir)
    set(tmp_files "")
    set(glob_config GLOB)
    if(${ARGC} GREATER 1 AND "${ARGV1}" STREQUAL "REC")
        set(glob_config GLOB_RECURSE)
    endif()
    set(mac_files "")
    if(APPLE)
        set(mac_files ${dir}/*.mm ${dir}/*.m)
    endif()
    file(${glob_config} tmp_files
            ${dir}/*.cpp
            ${dir}/*.c
            ${dir}/*.cc
            ${dir}/*.h
            ${mac_files}
            #${dir}/*.asm
            )
    foreach(entry ${BLACKLIST})
        list(REMOVE_ITEM tmp_files ${dir}/${entry})
    endforeach()
    LIST(APPEND ${PROJECT_NAME}_files "${tmp_files}")
    LIST(APPEND ${PROJECT_NAME}_paths "${dir}")
    message(STATUS "addPath ${PROJECT_NAME} : ${tmp_files}")
endmacro()

macro(addFile filename)
    LIST(APPEND ${PROJECT_NAME}_files "${filename}")
    message(STATUS "addFile ${PROJECT_NAME} : ${filename}")
endmacro()

# finds and adds sources files in a folder recursively
macro(addPathRec dir)
    addPath("${dir}" "REC")
endmacro()

macro(addFramework framework)
    if (APPLE)
        addLib("-framework ${framework}")
    endif()
endmacro()

macro(addLibPath dir)
    link_directories(${dir})
endmacro()

macro(addInclude incPath)
    if(TARGET ${PROJECT_NAME})
        set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY INCLUDE_DIRECTORIES "${incPath}")
    else()
        list(APPEND ${PROJECT_NAME}_includes ${incPath})
    endif()
endmacro()

macro(addLib libs)
    foreach(lib ${libs})
        # check if we can build it ourselfs
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libraries/${lib}.cmake")
            addLibSrc("${CMAKE_CURRENT_SOURCE_DIR}/libraries/${lib}.cmake")
        endif()
        # then link against it
        # two possibilities: a) target already known, so add it directly, or b) target not yet known, so add it to its cache
        if(TARGET ${PROJECT_NAME})
            target_link_libraries(${PROJECT_NAME} "${lib}")
        else()
            list(APPEND ${PROJECT_NAME}_libs ${lib})
        endif()
    endforeach()
endmacro()