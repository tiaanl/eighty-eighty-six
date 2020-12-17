#ifndef DISASSEMBLER_H_
#define DISASSEMBLER_H_

#include "instruction.h"

void disassemble(const struct instruction *instruction, u16 addr);

#endif  // DISASSEMBLER_H_
