#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleThinBeamRayImpactEffect(struct BattleAnimationGroup *group) asm("func_080DDC40");

void InitializeBattleThinBeamRayImpactEffect(struct BattleAnimationGroup *group) {
    int *effect_state_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(state));
    int *particle_updates_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.thin_beam_impact.particle_updates));
    *particle_updates_slot = 0;
    *effect_state_slot = 0;
}
