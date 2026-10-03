#include "m2c_prelude.h"
extern s32 D_03000010;
extern s32 CallFunctionR0(s32) asm("func_080ECD5C");

void sub_080D4BA4(void *arg0) {
    s32 *out;
    register s32 value asm("r2");
    s32 scaled;
    s32 scale;

    M2C_FIELD(arg0, s32 *, 0x8C) = 0;
    scaled = CallFunctionR0(D_03000010);
    out = (s32 *)((s32)arg0 + 0x90);
    value = M2C_FIELD(arg0, s32 *, 8) - 0x10;
    scale = scaled << 5;
    scale += scaled;
    value += (u32)scale >> 0xF;
    *out = value;
}
