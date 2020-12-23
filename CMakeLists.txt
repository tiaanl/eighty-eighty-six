cmake_minimum_required(VERSION 3.17)
project(eight-eighty-six)

add_custom_target(assemble
    WORKING_DIRECTORY
    ${PROJECT_SOURCE_DIR}/tests
    COMMAND
    nasm -f bin small.asm && xxd -i small > ${PROJECT_BINARY_DIR}/small.c
    )

set(SOURCE_FILES
    # ${PROJECT_BINARY_DIR}/small.c
    src/mod_rm.h
    src/address.h
    src/bus.c
    src/bus.h
    src/cpu.c
    src/cpu.h
    src/cpu_decoder.c
    src/cpu_simple.c
    src/disassembler.c
    src/disassembler.h
    src/instruction.c
    src/instruction.h
    src/platform.h
    src/print_format.h
    src/registers.h
    src/memory_reader.c
    )

add_executable(main src/main.c ${SOURCE_FILES})
set_target_properties(main PROPERTIES
    C_STANDARD 11
    C_EXTENSIONS NO
    )
add_dependencies(main assemble)

set(TEST_FILES
    src/mod_rm.c
    src/testing.c
    src/testing.h
    )

add_executable(testing ${TEST_FILES} ${SOURCE_FILES})
set_target_properties(testing PROPERTIES
    C_STANDARD 11
    C_EXTENSIONS NO
    )
target_compile_definitions(testing PUBLIC -DTESTING)

add_executable(ees-dis src/ees-dis.c src/decoder/decoder.c src/decoder/op_code_table.c ${SOURCE_FILES})
set_target_properties(ees-dis PROPERTIES
    C_STANDARD 11
    C_EXTENSIONS NO
    )
