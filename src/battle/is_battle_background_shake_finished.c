#include "m2c_prelude.h"
#include "battle_animation.h"
s32 IsBattleBackgroundShakeFinished(void) asm("func_080D222C");

s32 IsBattleBackgroundShakeFinished(void) {
    s32 finished;

    finished = 0;
    if (*(u8 *)0x02034869 == 0) {
        finished = 1;
    }
    return finished;
}
