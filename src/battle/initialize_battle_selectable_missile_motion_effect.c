#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleSelectableMissileMotionEffect(int group_address) asm("func_080DDDD0");

void InitializeBattleSelectableMissileMotionEffect(int group_address) {
    int non_priority_flags;
    *(int *)(group_address + BATTLE_ANIMATION_OFFSET(state)) = 0;
    non_priority_flags = *(int *)(group_address + BATTLE_ANIMATION_OFFSET(sprite_flags)) & ~BATTLE_ANIMATION_SPRITE_PRIORITY_MASK;
    if (non_priority_flags == 0)
        *(int *)(group_address + BATTLE_ANIMATION_OFFSET(effect.missile_motion_variant.use_rising_motion)) = non_priority_flags;
    else
        *(int *)(group_address + BATTLE_ANIMATION_OFFSET(effect.missile_motion_variant.use_rising_motion)) = 1;
    *(int *)(group_address + BATTLE_ANIMATION_OFFSET(sprite_flags)) = *(int *)(group_address + BATTLE_ANIMATION_OFFSET(sprite_flags)) & BATTLE_ANIMATION_SPRITE_PRIORITY_MASK;
}
