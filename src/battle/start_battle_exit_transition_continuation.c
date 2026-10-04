#include "m2c_prelude.h"
__attribute__((naked)) void StartBattleExitTransitionContinuation(int transition_kind) asm("func_080BE334");

__attribute__((naked)) void StartBattleExitTransitionContinuation(int transition_kind) {
    asm("movs r1, #8");
    asm("bl func_8096308");
    asm("b func_80BE3D6");
}
