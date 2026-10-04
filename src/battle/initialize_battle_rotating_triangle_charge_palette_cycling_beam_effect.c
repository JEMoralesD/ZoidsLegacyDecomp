#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleRotatingTriangleChargePaletteCyclingBeamEffect(struct BattleAnimationGroup *group) asm("func_080DEB5C");

void InitializeBattleRotatingTriangleChargePaletteCyclingBeamEffect(struct BattleAnimationGroup *group) {
    int *effect_state_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(state));
    int *rotation_angle_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.triangle_charge_palette_cycling_beam.phase.window.rotation_angle));
    int *yaw_angle_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.triangle_charge_palette_cycling_beam.phase.window.yaw_angle));
    int *narrowing_progress_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.triangle_charge_palette_cycling_beam.phase.window.narrowing_progress));
    int *charge_particle_flags_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.triangle_charge_palette_cycling_beam.charge_particle_flags));
    *charge_particle_flags_slot = 0;
    *narrowing_progress_slot = 0;
    *yaw_angle_slot = 0;
    *rotation_angle_slot = 0;
    *effect_state_slot = 0;
}
