cmake_minimum_required(VERSION 3.19)

project(RcNetwork)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CARGO_CMD cargo build)
    set(TARGET_DIR "debug")
else ()
    set(CARGO_CMD cargo build --release)
    set(TARGET_DIR "release")
endif ()


set(RCNETWORK_SO "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_DIR}/RcNetwork.so")

add_custom_target(RcNetwork ALL
        COMMENT "Compiling client module"
        COMMAND CARGO_TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR} ${CARGO_CMD}
        COMMAND cp ${RCNETWORK_SO} ${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY .)
set_target_properties(RcNetwork PROPERTIES LOCATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME rcnetwork_test
        COMMAND cargo test
        WORKING_DIRECTORY .)