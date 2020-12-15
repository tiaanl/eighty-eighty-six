#include "op_code_table.h"

#define DT_Xb 0xff // Memory addressed by the DS:SI register pair.
#define DT_Xw 0xff // Memory addressed by the DS:SI register pair.
#define DT_Yb 0xff // Memory addressed by the ES:DI register pair.
#define DT_Yw 0xff // Memory addressed by the ES:DI register pair.

#define DT_Mp 0xff // The ModR/M byte may refer only to memory.
#define DT_Ma 0xff // The ModR/M byte may refer only to memory.

#define RX 0xff // The mod field of the ModR/M byte may refer only to a general register.

#define DT_Ap 0xff // Direct address (p = 32 or 48 bit pointer size)

#define DT_Od 0xff

#define I1 0xff

#define DT_DX 0xff

#define DT_Fw 0xff // Flag register (16-bit)

// clang-format off
struct op_code_mapping op_code_table[] = {
    /* 00 */ {ADD,       DT_MEM_8,   DT_REG_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 01 */ {ADD,       DT_MEM_16,  DT_REG_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 02 */ {ADD,       DT_REG_8,   DT_MEM_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 03 */ {ADD,       DT_REG_16,  DT_MEM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 04 */ {ADD,       DT_REG_AL,  DT_IMM_8,   DT_NONE,   0},
    /* 05 */ {ADD,       DT_REG_AX,  DT_IMM_16,  DT_NONE,   0},
    /* 06 */ {PUSH,      DT_SEG_REG, DT_NONE,    DT_NONE,   0},
    /* 07 */ {POP,       DT_SEG_REG, DT_NONE,    DT_NONE,   0},
    /* 08 */ {OR,        DT_MEM_8,   DT_REG_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 09 */ {OR,        DT_MEM_16,  DT_REG_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 0a */ {OR,        DT_REG_8,   DT_MEM_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 0b */ {OR,        DT_REG_16,  DT_MEM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 0c */ {OR,        DT_REG_AL,  DT_IMM_8,   DT_NONE,   0},
    /* 0d */ {OR,        DT_REG_AX,  DT_IMM_16,  DT_NONE,   0},
    /* 0e */ {PUSH,      DT_SEG_REG, DT_NONE,    DT_NONE,   0},
    /* 0f */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},

    /* 10 */ {ADC,       DT_MEM_8,   DT_REG_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 11 */ {ADC,       DT_MEM_16,  DT_REG_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 12 */ {ADC,       DT_REG_8,   DT_MEM_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 13 */ {ADC,       DT_REG_16,  DT_MEM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 14 */ {ADC,       DT_REG_AL,  DT_IMM_8,   DT_NONE,   0},
    /* 15 */ {ADC,       DT_REG_AX,  DT_IMM_16,  DT_NONE,   0},
    /* 16 */ {PUSH,      DT_SEG_REG, DT_NONE,    DT_NONE,   0},
    /* 17 */ {POP,       DT_SEG_REG, DT_NONE,    DT_NONE,   0},
    /* 18 */ {SBB,       DT_MEM_8,   DT_REG_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 19 */ {SBB,       DT_MEM_16,  DT_REG_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 1a */ {SBB,       DT_REG_8,   DT_MEM_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 1b */ {SBB,       DT_REG_16,  DT_MEM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 1c */ {SBB,       DT_REG_AL,  DT_IMM_8,   DT_NONE,   0},
    /* 1d */ {SBB,       DT_REG_AX,  DT_IMM_16,  DT_NONE,   0},
    /* 1e */ {PUSH,      DT_SEG_REG, DT_NONE,    DT_NONE,   0},
    /* 1f */ {POP,       DT_SEG_REG, DT_NONE,    DT_NONE,   0},

    /* 20 */ {AND,       DT_MEM_8,   DT_REG_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 21 */ {AND,       DT_MEM_16,  DT_REG_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 22 */ {AND,       DT_REG_8,   DT_MEM_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 23 */ {AND,       DT_REG_16,  DT_MEM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 24 */ {AND,       DT_REG_AL,  DT_IMM_8,   DT_NONE,   0},
    /* 25 */ {AND,       DT_REG_AX,  DT_IMM_16,  DT_NONE,   0},
    /* 26 */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // Segment register override (ES)
    /* 27 */ {DAA,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* 28 */ {SUB,       DT_MEM_8,   DT_REG_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 29 */ {SUB,       DT_MEM_16,  DT_REG_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 2a */ {SUB,       DT_REG_8,   DT_MEM_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 2b */ {SUB,       DT_REG_16,  DT_MEM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 2c */ {SUB,       DT_REG_AL,  DT_IMM_8,   DT_NONE,   0},
    /* 2d */ {SUB,       DT_REG_AX,  DT_IMM_16,  DT_NONE,   0},
    /* 2e */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // Segment register override (CS)
    /* 2f */ {DAS,       DT_NONE,    DT_NONE,    DT_NONE,   0},

    /* 30 */ {XOR,       DT_MEM_8,   DT_REG_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 31 */ {XOR,       DT_MEM_16,  DT_REG_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 32 */ {XOR,       DT_REG_8,   DT_MEM_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 33 */ {XOR,       DT_REG_16,  DT_MEM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 34 */ {XOR,       DT_REG_AL,  DT_IMM_8,   DT_NONE,   0},
    /* 35 */ {XOR,       DT_REG_AX,  DT_IMM_16,  DT_NONE,   0},
    /* 36 */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // Segment register override (SS)
    /* 37 */ {AAA,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* 38 */ {CMP,       DT_MEM_8,   DT_REG_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 39 */ {CMP,       DT_MEM_16,  DT_REG_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 3a */ {CMP,       DT_REG_8,   DT_MEM_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 3b */ {CMP,       DT_REG_16,  DT_MEM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 3c */ {CMP,       DT_REG_AL,  DT_IMM_8,   DT_NONE,   0},
    /* 3d */ {CMP,       DT_REG_AX,  DT_IMM_16,  DT_NONE,   0},
    /* 3e */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // Segment register override (DS)
    /* 3f */ {AAS,       DT_NONE,    DT_NONE,    DT_NONE,   0},

    /* 40 */ {INC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 41 */ {INC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 42 */ {INC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 43 */ {INC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 44 */ {INC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 45 */ {INC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 46 */ {INC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 47 */ {INC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 48 */ {DEC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 49 */ {DEC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 4a */ {DEC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 4b */ {DEC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 4c */ {DEC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 4d */ {DEC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 4e */ {DEC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 4f */ {DEC,       DT_DST_16,  DT_NONE,    DT_NONE,   0},

    /* 50 */ {PUSH,      DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 51 */ {PUSH,      DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 52 */ {PUSH,      DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 53 */ {PUSH,      DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 54 */ {PUSH,      DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 55 */ {PUSH,      DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 56 */ {PUSH,      DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 57 */ {PUSH,      DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 58 */ {POP,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 59 */ {POP,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 5a */ {POP,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 5b */ {POP,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 5c */ {POP,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 5d */ {POP,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 5e */ {POP,       DT_DST_16,  DT_NONE,    DT_NONE,   0},
    /* 5f */ {POP,       DT_DST_16,  DT_NONE,    DT_NONE,   0},

    /* 60 */ {PUSHA,     DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* 61 */ {POPA,      DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* 62 */ {BOUND,     DT_REG_16,  DT_Ma,      DT_NONE,   0},
    /* 63 */ {ARPL,      DT_MEM_16,  DT_REG_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 64 */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // Segment override:              FS??
    /* 65 */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // Segment override:              GS??
    /* 66 */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // Operand size override
    /* 67 */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // Operand address size override
    /* 68 */ {PUSH,      DT_IMM_16,  DT_NONE,    DT_NONE,   0},
    /* 69 */ {IMUL,      DT_REG_16,  DT_MEM_16,  DT_IMM_16, DF_HAS_MOD_RM},
    /* 6a */ {PUSH,      DT_IMM_8,   DT_NONE,    DT_NONE,   0},
    /* 6b */ {IMUL,      DT_REG_16,  DT_MEM_16,  DT_IMM_8,  DF_HAS_MOD_RM},
    /* 6c */ {REP_INSB,  DT_Yb,      DT_DX,      DT_NONE,   0},
    /* 6d */ {REP_INSW,  DT_Yw,      DT_DX,      DT_NONE,   0},
    /* 6e */ {REP_OUTSB, DT_DX,      DT_Xb,      DT_NONE,   0},
    /* 6f */ {REP_OUTSW, DT_DX,      DT_Xw,      DT_NONE,   0},

    /* 70 */ {JO,        DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 71 */ {JNO,       DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 72 */ {JB,        DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 73 */ {JNB,       DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 74 */ {JZ,        DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 75 */ {JNZ,       DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 76 */ {JBE,       DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 77 */ {JNBE,      DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 78 */ {JS,        DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 79 */ {JNS,       DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 7a */ {JP,        DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 7b */ {JNP,       DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 7c */ {JL,        DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 7d */ {JNL,       DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 7e */ {JLE,       DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* 7f */ {JNLE,      DT_JMP_8,   DT_NONE,    DT_NONE,   0},

    /* 80 */ {CMP,       DT_MEM_8,   DT_IMM_8,   DT_NONE,   DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 81 */ {CMP,       DT_MEM_16,  DT_IMM_16,  DT_NONE,   DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 82 */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   DF_HAS_MOD_RM}, // Copy of 80??!
    /* 83 */ {CMP,       DT_MEM_16,  DT_IMM_8,   DT_NONE,   DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 84 */ {TEST,      DT_MEM_8,   DT_REG_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 85 */ {TEST,      DT_MEM_16,  DT_REG_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 86 */ {XCHG,      DT_MEM_8,   DT_REG_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 87 */ {XCHG,      DT_MEM_16,  DT_REG_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 88 */ {MOV,       DT_MEM_8,   DT_REG_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 89 */ {MOV,       DT_MEM_16,  DT_REG_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 8a */ {MOV,       DT_REG_8,   DT_MEM_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* 8b */ {MOV,       DT_REG_16,  DT_MEM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 8c */ {MOV,       DT_MEM_16,  DT_SEG_REG, DT_NONE,   DF_HAS_MOD_RM},
    /* 8d */ {LEA,       DT_REG_16,  DT_Mp,      DT_NONE,   DF_HAS_MOD_RM},
    /* 8e */ {MOV,       DT_SEG_REG, DT_MEM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* 8f */ {POP,       DT_MEM_16,  DT_NONE,    DT_NONE,   DF_HAS_MOD_RM}, // XOP prefix? ?!?!?

    /* 90 */ {XCHG,      RX,         DT_REG_AX,  DT_NONE,   0},
    /* 91 */ {XCHG,      RX,         DT_REG_AX,  DT_NONE,   0},
    /* 92 */ {XCHG,      RX,         DT_REG_AX,  DT_NONE,   0},
    /* 93 */ {XCHG,      RX,         DT_REG_AX,  DT_NONE,   0},
    /* 94 */ {XCHG,      RX,         DT_REG_AX,  DT_NONE,   0},
    /* 95 */ {XCHG,      RX,         DT_REG_AX,  DT_NONE,   0},
    /* 96 */ {XCHG,      RX,         DT_REG_AX,  DT_NONE,   0},
    /* 97 */ {XCHG,      RX,         DT_REG_AX,  DT_NONE,   0},
    /* 98 */ {CBW,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* 99 */ {CWD,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* 9a */ {CALLF,     DT_Ap,      DT_NONE,    DT_NONE,   0},
    /* 9b */ {FWAIT,     DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* 9c */ {PUSHF,     DT_Fw,      DT_NONE,    DT_NONE,   0},
    /* 9d */ {POPF,      DT_Fw,      DT_NONE,    DT_NONE,   0},
    /* 9e */ {SAHF,      DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* 9f */ {LAHF,      DT_NONE,    DT_NONE,    DT_NONE,   0},

    /* a0 */ {MOV,       DT_REG_AL,  DT_IMM_16,  DT_NONE,   0},
    /* a1 */ {MOV,       DT_REG_AX,  DT_IMM_16,  DT_NONE,   0},
    /* a2 */ {MOV,       DT_Od,      DT_REG_AL,  DT_NONE,   0},
    /* a3 */ {MOV,       DT_Od,      DT_REG_AX,  DT_NONE,   0},
    /* a4 */ {REP_MOVSB, DT_Yb,      DT_Xb,      DT_NONE,   0},
    /* a5 */ {REP_MOVSW, DT_Yw,      DT_Xw,      DT_NONE,   0},
    /* a6 */ {REP_CMPSB, DT_Xb,      DT_Yb,      DT_NONE,   0},
    /* a7 */ {REP_CMPSW, DT_Xw,      DT_Yw,      DT_NONE,   0},
    /* a8 */ {TEST,      DT_REG_AL,  DT_IMM_8,   DT_NONE,   0},
    /* a9 */ {TEST,      DT_REG_AX,  DT_IMM_16,  DT_NONE,   0},
    /* aa */ {REP_STOSB, DT_Yb,      DT_REG_AL,  DT_NONE,   0},
    /* ab */ {REP_STOSW, DT_Yw,      DT_REG_AX,  DT_NONE,   0},
    /* ac */ {REP_LODSB, DT_REG_AL,  DT_Xb,      DT_NONE,   0},
    /* ad */ {REP_LODSW, DT_REG_AX,  DT_Xw,      DT_NONE,   0},
    /* ae */ {REP_SCASB, DT_REG_AL,  DT_Yb,      DT_NONE,   0},
    /* af */ {REP_SCASW, DT_REG_AX,  DT_Yw,      DT_NONE,   0},

    /* b0 */ {MOV,       DT_DST_8,   DT_IMM_8,   DT_NONE,   0},
    /* b1 */ {MOV,       DT_DST_8,   DT_IMM_8,   DT_NONE,   0},
    /* b2 */ {MOV,       DT_DST_8,   DT_IMM_8,   DT_NONE,   0},
    /* b3 */ {MOV,       DT_DST_8,   DT_IMM_8,   DT_NONE,   0},
    /* b4 */ {MOV,       DT_DST_8,   DT_IMM_8,   DT_NONE,   0},
    /* b5 */ {MOV,       DT_DST_8,   DT_IMM_8,   DT_NONE,   0},
    /* b6 */ {MOV,       DT_DST_8,   DT_IMM_8,   DT_NONE,   0},
    /* b7 */ {MOV,       DT_DST_8,   DT_IMM_8,   DT_NONE,   0},
    /* b8 */ {MOV,       DT_DST_16,  DT_IMM_16,  DT_NONE,   0},
    /* b9 */ {MOV,       DT_DST_16,  DT_IMM_16,  DT_NONE,   0},
    /* ba */ {MOV,       DT_DST_16,  DT_IMM_16,  DT_NONE,   0},
    /* bb */ {MOV,       DT_DST_16,  DT_IMM_16,  DT_NONE,   0},
    /* bc */ {MOV,       DT_DST_16,  DT_IMM_16,  DT_NONE,   0},
    /* bd */ {MOV,       DT_DST_16,  DT_IMM_16,  DT_NONE,   0},
    /* be */ {MOV,       DT_DST_16,  DT_IMM_16,  DT_NONE,   0},
    /* bf */ {MOV,       DT_DST_16,  DT_IMM_16,  DT_NONE,   0},

    /* c0 */ {SAR,       DT_MEM_8,   DT_IMM_8,   DT_NONE,   DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|SHR|SAL|SAR
    /* c1 */ {SAR,       DT_MEM_16,  DT_IMM_8,   DT_NONE,   DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|SHR|SAL|SAR
    /* c2 */ {RET,       DT_IMM_16,  DT_NONE,    DT_NONE,   0},
    /* c3 */ {RET,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // no operands?
    /* c4 */ {LES,       DT_REG_16,  DT_MEM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* c5 */ {LDS,       DT_REG_16,  DT_MEM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* c6 */ {MOV,       DT_MEM_8,   DT_IMM_8,   DT_NONE,   DF_HAS_MOD_RM},
    /* c7 */ {MOV,       DT_MEM_16,  DT_IMM_16,  DT_NONE,   DF_HAS_MOD_RM},
    /* c8 */ {ENTER,     DT_IMM_16,  DT_IMM_8,   DT_NONE,   0},
    /* c9 */ {LEAVE,     DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* ca */ {RETF,      DT_IMM_16,  DT_NONE,    DT_NONE,   0},
    /* cb */ {RETF,      DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* cc */ {INT3,      DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* cd */ {INT,       DT_IMM_8,   DT_NONE,    DT_NONE,   0},
    /* ce */ {INTO,      DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* cf */ {IRET,      DT_NONE,    DT_NONE,    DT_NONE,   0},

    /* d0 */ {SAR,       DT_MEM_8,   I1,         DT_NONE,   DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d1 */ {SAR,       DT_MEM_16,  I1,         DT_NONE,   DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d2 */ {SAR,       DT_MEM_8,   DT_NONE,    DT_NONE,   DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d3 */ {SAR,       DT_MEM_16,  DT_NONE,    DT_NONE,   DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d4 */ {AAM,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* d5 */ {AAD,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* d6 */ {SALC,      DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* d7 */ {XLAT,      DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* d8 */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // FPU instructions
    /* d9 */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // FPU instructions
    /* da */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // FPU instructions
    /* db */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // FPU instructions
    /* dc */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // FPU instructions
    /* dd */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // FPU instructions
    /* de */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // FPU instructions
    /* df */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // FPU instructions

    /* e0 */ {LOOPNE,    DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* e1 */ {LOOPE,     DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* e2 */ {LOOP,      DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* e3 */ {JCXZ,      DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* e4 */ {IN,        DT_REG_AL,  DT_IMM_8,   DT_NONE,   0},
    /* e5 */ {IN,        DT_REG_AX,  DT_IMM_8,   DT_NONE,   0},
    /* e6 */ {OUT,       DT_IMM_8,   DT_REG_AL,  DT_NONE,   0},
    /* e7 */ {OUT,       DT_IMM_8,   DT_REG_AX,  DT_NONE,   0},
    /* e8 */ {CALL,      DT_JMP_16,  DT_NONE,    DT_NONE,   0},
    /* e9 */ {JMP,       DT_JMP_16,  DT_NONE,    DT_NONE,   0},
    /* ea */ {JMPF,      DT_Ap,      DT_NONE,    DT_NONE,   0},
    /* eb */ {JMP,       DT_JMP_8,   DT_NONE,    DT_NONE,   0},
    /* ec */ {IN,        DT_REG_AL,  DT_DX,      DT_NONE,   0},
    /* ed */ {IN,        DT_REG_AX,  DT_DX,      DT_NONE,   0},
    /* ee */ {OUT,       DT_DX,      DT_REG_AL,  DT_NONE,   0},
    /* ef */ {OUT,       DT_DX,      DT_REG_AX,  DT_NONE,   0},

    /* f0 */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // Lock?
    /* f1 */ {INT1,      DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* f2 */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // REPNE
    /* f3 */ {NOP,       DT_NONE,    DT_NONE,    DT_NONE,   0},             // REPE
    /* f4 */ {HLT,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* f5 */ {CMC,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* f6 */ {TEST,      DT_MEM_8,   DT_IMM_8,   DT_NONE,   DF_HAS_MOD_RM}, // TEST|NOT|NEG|MUL|IMUL|DIV|IDIV
    /* f7 */ {TEST,      DT_MEM_16,  DT_IMM_16,  DT_NONE,   DF_HAS_MOD_RM}, // TEST|NOT|NEG|MUL|IMUL|DIV|IDIV
    /* f8 */ {CLC,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* f9 */ {STC,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* fa */ {CLI,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* fb */ {STI,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* fc */ {CLD,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* fd */ {STD,       DT_NONE,    DT_NONE,    DT_NONE,   0},
    /* fe */ {DEC,       DT_MEM_8,   DT_NONE,    DT_NONE,   DF_HAS_MOD_RM},
    /* ff */ {INC,       DT_MEM_8,   DT_NONE,    DT_NONE,   DF_HAS_MOD_RM},
};
// clang-format on
