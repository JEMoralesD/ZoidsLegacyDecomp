#include "m2c_prelude.h"
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");                         /* extern */

void sub_080DC47C(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x8C) = 0;
    PlayBattleAnimationSound(0);
}
