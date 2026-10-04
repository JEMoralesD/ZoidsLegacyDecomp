#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleFixedXCyanBeamEffect(struct BattleAnimationGroup *group) asm("func_080DD218");

void InitializeBattleFixedXCyanBeamEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
