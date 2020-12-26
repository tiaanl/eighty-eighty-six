#include "op_code_table.h"

#define _ DT_NONE
#define __ 0

#include "decoder_funcs.h"

/**
 * Addressing Methods
 * ==================
 * A - Direct address: the instruction has no ModR/M byte; the address of the operand is encoded in
 *     the instruction.
 * E - A ModR/M byte follows the opcode and specifies the operand. The operand is either a
 *     general-purpose register or a memory address. If it is a memory address, the address is
 *     computed from a segment register and any of the following values: a base register, an index
 *     register, a scaling factor, a displacement.
 * F - FLAGS register.
 * G - The reg field of the ModR/M byte selects a general register.
 * I - Immediate data: the operand value is encoded in subsequent bytes of the instruction.
 * J - The instruction contains a relative offset to be added to the instruction pointer register.
 * M - The ModR/M byte may refer only to memory.
 * O - The instruction has no ModR/M byte. The offset of the operand is encoded as a word  in the
 *     instruction.
 * S - The reg field of the ModR/M byte selects a segment register.
 * X - Memory addressed by the DS:SI register pair.
 * Y - Memory addressed by the ES:DI register pair.
 *
 * Operand Type
 * ============
 * a - Two one-word operands in memory (used only by the BOUND instruction).
 * b - Byte.
 * p - 32-bit pointer.
 * w - Word.
 */

