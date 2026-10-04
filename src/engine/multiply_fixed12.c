#include "m2c_prelude.h"

s32 MultiplyFixed12(s32 left, s32 right) {
    s32 left_integer;
    s32 right_integer;
    s32 product;
    s32 right_magnitude;
    s32 left_magnitude;

    left_magnitude = left;
    if (left < 0) {
        left_magnitude = 0 - left;
    }
    right_magnitude = right;
    if (right < 0) {
        right_magnitude = 0 - right;
    }
    left_integer = left_magnitude >> 0xC;
    right_integer = right_magnitude >> 0xC;
    left_magnitude &= 0xFFF;
    right_magnitude &= 0xFFF;
    product = ((left_integer * right_integer) << 0xC) +
             (left_integer * right_magnitude) +
             (left_magnitude * right_integer) +
             ((s32)(left_magnitude * right_magnitude) >> 0xC);
    if (left < 0) {
        goto left_negative;
    }
    if (right >= 0) {
        goto done;
    }
    goto negate_product;
left_negative:
    if (right < 0) {
        goto done;
    }
negate_product:
    product = 0 - product;
done:
    return product;
}
