#include "m2c_prelude.h"
#include "battle_animation.h"
void StartBattleSpeedLineBackground(void) asm("func_080D18E4");

void StartBattleSpeedLineBackground(void) {
    *(s8 *)0x02034860 = 1;
}
