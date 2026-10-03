#include "m2c_prelude.h"
#include "battle.h"
#include "battle_animation.h"
extern s32 gBattleAnimationNextTile asm("D_0203486C"), gBattleAnimationNextPalette asm("D_02034870");
void ResetBattleAnimationResourceAllocation(void) asm("func_080D2328");

void ResetBattleAnimationResourceAllocation(void) {
    gBattleAnimationNextTile = 0x180;
    gBattleAnimationNextPalette = 3;
}
