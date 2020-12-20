cmake_minimum_required(VERSION 3.17)
project(eight-eighty-six)

add_custom_target(assemble
    WORKING_DIRECTORY
    ${PROJECT_SOURCE_DIR}/tests
    COMMAND
    nasm -f bin small.asm && xxd -i small > ${PROJECT_BINARY_DIR}/small.c
    )

set(SOURCE_FILES
    #${PROJECT_BINARY_DIR}/small.c
    src/address.h
    src/bus.c
    src/bus.h
    src/cpu.c
    src/cpu.h
    src/cpu_decoder.c
    src/cpu_simple.c
    src/decoder/decoder.c
    src/decoder/decoder.h
    src/decoder/mod_reg_rm.h
    src/decoder/op_code_table.c
    src/decoder/op_code_table.h
    src/disassembler.c
    src/disassembler.h
    src/instruction.c
    src/instruction.h
    src/platform.h
    src/print_format.h
    src/registers.h
    )

add_executable(main src/main.c ${SOURCE_FILES})
target_compile_options(main PUBLIC -Wall -Wextra -pedantic --coverage)
add_dependencies(main assemble)

set(TEST_FILES
    src/decoder/mod_reg_rm_tests.c
    src/decoder_tests.c
    src/testing.c
    src/testing.h
    )

add_executable(testing ${TEST_FILES} ${SOURCE_FILES})
target_compile_options(main PUBLIC -Wall -Wextra -pedantic --coverage)
target_compile_definitions(testing PUBLIC -DTESTING)
