#include "m2c_prelude.h"
#include "battle_animation.h"
extern u8 gBattleBackgroundShakeState asm("D_02034869");
extern u8 gBattleBackgroundShakeFrame asm("D_0203486A");
extern u16 D_0300004C;
extern s16 D_0400000A;
extern u16 D_04000008;
extern s32 D_03000054[];
extern s16 D_0300004E;
extern s16 D_03000050;

void StartBattleBackgroundShake(void) asm("func_080D218C");

void StartBattleBackgroundShake(void) {
    gBattleBackgroundShakeState = BATTLE_BACKGROUND_SHAKE_ACTIVE;
    gBattleBackgroundShakeFrame = 0;
    D_0300004C |= 0x200;
    D_0400000A = (0xFFFC & D_04000008) | 3;
    D_03000054[2] = D_03000054[0];
    D_03000054[3] = D_03000054[1];
    D_0300004E = 0x442;
    D_03000050 = 0x1010;
}
