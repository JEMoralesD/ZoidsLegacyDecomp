#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleRotatingTriangleChargeThinBeamEffect(struct BattleAnimationGroup *group) asm("func_080DD274");

void InitializeBattleRotatingTriangleChargeThinBeamEffect(struct BattleAnimationGroup *group) {
    int *effect_state_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(state));
    int *rotation_angle_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.triangle_charge_thin_beam.window.rotation_angle));
    int *yaw_angle_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.triangle_charge_thin_beam.window.yaw_angle));
    int *narrowing_progress_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.triangle_charge_thin_beam.window.narrowing_progress));
    *narrowing_progress_slot = 0;
    *yaw_angle_slot = 0;
    *rotation_angle_slot = 0;
    *effect_state_slot = 0;
}
