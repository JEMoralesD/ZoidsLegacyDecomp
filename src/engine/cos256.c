#include "m2c_prelude.h"
s16 Sin256(s32) asm("func_8092A90");                                 /* extern */

s16 Cos256(s32 angle) {
    return Sin256((s32) ((angle << 0x10) + 0x400000) >> 0x10);
}
