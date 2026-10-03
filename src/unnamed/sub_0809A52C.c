#include "m2c_prelude.h"
extern s32 D_087AA70C[];
extern s32 D_087AA96C[][8];
extern void BiosLz77ToWram() asm("func_80ECD38");
extern void QueueCopy() asm("func_8095208");

void sub_0809A52C(u8 arg0, u8 arg1, u16 arg2, u8 arg3, s32 arg4) {
    s32 t2;
    s32 sz;
    s32 idx;
    BiosLz77ToWram(D_087AA70C[arg0], arg4);
    idx = D_087AA96C[arg0][arg1];
    sz = 0x800;
    t2 = arg4 + sz;
    BiosLz77ToWram(idx, t2);
    QueueCopy(arg4, (arg2 << 5) + 0x06010000, sz);
    QueueCopy(t2, (arg3 << 5) + 0x05000200, 0x20);
}
