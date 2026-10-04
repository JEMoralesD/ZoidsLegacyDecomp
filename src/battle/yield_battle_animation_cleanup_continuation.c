#include "m2c_prelude.h"
__attribute__((naked)) void YieldBattleAnimationCleanupContinuation(void) asm("func_080CFD3C");

__attribute__((naked)) void YieldBattleAnimationCleanupContinuation(void) {
    asm("movs r0, #1");
    asm("bl func_80ED17C");
}
