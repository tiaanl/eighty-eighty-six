set(HEADER_FILES
    include/base/address.h
    include/base/platform.h
    include/base/print_format.h
    include/base/reader.h
    )

set(SOURCE_FILES
    src/reader.c
    )

add_library(base ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(base PUBLIC include)
