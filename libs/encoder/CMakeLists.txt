set(HEADER_FILES
    include/encoder/encoder.h
    )

set(SOURCE_FILES
    src/encoder.c
    )

add_library(encoder ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(encoder PUBLIC include)
target_link_libraries(encoder PUBLIC decoder)
