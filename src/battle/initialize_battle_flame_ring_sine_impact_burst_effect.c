#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 gRandomNumberCallback asm("D_03000010");
s32 CallFunctionR0(s32) asm("func_80ECD5C");

void InitializeBattleFlameRingSineImpactBurstEffect(struct BattleAnimationGroup *group) asm("func_080E0298");

void InitializeBattleFlameRingSineImpactBurstEffect(struct BattleAnimationGroup *group) {
    s32 *impact_y_slot;
    s32 impact_y;
    s32 random_bits;

    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    BATTLE_ANIMATION_FIELD(group, s32, effect.impact.x) = (BATTLE_ANIMATION_FIELD(group, s32, x) + ((u32)(CallFunctionR0(gRandomNumberCallback) * 0x41) >> 0xF)) - 0x20;
    random_bits = CallFunctionR0(gRandomNumberCallback);
    impact_y_slot = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.impact.y));
    impact_y = BATTLE_ANIMATION_FIELD(group, s32, y) - 0x10;
    *impact_y_slot = impact_y + ((u32)(random_bits * 0x21) >> 0xF);
}
