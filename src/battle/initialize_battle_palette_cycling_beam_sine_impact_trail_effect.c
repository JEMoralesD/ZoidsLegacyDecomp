#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattlePaletteCyclingBeamSineImpactTrailEffect(int group_address) asm("func_080DF068");

void InitializeBattlePaletteCyclingBeamSineImpactTrailEffect(int group_address) {
    s32 *effect_state_slot = (s32 *)(group_address + BATTLE_ANIMATION_OFFSET(state));
    s32 *impact_started_slot = (s32 *)(group_address + BATTLE_ANIMATION_OFFSET(effect.palette_cycling_beam_sine_impact.impact_started));
    s32 *emission_count_slot = (s32 *)(group_address + BATTLE_ANIMATION_OFFSET(effect.palette_cycling_beam_sine_impact.emission_count));
    *emission_count_slot = 0;
    *impact_started_slot = 0;
    *effect_state_slot = 0;
    *(s16 *)0x03000050 = 0x1010;
}
