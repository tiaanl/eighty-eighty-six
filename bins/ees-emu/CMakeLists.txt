set(SOURCE_FILES
    src/ees-emu.c
    )

add_executable(ees-emu ${SOURCE_FILES})
target_link_libraries(ees-emu PRIVATE cpu)
