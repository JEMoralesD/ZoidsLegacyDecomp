#include "m2c_prelude.h"
s16 BiosDiv(s32, s16) asm("func_080ECD30");                        /* extern */

s16 ReciprocalFixed8(s16 value) asm("func_08092B2C");

s16 ReciprocalFixed8(s16 value) {
    return BiosDiv(0x10000, value);
}
