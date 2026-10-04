#ifndef MATH3D_H
#define MATH3D_H

struct Point2s {
    s16 x;
    s16 y;
};

struct Vector3s {
    s16 x;
    s16 y;
    s16 z;
};

struct Vector3 {
    s32 x;
    s32 y;
    s32 z;
};

struct Matrix3x3Fixed8 {
    s16 elements[9];
};

/* Matrix elements use column order and eight fractional bits. */
#define VECTOR3_FIELD(vector, type, field) \
    M2C_FIELD(vector, type *, (s32)&((struct Vector3 *)0)->field)

#define VECTOR3S_FIELD(vector, type, field) \
    M2C_FIELD(vector, type *, (s32)&((struct Vector3s *)0)->field)

#define POINT2S_FIELD(point, type, field) \
    M2C_FIELD(point, type *, (s32)&((struct Point2s *)0)->field)

#define MATRIX3X3_FIELD(matrix, type, index) \
    M2C_FIELD(matrix, type *, (s32)&((struct Matrix3x3Fixed8 *)0)->elements[index])

#endif
