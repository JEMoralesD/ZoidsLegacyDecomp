#include "m2c_prelude.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");                      /* extern */
M2C_UNK QueueCopy(s32, s32, s32) asm("func_08095208");               /* extern */
void *func_080D2450(void *, s32, s32, s32, s32, s32, s32, s32); /* extern */
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");                         /* extern */

extern u8 D_087A2CCC[];
extern u8 D_087A2B0C[];
extern s32 gBattleAnimationPaletteBanks asm("D_020348B4");
extern u8 D_05000200[];

void sub_080DF288(void *arg0) {
    s32 temp_r1;
    register void *temp_r2 asm("r2");

    temp_r1 = M2C_FIELD(arg0, s32 *, 0x8C);
    if (temp_r1 == 0) {
        M2C_FIELD(arg0, void **, 0xC) = func_080D2450(arg0, 0, 1, 0x80, (s32) M2C_FIELD(arg0, s16 *, 8), 0x400, temp_r1, 1);
        PlayBattleAnimationSound(0);
        M2C_FIELD(arg0, s32 *, 0x8C) = (s32) (M2C_FIELD(arg0, s32 *, 0x8C) + 1);
        return;
    }
    temp_r2 = M2C_FIELD(arg0, void **, 0xC);
    if (temp_r2 == 0) {
        DestroySpriteGroup(arg0);
        return;
    }
    if (M2C_FIELD(temp_r2, u16 *, 0x16) == 0) {
        QueueCopy(
            ((s32)D_087A2CCC[M2C_FIELD(temp_r2, u16 *, 0x14)] << 5) +
                (s32)D_087A2B0C,
            (gBattleAnimationPaletteBanks << 5) + (s32)D_05000200,
            0x20);
    }
}
