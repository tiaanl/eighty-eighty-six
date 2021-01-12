set(SOURCE_FILES
    src/ees_as.c
    )

add_executable(ees-as ${SOURCE_FILES})
target_link_libraries(ees-as PRIVATE encoder)
