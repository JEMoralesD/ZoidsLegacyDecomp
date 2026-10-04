#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s16 gBattleBlendAlpha asm("D_03000050");
void InitializeBattleCyanColumnWhiteFadeExplosionEffect(struct BattleAnimationGroup *group) asm("func_080DFD64");

void InitializeBattleCyanColumnWhiteFadeExplosionEffect(struct BattleAnimationGroup *group) {
    *(int *)((char *)group + BATTLE_ANIMATION_OFFSET(state)) = 0;
    gBattleBlendAlpha = 0x1010;
}
