#include "m2c_prelude.h"
M2C_UNK ClearHBlankCallback(s32) asm("func_80942E0");                             /* extern */

void sub_08093B54(void) {
    u8 temp_r5;

    temp_r5 = *(u8 *)0x0300342D;
    if (temp_r5 == 1) {
        *(s16 *)0x04000208 = 0;
        ClearHBlankCallback(0);
        *(s16 *)0x04000208 = (s16) temp_r5;
    }
}
