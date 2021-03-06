#ifndef DECODE_FUNCS_H_
#define DECODE_FUNCS_H_

#include "decoder_context.h"

#include <base/reader.h>
#include <instructions/instructions.h>

void decode_AL_DX_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AL_Eb_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AL_Ib_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AL_Ob_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AL_Xb_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AL_Yb_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AX_DX_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AX_Ew_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AX_Ib_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AX_Iw_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AX_Ow_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AX_Rw_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AX_Xw_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_AX_Yw_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Ap_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_CS_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_DS_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_DX_AL_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_DX_AX_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_DX_Xb_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_DX_Xw_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_ES_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Eb_1_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Eb_Gb_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Eb_Ib_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Eb_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Ew_1_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Ew_Gw_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Ew_Ib_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Ew_Iw_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Ew_Sw_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Ew_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Fw_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Gb_Eb_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Gw_Ew_Ib(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Gw_Ew_Iw(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Gw_Ew_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Gw_M_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Gw_Ma_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Ib_AL_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Ib_AX_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Ib_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Iw_Ib_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Iw_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Jb_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Jw_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Ob_AL_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Ow_AX_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Rb_Ib_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Rw_Iw_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Rw_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_SS_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Sw_Ew_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Xb_Yb_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Xw_Yw_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Xx_Xx_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Yb_AL_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Yb_DX_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Yw_AX_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_Yw_DX_Xx(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_rep_prefix(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_repne_prefix(struct decoder_context *decoder_context, struct instruction *instruction);
void decode_seg_reg_override_cs(struct decoder_context *decoder_context,
                                struct instruction *instruction);
void decode_seg_reg_override_ds(struct decoder_context *decoder_context,
                                struct instruction *instruction);
void decode_seg_reg_override_es(struct decoder_context *decoder_context,
                                struct instruction *instruction);
void decode_seg_reg_override_ss(struct decoder_context *decoder_context,
                                struct instruction *instruction);

#endif // DECODE_FUNCS_H_
