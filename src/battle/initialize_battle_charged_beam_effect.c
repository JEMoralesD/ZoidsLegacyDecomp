#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleChargedBeamEffect(void *group) asm("func_080D59E8");

void InitializeBattleChargedBeamEffect(void *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
