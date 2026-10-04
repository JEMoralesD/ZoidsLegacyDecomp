#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern u32 CallFunctionR0(s32) asm("func_80ECD5C");
extern s32 gRandomNumberCallback asm("D_03000010");

void InitializeBattleRisingMissileImpactFragmentFanEffect(struct BattleAnimationGroup *group) asm("func_080DF630");

void InitializeBattleRisingMissileImpactFragmentFanEffect(struct BattleAnimationGroup *group) {
    u32 y_random;
    s32 *impact_y_slot;
    s32 impact_y_base;
    BATTLE_ANIMATION_FIELD(group, s32, state) = BATTLE_ANIMATION_FIELD(group, s32, effect.rising_missile_fragment_fan.unused) = 0;
    BATTLE_ANIMATION_FIELD(group, s32, effect.rising_missile_fragment_fan.impact_x) = BATTLE_ANIMATION_FIELD(group, s32, x) + ((CallFunctionR0(gRandomNumberCallback) * 65) >> 15) - 0x20;
    y_random = CallFunctionR0(gRandomNumberCallback);
    impact_y_slot = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.rising_missile_fragment_fan.impact_y));
    impact_y_base = BATTLE_ANIMATION_FIELD(group, s32, y) - 0x10;
    *impact_y_slot = impact_y_base + ((y_random * 33) >> 15);
}
