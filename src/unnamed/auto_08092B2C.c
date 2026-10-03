#include "m2c_prelude.h"
s16 BiosDiv(s32, s16) asm("func_080ECD30");                        /* extern */

s16 sub_08092B2C(s16 arg0) {
    return BiosDiv(0x10000, arg0);
}
