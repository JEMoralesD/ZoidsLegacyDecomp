#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern int CallFunctionR0(int) asm("func_80ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void InitializeBattleRandomHeightMissileEffect(int group_address) asm("func_080D6B00");

void InitializeBattleRandomHeightMissileEffect(int group_address) {
    *(s32 *)(group_address + BATTLE_ANIMATION_OFFSET(state)) = 0;
    *(s32 *)(group_address + BATTLE_ANIMATION_OFFSET(effect.random_height_missile.y_offset)) = ((u32)CallFunctionR0(gRandomNumberCallback) * 33 >> 15) - 16;
}
