#include "m2c_prelude.h"
#include "battle_animation.h"
s32 IsBattleAnimationCameraReady(void) asm("func_080D18CC");

s32 IsBattleAnimationCameraReady(void) {
    s32 ready;

    ready = 0;
    if ((u32) *(u8 *)0x02034030 <= 1U) {
        ready = 1;
    }
    return ready;
}
