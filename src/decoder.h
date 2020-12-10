#ifndef DECODER_H_
#define DECODER_H_

#include "instruction.h"
#include "platform.h"

int decode_instruction(const u8 *buffer, struct instruction *instruction);

#endif // DECODER_H_
