#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleScreenWidthSmokeTrailEffect(void *group) asm("func_080D54CC");

void InitializeBattleScreenWidthSmokeTrailEffect(void *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
