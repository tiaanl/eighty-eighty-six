#include "cpu.h"

#if !defined(SIMPLE_CPU)
void cpu_run(struct cpu *cpu) {
  print_registers(cpu);
  printf("\n");

  while (1) {
    u8 buffer[16];
    memset(buffer, 0, sizeof(buffer));
    unsigned buffer_size = cpu_prefetch(cpu, buffer, sizeof(buffer));

    // hex_dump(buffer, 16);

    struct instruction instruction;
    memset(&instruction, 0, sizeof(struct instruction));

#if !defined(NDEBUG)
    memcpy(instruction.buffer, buffer, sizeof(buffer));
#endif

    int instruction_size = decode_instruction(buffer, buffer_size, &instruction);
    instruction.instruction_size = instruction_size;

    if (instruction_size < 0) {
      printf("Could not decode instructions [");
      for (unsigned i = 0; i < sizeof(buffer); ++i) {
        printf("%02x", buffer[i]);
        if (i < sizeof(buffer) - 1) {
          printf(" ");
        }
      }
      printf("]\n");
      break;
    }

    disassemble(&instruction, segment_offset(cpu->segments[CS], cpu->ip));

    cpu->ip += instruction_size;

    if (instruction.type == HLT) {
      printf("HLT");
      break;
    }

    // Interpret the instruction after adjusting the IP.

    fflush(stdout);
    interpret_instruction(cpu, &instruction);

    print_registers(cpu);
    printf("\n");
  }
}
#endif  // !defined(SIMPLE_CPU)
