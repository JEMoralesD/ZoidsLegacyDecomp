#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleScreenWidthBeamImpactEffect(void *group) asm("func_080D6038");

void InitializeBattleScreenWidthBeamImpactEffect(void *group) {
    register s32 *state_slot asm("r3");
    register s32 *impact_started_slot asm("r2");
    register s32 *impact_count_slot asm("r1");
    register s32 zero asm("r0");

    state_slot = group;
    state_slot = (s32 *)((u8 *)state_slot + BATTLE_ANIMATION_OFFSET(state));
    impact_started_slot = group;
    impact_started_slot = (s32 *)((u8 *)impact_started_slot + BATTLE_ANIMATION_OFFSET(effect.screen_width_beam_impact.impact_started));
    impact_count_slot = group;
    impact_count_slot = (s32 *)((u8 *)impact_count_slot + BATTLE_ANIMATION_OFFSET(effect.screen_width_beam_impact.impact_count));
    zero = 0;
    *impact_count_slot = zero;
    *impact_started_slot = zero;
    *state_slot = zero;
}
