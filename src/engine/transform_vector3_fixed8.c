#include "m2c_prelude.h"
#include "math3d.h"
void TransformVector3Fixed8(void *matrix, void *input, void *output) asm("func_0809370C");

void TransformVector3Fixed8(void *matrix, void *input, void *output) {
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r0_3;

    var_r0 = (MATRIX3X3_FIELD(matrix, s16, 0) * VECTOR3_FIELD(input, s32, x)) + (VECTOR3_FIELD(input, s32, y) * MATRIX3X3_FIELD(matrix, s16, 3)) + (VECTOR3_FIELD(input, s32, z) * MATRIX3X3_FIELD(matrix, s16, 6));
    if (var_r0 < 0) {
        var_r0 += 0xFF;
    }
    VECTOR3_FIELD(output, s32, x) = (s32) (var_r0 >> 8);
    var_r0_2 = (MATRIX3X3_FIELD(matrix, s16, 1) * VECTOR3_FIELD(input, s32, x)) + (VECTOR3_FIELD(input, s32, y) * MATRIX3X3_FIELD(matrix, s16, 4)) + (VECTOR3_FIELD(input, s32, z) * MATRIX3X3_FIELD(matrix, s16, 7));
    if (var_r0_2 < 0) {
        var_r0_2 += 0xFF;
    }
    VECTOR3_FIELD(output, s32, y) = (s32) (var_r0_2 >> 8);
    var_r0_3 = (MATRIX3X3_FIELD(matrix, s16, 2) * VECTOR3_FIELD(input, s32, x)) + (VECTOR3_FIELD(input, s32, y) * MATRIX3X3_FIELD(matrix, s16, 5)) + (VECTOR3_FIELD(input, s32, z) * MATRIX3X3_FIELD(matrix, s16, 8));
    if (var_r0_3 < 0) {
        var_r0_3 += 0xFF;
    }
    VECTOR3_FIELD(output, s32, z) = (s32) (var_r0_3 >> 8);
}
