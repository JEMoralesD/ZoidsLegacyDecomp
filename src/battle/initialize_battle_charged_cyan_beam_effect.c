#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleChargedCyanBeamEffect(struct BattleAnimationGroup *group) asm("func_080DCEA8");

void InitializeBattleChargedCyanBeamEffect(struct BattleAnimationGroup *group) {
    register s32 *effect_state_slot asm("r3");
    register s32 *charge_updates_slot asm("r2");
    register s32 *beam_delay_updates_slot asm("r1");
    register s32 initial_value asm("r0");

    effect_state_slot = group;
    effect_state_slot += BATTLE_ANIMATION_OFFSET(state) / sizeof(s32);
    charge_updates_slot = group;
    charge_updates_slot += BATTLE_ANIMATION_OFFSET(effect.charged_cyan_beam.charge_updates) / sizeof(s32);
    beam_delay_updates_slot = group;
    beam_delay_updates_slot += BATTLE_ANIMATION_OFFSET(effect.charged_cyan_beam.beam_delay_updates) / sizeof(s32);
    initial_value = 0;
    *beam_delay_updates_slot = initial_value;
    *charge_updates_slot = initial_value;
    *effect_state_slot = initial_value;
}
