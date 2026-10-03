#include "m2c_prelude.h"
M2C_UNK CallFunctionR2(s32, s32, s32) asm("func_080ECD64");               /* extern */

void sub_080929B0(void *arg0) {
    s32 temp_r1;

    temp_r1 = M2C_FIELD(arg0, s32 *, 4);
    CallFunctionR2(temp_r1, temp_r1, M2C_FIELD(arg0, s32 *, 0));
}
