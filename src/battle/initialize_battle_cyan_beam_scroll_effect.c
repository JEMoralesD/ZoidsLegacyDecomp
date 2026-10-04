#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleCyanBeamScrollEffect(struct BattleAnimationGroup *group) asm("func_080DB8C4");

void InitializeBattleCyanBeamScrollEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
