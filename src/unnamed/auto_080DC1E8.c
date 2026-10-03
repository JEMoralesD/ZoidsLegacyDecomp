#include "m2c_prelude.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");                      /* extern */
s32 func_080D2660(void *, s32, s32, s16, s32, s32, s32, s32, s32); /* extern */
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");                         /* extern */
s32 CallFunctionR0(s32) asm("func_080ECD5C");                             /* extern */

void sub_080DC1E8(void *arg0) {
    s32 temp_r6;

    temp_r6 = M2C_FIELD(arg0, s32 *, 0x8C);
    if (temp_r6 == 0) {
        M2C_FIELD(arg0, s32 *, 0xC) = func_080D2660(arg0, 0, 0, (s16) (M2C_FIELD(arg0, s32 *, 4) + ((u32) (CallFunctionR0(*(s32 *)0x03000010) * 0x41) >> 0xF)), temp_r6, 0x120, 0x4A, 0xC00, 1);
        PlayBattleAnimationSound(0);
        M2C_FIELD(arg0, s32 *, 0x8C) = (s32) (M2C_FIELD(arg0, s32 *, 0x8C) + 1);
        return;
    }
    if (M2C_FIELD(arg0, s32 *, 0xC) == 0) {
        DestroySpriteGroup(arg0);
    }
}
