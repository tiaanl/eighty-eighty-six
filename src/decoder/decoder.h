#ifndef DECODER_H_
#define DECODER_H_

#include "../instruction.h"
#include "../memory_reader.h"

void decode_instruction(struct memory_reader *reader, struct instruction *instruction);

#endif // DECODER_H_
