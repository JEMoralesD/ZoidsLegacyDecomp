#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleRisingGreenOrbSmokeEffect(void *group) asm("func_080D8DE4");

void InitializeBattleRisingGreenOrbSmokeEffect(void *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
