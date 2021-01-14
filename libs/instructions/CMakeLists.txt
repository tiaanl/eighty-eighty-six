set(HEADER_FILES
    include/instructions/instructions.h
    include/instructions/registers.h
    )

set(SOURCE_FILES
    src/instructions.c
    src/registers.c
    )

add_library(instructions ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(instructions PUBLIC include)
target_link_libraries(instructions PUBLIC base)
