#include "m2c_prelude.h"

s16 BiosDiv(s32) asm("func_080ECD30");

s32 DivideFixed8(u16 numerator, s32 denominator) asm("func_08092B04");

s32 DivideFixed8(u16 numerator, s32 denominator) {
    s32 quotient;
    register s32 signed_denominator asm("r1");

    signed_denominator = (s16)denominator;
    asm volatile("" : "+r"(signed_denominator));
    if (signed_denominator == 0) {
        goto zero;
    }
    quotient = (s16)BiosDiv((s32)(numerator << 16) >> 8);
    goto done;
zero:
    quotient = 0x7FFF;
done:
    return quotient;
}
