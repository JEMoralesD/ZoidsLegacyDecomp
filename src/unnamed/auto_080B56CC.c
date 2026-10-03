#include "m2c_prelude.h"
s32 CreateSprite(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32, s32) asm("func_8094484"); /* extern */
M2C_UNK DestroySprite(s32) asm("func_8094554");                             /* extern */
M2C_UNK RunMenuScript(M2C_UNK) asm("func_8098BB4");                         /* extern */
M2C_UNK func_809A94C(s32, s32, s32, s32, s32);         /* extern */

void sub_080B56CC(void) {
    s32 temp_r4;

    func_809A94C(5, 0, 0, 0, 0);
    temp_r4 = CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0, 0, 8, 0);
    RunMenuScript(0x080035DE);
    DestroySprite(temp_r4);
}
