#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern volatile u16 gBattleBlendAlpha asm("D_03000050");

void InitializeBattleFixedXPaletteCyclingBeamEffect(struct BattleAnimationGroup *group) asm("func_080DF270");

void InitializeBattleFixedXPaletteCyclingBeamEffect(struct BattleAnimationGroup *group)
{
    group->state = 0;
    gBattleBlendAlpha = 0x1010;
}
