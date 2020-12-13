#include "disassembler.h"

#include "cpu.h"
#include "print_format.h"

#include <assert.h>
#include <stdio.h>

#define MNEMONIC "%-6s"

const char *register_encoding_to_string(enum register_encoding encoding, enum operand_size size) {
  static const char *reg_values[2][8] = {{"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"},
                                         {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"}};

  return reg_values[size][encoding];
}

const char *indirect_register_encoding_to_string(enum indirect_register_encoding encoding) {
  static const char *reg_values[8] = {
      "bx:si", "bx:di", "bp:si", "bp:di", "si", "di", "bp", "bx",
  };

  return reg_values[encoding];
}

const char *segment_register_encoding_to_string(enum segment_register_encoding encoding) {
  static const char *values[] = {
      "es",
      "cs",
      "ss",
      "ds",
  };

  return values[encoding];
}

void print_mnemonic(const struct instruction *instruction) {
  switch (instruction->type) {
    case ADD:
      printf(MNEMONIC, "add");
      break;

    case HLT:
      printf(MNEMONIC, "hlt");
      break;

    case INC:
      printf(MNEMONIC, "inc");
      break;

    case JZ:
      printf(MNEMONIC, "je");
      break;

    case LOOP:
      printf(MNEMONIC, "loop");
      break;

    case MOV:
      printf(MNEMONIC, "mov");
      break;

    case NOP:
      printf(MNEMONIC, "nop");
      break;

    case TEST:
      printf(MNEMONIC, "test");
      break;

    case PUSH:
      printf(MNEMONIC, "push");
      break;

    case POP:
      printf(MNEMONIC, "pop");
      break;

    case INT:
      printf(MNEMONIC, "int");
      break;

    case OR:
      printf(MNEMONIC, "or");
      break;

    case CMP:
      printf(MNEMONIC, "cmp");
      break;

    case JAE:
      printf(MNEMONIC, "jae");
      break;

    case RET:
      printf(MNEMONIC, "ret");
      break;

    default:
      assert(0);
  }
}

void print_immediate(const struct operand *operand) {
  switch (operand->size) {
    case OPERAND_SIZE_8:
      printf(HEX_8, operand->immediate8);
      break;

    case OPERAND_SIZE_16:
      printf(HEX_16, operand->immediate16);
      break;

    default:
      assert(0);
      break;
  }
}

void print_operand(const struct operand *operand, enum segment_register segment_register) {
  switch (operand->mode) {
    case OPERAND_MODE_INDIRECT:
      if (operand->size == OPERAND_SIZE_8) {
        printf("BYTE ");
      } else if (operand->size == OPERAND_SIZE_16) {
        printf("WORD ");
      }
      printf("[%s]", indirect_register_encoding_to_string(operand->indirect_reg));
      break;

    case OPERAND_MODE_DISPLACEMENT_8:
      printf("%d", operand->disp8);
      break;

    case OPERAND_MODE_DISPLACEMENT_16:
      printf("%d", operand->disp16);
      break;

    case OPERAND_MODE_REGISTER:
      printf("%s", register_encoding_to_string(operand->reg, operand->size));
      break;

    case OPERAND_MODE_DIRECT:
      printf("[%s:" HEX_16 "]", segment_register_to_string(segment_register), operand->disp16);
      break;

    case OPERAND_MODE_IMMEDIATE:
      print_immediate(operand);
      break;

    case OPERAND_MODE_SEGMENT_REGISTER:
      printf("%s", segment_register_encoding_to_string(operand->segment_reg));
      break;

    case OPERAND_MODE_NONE:
      break;
  }
}

void disassemble(const struct instruction *instruction) {
  print_mnemonic(instruction);

  print_operand(&instruction->destination, instruction->segment_register);

  if (instruction->source.mode != OPERAND_MODE_NONE) {
    printf(", ");
  }

  print_operand(&instruction->source, instruction->segment_register);

  printf("\n");
}

void disassemble_addr(const struct instruction *instruction, u16 addr) {
  printf(HEX_16 "  ", addr);
  disassemble(instruction);
}
