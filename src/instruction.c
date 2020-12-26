#include "instruction.h"

void instruction_init(struct instruction *instruction) {
  memset(instruction, 0, sizeof(*instruction));
}

const char *instruction_type_to_string(enum instruction_type instruction_type) {
  static const char *mnemonics[] = {
      "aaa",     "aad",       "aam",   "aas",   "adc",   "add",   "and",   "arpl",  "bound",
      "call",    "callf",     "cbw",   "clc",   "cld",   "cli",   "cmc",   "cmp",   "cmpsb",
      "cmpsw",   "cwd",       "daa",   "das",   "dec",   "enter", "fwait", "hlt",   "imul",
      "in",      "inc",       "insb",  "insw",  "int",   "int1",  "int3",  "into",  "iret",
      "jae",     "jb",        "jbe",   "jcxz",  "jl",    "jle",   "jmp",   "jnb",   "jnbe",
      "jnl",     "jnle",      "jno",   "jnp",   "jns",   "jnz",   "jo",    "jp",    "js",
      "jz",      "lahf",      "lds",   "lea",   "leave", "les",   "lodsb", "lodsw", "loop",
      "loope",   "loopne",    "mov",   "movsb", "movsw", "mul",   "neg",   "div",   "idiv",
      "noop",    "not",       "or",    "out",   "outsb", "outsw", "pop",   "popa",  "popf",
      "push",    "pusha",     "pushf", "ret",   "retf",  "sahf",  "salc",  "sar",   "sbb",
      "scasb",   "scasw",     "stc",   "std",   "sti",   "stosb", "stosw", "sub",   "test",
      "xchg",    "xlat",      "xor",

      "<group>", "<invalid>",
  };

  assert((int)instruction_type < ARRAY_SIZE(mnemonics));

  return mnemonics[instruction_type];
}
