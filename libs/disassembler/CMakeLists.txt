set(HEADER_FILES
    include/disassembler/disassembler.h
    )

set(SOURCE_FILES
    src/disassembler.c
    )

add_library(disassembler ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(disassembler PUBLIC include)
target_link_libraries(disassembler PUBLIC decoder)
