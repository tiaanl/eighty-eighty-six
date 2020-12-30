#ifndef CPU_FLAGS_H_
#define CPU_FLAGS_H_

union flags {
  struct {
    byte carry;      // 0
    byte reserved_1; // 1
    byte parity;     // 2
    byte reserved_3; // 3
    byte adjust;     // 4
    byte reserved_5; // 5
    byte zero;       // 6
    byte sign;       // 7
    byte trap;       // 8
    byte interrupt;  // 9
    byte direction;  // 10
    byte overflow;   // 11
  };
  byte flag[16];
};

#endif // CPU_FLAGS_H_
