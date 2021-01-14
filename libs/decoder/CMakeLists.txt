set(HEADER_FILES
    include/decoder/decoder.h
    )

set(SOURCE_FILES
    src/decoder.c
    src/decoder_funcs.c
    src/mod_rm.c
    src/op_code_table.c
    )

add_library(decoder ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(decoder PUBLIC include)
target_link_libraries(decoder PUBLIC instructions)
