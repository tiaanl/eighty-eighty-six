set(SOURCE_FILES
    src/ees-dis.c
    )

add_executable(ees-dis ${SOURCE_FILES})
target_link_libraries(ees-dis PRIVATE disassembler)
set_target_properties(ees-dis PROPERTIES
    C_STANDARD 11
    C_EXTENSIONS NO
    )
