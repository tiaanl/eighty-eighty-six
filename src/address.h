#ifndef ADDRESS_H_
#define ADDRESS_H_

#include "platform.h"

struct address {
  u16 segment;
  u16 offset;
};

static inline u32 flatten_address(struct address address) {
  return (address.segment << 4) + address.offset;
}

static inline struct address segment_offset(u16 segment, u16 offset) {
  struct address result;

  result.segment = segment;
  result.offset = offset;

  return result;
}

static inline struct address address_offset(struct address address, int offset) {
  struct address result;

  result.segment = address.segment;
  result.offset = address.offset + offset;

  return result;
}

#endif // ADDRESS_H_
