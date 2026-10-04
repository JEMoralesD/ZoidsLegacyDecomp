#include "m2c_prelude.h"
#include "math3d.h"
void MultiplyMatrix3x3Fixed8(s16 *left, s16 *right, s16 *output) asm("func_080934DC");

void MultiplyMatrix3x3Fixed8(s16 *left, s16 *right, s16 *output)
{
    s32 value;

    value = left[0] * right[0] + left[3] * right[1] + left[6] * right[2];
    if (value < 0)
        value += 0xFF;
    output[0] = value >> 8;

    value = left[0] * right[3] + left[3] * right[4] + left[6] * right[5];
    if (value < 0)
        value += 0xFF;
    output[3] = value >> 8;

    value = left[0] * right[6] + left[3] * right[7] + left[6] * right[8];
    if (value < 0)
        value += 0xFF;
    output[6] = value >> 8;

    value = left[1] * right[0] + left[4] * right[1] + left[7] * right[2];
    if (value < 0)
        value += 0xFF;
    output[1] = value >> 8;

    value = left[1] * right[3] + left[4] * right[4] + left[7] * right[5];
    if (value < 0)
        value += 0xFF;
    output[4] = value >> 8;

    value = left[1] * right[6] + left[4] * right[7] + left[7] * right[8];
    if (value < 0)
        value += 0xFF;
    output[7] = value >> 8;

    value = left[2] * right[0] + left[5] * right[1] + left[8] * right[2];
    if (value < 0)
        value += 0xFF;
    output[2] = value >> 8;

    value = left[2] * right[3] + left[5] * right[4] + left[8] * right[5];
    if (value < 0)
        value += 0xFF;
    output[5] = value >> 8;

    value = left[2] * right[6] + left[5] * right[7] + left[8] * right[8];
    if (value < 0)
        value += 0xFF;
    output[8] = value >> 8;
}
