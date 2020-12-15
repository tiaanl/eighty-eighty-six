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
  static const char *mnemonics[] = {
      "aaa",       "aad",       "aam",       "aas",       "adc",       "add",       "and",
      "arpl",      "bound",     "call",      "callf",     "cbw",       "clc",       "cld",
      "cli",       "cmc",       "cmp",       "cwd",       "daa",       "das",       "dec",
      "enter",     "fwait",     "hlt",       "imul",      "in",        "inc",       "int",
      "int 1",     "int 3",     "into",      "iret",      "jae",       "jb",        "jbe",
      "jcxz",      "jl",        "jle",       "jmp",       "jmpf",      "jnb",       "jnbe",
      "jnl",       "jnle",      "jno",       "jnp",       "jns",       "jnz",       "jo",
      "jp",        "js",        "jz",        "lahf",      "lds",       "lea",       "leave",
      "les",       "loop",      "loope",     "loopne",    "mov",       "nop",       "or",
      "out",       "pop",       "popa",      "popf",      "push",      "pusha",     "pushf",
      "rep cmpsb", "rep cmpsw", "rep insb",  "rep insw",  "rep lodsb", "rep lodsw", "rep movsb",
      "rep movsw", "rep outsb", "rep outsw", "rep scasb", "rep scasw", "rep stosb", "rep stosw",
      "ret",       "retf",      "sahf",      "salc",      "sar",       "sbb",       "stc",
      "std",       "sti",       "sub",       "test",      "xchg",      "xlat",      "xor",
  };

  printf(MNEMONIC, mnemonics[instruction->type]);
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

void disassemble(const struct instruction *instruction) {
  print_mnemonic(instruction);

  print_operand(&instruction->destination, instruction->segment_register);

  if (instruction->source.type != OT_NONE) {
    printf(", ");
  }

  print_operand(&instruction->source, instruction->segment_register);

  printf("\n");
}

void disassemble_addr(const struct instruction *instruction, u16 addr) {
  printf(HEX_16 "  ", addr);
  disassemble(instruction);
}
