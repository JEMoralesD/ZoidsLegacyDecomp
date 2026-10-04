#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleSmokeTrailRingImpactEffect(void *group) asm("func_080D5584");

void InitializeBattleSmokeTrailRingImpactEffect(void *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
