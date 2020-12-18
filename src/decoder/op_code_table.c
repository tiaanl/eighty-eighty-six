#include "op_code_table.h"

#define _ DT_NONE
#define __ 0

// clang-format off
struct op_code_mapping op_code_table[] = {
    /* 00 Eb Gb    */ {ADD,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 01 Ew Gw    */ {ADD,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 02 Gb Eb    */ {ADD,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 03 Gw Ew    */ {ADD,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 04 AL Ib    */ {ADD,       DT_AL,             DT_IMM_8,          _,         __},
    /* 05 AX Iw    */ {ADD,       DT_AX,             DT_IMM_16,         _,         __},
    /* 06 ES       */ {PUSH,      DT_SEGMENT_REG,    _,                 _,         __},
    /* 07 ES       */ {POP,       DT_SEGMENT_REG,    _,                 _,         __},
    /* 08 Eb Gb    */ {OR,        DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 09 Ew Gw    */ {OR,        DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 0a Gb Eb    */ {OR,        DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 0b Gw Ew    */ {OR,        DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 0c AL Ib    */ {OR,        DT_AL,             DT_IMM_8,          _,         __},
    /* 0d AX Iw    */ {OR,        DT_AX,             DT_IMM_16,         _,         __},
    /* 0e CS       */ {PUSH,      DT_SEGMENT_REG,    _,                 _,         __},
    /* 0f          */ {INVALID,   _,                 _,                 _,         __},            // Extended op codes.

    /* 10 Eb Gb    */ {ADC,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 11 Ew Gw    */ {ADC,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 12 Gb Eb    */ {ADC,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 13 Gw Ew    */ {ADC,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 14 AL Ib    */ {ADC,       DT_AL,             DT_IMM_8,          _,         __},
    /* 15 AX Iw    */ {ADC,       DT_AX,             DT_IMM_16,         _,         __},
    /* 16 SS       */ {PUSH,      DT_SEGMENT_REG,    _,                 _,         __},
    /* 17 SS       */ {POP,       DT_SEGMENT_REG,    _,                 _,         __},
    /* 18 Eb Gb    */ {SBB,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 19 Ew Gw    */ {SBB,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 1a Gb Eb    */ {SBB,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 1b Gw Ew    */ {SBB,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 1c AL Ib    */ {SBB,       DT_AL,             DT_IMM_8,          _,         __},
    /* 1d AX Iw    */ {SBB,       DT_AX,             DT_IMM_16,         _,         __},
    /* 1e DS       */ {PUSH,      DT_SEGMENT_REG,    _,                 _,         __},
    /* 1f DS       */ {POP,       DT_SEGMENT_REG,    _,                 _,         __},

    /* 20 Eb Gb    */ {AND,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 21 Ew Gw    */ {AND,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 22 Gb Eb    */ {AND,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 23 Gw Ew    */ {AND,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 24 AL Ib    */ {AND,       DT_AL,             DT_IMM_8,          _,         __},
    /* 25 AX Iw    */ {AND,       DT_AX,             DT_IMM_16,         _,         __},
    /* 26          */ {INVALID,   _,                 _,                 _,         __},            // Segment register override (ES)
    /* 27          */ {DAA,       _,                 _,                 _,         __},
    /* 28 Eb Gb    */ {SUB,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 29 Ew Gw    */ {SUB,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 2a Gb Eb    */ {SUB,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 2b Gw Ew    */ {SUB,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 2c AL Ib    */ {SUB,       DT_AL,             DT_IMM_8,          _,         __},
    /* 2d AX Iw    */ {SUB,       DT_AX,             DT_IMM_16,         _,         __},
    /* 2e          */ {INVALID,   _,                 _,                 _,         __},            // Segment register override (CS)
    /* 2f          */ {DAS,       _,                 _,                 _,         __},

    /* 30 Eb Gb    */ {XOR,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 31 Ew Gw    */ {XOR,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 32 Gb Eb    */ {XOR,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 33 Gw Ew    */ {XOR,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 34 AL Ib    */ {XOR,       DT_AL,             DT_IMM_8,          _,         __},
    /* 35 AX Iw    */ {XOR,       DT_AX,             DT_IMM_16,         _,         __},
    /* 36          */ {INVALID,   _,                 _,                 _,         __},            // Segment register override (SS)
    /* 37          */ {AAA,       _,                 _,                 _,         __},
    /* 38 Eb Gb    */ {CMP,       DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 39 Ew Gw    */ {CMP,       DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 3a Gb Eb    */ {CMP,       DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 3b Gw Ew    */ {CMP,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 3c AL Ib    */ {CMP,       DT_AL,             DT_IMM_8,          _,         __},
    /* 3d AX Iw    */ {CMP,       DT_AX,             DT_IMM_16,         _,         __},
    /* 3e          */ {INVALID,   _,                 _,                 _,         __},            // Segment register override (DS)
    /* 3f          */ {AAS,       _,                 _,                 _,         __},

    /* 40 AX       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 41 CX       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 42 DX       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 43 BX       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 44 SP       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 45 BP       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 46 SI       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 47 DI       */ {INC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 48 AX       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 49 CX       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4a DX       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4b BX       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4c SP       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4d BP       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4e SI       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4f DI       */ {DEC,       DT_OP_CODE_REG_16, _,                 _,         __},

    /* 50 AX       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         __},
    /* 51 CX       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         __},
    /* 52 DX       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         __},
    /* 53 BX       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         __},
    /* 54 SP       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         __},
    /* 55 BP       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         __},
    /* 56 SI       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         __},
    /* 57 DI       */ {PUSH,      DT_OP_CODE_REG_16, _,                 _,         __},
    /* 58 AX       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 59 CX       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5a DX       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5b BX       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5c SP       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5d BP       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5e SI       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5f DI       */ {POP,       DT_OP_CODE_REG_16, _,                 _,         __},

    /* 60          */ {PUSHA,     _,                 _,                 _,         __},
    /* 61          */ {POPA,      _,                 _,                 _,         __},
    /* 62 Gw Ma    */ {BOUND,     DT_MOD_RM_REG_16,  DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 63 Ew Gw    */ {ARPL,      DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 64          */ {INVALID,   _,                 _,                 _,         __},            // Segment override:              FS??
    /* 65          */ {INVALID,   _,                 _,                 _,         __},            // Segment override:              GS??
    /* 66          */ {INVALID,   _,                 _,                 _,         __},            // Operand size override
    /* 67          */ {INVALID,   _,                 _,                 _,         __},            // Operand address size override
    /* 68 Iw       */ {PUSH,      DT_IMM_16,         _,                 _,         __},
    /* 69 Gw Ew Iw */ {IMUL,      DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   DT_IMM_16, DF_HAS_MOD_RM},
    /* 6a Ib       */ {PUSH,      DT_IMM_8,          _,                 _,         __},
    /* 6b Gw Ew Ib */ {IMUL,      DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   DT_IMM_8,  DF_HAS_MOD_RM},
    /* 6c Yb DX    */ {REP_INSB,  _,                 DT_DX,             _,         __},
    /* 6d Yw DX    */ {REP_INSW,  _,                 DT_DX,             _,         __},
    /* 6e DX Xb    */ {REP_OUTSB, DT_DX,             _,                 _,         __},
    /* 6f DX Xw    */ {REP_OUTSW, DT_DX,             _,                 _,         __},

    /* 70 Jb       */ {JO,        DT_JMP_8,          _,                 _,         __},
    /* 71 Jb       */ {JNO,       DT_JMP_8,          _,                 _,         __},
    /* 72 Jb       */ {JB,        DT_JMP_8,          _,                 _,         __},
    /* 73 Jb       */ {JNB,       DT_JMP_8,          _,                 _,         __},
    /* 74 Jb       */ {JZ,        DT_JMP_8,          _,                 _,         __},
    /* 75 Jb       */ {JNZ,       DT_JMP_8,          _,                 _,         __},
    /* 76 Jb       */ {JBE,       DT_JMP_8,          _,                 _,         __},
    /* 77 Jb       */ {JNBE,      DT_JMP_8,          _,                 _,         __},
    /* 78 Jb       */ {JS,        DT_JMP_8,          _,                 _,         __},
    /* 79 Jb       */ {JNS,       DT_JMP_8,          _,                 _,         __},
    /* 7a Jb       */ {JP,        DT_JMP_8,          _,                 _,         __},
    /* 7b Jb       */ {JNP,       DT_JMP_8,          _,                 _,         __},
    /* 7c Jb       */ {JL,        DT_JMP_8,          _,                 _,         __},
    /* 7d Jb       */ {JNL,       DT_JMP_8,          _,                 _,         __},
    /* 7e Jb       */ {JLE,       DT_JMP_8,          _,                 _,         __},
    /* 7f Jb       */ {JNLE,      DT_JMP_8,          _,                 _,         __},

    /* 80 Eb Ib    */ {CMP,       DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 81 Ew Iw    */ {CMP,       DT_MOD_RM_RM_16,   DT_IMM_16,         _,         DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 82          */ {INVALID,   _,                 _,                 _,         DF_HAS_MOD_RM}, // Copy of 80??!
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
    /* 8d Gw M     */ {LEA,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 8e Sw Ew    */ {MOV,       DT_SEGMENT_REG,    DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 8f Ew       */ {POP,       DT_MOD_RM_RM_16,   _,                 _,         DF_HAS_MOD_RM}, // XOP prefix? ?!?!?

    /* 90          */ {INVALID,   _,                 _,                 _,         __},
    /* 91 AX CX    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 92 AX DX    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 93 AX BX    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 94 AX SP    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 95 AX BP    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 96 AX SI    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 97 AX DI    */ {XCHG,      DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 98          */ {CBW,       _,                 _,                 _,         __},
    /* 99          */ {CWD,       _,                 _,                 _,         __},
    /* 9a Ap       */ {CALLF,     DT_Ap,             _,                 _,         __},
    /* 9b          */ {FWAIT,     _,                 _,                 _,         __},
    /* 9c Fw       */ {PUSHF,     DT_Fw,             _,                 _,         __},
    /* 9d Fw       */ {POPF,      DT_Fw,             _,                 _,         __},
    /* 9e          */ {SAHF,      _,                 _,                 _,         __},
    /* 9f          */ {LAHF,      _,                 _,                 _,         __},

    /* a0 AL Ob    */ {MOV,       DT_AL,             DT_JMP_16,         _,         __},
    /* a1 AX Ow    */ {MOV,       DT_AX,             DT_JMP_16,         _,         __},
    /* a2 Ob AL    */ {MOV,       DT_JMP_16,         DT_AL,             _,         __},
    /* a3 Ow AX    */ {MOV,       DT_JMP_16,         DT_AX,             _,         __},
    /* a4 Xb Yb    */ {REP_MOVSB, _,                 _,                 _,         __},
    /* a5 Xw Yw    */ {REP_MOVSW, _,                 _,                 _,         __},
    /* a6 Xb Yb    */ {REP_CMPSB, _,                 _,                 _,         __},
    /* a7 Xw Yw    */ {REP_CMPSW, _,                 _,                 _,         __},
    /* a8 AL Ib    */ {TEST,      DT_AL,             DT_IMM_8,          _,         __},
    /* a9 AX Iw    */ {TEST,      DT_AX,             DT_IMM_16,         _,         __},
    /* aa Yb AL    */ {REP_STOSB, _,                 _,                 _,         __},            // rep stosb [ES:DI+offset],          al
    /* ab Yw AX    */ {REP_STOSW, _,                 _,                 _,         __},
    /* ac AL Xb    */ {REP_LODSB, _,                 _,                 _,         __},
    /* ad AX Xw    */ {REP_LODSW, _,                 _,                 _,         __},
    /* ae AL Yb    */ {REP_SCASB, _,                 _,                 _,         __},
    /* af AX Yw    */ {REP_SCASW, _,                 _,                 _,         __},

    /* b0 AL Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b1 CL Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b2 DL Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b3 BL Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b4 AH Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b5 CH Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b6 DH Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b7 BH Ib    */ {MOV,       DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b8 AX Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* b9 CX Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* ba DX Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* bb BX Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* bc BP Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* bd SP Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* be DI Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* bf SI Iw    */ {MOV,       DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},

    /* c0 Eb Ib    */ {SAR,       DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|SHR|SAL|SAR
    /* c1 Ew Ib    */ {SAR,       DT_MOD_RM_RM_16,   DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|SHR|SAL|SAR
    /* c2 Iw       */ {RET,       DT_IMM_16,         _,                 _,         __},
    /* c3          */ {RET,       _,                 _,                 _,         __},            // no operands?
    /* c4 Gw Ew    */ {LES,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* c5 Gw Ew    */ {LDS,       DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* c6 Eb Ib    */ {MOV,       DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM},
    /* c7 Ew Iw    */ {MOV,       DT_MOD_RM_RM_16,   DT_IMM_16,         _,         DF_HAS_MOD_RM},
    /* c8 Iw Ib    */ {ENTER,     DT_IMM_16,         DT_IMM_8,          _,         __},
    /* c9          */ {LEAVE,     _,                 _,                 _,         __},
    /* ca Iw       */ {RETF,      DT_IMM_16,         _,                 _,         __},
    /* cb          */ {RETF,      _,                 _,                 _,         __},
    /* cc          */ {INT3,      _,                 _,                 _,         __},
    /* cd Ib       */ {INT,       DT_IMM_8,          _,                 _,         __},
    /* ce          */ {INTO,      _,                 _,                 _,         __},
    /* cf          */ {IRET,      _,                 _,                 _,         __},

    /* d0 Eb 1     */ {SAR,       DT_MOD_RM_RM_8,    1,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d1 Ew 1     */ {SAR,       DT_MOD_RM_RM_16,   1,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d2 Eb       */ {SAR,       DT_MOD_RM_RM_8,    _,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d3 Ew       */ {SAR,       DT_MOD_RM_RM_16,   _,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d4          */ {AAM,       _,                 _,                 _,         __},
    /* d5          */ {AAD,       _,                 _,                 _,         __},
    /* d6          */ {SALC,      _,                 _,                 _,         __},
    /* d7          */ {XLAT,      _,                 _,                 _,         __},
    /* d8          */ {INVALID,   _,                 _,                 _,         __},            // FPU instructions
    /* d9          */ {INVALID,   _,                 _,                 _,         __},            // FPU instructions
    /* da          */ {INVALID,   _,                 _,                 _,         __},            // FPU instructions
    /* db          */ {INVALID,   _,                 _,                 _,         __},            // FPU instructions
    /* dc          */ {INVALID,   _,                 _,                 _,         __},            // FPU instructions
    /* dd          */ {INVALID,   _,                 _,                 _,         __},            // FPU instructions
    /* de          */ {INVALID,   _,                 _,                 _,         __},            // FPU instructions
    /* df          */ {INVALID,   _,                 _,                 _,         __},            // FPU instructions

    /* e0 Jb       */ {LOOPNE,    DT_JMP_8,          _,                 _,         __},
    /* e1 Jb       */ {LOOPE,     DT_JMP_8,          _,                 _,         __},
    /* e2 Jb       */ {LOOP,      DT_JMP_8,          _,                 _,         __},
    /* e3 Jb       */ {JCXZ,      DT_JMP_8,          _,                 _,         __},
    /* e4 AL Ib    */ {IN,        DT_AL,             DT_IMM_8,          _,         __},
    /* e5 AX Ib    */ {IN,        DT_AX,             DT_IMM_8,          _,         __},
    /* e6 Ib AL    */ {OUT,       DT_IMM_8,          DT_AL,             _,         __},
    /* e7 Ib AX    */ {OUT,       DT_IMM_8,          DT_AX,             _,         __},
    /* e8 Jw       */ {CALL,      DT_JMP_16,         _,                 _,         __},
    /* e9 Jw       */ {JMP,       DT_JMP_16,         _,                 _,         __},
    /* ea Ap       */ {JMP,       DT_SEG_DIRECT,     _,                 _,         __},            // mov cs:ip
    /* eb Jb       */ {JMP,       DT_JMP_8,          _,                 _,         __},
    /* ec AL DX    */ {IN,        DT_AL,             DT_DX,             _,         __},
    /* ed AX DX    */ {IN,        DT_AX,             DT_DX,             _,         __},
    /* ee DX AL    */ {OUT,       DT_DX,             DT_AL,             _,         __},
    /* ef DX AX    */ {OUT,       DT_DX,             DT_AX,             _,         __},

    /* f0          */ {INVALID,   _,                 _,                 _,         __},            // Lock?
    /* f1          */ {INT1,      _,                 _,                 _,         __},
    /* f2          */ {INVALID,   _,                 _,                 _,         __},            // repne prefix
    /* f3          */ {INVALID,   _,                 _,                 _,         __},            // rep prefix
    /* f4          */ {HLT,       _,                 _,                 _,         __},
    /* f5          */ {CMC,       _,                 _,                 _,         __},
    /* f6 Eb Ib    */ {TEST,      DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM}, // TEST|NOT|NEG|MUL|IMUL|DIV|IDIV
    /* f7 Ew Iw    */ {TEST,      DT_MOD_RM_RM_16,   DT_IMM_16,         _,         DF_HAS_MOD_RM}, // TEST|NOT|NEG|MUL|IMUL|DIV|IDIV
    /* f8          */ {CLC,       _,                 _,                 _,         __},
    /* f9          */ {STC,       _,                 _,                 _,         __},
    /* fa          */ {CLI,       _,                 _,                 _,         __},
    /* fb          */ {STI,       _,                 _,                 _,         __},
    /* fc          */ {CLD,       _,                 _,                 _,         __},
    /* fd          */ {STD,       _,                 _,                 _,         __},
    /* fe Eb       */ {DEC,       DT_MOD_RM_RM_8,    _,                 _,         DF_HAS_MOD_RM},
    /* ff Eb       */ {INC,       DT_MOD_RM_RM_8,    _,                 _,         DF_HAS_MOD_RM},
};
// clang-format on
