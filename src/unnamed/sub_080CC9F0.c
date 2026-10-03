#include "m2c_prelude.h"
M2C_UNK DestroySprite(s32) asm("func_8094554");

void sub_080CC9F0(void) {
    u8 i = 0;
    s32 *base = (s32 *)0x02033F58;
    for (; i <= 6; i++) {
        DestroySprite(base[i]);
    }
}
