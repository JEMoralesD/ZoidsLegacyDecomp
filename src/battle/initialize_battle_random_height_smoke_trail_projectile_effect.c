#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 gRandomNumberCallback asm("D_03000010");
extern s32 CallFunctionR0(s32) asm("func_080ECD5C");

void InitializeBattleRandomHeightSmokeTrailProjectileEffect(void *group) asm("func_080D4BA4");

void InitializeBattleRandomHeightSmokeTrailProjectileEffect(void *group) {
    s32 *random_height;
    register s32 random_y asm("r2");
    s32 random;
    s32 spread;

    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    random = CallFunctionR0(gRandomNumberCallback);
    random_height = (s32 *)((s32)group + BATTLE_ANIMATION_OFFSET(effect.random_height.y));
    random_y = M2C_FIELD(group, s32 *, 8) - 0x10;
    spread = random << 5;
    spread += random;
    random_y += (u32)spread >> 0xF;
    *random_height = random_y;
}
