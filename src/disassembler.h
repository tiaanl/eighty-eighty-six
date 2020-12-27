#ifndef DISASSEMBLER_H_
#define DISASSEMBLER_H_

#include "address.h"
#include "instruction.h"

int disassemble(char *buffer, size_t buffer_size, const struct instruction *instruction,
                struct address address);

#endif // DISASSEMBLER_H_
