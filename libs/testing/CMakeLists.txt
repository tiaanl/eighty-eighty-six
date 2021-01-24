set(HEADER_FILES
    include/testing/testing.h
    )

set(SOURCE_FILES
    src/testing.c
    )

add_library(testing ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(testing PUBLIC include)
