#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern volatile u16 gBattleBlendAlpha asm("D_03000050");

void InitializeBattleLightningFadeEffect(struct BattleAnimationGroup *group) asm("func_080DCC88");

void InitializeBattleLightningFadeEffect(struct BattleAnimationGroup *group)
{
    s32 *effect_state_slot = &group->state;
    s32 *fade_updates_slot = &group->effect.lightning_fade.fade_updates;
    s32 *blend_progress_slot = &group->effect.lightning_fade.backdrop_blend_progress;

    *blend_progress_slot = 0;
    *fade_updates_slot = 0;
    *effect_state_slot = 0;
    gBattleBlendAlpha = 0x1010;
}
