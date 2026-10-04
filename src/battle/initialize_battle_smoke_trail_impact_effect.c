#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 gRandomNumberCallback asm("D_03000010");
s32 CallFunctionR0(s32) asm("func_80ECD5C");

void InitializeBattleSmokeTrailImpactEffect(struct BattleAnimationGroup *group) asm("func_080D4520");

void InitializeBattleSmokeTrailImpactEffect(struct BattleAnimationGroup *group) {
    s32 *impact_y;
    s32 base_y;
    s32 random;

    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    BATTLE_ANIMATION_FIELD(group, s32, effect.impact.x) = (BATTLE_ANIMATION_FIELD(group, s32, x) + ((u32)(CallFunctionR0(gRandomNumberCallback) * 0x41) >> 0xF)) - 0x20;
    random = CallFunctionR0(gRandomNumberCallback);
    impact_y = &BATTLE_ANIMATION_FIELD(group, s32, effect.impact.y);
    base_y = BATTLE_ANIMATION_FIELD(group, s32, y) - 0x10;
    *impact_y = base_y + ((u32)(random * 0x21) >> 0xF);
}
