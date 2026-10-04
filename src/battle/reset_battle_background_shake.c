#include "m2c_prelude.h"
#include "battle_animation.h"
void ResetBattleBackgroundShake(void) asm("func_080D2180");

void ResetBattleBackgroundShake(void) {
    *(s8 *)0x02034869 = BATTLE_BACKGROUND_SHAKE_IDLE;
}
