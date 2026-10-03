#include "m2c_prelude.h"
extern void PlaySong(s32) asm("func_8092E84");
extern void SetSpriteAnimation(s32 *, s32) asm("func_8094564");
extern void func_80E2C24(void);
extern s32 *D_02032E68[];

void sub_080E2EA4(void) {
    u16 *p = (u16 *)0x0300000E;
    if ((0x40 & *p) && !(*D_02032E68[0] & 0x20000)) {
        PlaySong(0x40);
        *(u8 *)0x02032E65 -= 1;
        func_80E2C24();
        SetSpriteAnimation(D_02032E68[0], 1);
        return;
    }
    if ((0x80 & *p) && !(*D_02032E68[1] & 0x20000)) {
        PlaySong(0x40);
        *(u8 *)0x02032E65 += 1;
        func_80E2C24();
        SetSpriteAnimation(D_02032E68[1], 1);
    }
}
