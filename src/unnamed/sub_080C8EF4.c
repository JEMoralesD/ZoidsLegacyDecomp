#include "m2c_prelude.h"
extern int Sin256(s32) asm("func_8092A90");
extern void DestroySprite(void *) asm("func_8094554");

struct S {
    u32 w0;
    u8 pad4[6];
    s16 hA;
    u8 pad[0x1c];
    s32 w28;
    s32 w2c;
};

void sub_080C8EF4(struct S *arg0) {
    s32 v = arg0->w28;
    if (v <= 8) {
        if (v >= 0) {
            if (v == 0) {
                arg0->w0 &= 0xFFFDFFFF;
            }
            arg0->hA = -(s16)Sin256((arg0->w28 << 20) >> 16) / 32;
        }
    } else {
        if (v == arg0->w2c) {
            DestroySprite(arg0);
        }
    }
    arg0->w28 += 1;
}
