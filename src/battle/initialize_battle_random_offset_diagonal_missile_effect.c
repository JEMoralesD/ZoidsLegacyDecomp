#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern s32 gRandomNumberCallback asm("D_03000010");
s32 CallFunctionR0(s32) asm("func_080ECD5C");                             /* extern */

void InitializeBattleRandomOffsetDiagonalMissileEffect(void *group) asm("func_080D7B0C");

void InitializeBattleRandomOffsetDiagonalMissileEffect(void *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    BATTLE_ANIMATION_FIELD(group, u32, effect.random_diagonal_launch.launch_x_offset_half) = (u32) ((u32) (CallFunctionR0(gRandomNumberCallback) * 0x21) >> 0xF);
}
