#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
s32 CallFunctionR0(s32) asm("func_080ECD5C");
extern s32 gRandomNumberCallback asm("D_03000010");
void InitializeBattleYellowStreakSineImpactBurstEffect(struct BattleAnimationGroup *group) asm("func_080E0554");

void InitializeBattleYellowStreakSineImpactBurstEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    BATTLE_ANIMATION_FIELD(group, s32, effect.impact.x) = BATTLE_ANIMATION_FIELD(group, s32, x) + ((u32)(CallFunctionR0(gRandomNumberCallback) * 0x41) >> 15) - 0x20;
    {
        s32 random_bits = CallFunctionR0(gRandomNumberCallback);
        register s32 *impact_y_slot asm("r3") = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.impact.y));
        register s32 impact_y asm("r2") = BATTLE_ANIMATION_FIELD(group, s32, y);
        register u32 random_y_offset asm("r1");
        impact_y -= 0x10;
        random_y_offset = (u32)(random_bits * 0x21) >> 15;
        impact_y += random_y_offset;
        *impact_y_slot = impact_y;
    }
}
