#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
s32 CallFunctionR0(s32) asm("func_080ECD5C");

void InitializeBattleRandomOffsetSelectableMissileTrailEffect(struct BattleAnimationGroup *group) asm("func_080DE254");

void InitializeBattleRandomOffsetSelectableMissileTrailEffect(struct BattleAnimationGroup *group) {
    s32 non_priority_flags;
    register s32 random asm("r0");
    u32 *launch_offset_slot;
    u32 launch_offset_bits;

    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    non_priority_flags = BATTLE_ANIMATION_FIELD(group, s32, sprite_flags) & ~BATTLE_ANIMATION_SPRITE_PRIORITY_MASK;
    if (non_priority_flags == 0) {
        BATTLE_ANIMATION_FIELD(group, s32, effect.random_offset_selectable_missile.use_diagonal_motion) = non_priority_flags;
        random = CallFunctionR0(*(s32 *)0x03000010);
        launch_offset_slot = (u32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.random_offset_selectable_missile.launch_offset_pixels));
        launch_offset_bits = ((u32) (random * 0x21) >> 0xF) - 0x10;
    } else {
        BATTLE_ANIMATION_FIELD(group, s32, effect.random_offset_selectable_missile.use_diagonal_motion) = 1;
        random = CallFunctionR0(*(s32 *)0x03000010);
        launch_offset_slot = (u32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.random_offset_selectable_missile.launch_offset_pixels));
        launch_offset_bits = (u32) (random * 0x21) >> 0xF;
    }
    *launch_offset_slot = launch_offset_bits;
    BATTLE_ANIMATION_FIELD(group, s32, sprite_flags) = (s32) (BATTLE_ANIMATION_FIELD(group, s32, sprite_flags) & BATTLE_ANIMATION_SPRITE_PRIORITY_MASK);
}
