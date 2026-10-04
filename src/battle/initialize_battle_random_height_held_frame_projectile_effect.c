#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern u32 gRandomNumberCallback asm("D_03000010");
u32 CallFunctionR0() asm("func_080ECD5C");

void InitializeBattleRandomHeightHeldFrameProjectileEffect(struct BattleAnimationGroup *group) asm("func_080D3720");

void InitializeBattleRandomHeightHeldFrameProjectileEffect(struct BattleAnimationGroup *group) {
    u32 random;
    s32 base_y;
    s32 *random_height;
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    random = CallFunctionR0(gRandomNumberCallback);
    random_height = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.random_height.y));
    base_y = BATTLE_ANIMATION_FIELD(group, s32, y) - 16;
    *random_height = base_y + ((random * 33) >> 15);
}
