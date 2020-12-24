#ifndef MEMORY_READER_H_
#define MEMORY_READER_H_

#include "platform.h"

typedef u8 (*fetch_func)(void *context, u32 position);

struct memory_reader {
  void *context;
  fetch_func fetch_func;
  u32 position;
};

void memory_reader_init(struct memory_reader *memory_reader, void *context, fetch_func fetch_func);

u8 memory_reader_fetch_u8(struct memory_reader *memory_reader);
u16 memory_reader_fetch_u16(struct memory_reader *memory_reader);

i8 memory_reader_fetch_i8(struct memory_reader *memory_reader);
i16 memory_reader_fetch_i16(struct memory_reader *memory_reader);

#endif
