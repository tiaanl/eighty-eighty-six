#include "op_code_table.h"

#define _ DT_NONE

// clang-format off
struct op_code_mapping op_code_table[] = {
    /* 00 Eb Gb    */ {ADD,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 01 Ew Gw    */ {ADD,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 02 Gb Eb    */ {ADD,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 03 Gw Ew    */ {ADD,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 04 AL Ib    */ {ADD,       DT_AL,             DT_IMM_8,          _,         0},
    /* 05 AX Iw    */ {ADD,       DT_AX,             DT_IMM_16,         _,         0},
    /* 06 ES       */ {PUSH,      DT_SEGMENT_REG,    _,                 _,         0},
    /* 07 ES       */ {POP,       DT_SEGMENT_REG,    _,                 _,         0},
    /* 08 Eb Gb    */ {OR,        DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 09 Ew Gw    */ {OR,        DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 0a Gb Eb    */ {OR,        DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 0b Gw Ew    */ {OR,        DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 0c AL Ib    */ {OR,        DT_AL,             DT_IMM_8,          _,         0},
    /* 0d AX Iw    */ {OR,        DT_AX,             DT_IMM_16,         _,         0},
    /* 0e CS       */ {PUSH,      DT_SEGMENT_REG,    _,                 _,         0},
    /* 0f          */ {NOP,       _,                 _,                 _,         0},

    /* 10 Eb Gb    */ {ADC,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 11 Ew Gw    */ {ADC,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 12 Gb Eb    */ {ADC,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 13 Gw Ew    */ {ADC,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 14 AL Ib    */ {ADC,       DT_AL,             DT_IMM_8,          _,         0},
    /* 15 AX Iw    */ {ADC,       DT_AX,             DT_IMM_16,         _,         0},
    /* 16 SS       */ {PUSH,      DT_SEGMENT_REG,    _,                 _,         0},
    /* 17 SS       */ {POP,       DT_SEGMENT_REG,    _,                 _,         0},
    /* 18 Eb Gb    */ {SBB,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 19 Ew Gw    */ {SBB,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 1a Gb Eb    */ {SBB,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 1b Gw Ew    */ {SBB,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 1c AL Ib    */ {SBB,       DT_AL,             DT_IMM_8,          _,         0},
    /* 1d AX Iw    */ {SBB,       DT_AX,             DT_IMM_16,         _,         0},
    /* 1e DS       */ {PUSH,      DT_SEGMENT_REG,    _,                 _,         0},
    /* 1f DS       */ {POP,       DT_SEGMENT_REG,    _,                 _,         0},

    /* 20 Eb Gb    */ {AND,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 21 Ew Gw    */ {AND,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 22 Gb Eb    */ {AND,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 23 Gw Ew    */ {AND,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 24 AL Ib    */ {AND,       DT_AL,             DT_IMM_8,          _,         0},
    /* 25 AX Iw    */ {AND,       DT_AX,             DT_IMM_16,         _,         0},
    /* 26          */ {NOP,       _,                 _,                 _,         0},             // Segment register override (ES)
    /* 27          */ {DAA,       _,                 _,                 _,         0},
    /* 28 Eb Gb    */ {SUB,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 29 Ew Gw    */ {SUB,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 2a Gb Eb    */ {SUB,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 2b Gw Ew    */ {SUB,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 2c AL Ib    */ {SUB,       DT_AL,             DT_IMM_8,          _,         0},
    /* 2d AX Iw    */ {SUB,       DT_AX,             DT_IMM_16,         _,         0},
    /* 2e          */ {NOP,       _,                 _,                 _,         0},             // Segment register override (CS)
    /* 2f          */ {DAS,       _,                 _,                 _,         0},

    /* 30 Eb Gb    */ {XOR,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 31 Ew Gw    */ {XOR,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 32 Gb Eb    */ {XOR,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 33 Gw Ew    */ {XOR,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 34 AL Ib    */ {XOR,       DT_AL,             DT_IMM_8,          _,         0},
    /* 35 AX Iw    */ {XOR,       DT_AX,             DT_IMM_16,         _,         0},
    /* 36          */ {NOP,       _,                 _,                 _,         0},             // Segment register override (SS)
    /* 37          */ {AAA,       _,                 _,                 _,         0},
    /* 38 Eb Gb    */ {CMP,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 39 Ew Gw    */ {CMP,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 3a Gb Eb    */ {CMP,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 3b Gw Ew    */ {CMP,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 3c AL Ib    */ {CMP,       DT_AL,             DT_IMM_8,          _,         0},
    /* 3d AX Iw    */ {CMP,       DT_AX,             DT_IMM_16,         _,         0},
    /* 3e          */ {NOP,       _,                 _,                 _,         0},             // Segment register override (DS)
    /* 3f          */ {AAS,       _,                 _,                 _,         0},

    /* 40 AX       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 41 CX       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 42 DX       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 43 BX       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 44 SP       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 45 BP       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 46 SI       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 47 DI       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 48 AX       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 49 CX       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 4a DX       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 4b BX       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 4c SP       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 4d BP       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 4e SI       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 4f DI       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         0},

    /* 50 AX       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         0},
    /* 51 CX       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         0},
    /* 52 DX       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         0},
    /* 53 BX       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         0},
    /* 54 SP       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         0},
    /* 55 BP       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         0},
    /* 56 SI       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         0},
    /* 57 DI       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         0},
    /* 58 AX       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 59 CX       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 5a DX       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 5b BX       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 5c SP       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 5d BP       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 5e SI       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         0},
    /* 5f DI       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         0},

    /* 60          */ {PUSHA,     _,                 _,                 _,         0},
    /* 61          */ {POPA,      _,                 _,                 _,         0},
    /* 62 Gw Ma    */ {BOUND,     DT_MOD_RM_REG_16,  DT_Ma,             _,         0},
    /* 63 Ew Gw    */ {ARPL,      DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 64          */ {NOP,       _,                 _,                 _,         0},             // Segment override:              FS??
    /* 65          */ {NOP,       _,                 _,                 _,         0},             // Segment override:              GS??
    /* 66          */ {NOP,       _,                 _,                 _,         0},             // Operand size override
    /* 67          */ {NOP,       _,                 _,                 _,         0},             // Operand address size override
    /* 68 Iw       */ {PUSH,      DT_IMM_16,         _,                 _,         0},
    /* 69 Gw Ew Iw */ {IMUL,      DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   DT_IMM_16, DF_HAS_MOD_RM},
    /* 6a Ib       */ {PUSH,      DT_IMM_8,          _,                 _,         0},
    /* 6b Gw Ew Ib */ {IMUL,      DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   DT_IMM_8,  DF_HAS_MOD_RM},
    /* 6c Yb DX    */ {REP_INSB,  DT_Yb,             DT_DX,             _,         0},
    /* 6d Yw DX    */ {REP_INSW,  DT_Yw,             DT_DX,             _,         0},
    /* 6e DX Xb    */ {REP_OUTSB, DT_DX,             DT_Xb,             _,         0},
    /* 6f DX Xw    */ {REP_OUTSW, DT_DX,             DT_Xw,             _,         0},

    /* 70 Jb       */ {JO,        DT_JMP_8,          _,                 _,         0},
    /* 71 Jb       */ {JNO,       DT_JMP_8,          _,                 _,         0},
    /* 72 Jb       */ {JB,        DT_JMP_8,          _,                 _,         0},
    /* 73 Jb       */ {JNB,       DT_JMP_8,          _,                 _,         0},
    /* 74 Jb       */ {JZ,        DT_JMP_8,          _,                 _,         0},
    /* 75 Jb       */ {JNZ,       DT_JMP_8,          _,                 _,         0},
    /* 76 Jb       */ {JBE,       DT_JMP_8,          _,                 _,         0},
    /* 77 Jb       */ {JNBE,      DT_JMP_8,          _,                 _,         0},
    /* 78 Jb       */ {JS,        DT_JMP_8,          _,                 _,         0},
    /* 79 Jb       */ {JNS,       DT_JMP_8,          _,                 _,         0},
    /* 7a Jb       */ {JP,        DT_JMP_8,          _,                 _,         0},
    /* 7b Jb       */ {JNP,       DT_JMP_8,          _,                 _,         0},
    /* 7c Jb       */ {JL,        DT_JMP_8,          _,                 _,         0},
    /* 7d Jb       */ {JNL,       DT_JMP_8,          _,                 _,         0},
    /* 7e Jb       */ {JLE,       DT_JMP_8,          _,                 _,         0},
    /* 7f Jb       */ {JNLE,      DT_JMP_8,          _,                 _,         0},

    /* 80 Eb Ib    */ {CMP,       DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 81 Ew Iw    */ {CMP,       DT_MOD_RM_RM_16,   DT_IMM_16,         _,         DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 82          */ {NOP,       _,                 _,                 _,         DF_HAS_MOD_RM}, // Copy of 80??!
    /* 83 Ew Ib    */ {CMP,       DT_MOD_RM_RM_16,   DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 84 Eb Gb    */ {TEST,      DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 85 Ew Gw    */ {TEST,      DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 86 Eb Gb    */ {XCHG,      DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 87 Ew Gw    */ {XCHG,      DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 88 Eb Gb    */ {MOV,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 89 Ew Gw    */ {MOV,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 8a Gb Eb    */ {MOV,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 8b Gw Ew    */ {MOV,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 8c Ew Sw    */ {MOV,       DT_MOD_RM_RM_16,   DT_SEGMENT_REG,    _,         DF_HAS_MOD_RM},
    /* 8d Gw M     */ {LEA,       DT_MOD_RM_REG_16,  DT_M,              _,         DF_HAS_MOD_RM},
    /* 8e Sw Ew    */ {MOV,       DT_SEGMENT_REG,    DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 8f Ew       */ {POP,       DT_MOD_RM_RM_16,   _,                 _,         DF_HAS_MOD_RM}, // XOP prefix? ?!?!?

    /* 90          */ {NOP,       _,                 _,                 _,         0},
    /* 91 AX CX    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         0},
    /* 92 AX DX    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         0},
    /* 93 AX BX    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         0},
    /* 94 AX SP    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         0},
    /* 95 AX BP    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         0},
    /* 96 AX SI    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         0},
    /* 97 AX DI    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         0},
    /* 98          */ {CBW,       _,                 _,                 _,         0},
    /* 99          */ {CWD,       _,                 _,                 _,         0},
    /* 9a Ap       */ {CALLF,     DT_Ap,             _,                 _,         0},
    /* 9b          */ {FWAIT,     _,                 _,                 _,         0},
    /* 9c Fw       */ {PUSHF,     DT_Fw,             _,                 _,         0},
    /* 9d Fw       */ {POPF,      DT_Fw,             _,                 _,         0},
    /* 9e          */ {SAHF,      _,                 _,                 _,         0},
    /* 9f          */ {LAHF,      _,                 _,                 _,         0},

    /* a0 AL Ob    */ {MOV,       DT_AL,             DT_Ob,             _,         0},
    /* a1 AX Ow    */ {MOV,       DT_AX,             DT_Ow,             _,         0},
    /* a2 Ob AL    */ {MOV,       DT_Ob,             DT_AL,             _,         0},
    /* a3 Ow AX    */ {MOV,       DT_Ow,             DT_AX,             _,         0},
    /* a4 Xb Yb    */ {REP_MOVSB, DT_Xb,             DT_Yb,             _,         0},
    /* a5 Xw Yw    */ {REP_MOVSW, DT_Xw,             DT_Yw,             _,         0},
    /* a6 Xb Yb    */ {REP_CMPSB, DT_Xb,             DT_Yb,             _,         0},
    /* a7 Xw Yw    */ {REP_CMPSW, DT_Xw,             DT_Yw,             _,         0},
    /* a8 AL Ib    */ {TEST,      DT_AL,             DT_IMM_8,          _,         0},
    /* a9 AX Iw    */ {TEST,      DT_AX,             DT_IMM_16,         _,         0},
    /* aa Yb AL    */ {REP_STOSB, DT_Yb,             DT_AL,             _,         0},
    /* ab Yw AX    */ {REP_STOSW, DT_Yw,             DT_AX,             _,         0},
    /* ac AL Xb    */ {REP_LODSB, DT_AL,             DT_Xb,             _,         0},
    /* ad AX Xw    */ {REP_LODSW, DT_AX,             DT_Xw,             _,         0},
    /* ae AL Yb    */ {REP_SCASB, DT_AL,             DT_Yb,             _,         0},
    /* af AX Yw    */ {REP_SCASW, DT_AX,             DT_Yw,             _,         0},

    /* b0 AL Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         0},
    /* b1 CL Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         0},
    /* b2 DL Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         0},
    /* b3 BL Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         0},
    /* b4 AH Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         0},
    /* b5 CH Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         0},
    /* b6 DH Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         0},
    /* b7 BH Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         0},
    /* b8 AX Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         0},
    /* b9 CX Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         0},
    /* ba DX Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         0},
    /* bb BX Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         0},
    /* bc BP Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         0},
    /* bd SP Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         0},
    /* be DI Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         0},
    /* bf SI Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         0},

    /* c0 Eb Ib    */ {SAR,       DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|SHR|SAL|SAR
    /* c1 Ew Ib    */ {SAR,       DT_MOD_RM_RM_16,   DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|SHR|SAL|SAR
    /* c2 Iw       */ {RET,       DT_IMM_16,         _,                 _,         0},
    /* c3          */ {RET,       _,                 _,                 _,         0},             // no operands?
    /* c4 Gw Ew    */ {LES,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* c5 Gw Ew    */ {LDS,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* c6 Eb Ib    */ {MOV,       DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM},
    /* c7 Ew Iw    */ {MOV,       DT_MOD_RM_RM_16,   DT_IMM_16,         _,         DF_HAS_MOD_RM},
    /* c8 Iw Ib    */ {ENTER,     DT_IMM_16,         DT_IMM_8,          _,         0},
    /* c9          */ {LEAVE,     _,                 _,                 _,         0},
    /* ca Iw       */ {RETF,      DT_IMM_16,         _,                 _,         0},
    /* cb          */ {RETF,      _,                 _,                 _,         0},
    /* cc          */ {INT3,      _,                 _,                 _,         0},
    /* cd Ib       */ {INT,       DT_IMM_8,          _,                 _,         0},
    /* ce          */ {INTO,      _,                 _,                 _,         0},
    /* cf          */ {IRET,      _,                 _,                 _,         0},

    /* d0 Eb 1     */ {SAR,       DT_MOD_RM_RM_8,    1,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d1 Ew 1     */ {SAR,       DT_MOD_RM_RM_16,   1,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d2 Eb       */ {SAR,       DT_MOD_RM_RM_8,    _,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d3 Ew       */ {SAR,       DT_MOD_RM_RM_16,   _,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d4          */ {AAM,       _,                 _,                 _,         0},
    /* d5          */ {AAD,       _,                 _,                 _,         0},
    /* d6          */ {SALC,      _,                 _,                 _,         0},
    /* d7          */ {XLAT,      _,                 _,                 _,         0},
    /* d8          */ {NOP,       _,                 _,                 _,         0},             // FPU instructions
    /* d9          */ {NOP,       _,                 _,                 _,         0},             // FPU instructions
    /* da          */ {NOP,       _,                 _,                 _,         0},             // FPU instructions
    /* db          */ {NOP,       _,                 _,                 _,         0},             // FPU instructions
    /* dc          */ {NOP,       _,                 _,                 _,         0},             // FPU instructions
    /* dd          */ {NOP,       _,                 _,                 _,         0},             // FPU instructions
    /* de          */ {NOP,       _,                 _,                 _,         0},             // FPU instructions
    /* df          */ {NOP,       _,                 _,                 _,         0},             // FPU instructions

    /* e0 Jb       */ {LOOPNE,    DT_JMP_8,          _,                 _,         0},
    /* e1 Jb       */ {LOOPE,     DT_JMP_8,          _,                 _,         0},
    /* e2 Jb       */ {LOOP,      DT_JMP_8,          _,                 _,         0},
    /* e3 Jb       */ {JCXZ,      DT_JMP_8,          _,                 _,         0},
    /* e4 AL Ib    */ {IN,        DT_AL,             DT_IMM_8,          _,         0},
    /* e5 AX Ib    */ {IN,        DT_AX,             DT_IMM_8,          _,         0},
    /* e6 Ib AL    */ {OUT,       DT_IMM_8,          DT_AL,             _,         0},
    /* e7 Ib AX    */ {OUT,       DT_IMM_8,          DT_AX,             _,         0},
    /* e8 Jw       */ {CALL,      DT_JMP_16,         _,                 _,         0},
    /* e9 Jw       */ {JMP,       DT_JMP_16,         _,                 _,         0},
    /* ea Ap       */ {JMPF,      DT_Ap,             _,                 _,         0},
    /* eb Jb       */ {JMP,       DT_JMP_8,          _,                 _,         0},
    /* ec AL DX    */ {IN,        DT_AL,             DT_DX,             _,         0},
    /* ed AX DX    */ {IN,        DT_AX,             DT_DX,             _,         0},
    /* ee DX AL    */ {OUT,       DT_DX,             DT_AL,             _,         0},
    /* ef DX AX    */ {OUT,       DT_DX,             DT_AX,             _,         0},

    /* f0          */ {NOP,       _,                 _,                 _,         0},             // Lock?
    /* f1          */ {INT1,      _,                 _,                 _,         0},
    /* f2          */ {NOP,       _,                 _,                 _,         0},             // REPNE
    /* f3          */ {NOP,       _,                 _,                 _,         0},             // REPE
    /* f4          */ {HLT,       _,                 _,                 _,         0},
    /* f5          */ {CMC,       _,                 _,                 _,         0},
    /* f6 Eb Ib    */ {TEST,      DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM}, // TEST|NOT|NEG|MUL|IMUL|DIV|IDIV
    /* f7 Ew Iw    */ {TEST,      DT_MOD_RM_RM_16,   DT_IMM_16,         _,         DF_HAS_MOD_RM}, // TEST|NOT|NEG|MUL|IMUL|DIV|IDIV
    /* f8          */ {CLC,       _,                 _,                 _,         0},
    /* f9          */ {STC,       _,                 _,                 _,         0},
    /* fa          */ {CLI,       _,                 _,                 _,         0},
    /* fb          */ {STI,       _,                 _,                 _,         0},
    /* fc          */ {CLD,       _,                 _,                 _,         0},
    /* fd          */ {STD,       _,                 _,                 _,         0},
    /* fe Eb       */ {DEC,       DT_MOD_RM_RM_8,    _,                 _,         DF_HAS_MOD_RM},
    /* ff Eb       */ {INC,       DT_MOD_RM_RM_8,    _,                 _,         DF_HAS_MOD_RM},
};
// clang-format on
