#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleCyanBeamSineImpactTrailEffect(struct BattleAnimationGroup *group) asm("func_080DD054");

void InitializeBattleCyanBeamSineImpactTrailEffect(struct BattleAnimationGroup *group) {
    register s32 *effect_state_slot asm("r3");
    register s32 *impact_started_slot asm("r2");
    register s32 *emission_count_slot asm("r1");
    register s32 initial_value asm("r0");

    effect_state_slot = group;
    effect_state_slot += BATTLE_ANIMATION_OFFSET(state) / sizeof(s32);
    impact_started_slot = group;
    impact_started_slot += BATTLE_ANIMATION_OFFSET(effect.cyan_beam_sine_impact.impact_started) / sizeof(s32);
    emission_count_slot = group;
    emission_count_slot += BATTLE_ANIMATION_OFFSET(effect.cyan_beam_sine_impact.emission_count) / sizeof(s32);
    initial_value = 0;
    *emission_count_slot = initial_value;
    *impact_started_slot = initial_value;
    *effect_state_slot = initial_value;
}
