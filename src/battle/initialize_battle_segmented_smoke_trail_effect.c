#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleSegmentedSmokeTrailEffect(struct BattleAnimationGroup *group) asm("func_080D4C7C");

void InitializeBattleSegmentedSmokeTrailEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
