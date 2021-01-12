#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <stddef.h>

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned long long qword;

typedef size_t usize;
typedef ptrdiff_t isize;

#define UNUSED(x) (void)(x)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static inline u8 read_u8(const u8 *buffer, unsigned buffer_size) {
  if (buffer_size == 0) {
    return 0;
  } else {
    return *buffer;
  }
}

static inline i8 read_i8(const u8 *buffer, unsigned buffer_size) {
  return (i8)read_u8(buffer, buffer_size);
}

static inline u16 read_u16(const u8 *buffer, unsigned buffer_size) {
  if (buffer_size == 0) {
    return 0;
  } else if (buffer_size == 1) {
    return *buffer;
  } else {
    return buffer[0] + (buffer[1] << 8);
  }
}

static inline i16 read_i16(const u8 *buffer, unsigned buffer_size) {
  return (i16)read_u16(buffer, buffer_size);
}

#endif // PLATFORM_H_
