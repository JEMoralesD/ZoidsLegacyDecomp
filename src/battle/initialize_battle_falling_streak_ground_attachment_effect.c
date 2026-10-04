#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern u32 CallFunctionR0(s32) asm("func_080ECD5C");
extern s32 gRandomNumberCallback asm("D_03000010");

void InitializeBattleFallingStreakGroundAttachmentEffect(struct BattleAnimationGroup *group) asm("func_080DC258");

void InitializeBattleFallingStreakGroundAttachmentEffect(struct BattleAnimationGroup *group) {
    register s32 *phase_or_priority asm("r2") = (s32 *)((s32)group + BATTLE_ANIMATION_OFFSET(state));
    register s32 *effect_parameter_slot asm("r1") = (s32 *)((s32)group + BATTLE_ANIMATION_OFFSET(effect.falling_streak_ground.elapsed_updates));
    u32 depth_variant;
    *effect_parameter_slot = 0;
    *phase_or_priority = 0;
    depth_variant = CallFunctionR0(gRandomNumberCallback);
    effect_parameter_slot = (s32 *)((s32)group + BATTLE_ANIMATION_OFFSET(effect.falling_streak_ground.depth_variant));
    depth_variant = depth_variant >> 0xE;
    *effect_parameter_slot = depth_variant;
    effect_parameter_slot += (BATTLE_ANIMATION_OFFSET(sprite_flags) - BATTLE_ANIMATION_OFFSET(effect.falling_streak_ground.depth_variant)) / sizeof(s32);
    phase_or_priority = (s32 *)BATTLE_ANIMATION_HARDWARE_SPRITE_PRIORITY_3;
    if (depth_variant == 0) phase_or_priority = (s32 *)BATTLE_ANIMATION_HARDWARE_SPRITE_PRIORITY_1;
    *effect_parameter_slot = (s32)phase_or_priority;
}
