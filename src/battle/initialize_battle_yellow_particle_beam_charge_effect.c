#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleYellowParticleBeamChargeEffect(struct BattleAnimationGroup *group) asm("func_080E04E8");

void InitializeBattleYellowParticleBeamChargeEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
