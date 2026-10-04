#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleFixedXThinCyanBeamEffect(struct BattleAnimationGroup *group) asm("func_080DDBE4");

void InitializeBattleFixedXThinCyanBeamEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
