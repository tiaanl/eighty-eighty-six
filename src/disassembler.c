#include "disassembler.h"

#include "cpu.h"
#include "print_format.h"

#include <assert.h>
#include <stdio.h>

#define MNEMONIC "%-6s"

const char *indirect_encoding_to_string(enum indirect_encoding encoding) {
  static const char *mapping[8] = {
      "bx:si", "bx:di", "bp:si", "bp:di", "si", "di", "bp", "bx",
  };

  return mapping[encoding];
}

void print_mnemonic(const struct instruction *instruction) {
  printf(MNEMONIC, instruction_type_to_string(instruction->type));
}

void print_immediate(const struct operand *operand) {
  switch (operand->size) {
    case OS_8:
      printf(HEX_8, operand->as_immediate.immediate_8);
      break;

    case OS_16:
      printf(HEX_16, operand->as_immediate.immediate_16);
      break;

    default:
      assert(0);
      break;
  }
}

void print_operand(const struct operand *operand, enum segment_register segment_register) {
  switch (operand->type) {
    case OT_INDIRECT:
      if (operand->size == OS_8) {
        printf("BYTE ");
      } else if (operand->size == OS_16) {
        printf("WORD ");
      }
      printf("[%s]", indirect_encoding_to_string(operand->as_indirect.encoding));
      break;

    case OT_DISPLACEMENT:
      printf("%d", operand->as_displacement.displacement);
      break;

    case OT_REGISTER:
      switch (operand->size) {
        case OS_8:
          printf("%s", register_8_to_string(operand->as_register.reg_8));
          break;

        case OS_16:
          printf("%s", register_16_to_string(operand->as_register.reg_16));
          break;

        default:
          assert(0);
      }
      break;

    case OT_DIRECT:
      printf("[%s:" HEX_16 "]", segment_register_to_string(segment_register),
             operand->as_direct.address);
      break;

    case OT_DIRECT_WITH_SEGMENT:
      printf(HEX_16 ":" HEX_16, operand->as_direct_with_segment.address.segment,
             operand->as_direct_with_segment.address.offset);
      break;

    case OT_IMMEDIATE:
      print_immediate(operand);
      break;

    case OT_SEGMENT_REGISTER:
      printf("%s", segment_register_to_string(operand->as_segment_register.reg));
      break;

    case OT_JUMP:
      printf("%d", operand->as_jump.offset);
      break;

    case OT_NONE:
      break;
  }
}

#if !defined(NDEBUG)
void print_buffer(const struct instruction *instruction) {
  unsigned i = 0;
  for (; i < instruction->instruction_size; ++i) {
    printf("%02x ", instruction->buffer[i]);
  }
  for (; i < 16; ++i) {
    printf("   ");
  }
}
#endif

void disassemble(const struct instruction *instruction, struct address address) {
  printf(HEX_16 ":" HEX_16 "  ", address.segment, address.offset);

#if !defined(NDEBUG)
  print_buffer(instruction);
#endif

  print_mnemonic(instruction);

  print_operand(&instruction->destination, instruction->segment_register);

  if (instruction->source.type != OT_NONE) {
    printf(", ");
  }

  print_operand(&instruction->source, instruction->segment_register);

  printf("\n");
}
