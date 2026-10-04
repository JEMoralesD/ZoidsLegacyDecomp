#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
s32 CallFunctionR0(s32) asm("func_080ECD5C");

void InitializeBattleRandomHeightProjectileEffect(void *group) asm("func_080D2FE4");

void InitializeBattleRandomHeightProjectileEffect(void *group) {
    register s32 random asm("r0");
    register s32 *random_height asm("r3");
    register s32 base_y asm("r2");

    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    random = CallFunctionR0(*(s32 *)0x03000010);
    random_height = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.random_height.y));
    asm volatile("" : "+r"(random_height));
    base_y = BATTLE_ANIMATION_FIELD(group, s32, y) - 0x10;
    asm volatile("" : "+r"(base_y));
    *random_height = base_y + ((u32)(random * 0x21) >> 0xF);
}