// clang-format off
struct op_code_mapping op_code_table[] = {
    /* 00 */ {it_add,     am_Eb, am_Gb, am_Xx, decode_Eb_Gb_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 01 */ {it_add,     am_Ew, am_Gw, am_Xx, decode_Ew_Gw_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 02 */ {it_add,     am_Gb, am_Eb, am_Xx, decode_Gb_Eb_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 03 */ {it_add,     am_Gw, am_Ew, am_Xx, decode_Gw_Ew_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 04 */ {it_add,     am_AL, am_Ib, am_Xx, decode_AL_Ib_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 05 */ {it_add,     am_AX, am_Iw, am_Xx, decode_AX_Iw_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 06 */ {it_push,    am_ES, am_Xx, am_Xx, decode_ES_Xx_Xx, DT_SEGMENT_REG,    _,                 _,         __},
    /* 07 */ {it_pop,     am_ES, am_Xx, am_Xx, decode_ES_Xx_Xx, DT_SEGMENT_REG,    _,                 _,         __},
    /* 08 */ {it_or,      am_Eb, am_Gb, am_Xx, decode_Eb_Gb_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 09 */ {it_or,      am_Ew, am_Gw, am_Xx, decode_Ew_Gw_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 0a */ {it_or,      am_Gb, am_Eb, am_Xx, decode_Gb_Eb_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 0b */ {it_or,      am_Gw, am_Ew, am_Xx, decode_Gw_Ew_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 0c */ {it_or,      am_AL, am_Ib, am_Xx, decode_AL_Ib_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 0d */ {it_or,      am_AX, am_Iw, am_Xx, decode_AX_Iw_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 0e */ {it_push,    am_CS, am_Xx, am_Xx, decode_CS_Xx_Xx, DT_SEGMENT_REG,    _,                 _,         __},
    /* 0f */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // Extended op codes.

    /* 10 */ {it_adc,     am_Eb, am_Gb, am_Xx, decode_Eb_Gb_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 11 */ {it_adc,     am_Ew, am_Gw, am_Xx, decode_Ew_Gw_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 12 */ {it_adc,     am_Gb, am_Eb, am_Xx, decode_Gb_Eb_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 13 */ {it_adc,     am_Gw, am_Ew, am_Xx, decode_Gw_Ew_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 14 */ {it_adc,     am_AL, am_Ib, am_Xx, decode_AL_Ib_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 15 */ {it_adc,     am_AX, am_Iw, am_Xx, decode_AX_Iw_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 16 */ {it_push,    am_SS, am_Xx, am_Xx, decode_SS_Xx_Xx, DT_SEGMENT_REG,    _,                 _,         __},
    /* 17 */ {it_pop,     am_SS, am_Xx, am_Xx, decode_SS_Xx_Xx, DT_SEGMENT_REG,    _,                 _,         __},
    /* 18 */ {it_sbb,     am_Eb, am_Gb, am_Xx, decode_Eb_Gb_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 19 */ {it_sbb,     am_Ew, am_Gw, am_Xx, decode_Ew_Gw_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 1a */ {it_sbb,     am_Gb, am_Eb, am_Xx, decode_Gb_Eb_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 1b */ {it_sbb,     am_Gw, am_Ew, am_Xx, decode_Gw_Ew_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 1c */ {it_sbb,     am_AL, am_Ib, am_Xx, decode_AL_Ib_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 1d */ {it_sbb,     am_AX, am_Iw, am_Xx, decode_AX_Iw_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 1e */ {it_push,    am_DS, am_Xx, am_Xx, decode_DS_Xx_Xx, DT_SEGMENT_REG,    _,                 _,         __},
    /* 1f */ {it_pop,     am_DS, am_Xx, am_Xx, decode_DS_Xx_Xx, DT_SEGMENT_REG,    _,                 _,         __},

    /* 20 */ {it_and,     am_Eb, am_Gb, am_Xx, decode_Eb_Gb_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 21 */ {it_and,     am_Ew, am_Gw, am_Xx, decode_Ew_Gw_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 22 */ {it_and,     am_Gb, am_Eb, am_Xx, decode_Gb_Eb_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 23 */ {it_and,     am_Gw, am_Ew, am_Xx, decode_Gw_Ew_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 24 */ {it_and,     am_AL, am_Ib, am_Xx, decode_AL_Ib_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 25 */ {it_and,     am_AX, am_Iw, am_Xx, decode_AX_Iw_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 26 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // Segment register override (ES)
    /* 27 */ {it_daa,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* 28 */ {it_sub,     am_Eb, am_Gb, am_Xx, decode_Eb_Gb_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 29 */ {it_sub,     am_Ew, am_Gw, am_Xx, decode_Ew_Gw_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 2a */ {it_sub,     am_Gb, am_Eb, am_Xx, decode_Gb_Eb_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 2b */ {it_sub,     am_Gw, am_Ew, am_Xx, decode_Gw_Ew_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 2c */ {it_sub,     am_AL, am_Ib, am_Xx, decode_AL_Ib_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 2d */ {it_sub,     am_AX, am_Iw, am_Xx, decode_AX_Iw_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 2e */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // Segment register override (CS)
    /* 2f */ {it_das,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},

    /* 30 */ {it_xor,     am_Eb, am_Gb, am_Xx, decode_Eb_Gb_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 31 */ {it_xor,     am_Ew, am_Gw, am_Xx, decode_Ew_Gw_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 32 */ {it_xor,     am_Gb, am_Eb, am_Xx, decode_Gb_Eb_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 33 */ {it_xor,     am_Gw, am_Ew, am_Xx, decode_Gw_Ew_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 34 */ {it_xor,     am_AL, am_Ib, am_Xx, decode_AL_Ib_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 35 */ {it_xor,     am_AX, am_Iw, am_Xx, decode_AX_Iw_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 36 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // Segment register override (SS)
    /* 37 */ {it_aaa,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* 38 */ {it_cmp,     am_Eb, am_Gb, am_Xx, decode_Eb_Gb_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 39 */ {it_cmp,     am_Ew, am_Gw, am_Xx, decode_Ew_Gw_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 3a */ {it_cmp,     am_Gb, am_Eb, am_Xx, decode_Gb_Eb_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 3b */ {it_cmp,     am_Gw, am_Ew, am_Xx, decode_Gw_Ew_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 3c */ {it_cmp,     am_AL, am_Ib, am_Xx, decode_AL_Ib_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* 3d */ {it_cmp,     am_AX, am_Iw, am_Xx, decode_AX_Iw_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* 3e */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // Segment register override (DS)
    /* 3f */ {it_aas,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},

    /* 40 */ {it_inc,     am_AX, am_Xx, am_Xx, decode_AX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 41 */ {it_inc,     am_CX, am_Xx, am_Xx, decode_CX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 42 */ {it_inc,     am_DX, am_Xx, am_Xx, decode_DX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 43 */ {it_inc,     am_BX, am_Xx, am_Xx, decode_BX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 44 */ {it_inc,     am_SP, am_Xx, am_Xx, decode_SP_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 45 */ {it_inc,     am_BP, am_Xx, am_Xx, decode_BP_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 46 */ {it_inc,     am_SI, am_Xx, am_Xx, decode_SI_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 47 */ {it_inc,     am_DI, am_Xx, am_Xx, decode_DI_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 48 */ {it_dec,     am_AX, am_Xx, am_Xx, decode_AX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 49 */ {it_dec,     am_CX, am_Xx, am_Xx, decode_CX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4a */ {it_dec,     am_DX, am_Xx, am_Xx, decode_DX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4b */ {it_dec,     am_BX, am_Xx, am_Xx, decode_BX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4c */ {it_dec,     am_SP, am_Xx, am_Xx, decode_SP_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4d */ {it_dec,     am_BP, am_Xx, am_Xx, decode_BP_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4e */ {it_dec,     am_SI, am_Xx, am_Xx, decode_SI_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 4f */ {it_dec,     am_DI, am_Xx, am_Xx, decode_DI_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},

    /* 50 */ {it_push,    am_AX, am_Xx, am_Xx, decode_AX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 51 */ {it_push,    am_CX, am_Xx, am_Xx, decode_CX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 52 */ {it_push,    am_DX, am_Xx, am_Xx, decode_DX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 53 */ {it_push,    am_BX, am_Xx, am_Xx, decode_BX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 54 */ {it_push,    am_SP, am_Xx, am_Xx, decode_SP_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 55 */ {it_push,    am_BP, am_Xx, am_Xx, decode_BP_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 56 */ {it_push,    am_SI, am_Xx, am_Xx, decode_SI_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 57 */ {it_push,    am_DI, am_Xx, am_Xx, decode_DI_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 58 */ {it_pop,     am_AX, am_Xx, am_Xx, decode_AX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 59 */ {it_pop,     am_CX, am_Xx, am_Xx, decode_CX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5a */ {it_pop,     am_DX, am_Xx, am_Xx, decode_DX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5b */ {it_pop,     am_BX, am_Xx, am_Xx, decode_BX_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5c */ {it_pop,     am_SP, am_Xx, am_Xx, decode_SP_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5d */ {it_pop,     am_BP, am_Xx, am_Xx, decode_BP_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5e */ {it_pop,     am_SI, am_Xx, am_Xx, decode_SI_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},
    /* 5f */ {it_pop,     am_DI, am_Xx, am_Xx, decode_DI_Xx_Xx, DT_OP_CODE_REG_16, _,                 _,         __},

    /* 60 */ {it_pusha,   am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* 61 */ {it_popa,    am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* 62 */ {it_bound,   am_Gw, am_Ma, am_Xx, decode_Gw_Ma_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 63 */ {it_arpl,    am_Ew, am_Gw, am_Xx, decode_Ew_Gw_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 64 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // Segment override:              FS??
    /* 65 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // Segment override:              GS??
    /* 66 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // Operand size override
    /* 67 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // Operand address size override
    /* 68 */ {it_push,    am_Iw, am_Xx, am_Xx, decode_Iw_Xx_Xx, DT_IMM_16,         _,                 _,         __},
    /* 69 */ {it_imul,    am_Gw, am_Ew, am_Iw, decode_Gw_Ew_Iw, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   DT_IMM_16, DF_HAS_MOD_RM},
    /* 6a */ {it_push,    am_Ib, am_Xx, am_Xx, decode_Ib_Xx_Xx, DT_IMM_8,          _,                 _,         __},
    /* 6b */ {it_imul,    am_Gw, am_Ew, am_Ib, decode_Gw_Ew_Ib, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   DT_IMM_8,  DF_HAS_MOD_RM},
    /* 6c */ {it_insb,    am_Yb, am_DX, am_Xx, decode_Yb_DX_Xx, _,                 DT_DX,             _,         __},
    /* 6d */ {it_insw,    am_Yw, am_DX, am_Xx, decode_Yw_DX_Xx, _,                 DT_DX,             _,         __},
    /* 6e */ {it_outsb,   am_DX, am_Xb, am_Xx, decode_DX_Xb_Xx, DT_DX,             _,                 _,         __},
    /* 6f */ {it_outsw,   am_DX, am_Xw, am_Xx, decode_DX_Xw_Xx, DT_DX,             _,                 _,         __},

    /* 70 */ {it_jo,      am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 71 */ {it_jno,     am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 72 */ {it_jb,      am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 73 */ {it_jnb,     am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 74 */ {it_jz,      am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 75 */ {it_jnz,     am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 76 */ {it_jbe,     am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 77 */ {it_jnbe,    am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 78 */ {it_js,      am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 79 */ {it_jns,     am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 7a */ {it_jp,      am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 7b */ {it_jnp,     am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 7c */ {it_jl,      am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 7d */ {it_jnl,     am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 7e */ {it_jle,     am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* 7f */ {it_jnle,    am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},

    /* 80 */ {it_cmp,     am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx, DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 81 */ {it_cmp,     am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx, DT_MOD_RM_RM_16,   DT_IMM_16,         _,         DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 82 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         DF_HAS_MOD_RM}, // Copy of 80??!
    /* 83 */ {it_cmp,     am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx, DT_MOD_RM_RM_16,   DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ADD|OR|ADC|SBB|AND|SUB|XOR
    /* 84 */ {it_test,    am_Eb, am_Gb, am_Xx, decode_Eb_Gb_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 85 */ {it_test,    am_Ew, am_Gw, am_Xx, decode_Ew_Gw_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 86 */ {it_xchg,    am_Eb, am_Gb, am_Xx, decode_Eb_Gb_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 87 */ {it_xchg,    am_Ew, am_Gw, am_Xx, decode_Ew_Gw_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 88 */ {it_mov,     am_Eb, am_Gb, am_Xx, decode_Eb_Gb_Xx, DT_MOD_RM_RM_8,    DT_MOD_RM_REG_8,   _,         DF_HAS_MOD_RM},
    /* 89 */ {it_mov,     am_Ew, am_Gw, am_Xx, decode_Ew_Gw_Xx, DT_MOD_RM_RM_16,   DT_MOD_RM_REG_16,  _,         DF_HAS_MOD_RM},
    /* 8a */ {it_mov,     am_Gb, am_Eb, am_Xx, decode_Gb_Eb_Xx, DT_MOD_RM_REG_8,   DT_MOD_RM_RM_8,    _,         DF_HAS_MOD_RM},
    /* 8b */ {it_mov,     am_Gw, am_Ew, am_Xx, decode_Gw_Ew_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 8c */ {it_mov,     am_Ew, am_Sw, am_Xx, decode_Ew_Sw_Xx, DT_MOD_RM_RM_16,   DT_SEGMENT_REG,    _,         DF_HAS_MOD_RM},
    /* 8d */ {it_lea,     am_Gw, am_M,  am_Xx, decode_Gw_M_Xx,  DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 8e */ {it_mov,     am_Sw, am_Ew, am_Xx, decode_Sw_Ew_Xx, DT_SEGMENT_REG,    DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* 8f */ {it_pop,     am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx, DT_MOD_RM_RM_16,   _,                 _,         DF_HAS_MOD_RM}, // XOP prefix? ?!?!?

    /* 90 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* 91 */ {it_xchg,    am_CX, am_AX, am_Xx, decode_CX_AX_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 92 */ {it_xchg,    am_DX, am_AX, am_Xx, decode_DX_AX_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 93 */ {it_xchg,    am_BX, am_AX, am_Xx, decode_BX_AX_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 94 */ {it_xchg,    am_SP, am_AX, am_Xx, decode_SP_AX_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 95 */ {it_xchg,    am_BP, am_AX, am_Xx, decode_BP_AX_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 96 */ {it_xchg,    am_SI, am_AX, am_Xx, decode_SI_AX_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 97 */ {it_xchg,    am_DI, am_AX, am_Xx, decode_DI_AX_Xx, DT_AX,             DT_OP_CODE_REG_16, _,         __},
    /* 98 */ {it_cbw,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* 99 */ {it_cwd,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* 9a */ {it_callf,   am_Ap, am_Xx, am_Xx, decode_Ap_Xx_Xx, DT_Ap,             _,                 _,         __},
    /* 9b */ {it_fwait,   am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* 9c */ {it_pushf,   am_Fw, am_Xx, am_Xx, decode_Fw_Xx_Xx, DT_Fw,             _,                 _,         __},
    /* 9d */ {it_popf,    am_Fw, am_Xx, am_Xx, decode_Fw_Xx_Xx, DT_Fw,             _,                 _,         __},
    /* 9e */ {it_sahf,    am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* 9f */ {it_lahf,    am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},

    /* a0 */ {it_mov,     am_AL, am_Ob, am_Xx, decode_AL_Ob_Xx, DT_AL,             DT_JMP_16,         _,         __},
    /* a1 */ {it_mov,     am_AX, am_Ow, am_Xx, decode_AX_Ow_Xx, DT_AX,             DT_JMP_16,         _,         __},
    /* a2 */ {it_mov,     am_Ob, am_AL, am_Xx, decode_Ob_AL_Xx, DT_JMP_16,         DT_AL,             _,         __},
    /* a3 */ {it_mov,     am_Ow, am_AX, am_Xx, decode_Ow_AX_Xx, DT_JMP_16,         DT_AX,             _,         __},
    /* a4 */ {it_movsb,   am_Xb, am_Yb, am_Xx, decode_Xb_Yb_Xx, _,                 _,                 _,         __},
    /* a5 */ {it_movsw,   am_Xw, am_Yw, am_Xx, decode_Xw_Yw_Xx, _,                 _,                 _,         __},
    /* a6 */ {it_cmpsb,   am_Xb, am_Yb, am_Xx, decode_Xb_Yb_Xx, _,                 _,                 _,         __},
    /* a7 */ {it_cmpsw,   am_Xw, am_Yw, am_Xx, decode_Xw_Yw_Xx, _,                 _,                 _,         __},
    /* a8 */ {it_test,    am_AL, am_Ib, am_Xx, decode_AL_Ib_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* a9 */ {it_test,    am_AX, am_Iw, am_Xx, decode_AX_Iw_Xx, DT_AX,             DT_IMM_16,         _,         __},
    /* aa */ {it_stosb,   am_Yb, am_AL, am_Xx, decode_Yb_AL_Xx, _,                 _,                 _,         __},            // rep stosb [ES:DI+offset],          al
    /* ab */ {it_stosw,   am_Yw, am_AX, am_Xx, decode_Yw_AX_Xx, _,                 _,                 _,         __},
    /* ac */ {it_lodsb,   am_AL, am_Xb, am_Xx, decode_AL_Xb_Xx, _,                 _,                 _,         __},
    /* ad */ {it_lodsw,   am_AX, am_Xw, am_Xx, decode_AX_Xw_Xx, _,                 _,                 _,         __},
    /* ae */ {it_scasb,   am_AL, am_Yb, am_Xx, decode_AL_Yb_Xx, _,                 _,                 _,         __},
    /* af */ {it_scasw,   am_AX, am_Yw, am_Xx, decode_AX_Yw_Xx, _,                 _,                 _,         __},

    /* b0 */ {it_mov,     am_AL, am_Ib, am_Xx, decode_AL_Ib_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b1 */ {it_mov,     am_CL, am_Ib, am_Xx, decode_CL_Ib_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b2 */ {it_mov,     am_DL, am_Ib, am_Xx, decode_DL_Ib_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b3 */ {it_mov,     am_BL, am_Ib, am_Xx, decode_BL_Ib_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b4 */ {it_mov,     am_AH, am_Ib, am_Xx, decode_AH_Ib_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b5 */ {it_mov,     am_CH, am_Ib, am_Xx, decode_CH_Ib_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b6 */ {it_mov,     am_DH, am_Ib, am_Xx, decode_DH_Ib_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b7 */ {it_mov,     am_BH, am_Ib, am_Xx, decode_BH_Ib_Xx, DT_OP_CODE_REG_8,  DT_IMM_8,          _,         __},
    /* b8 */ {it_mov,     am_AX, am_Iw, am_Xx, decode_AX_Iw_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* b9 */ {it_mov,     am_CX, am_Iw, am_Xx, decode_CX_Iw_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* ba */ {it_mov,     am_DX, am_Iw, am_Xx, decode_DX_Iw_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* bb */ {it_mov,     am_BX, am_Iw, am_Xx, decode_BX_Iw_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* bc */ {it_mov,     am_BP, am_Iw, am_Xx, decode_BP_Iw_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* bd */ {it_mov,     am_SP, am_Iw, am_Xx, decode_SP_Iw_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* be */ {it_mov,     am_DI, am_Iw, am_Xx, decode_DI_Iw_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},
    /* bf */ {it_mov,     am_SI, am_Iw, am_Xx, decode_SI_Iw_Xx, DT_OP_CODE_REG_16, DT_IMM_16,         _,         __},

    /* c0 */ {it_sar,     am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx, DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|SHR|SAL|SAR
    /* c1 */ {it_sar,     am_Ew, am_Ib, am_Xx, decode_Ew_Ib_Xx, DT_MOD_RM_RM_16,   DT_IMM_8,          _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|SHR|SAL|SAR
    /* c2 */ {it_ret,     am_Iw, am_Xx, am_Xx, decode_Iw_Xx_Xx, DT_IMM_16,         _,                 _,         __},
    /* c3 */ {it_ret,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // no operands?
    /* c4 */ {it_les,     am_Gw, am_Ew, am_Xx, decode_Gw_Ew_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* c5 */ {it_lds,     am_Gw, am_Ew, am_Xx, decode_Gw_Ew_Xx, DT_MOD_RM_REG_16,  DT_MOD_RM_RM_16,   _,         DF_HAS_MOD_RM},
    /* c6 */ {it_mov,     am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx, DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM},
    /* c7 */ {it_mov,     am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx, DT_MOD_RM_RM_16,   DT_IMM_16,         _,         DF_HAS_MOD_RM},
    /* c8 */ {it_enter,   am_Iw, am_Ib, am_Xx, decode_Iw_Ib_Xx, DT_IMM_16,         DT_IMM_8,          _,         __},
    /* c9 */ {it_leave,   am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* ca */ {it_retf,    am_Iw, am_Xx, am_Xx, decode_Iw_Xx_Xx, DT_IMM_16,         _,                 _,         __},
    /* cb */ {it_retf,    am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* cc */ {it_int3,    am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* cd */ {it_int,     am_Ib, am_Xx, am_Xx, decode_Ib_Xx_Xx, DT_IMM_8,          _,                 _,         __},
    /* ce */ {it_into,    am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* cf */ {it_iret,    am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},

    /* d0 */ {it_sar,     am_Eb, am_1,  am_Xx, decode_Eb_1_Xx,  DT_MOD_RM_RM_8,    1,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d1 */ {it_sar,     am_Ew, am_1,  am_Xx, decode_Ew_1_Xx,  DT_MOD_RM_RM_16,   1,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d2 */ {it_sar,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx, DT_MOD_RM_RM_8,    _,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d3 */ {it_sar,     am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx, DT_MOD_RM_RM_16,   _,                 _,         DF_HAS_MOD_RM}, // ROL|ROR|RCL|RCR|SHL|RHR|SAL|SAR
    /* d4 */ {it_aam,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* d5 */ {it_aad,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* d6 */ {it_salc,    am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* d7 */ {it_xlat,    am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* d8 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* d9 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* da */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* db */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* dc */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* dd */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* de */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // FPU instructions
    /* df */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // FPU instructions

    /* e0 */ {it_loopne,  am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* e1 */ {it_loope,   am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* e2 */ {it_loop,    am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* e3 */ {it_jcxz,    am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* e4 */ {it_in,      am_AL, am_Ib, am_Xx, decode_AL_Ib_Xx, DT_AL,             DT_IMM_8,          _,         __},
    /* e5 */ {it_in,      am_AX, am_Ib, am_Xx, decode_AX_Ib_Xx, DT_AX,             DT_IMM_8,          _,         __},
    /* e6 */ {it_out,     am_Ib, am_AL, am_Xx, decode_Ib_AL_Xx, DT_IMM_8,          DT_AL,             _,         __},
    /* e7 */ {it_out,     am_Ib, am_AX, am_Xx, decode_Ib_AX_Xx, DT_IMM_8,          DT_AX,             _,         __},
    /* e8 */ {it_call,    am_Jw, am_Xx, am_Xx, decode_Jw_Xx_Xx, DT_JMP_16,         _,                 _,         __},
    /* e9 */ {it_jmp,     am_Jw, am_Xx, am_Xx, decode_Jw_Xx_Xx, DT_JMP_16,         _,                 _,         __},
    /* ea */ {it_jmp,     am_Ap, am_Xx, am_Xx, decode_Ap_Xx_Xx, DT_SEG_DIRECT,     _,                 _,         __},            // mov cs:ip
    /* eb */ {it_jmp,     am_Jb, am_Xx, am_Xx, decode_Jb_Xx_Xx, DT_JMP_8,          _,                 _,         __},
    /* ec */ {it_in,      am_AL, am_DX, am_Xx, decode_AL_DX_Xx, DT_AL,             DT_DX,             _,         __},
    /* ed */ {it_in,      am_AX, am_DX, am_Xx, decode_AX_DX_Xx, DT_AX,             DT_DX,             _,         __},
    /* ee */ {it_out,     am_DX, am_AL, am_Xx, decode_DX_AL_Xx, DT_DX,             DT_AL,             _,         __},
    /* ef */ {it_out,     am_DX, am_AX, am_Xx, decode_DX_AX_Xx, DT_DX,             DT_AX,             _,         __},

    /* f0 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // Lock?
    /* f1 */ {it_int1,    am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* f2 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // repne prefix
    /* f3 */ {it_invalid, am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},            // rep prefix
    /* f4 */ {it_hlt,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* f5 */ {it_cmc,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* f6 */ {it_test,    am_Eb, am_Ib, am_Xx, decode_Eb_Ib_Xx, DT_MOD_RM_RM_8,    DT_IMM_8,          _,         DF_HAS_MOD_RM}, // TEST|NOT|NEG|MUL|IMUL|DIV|IDIV
    /* f7 */ {it_group,   am_Xx, am_Xx, am_Xx, 0,               DT_MOD_RM_RM_16,   DT_IMM_16,         _,         DF_HAS_MOD_RM}, // TEST|NOT|NEG|MUL|IMUL|DIV|IDIV
    /* f8 */ {it_clc,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* f9 */ {it_stc,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* fa */ {it_cli,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* fb */ {it_sti,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* fc */ {it_cld,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* fd */ {it_std,     am_Xx, am_Xx, am_Xx, decode_Xx_Xx_Xx, _,                 _,                 _,         __},
    /* fe */ {it_dec,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx, DT_MOD_RM_RM_8,    _,                 _,         DF_HAS_MOD_RM},
    /* ff */ {it_inc,     am_Eb, am_Xx, am_Xx, decode_Eb_Xx_Xx, DT_MOD_RM_RM_8,    _,                 _,         DF_HAS_MOD_RM},
};

struct op_code_mapping op_code_table_group_f7[] = {
    /* 00 */ {it_test, am_Ew, am_Iw, am_Xx, decode_Ew_Iw_Xx, _, _, _, DF_HAS_MOD_RM},
    /* 01 */ {it_invalid},
    /* 02 */ {it_not,  am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx, _, _, _, DF_HAS_MOD_RM},
    /* 03 */ {it_neg,  am_Ew, am_Xx, am_Xx, decode_Ew_Xx_Xx, _, _, _, DF_HAS_MOD_RM},
    /* 04 */ {it_mul,  am_AX, am_Ew, am_Xx, decode_AX_Ew_Xx, _, _, _, DF_HAS_MOD_RM},
    /* 05 */ {it_imul, am_AX, am_Ew, am_Xx, decode_AX_Ew_Xx, _, _, _, DF_HAS_MOD_RM},
    /* 06 */ {it_div,  am_AX, am_Ew, am_Xx, decode_AX_Ew_Xx, _, _, _, DF_HAS_MOD_RM},
    /* 07 */ {it_idiv, am_AX, am_Ew, am_Xx, decode_AX_Ew_Xx, _, _, _, DF_HAS_MOD_RM},
};
// clang-format on
