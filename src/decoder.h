#ifndef DECODER_H_
#define DECODER_H_

#include "instruction.h"

int decode_instruction(const u8 *buffer, unsigned buffer_size, struct instruction *instruction);

#endif // DECODER_H_
