#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleLightningFadeAtSortPriority3Effect(struct BattleAnimationGroup *group) asm("func_080DC914");

void InitializeBattleLightningFadeAtSortPriority3Effect(struct BattleAnimationGroup *group) {
    register s32 *effect_state_slot asm("r3");
    register s32 *fade_updates_slot asm("r2");
    register s32 *blend_progress_slot asm("r1");
    register s32 initial_value asm("r0");

    effect_state_slot = group;
    effect_state_slot += BATTLE_ANIMATION_OFFSET(state) / sizeof(s32);
    fade_updates_slot = group;
    fade_updates_slot += BATTLE_ANIMATION_OFFSET(effect.lightning_fade.fade_updates) / sizeof(s32);
    blend_progress_slot = group;
    blend_progress_slot += BATTLE_ANIMATION_OFFSET(effect.lightning_fade.backdrop_blend_progress) / sizeof(s32);
    initial_value = 0;
    *blend_progress_slot = initial_value;
    *fade_updates_slot = initial_value;
    *effect_state_slot = initial_value;
}
