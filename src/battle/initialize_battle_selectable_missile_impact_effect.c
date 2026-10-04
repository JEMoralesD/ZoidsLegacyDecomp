#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
s32 CallFunctionR0(s32) asm("func_080ECD5C");

void InitializeBattleSelectableMissileImpactEffect(struct BattleAnimationGroup *group) asm("func_080DDF24");

void InitializeBattleSelectableMissileImpactEffect(struct BattleAnimationGroup *group) {
    s32 non_priority_flags;

    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    BATTLE_ANIMATION_FIELD(group, s32, effect.missile_impact_variant.x) = (s32)((BATTLE_ANIMATION_FIELD(group, s32, x) + ((u32)(CallFunctionR0(*(s32 *)0x03000010) * 0x41) >> 0xF)) - 0x20);
    {
        s32 y_random;
        s32 impact_y;
        s32 *impact_y_slot;

        y_random = CallFunctionR0(*(s32 *)0x03000010);
        impact_y_slot = &BATTLE_ANIMATION_FIELD(group, s32, effect.missile_impact_variant.y);
        impact_y = BATTLE_ANIMATION_FIELD(group, s32, y);
        impact_y -= 0x10;
        impact_y += (u32)(y_random * 0x21) >> 0xF;
        *impact_y_slot = impact_y;
    }
    non_priority_flags = BATTLE_ANIMATION_FIELD(group, s32, sprite_flags) & ~BATTLE_ANIMATION_SPRITE_PRIORITY_MASK;
    if (non_priority_flags == 0) {
        BATTLE_ANIMATION_FIELD(group, s32, effect.missile_impact_variant.use_diagonal_motion) = non_priority_flags;
    } else {
        BATTLE_ANIMATION_FIELD(group, s32, effect.missile_impact_variant.use_diagonal_motion) = 1;
    }
    BATTLE_ANIMATION_FIELD(group, s32, sprite_flags) = (s32)(BATTLE_ANIMATION_FIELD(group, s32, sprite_flags) & BATTLE_ANIMATION_SPRITE_PRIORITY_MASK);
}
