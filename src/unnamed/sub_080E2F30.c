#include "m2c_prelude.h"
extern s32 D_02032E68[];
M2C_UNK DestroySprite(s32) asm("func_8094554");
M2C_UNK UpdateBattleAnimationCamera(void) asm("func_080D12DC");
M2C_UNK func_80ED17C(s32);

void sub_080E2F30(void) {
    DestroySprite(D_02032E68[0]);
    DestroySprite(D_02032E68[1]);
}

void sub_080E2F4C(void) {
loop_1:
    UpdateBattleAnimationCamera();
    func_80ED17C(1);
    goto loop_1;
}

void sub_080E2F5C(void) {
    *(u16 *)0x04000000 |= 0x800;
}
