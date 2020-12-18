#ifndef DISASSEMBLER_H_
#define DISASSEMBLER_H_

#include "address.h"
#include "instruction.h"

void disassemble(const struct instruction *instruction, struct address address);

#endif // DISASSEMBLER_H_
