#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void InitializeBattleLightningFadeSparkAndParticleEffect(struct BattleAnimationGroup *group) asm("func_080DCA44");

void InitializeBattleLightningFadeSparkAndParticleEffect(struct BattleAnimationGroup *group) {
    s32 *effect_state_slot = &group->state;
    s32 *fade_updates_slot = &group->effect.lightning_fade.fade_updates;
    s32 *blend_progress_slot = &group->effect.lightning_fade.backdrop_blend_progress;
    *blend_progress_slot = 0;
    *fade_updates_slot = 0;
    *effect_state_slot = 0;
    *(s16 *)0x03000050 = 0x1010;
}
