#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern volatile u16 gBattleBlendAlpha asm("D_03000050");

void InitializeBattleChargedCyanOrbProjectileEffect(struct BattleAnimationGroup *group) asm("func_080DF82C");

void InitializeBattleChargedCyanOrbProjectileEffect(struct BattleAnimationGroup *group)
{
    s32 *effect_state_slot = &group->state;
    s32 *charge_updates_slot = (s32 *)&group->effect.charged_cyan_orb_projectile.progress.charge_updates;

    *charge_updates_slot = 0;
    *effect_state_slot = 0;
    gBattleBlendAlpha = 0x1010;
}
