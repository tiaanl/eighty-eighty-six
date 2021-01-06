set(SOURCE_FILES
    src/cp437.c
    src/debugger.c
    src/window_callbacks.c
    )

add_library(debugger ${SOURCE_FILES})
target_include_directories(debugger PUBLIC include)
target_compile_definitions(debugger PRIVATE -DGLFW_INCLUDE_NONE)
target_link_libraries(debugger PUBLIC cpu)
target_link_libraries(debugger PRIVATE glfw)
target_link_libraries(debugger PRIVATE glad::glad)
