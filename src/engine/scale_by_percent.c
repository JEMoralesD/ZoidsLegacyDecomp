#include "m2c_prelude.h"
M2C_UNK DivideSigned32(s32, s32) asm("func_80ECD98");                        /* extern */

s32 ScaleByPercent(s32 value, s32 percent) {
    return DivideSigned32(value * percent, 0x64);
}
