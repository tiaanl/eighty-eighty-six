#ifndef DISASSEMBLER_H_
#define DISASSEMBLER_H_

#include <base/address.h>
#include <decoder/instruction.h>

int disassemble(char *buffer, size_t buffer_size, const struct instruction *instruction,
                u32 offset);

#endif // DISASSEMBLER_H_
