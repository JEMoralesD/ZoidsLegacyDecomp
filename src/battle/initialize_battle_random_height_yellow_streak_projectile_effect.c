#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 gRandomNumberCallback asm("D_03000010");
s32 CallFunctionR0(s32) asm("func_080ECD5C");

void InitializeBattleRandomHeightYellowStreakProjectileEffect(struct BattleAnimationGroup *group) asm("func_080E07A4");

void InitializeBattleRandomHeightYellowStreakProjectileEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = BATTLE_YELLOW_STREAK_PROJECTILE_CREATE;
    {
        s32 random_bits;
        s32 launch_y;
        s32 *launch_y_slot;

        random_bits = CallFunctionR0(gRandomNumberCallback);
        launch_y_slot = &BATTLE_ANIMATION_FIELD(group, s32, effect.random_height.y);
        launch_y = BATTLE_ANIMATION_FIELD(group, s32, y);
        launch_y -= 0x10;
        launch_y += (u32)(random_bits * 0x21) >> 0xF;
        *launch_y_slot = launch_y;
    }
}
