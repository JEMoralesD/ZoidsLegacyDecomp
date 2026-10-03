#include "m2c_prelude.h"
M2C_UNK DestroySprite(s32) asm("func_8094554");
void sub_080ACBA0(u8 arg0) {
    u8 i = 0;
    if (i < arg0) {
        s32 *a = (s32 *)0x02032A88;
        do {
            DestroySprite(a[i]);
            DestroySprite(*(s32 *)(0x02032AA8 + i * 4));
            i++;
        } while (i < arg0);
    }
}
