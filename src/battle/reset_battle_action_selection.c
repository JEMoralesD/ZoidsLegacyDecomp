#include "m2c_prelude.h"
#include "battle.h"
extern u8 gBattleState[];
void ResetBattleActionSelection(void) asm("func_080CA184");

void ResetBattleActionSelection(void) {
    gBattleState[0xA1AF] = 0;
    *(s16 *)0x0203EFB2 = 0;
}
