#include "m2c_prelude.h"
#include "math3d.h"
void TransformVector3sFixed8(void *matrix, void *input, void *output) asm("func_08093678");

void TransformVector3sFixed8(void *matrix, void *input, void *output) {
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r0_3;

    var_r0 = (MATRIX3X3_FIELD(matrix, s16, 0) * VECTOR3S_FIELD(input, s16, x)) + (MATRIX3X3_FIELD(matrix, s16, 3) * VECTOR3S_FIELD(input, s16, y)) + (MATRIX3X3_FIELD(matrix, s16, 6) * VECTOR3S_FIELD(input, s16, z));
    if (var_r0 < 0) {
        var_r0 += 0xFF;
    }
    VECTOR3S_FIELD(output, s16, x) = (s16) (var_r0 >> 8);
    var_r0_2 = (MATRIX3X3_FIELD(matrix, s16, 1) * VECTOR3S_FIELD(input, s16, x)) + (MATRIX3X3_FIELD(matrix, s16, 4) * VECTOR3S_FIELD(input, s16, y)) + (MATRIX3X3_FIELD(matrix, s16, 7) * VECTOR3S_FIELD(input, s16, z));
    if (var_r0_2 < 0) {
        var_r0_2 += 0xFF;
    }
    VECTOR3S_FIELD(output, s16, y) = (s16) (var_r0_2 >> 8);
    var_r0_3 = (MATRIX3X3_FIELD(matrix, s16, 2) * VECTOR3S_FIELD(input, s16, x)) + (MATRIX3X3_FIELD(matrix, s16, 5) * VECTOR3S_FIELD(input, s16, y)) + (MATRIX3X3_FIELD(matrix, s16, 8) * VECTOR3S_FIELD(input, s16, z));
    if (var_r0_3 < 0) {
        var_r0_3 += 0xFF;
    }
    VECTOR3S_FIELD(output, s16, z) = (s16) (var_r0_3 >> 8);
}
