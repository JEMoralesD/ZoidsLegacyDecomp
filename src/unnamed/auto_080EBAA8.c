#include "m2c_prelude.h"
M2C_UNK CallFunctionR1(M2C_UNK, s32) asm("func_80ECD60");                    /* extern */

void sub_080EBAA8(M2C_UNK arg0) {
    CallFunctionR1(arg0, *(s32 *)0x03007538);
}
