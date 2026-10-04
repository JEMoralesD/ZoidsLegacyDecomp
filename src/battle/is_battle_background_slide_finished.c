#include "m2c_prelude.h"
#include "battle_animation.h"
s32 IsBattleBackgroundSlideFinished(void) asm("func_080D1B58");

s32 IsBattleBackgroundSlideFinished(void) {
    s32 finished;

    finished = 0;
    if ((u32) (u8) (*(u8 *)0x02034861 - 1) > 0x3FU) {
        finished = 1;
    }
    return finished;
}
