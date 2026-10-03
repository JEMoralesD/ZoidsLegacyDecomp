#include "m2c_prelude.h"
#include "battle_animation.h"
void StopBattleBackgroundShake(void) asm("func_080D220C");

void StopBattleBackgroundShake(void) {
    *(u16 *)0x0300004C &= 0xFDFF;
    *(s8 *)0x02034869 = BATTLE_BACKGROUND_SHAKE_IDLE;
}
