#include "m2c_prelude.h"
extern void PlayBattleAnimationSound() asm("func_080D2790");

void sub_080DC560(int arg0) {
    *(u32 *)(arg0 + 0x8c) = 0;
    *(u16 *)0x03000050 = 0x1010;
    PlayBattleAnimationSound(0);
}
