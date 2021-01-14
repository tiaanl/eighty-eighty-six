#include "instructions/instructions.h"

#include <assert.h>

void instruction_init(struct instruction *instruction) {
  memset(instruction, 0, sizeof(*instruction));
}

const char *instruction_type_to_string(enum instruction_type instruction_type) {
  static const char *mnemonics[] = {
      "aaa",    "aad",     "aam",       "aas",   "adc",   "add",   "and",   "arpl",  "bound",
      "call",   "callf",   "cbw",       "clc",   "cld",   "cli",   "cmc",   "cmp",   "cmps",
      "cwd",    "daa",     "das",       "dec",   "div",   "enter", "fwait", "hlt",   "idiv",
      "imul",   "in",      "inc",       "insb",  "int",   "int1",  "int3",  "into",  "iret",
      "jb",     "jbe",     "jcxz",      "jl",    "jle",   "jmp",   "jnb",   "jnbe",  "jnl",
      "jnle",   "jno",     "jnp",       "jns",   "jnz",   "jo",    "jp",    "js",    "jz",
      "lahf",   "lds",     "lea",       "leave", "les",   "lods",  "loop",  "loope", "loopne",
      "mov",    "movs",    "mul",       "neg",   "not",   "or",    "out",   "outs",  "pop",
      "popa",   "popf",    "push",      "pusha", "pushf", "rcl",   "rcr",   "ret",   "retf",
      "rol",    "ror",     "sahf",      "salc",  "sar",   "sbb",   "scas",  "shl",   "shr",
      "stc",    "std",     "sti",       "stos",  "sub",   "test",  "xchg",  "xlat",  "xor",

      "prefix", "<group>", "<invalid>",
  };

  assert(ARRAY_SIZE(mnemonics) == instruction_type_count);
  assert((int)instruction_type < instruction_type_count);

  return mnemonics[instruction_type];
}
