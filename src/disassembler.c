#include "disassembler.h"

#include "cpu.h"
#include "print_format.h"

#include <assert.h>
#include <stdio.h>

#define MNEMONIC "%-6s"

const char *indirect_encoding_to_string(enum mod_rm_mem encoding) {
  static const char *mapping[8] = {
      "bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx",
  };

  return mapping[encoding];
}

void print_mnemonic(const struct instruction *instruction) {
  printf(MNEMONIC, instruction_type_to_string(instruction->type));
}

void print_immediate(const struct operand *operand) {
  switch (operand->size) {
    case os_8:
      printf(HEX_8, operand->data.as_immediate.immediate_8);
      break;

    case os_16:
      printf(HEX_16, operand->data.as_immediate.immediate_16);
      break;

    default:
      assert(0);
      break;
  }
}

void print_pointer_size(enum operand_size size) {
  switch (size) {
    case os_8:
      printf("BYTE PTR ");
      break;

    case os_16:
      printf("WORD PTR ");
      break;

    default:
      assert(0);
      break;
  }
}

void print_operand(const struct operand *operand, struct address address, u8 instruction_size,
                   enum segment_register segment_register) {
  switch (operand->type) {
    case ot_indirect:
      print_pointer_size(operand->size);
      printf("[%s]", indirect_encoding_to_string(operand->data.as_indirect.encoding));
      break;

    case ot_displacement:
      print_pointer_size(operand->size);
      printf("[%s+" HEX_16 "]", indirect_encoding_to_string(operand->data.as_indirect.encoding),
             operand->data.as_displacement.displacement);
      break;

    case ot_register:
      switch (operand->size) {
        case os_8:
          printf("%s", register_8_to_string(operand->data.as_register.reg_8));
          break;

        case os_16:
          printf("%s", register_16_to_string(operand->data.as_register.reg_16));
          break;

        default:
          assert(0);
      }
      break;

    case ot_direct:
      print_pointer_size(operand->size);
      printf("%s:" HEX_16, segment_register_to_string(segment_register),
             operand->data.as_direct.address);
      break;

    case ot_direct_with_segment:
      printf(HEX_16 ":" HEX_16, operand->data.as_direct_with_segment.address.segment,
             operand->data.as_direct_with_segment.address.offset);
      break;

    case ot_immediate:
      print_immediate(operand);
      break;

    case ot_segment_register:
      printf("%s", segment_register_to_string(operand->data.as_segment_register.reg));
      break;

    case ot_jump: {
      u16 new_addr = flatten_address(address) + instruction_size + operand->data.as_jump.offset;
      printf(HEX_16, new_addr);
      break;
    }

    case ot_far_jump:
      printf(HEX_16 ":" HEX_16, operand->data.as_far_jump.segment,
             operand->data.as_far_jump.offset);
      break;

    case ot_offset:
      printf("%s:", segment_register_to_string(segment_register));
      switch (operand->size) {
        case os_8:
          printf(HEX_8, operand->data.as_offset.offset);
          break;

        case os_16:
          printf(HEX_16, operand->data.as_offset.offset);
          break;

        default:
          assert(0);
          break;
      }

      break;

    case ot_ds_si:
      printf("ds:si");
      break;

    case ot_es_di:
      printf("es:di");
      break;

    case ot_none:
      break;

    default:
      assert(0);
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

  print_operand(&instruction->destination, address, instruction->instruction_size,
                instruction->segment_register);

  if (instruction->source.type != ot_none) {
    printf(", ");
  }

  print_operand(&instruction->source, address, instruction->instruction_size,
                instruction->segment_register);

  printf("\n");
}
