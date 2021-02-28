#include "disassembler/disassembler.h"

#include <assert.h>
#include <base/print_format.h>
#include <stdbool.h>
#include <stdio.h>

#define MNEMONIC "%-8s"

const char *indirect_memory_encoding_to_string(enum indirect_memory_encoding encoding) {
  static const char *mapping[8] = {
      "bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx",
  };

  return mapping[encoding];
}

int print_prefix(char *buffer, size_t buffer_size, const struct instruction *instruction) {
  switch (instruction->rep_mode) {
    case rm_rep:
      return snprintf(buffer, buffer_size, "rep ");

    case rm_repne:
      return snprintf(buffer, buffer_size, "repne ");

    case rm_none:
      break;

    default:
      assert(0);
      break;
  }

  return 0;
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

int print_indirect(char *buffer, size_t buffer_size, enum operand_size size,
                   enum segment_register segment_register, enum indirect_memory_encoding ime,
                   i16 displacement) {
  int inc = 0;

  inc += print_pointer_size(buffer, buffer_size, size);

  inc += snprintf(buffer + inc, buffer_size - inc, "%s:[%s",
                  segment_register_to_string(segment_register),
                  indirect_memory_encoding_to_string(ime));

  if (displacement == 0) {
    return inc + snprintf(buffer + inc, buffer_size - inc, "]");
  }

  if (displacement < 0) {
    inc += snprintf(buffer + inc, buffer_size - inc, "-");
    displacement *= -1;
  } else {
    inc += snprintf(buffer + inc, buffer_size - inc, "+");
  }

  switch (size) {
    case os_8:
      return inc + snprintf(buffer + inc, buffer_size - inc, HEX_8 "]", displacement);

    case os_16:
      return inc + snprintf(buffer + inc, buffer_size - inc, HEX_16 "]", displacement);

    default:
      assert(0);
      return 0;
  }
}

int print_operand(char *buffer, size_t buffer_size, const struct operand *operand, u32 offset,
                  u8 instruction_size) {
  switch (operand->type) {
    case ot_indirect: {
      return print_indirect(buffer, buffer_size, operand->size, operand->data.as_indirect.seg_reg,
                            operand->data.as_indirect.encoding, 0);
    }

    case ot_displacement: {
      return print_indirect(
          buffer, buffer_size, operand->size, operand->data.as_displacement.seg_reg,
          operand->data.as_displacement.encoding, operand->data.as_displacement.displacement);
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
                            segment_register_to_string(operand->data.as_direct.seg_reg),
                            operand->data.as_direct.address);
    }

    case ot_direct_with_segment:
      return snprintf(buffer, buffer_size, HEX_16 ":" HEX_16,
                      operand->data.as_direct_with_segment.segment,
                      operand->data.as_direct_with_segment.offset);

    case ot_immediate:
      return print_immediate(buffer, buffer_size, operand);

    case ot_segment_register:
      return snprintf(buffer, buffer_size, "%s",
                      segment_register_to_string(operand->data.as_segment_register.reg));

    case ot_jump: {
      u16 new_addr = offset + instruction_size + operand->data.as_jump.offset;
      return snprintf(buffer, buffer_size, HEX_16, new_addr);
    }

    case ot_far_jump:
      return snprintf(buffer, buffer_size, HEX_16 ":" HEX_16, operand->data.as_far_jump.segment,
                      operand->data.as_far_jump.offset);

    case ot_offset: {
      int inc = snprintf(buffer, buffer_size,
                         "%s:", segment_register_to_string(operand->data.as_offset.seg_reg));
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

    case ot_ds_si: {
      int inc = print_pointer_size(buffer, buffer_size, operand->size);
      return inc + snprintf(buffer + inc, buffer_size - inc, "ds:[si]");
    }

    case ot_es_di: {
      int inc = print_pointer_size(buffer, buffer_size, operand->size);
      return inc + snprintf(buffer + inc, buffer_size - inc, "es:[di]");
    }

    case ot_none:
      return 0;

    default:
      assert(0);
      return 0;
  }
}

int print_buffer(char *buffer, size_t buffer_size, const struct instruction *instruction) {
  int inc = 0;
  unsigned i = 0;
  for (; i < instruction->instruction_size; ++i) {
    inc += snprintf(buffer + inc, buffer_size - inc, "%02x ", instruction->buffer[i]);
  }
  for (; i < 8; ++i) {
    inc += snprintf(buffer + inc, buffer_size - inc, "   ");
  }

  return inc;
}

static enum instruction_type no_operand_mnemonics[] = {
    //    it_lodsb,
    //    it_lodsw,
    it_popf,
    it_pushf,
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
                u32 offset) {
  int inc = 0;

  inc += snprintf(buffer + inc, buffer_size - inc, HEX_16 "  ", offset);

  inc += print_buffer(buffer + inc, buffer_size - inc, instruction);

  inc += print_prefix(buffer + inc, buffer_size - inc, instruction);

  inc += print_mnemonic(buffer + inc, buffer_size - inc, instruction);

  bool mpo = must_print_operands(instruction->type);

  if (mpo) {
    inc += print_operand(buffer + inc, buffer_size - inc, &instruction->destination, offset,
                         instruction->instruction_size);

    if (instruction->source.type != ot_none) {
      inc += snprintf(buffer + inc, buffer_size - inc, ", ");
    }

    inc += print_operand(buffer + inc, buffer_size - inc, &instruction->source, offset,
                         instruction->instruction_size);
  }

  return inc;
}
