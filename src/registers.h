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

#endif // REGISTERS_H_
