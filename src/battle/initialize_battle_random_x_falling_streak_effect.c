#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleRandomXFallingStreakEffect(struct BattleAnimationGroup *group) asm("func_080DC1E0");

void InitializeBattleRandomXFallingStreakEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
