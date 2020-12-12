#ifndef REGISTERS_H_
#define REGISTERS_H_

enum register_8 {
  AL,
  CL,
  DL,
  BL,

  AH,
  CH,
  DH,
  BH,

  REGISTER_8_COUNT,
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

  IP,
  FLAGS,

  REGISTER_16_COUNT,
};

enum segment_register {
  ES,
  CS,
  SS,
  DS,

  SEGMENT_REGISTER_COUNT,
};

const char *register_8_to_string(enum register_8 reg);
const char *register_16_to_string(enum register_16 reg);

#endif // REGISTERS_H_
