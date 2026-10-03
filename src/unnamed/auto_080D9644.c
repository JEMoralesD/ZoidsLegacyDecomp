#include "m2c_prelude.h"
s32 CallFunctionR0(s32) asm("func_080ECD5C");                             /* extern */

void sub_080D9644(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x8C) = 0;
    M2C_FIELD(arg0, u32 *, 0x94) = (u32) ((u32) (CallFunctionR0(*(s32 *)0x03000010) * 0x21) >> 0xF);
}
