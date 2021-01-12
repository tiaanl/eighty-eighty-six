#ifndef ENCODER_ENCODER_H_
#define ENCODER_ENCODER_H_

#include <decoder/instruction.h>

int encode(const struct instruction *instruction, u8 *buffer, usize buffer_size);

#endif // ENCODER_ENCODER_H_
