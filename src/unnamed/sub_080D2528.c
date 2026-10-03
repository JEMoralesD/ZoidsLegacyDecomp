#include "m2c_prelude.h"
extern void DestroySprite(void *) asm("func_8094554");

struct S {
    u32 f0;
    u16 f4;
    u8 pad[0x28 - 6];
    s32 f28;
};

void sub_080D2528(struct S *obj) {
    register int v asm("r1");
    int e;
    if ((obj->f0 & 0x8000) == 0) {
        e = obj->f28;
        v = obj->f4;
        v -= e;
        obj->f4 = v;
        if ((s16)v < -16) DestroySprite(obj);
    } else {
        e = obj->f28;
        v = obj->f4;
        v += e;
        obj->f4 = v;
        if ((s16)v > 256) DestroySprite(obj);
    }
}
