#include "m2c_prelude.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");                      /* extern */
M2C_UNK StartBattleScanlineWindowOpening() asm("func_080D1C70");                            /* extern */
u8 GetBattleScanlineWindowPhase() asm("func_080D1E38");                                 /* extern */
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");                         /* extern */

void sub_080DBE60(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x8C) == 0) {
        StartBattleScanlineWindowOpening();
        PlayBattleAnimationSound(0);
        M2C_FIELD(arg0, s32 *, 0x8C) = (s32) (M2C_FIELD(arg0, s32 *, 0x8C) + 1);
        return;
    }
    if (GetBattleScanlineWindowPhase() == 1) {
        DestroySpriteGroup(arg0);
    }
}
