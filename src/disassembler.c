#include "disassembler.h"

#include "cpu.h"
#include "print_format.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define MNEMONIC "%-6s"

const char *indirect_encoding_to_string(enum mod_rm_mem encoding) {
  static const char *mapping[8] = {
      "bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx",
  };

  return mapping[encoding];
}

int print_mnemonic(char *buffer, size_t buffer_size, const struct instruction *instruction) {
  return snprintf(buffer, buffer_size, MNEMONIC, instruction_type_to_string(instruction->type));
}

int print_immediate(char *buffer, size_t buffer_size, const struct operand *operand) {
  switch (operand->size) {
    case os_8:
      return snprintf(buffer, buffer_size, HEX_8, operand->data.as_immediate.immediate_8);

    case os_16:
      return snprintf(buffer, buffer_size, HEX_16, operand->data.as_immediate.immediate_16);

    default:
      assert(0);
      return 0;
  }
}

int print_pointer_size(char *buffer, size_t buffer_size, enum operand_size size) {
  switch (size) {
    case os_8:
      return snprintf(buffer, buffer_size, "BYTE PTR ");

    case os_16:
      return snprintf(buffer, buffer_size, "WORD PTR ");

    default:
      assert(0);
      return 0;
  }
}

int print_operand(char *buffer, size_t buffer_size, const struct operand *operand,
                  struct address address, u8 instruction_size,
                  enum segment_register segment_register) {
  switch (operand->type) {
    case ot_indirect: {
      int inc = print_pointer_size(buffer, buffer_size, operand->size);
      return inc + snprintf(buffer, buffer_size, "[%s]",
                            indirect_encoding_to_string(operand->data.as_indirect.encoding));
      break;
    }

    case ot_displacement: {
      int inc = print_pointer_size(buffer, buffer_size, operand->size);
      return inc + snprintf(buffer + inc, buffer_size - inc, "[%s+" HEX_16 "]",
                            indirect_encoding_to_string(operand->data.as_indirect.encoding),
                            operand->data.as_displacement.displacement);
    }

    case ot_register:
      switch (operand->size) {
        case os_8:
          return snprintf(buffer, buffer_size, "%s",
                          register_8_to_string(operand->data.as_register.reg_8));

        case os_16:
          return snprintf(buffer, buffer_size, "%s",
                          register_16_to_string(operand->data.as_register.reg_16));

        default:
          assert(0);
          return 0;
      }

    case ot_direct: {
      int inc = print_pointer_size(buffer, buffer_size, operand->size);
      return inc + snprintf(buffer + inc, buffer_size - inc, "%s:" HEX_16,
                            segment_register_to_string(segment_register),
                            operand->data.as_direct.address);
    }

    case ot_direct_with_segment:
      return snprintf(buffer, buffer_size, HEX_16 ":" HEX_16,
                      operand->data.as_direct_with_segment.address.segment,
                      operand->data.as_direct_with_segment.address.offset);

    case ot_immediate:
      return print_immediate(buffer, buffer_size, operand);

    case ot_segment_register:
      return snprintf(buffer, buffer_size, "%s",
                      segment_register_to_string(operand->data.as_segment_register.reg));

    case ot_jump: {
      u16 new_addr = flatten_address(address) + instruction_size + operand->data.as_jump.offset;
      return snprintf(buffer, buffer_size, HEX_16, new_addr);
    }

    case ot_far_jump:
      return snprintf(buffer, buffer_size, HEX_16 ":" HEX_16, operand->data.as_far_jump.segment,
                      operand->data.as_far_jump.offset);

    case ot_offset: {
      int inc = snprintf(buffer, buffer_size, "%s:", segment_register_to_string(segment_register));
      switch (operand->size) {
        case os_8:
          return inc +
                 snprintf(buffer + inc, buffer_size - inc, HEX_8, operand->data.as_offset.offset);

        case os_16:
          return inc +
                 snprintf(buffer + inc, buffer_size - inc, HEX_16, operand->data.as_offset.offset);

        default:
          assert(0);
          return 0;
      }
    }

    case ot_ds_si:
      return snprintf(buffer, buffer_size, "ds:si");

    case ot_es_di:
      return snprintf(buffer, buffer_size, "es:di");

    case ot_none:
      return 0;

    default:
      assert(0);
      break;
  }
}

#if !defined(NDEBUG)
int print_buffer(char *buffer, size_t buffer_size, const struct instruction *instruction) {
  int inc = 0;
  unsigned i = 0;
  for (; i < instruction->instruction_size; ++i) {
    inc += snprintf(buffer + inc, buffer_size - inc, "%02x ", instruction->buffer[i]);
  }
  for (; i < 16; ++i) {
    inc += snprintf(buffer + inc, buffer_size - inc, "   ");
  }

  return inc;
}
#endif

static enum instruction_type no_operand_mnemonics[] = {
    it_lodsb,
    it_lodsw,
};

static bool must_print_operands(enum instruction_type it) {
  for (unsigned i = 0; i < ARRAY_SIZE(no_operand_mnemonics); ++i) {
    if (it == no_operand_mnemonics[i]) {
      return false;
    }
  }

  return true;
}

int disassemble(char *buffer, size_t buffer_size, const struct instruction *instruction,
                struct address address) {
  int inc = 0;

  inc += snprintf(buffer + inc, buffer_size - inc, HEX_16 ":" HEX_16 "  ", address.segment,
                  address.offset);

#if !defined(NDEBUG)
  inc = print_buffer(buffer + inc, buffer_size - inc, instruction);
#endif

  inc += print_mnemonic(buffer + inc, buffer_size - inc, instruction);

  bool mpo = must_print_operands(instruction->type);

  if (mpo) {
    inc += print_operand(buffer + inc, buffer_size - inc, &instruction->destination, address,
                         instruction->instruction_size, instruction->segment_register);

    if (instruction->source.type != ot_none) {
      inc += snprintf(buffer + inc, buffer_size - inc, ", ");
    }

    inc += print_operand(buffer + inc, buffer_size - inc, &instruction->source, address,
                         instruction->instruction_size, instruction->segment_register);
  }

  return inc;
}
