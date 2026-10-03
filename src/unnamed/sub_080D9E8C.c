#include "m2c_prelude.h"
s32 CallFunctionR0(s32) asm("func_080ECD5C");
extern s32 D_03000010;

void sub_080D9E8C(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x8C) = 0;
    M2C_FIELD(arg0, s32 *, 0x94) = (s32)(((u32)(CallFunctionR0(D_03000010) * 0x21) >> 0xF) - 0x10);
}
