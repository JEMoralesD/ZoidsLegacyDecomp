#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
s32 CallFunctionR0(s32) asm("func_080ECD5C");
extern s32 gRandomNumberCallback asm("D_03000010");

void InitializeBattleRandomHeightYellowOrbEffect(struct BattleAnimationGroup *group) asm("func_080D9E8C");

void InitializeBattleRandomHeightYellowOrbEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    BATTLE_ANIMATION_FIELD(group, s32, effect.random_height_offset.y_offset) =
        (s32)(((u32)(CallFunctionR0(gRandomNumberCallback) * 0x21) >> 0xF) - 0x10);
}
