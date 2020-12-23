#include "instruction.h"

void instruction_init(struct instruction *instruction) {
  memset(instruction, 0, sizeof(*instruction));
}

const char *instruction_type_to_string(enum instruction_type instruction_type) {
  static const char *mnemonics[] = {
      "<invalid>", "<noop>", "aaa",   "aad",   "aam",   "aas",   "adc",   "add",   "and",   "arpl",
      "bound",     "call",   "callf", "cbw",   "clc",   "cld",   "cli",   "cmc",   "cmp",   "cwd",
      "daa",       "das",    "dec",   "enter", "fwait", "hlt",   "imul",  "in",    "inc",   "int",
      "int1",      "int3",   "into",  "iret",  "jae",   "jb",    "jbe",   "jcxz",  "jl",    "jle",
      "jmp",       "jnb",    "jnbe",  "jnl",   "jnle",  "jno",   "jnp",   "jns",   "jnz",   "jo",
      "jp",        "js",     "jz",    "lahf",  "lds",   "lea",   "leave", "les",   "loop",  "loope",
      "loopne",    "mov",    "or",    "out",   "pop",   "popa",  "popf",  "push",  "pusha", "pushf",
      "cmpsb",     "cmpsw",  "insb",  "insw",  "lodsb", "lodsw", "movsb", "movsw", "outsb", "outsw",
      "scasb",     "scasw",  "stosb", "stosw", "ret",   "retf",  "sahf",  "salc",  "sar",   "sbb",
      "stc",       "std",    "sti",   "sub",   "test",  "xchg",  "xlat",  "xor",
  };

  return mnemonics[instruction_type];
}
