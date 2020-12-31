set(HEADER_FILES
    include/cpu/bus.h
    include/cpu/cpu.h
    include/cpu/flags.h
    include/cpu/ports.h
    )

set(SOURCE_FILES
    src/bus.c
    src/cpu.c
    src/instr_map.c
    src/ports.c
    )

add_library(cpu ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(cpu PUBLIC include)
target_link_libraries(cpu PUBLIC decoder)
target_link_libraries(cpu PUBLIC disassembler)
