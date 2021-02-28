set(HEADER_FILES
    include/decoder/decoder.h
    )

set(SOURCE_FILES
    src/decoder.c
    src/decoder_context.c
    src/decoder_funcs.c
    src/op_code_table.c
    )

add_library(decoder ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(decoder PUBLIC include)
target_link_libraries(decoder PUBLIC instructions)

add_executable(decoder_tests tests/decoder_tests.c tests/mod_rm_tests.c)
target_compile_definitions(decoder_tests PRIVATE -DTESTING)
target_link_libraries(decoder_tests PRIVATE decoder testing)
