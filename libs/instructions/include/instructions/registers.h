#ifndef REGISTERS_H_
#define REGISTERS_H_

enum register_8 {
#if 0 // big-endian
  AH,
  AL,
  CH,
  CL,
  DH,
  DL,
  BH,
  BL,
#else
  AL,
  AH,
  CL,
  CH,
  DL,
  DH,
  BL,
  BH,
#endif

  register_8_count,
};

enum register_16 {
  AX,
  CX,
  DX,
  BX,

  SP,
  BP,
  SI,
  DI,

  register_16_count,
};

enum segment_register {
  ES,
  CS,
  SS,
  DS,

  segment_register_count,
};

const char *register_8_to_string(enum register_8 reg);
const char *register_16_to_string(enum register_16 reg);
const char *segment_register_to_string(enum segment_register reg);

#endif // REGISTERS_H_
