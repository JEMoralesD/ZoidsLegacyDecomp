#include "m2c_prelude.h"
void *GetWindow(u8) asm("func_809716C");                                /* extern */
M2C_UNK BringWindowToFront(u8) asm("func_80971AC");                              /* extern */
M2C_UNK func_8097DA8(void *, M2C_UNK);                 /* extern */

void sub_08098248(M2C_UNK arg0, u8 arg1, u8 arg2) {
    u8 temp_r6;
    void *temp_r0;

    temp_r6 = arg2;
    temp_r0 = GetWindow(temp_r6);
    M2C_FIELD(temp_r0, u8 *, 0x12) = arg1;
    func_8097DA8(temp_r0, arg0);
    M2C_FIELD(temp_r0, s32 *, 0) = (s32) (M2C_FIELD(temp_r0, s32 *, 0) | 2);
    BringWindowToFront(temp_r6);
}
