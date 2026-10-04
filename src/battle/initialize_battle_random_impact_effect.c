#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 gRandomNumberCallback asm("D_03000010");
extern s32 CallFunctionR0(s32) asm("func_080ECD5C");

void InitializeBattleRandomImpactEffect(void *group) asm("func_080D2B54");

void InitializeBattleRandomImpactEffect(void *group) {
    register s32 random_or_x asm("r0");
    register s32 spread asm("r1");
    register s32 x_pointer_or_y asm("r2");
    register s32 impact_y_address asm("r3");

    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    random_or_x = CallFunctionR0(gRandomNumberCallback);
    x_pointer_or_y = (s32)group;
    x_pointer_or_y += BATTLE_ANIMATION_OFFSET(effect.impact.x);
    spread = random_or_x << 6;
    spread += random_or_x;
    spread = (u32)spread >> 0xF;
    random_or_x = BATTLE_ANIMATION_FIELD(group, s32, x) + spread;
    random_or_x -= 0x20;
    *(s32 *)x_pointer_or_y = random_or_x;
    random_or_x = CallFunctionR0(gRandomNumberCallback);
    impact_y_address = (s32)group;
    impact_y_address += BATTLE_ANIMATION_OFFSET(effect.impact.y);
    x_pointer_or_y = BATTLE_ANIMATION_FIELD(group, s32, y);
    x_pointer_or_y -= 0x10;
    spread = random_or_x << 5;
    spread += random_or_x;
    spread = (u32)spread >> 0xF;
    x_pointer_or_y += spread;
    *(s32 *)impact_y_address = x_pointer_or_y;
}
