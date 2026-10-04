#include "m2c_prelude.h"
#include "camera.h"


void TransformVector3sFixed8(void *, s32, struct Vector3s *) asm("func_08093678");
s32 BiosDiv(s32, s32) asm("func_080ECD30");

s16 ProjectTransformedPoint(s32 point, s16 *screen_position, u8 *transform) asm("func_0809398C");

s16 ProjectTransformedPoint(s32 point, s16 *screen_position, u8 *transform) {
    s32 translated_position[3];
    struct Vector3s local;

    TransformVector3sFixed8(transform, point, &local);
    translated_position[0] = local.x + ((struct PointProjectionTransform *)transform)->translation.x;
    translated_position[1] = local.y + ((struct PointProjectionTransform *)transform)->translation.y;
    translated_position[2] = local.z + ((struct PointProjectionTransform *)transform)->translation.z;
    screen_position[0] = (s16)(BiosDiv(translated_position[0] << 8, translated_position[2]) + 0x78);
    screen_position[1] = (s16)(BiosDiv(translated_position[1] << 8, translated_position[2]) + 0x50);
    return (s16)translated_position[2];
}
