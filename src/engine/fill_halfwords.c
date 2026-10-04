#include "m2c_prelude.h"
void FillHalfwords(u16 value, u16 *destination, s32 count) asm("func_08092C48");

void FillHalfwords(u16 value, u16 *destination, s32 count) {
    s32 remaining;
    u16 *output;

    output = destination;
    remaining = count - 1;
    if (remaining != -1) {
        do {
            *output = value;
            output += 1;
            remaining -= 1;
        } while (remaining != -1);
    }
}
