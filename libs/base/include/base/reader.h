#ifndef READER_H_
#define READER_H_

#include "platform.h"

typedef u8 (*reader_func)(void *context, u32 position);

struct reader {
  void *context;
  reader_func reader_func;
};

void reader_init(struct reader *reader, void *context, reader_func reader_func);

u8 reader_fetch_u8(struct reader *reader, u32 position);
u16 reader_fetch_u16(struct reader *reader, u32 position);

i8 reader_fetch_i8(struct reader *reader, u32 position);
i16 reader_fetch_i16(struct reader *reader, u32 position);

#endif // READER_H_
