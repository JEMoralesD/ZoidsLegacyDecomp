#include "m2c_prelude.h"
#include "battle_animation.h"
void RequestBattleBackgroundShakeStop(void) asm("func_080D2200");

void RequestBattleBackgroundShakeStop(void) {
    *(s8 *)0x02034869 = BATTLE_BACKGROUND_SHAKE_STOPPING;
}
