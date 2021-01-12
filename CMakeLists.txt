cmake_minimum_required(VERSION 3.17)
project(eight-eighty-six)

add_subdirectory(bins)
add_subdirectory(libs)

add_custom_target(assemble
    WORKING_DIRECTORY
    ${PROJECT_SOURCE_DIR}/tests
    COMMAND
    nasm -f bin small.asm && xxd -i small > ${PROJECT_BINARY_DIR}/small.c
    )
