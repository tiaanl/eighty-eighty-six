#ifndef DECODER_H_
#define DECODER_H_

#include <base/reader.h>
#include <instructions/instructions.h>

int decode_instruction(struct reader *reader, u32 position, struct instruction *instruction);

#endif // DECODER_H_
