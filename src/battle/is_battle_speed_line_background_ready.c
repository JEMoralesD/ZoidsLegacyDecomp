#include "m2c_prelude.h"
#include "battle_animation.h"
s32 IsBattleSpeedLineBackgroundReady(void) asm("func_080D1A24");

s32 IsBattleSpeedLineBackgroundReady(void) {
    s32 ready;

    ready = 0;
    if ((u32) (u8) (*(u8 *)0x02034860 - 1) > 0x1FU) {
        ready = 1;
    }
    return ready;
}
