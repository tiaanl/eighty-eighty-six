cmake_minimum_required(VERSION 3.17)
project(eight-eighty-six)

add_custom_target(assemble
    WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}/tests
    COMMAND
        /usr/bin/nasm -f bin small.asm && xxd -i small > ${PROJECT_BINARY_DIR}/small.c
    )

set(SOURCE_FILES
    ${PROJECT_BINARY_DIR}/small.c
    src/cpu.c
    src/cpu.h
    src/decoder.c
    src/decoder.h
    src/instruction.h
    src/instructions.c
    src/instructions.h
    src/main.c
    src/op_code_table.c
    src/platform.h
    src/registers.c
    src/registers.h
    )

add_executable(main ${SOURCE_FILES})
add_dependencies(main assemble)
