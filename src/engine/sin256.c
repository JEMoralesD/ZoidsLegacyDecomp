#include "m2c_prelude.h"
extern u16 gSineTable[];

s32 Sin256(s32 angle) {
    s32 signed_sample;
    u16 sample;
    u16 table_index;
    register u32 wrapped_angle asm("r2");
    register u32 quadrant_angle asm("r1");
    u32 angle_bits;

    angle_bits = angle << 0x10;
    quadrant_angle = 0xFF0000;
    quadrant_angle &= angle_bits;
    wrapped_angle = quadrant_angle >> 0x10;
    quadrant_angle = wrapped_angle;
    if ((s32) (0x7F & quadrant_angle) <= 0x3F) {
        table_index = 0x3F & quadrant_angle;
    } else {
        quadrant_angle &= 0x3F;
        table_index = 0x40 - quadrant_angle;
    }
    sample = gSineTable[(s32) (table_index << 0x10) >> 0x10];
    if ((s32) wrapped_angle <= 0x7F) {
        signed_sample = sample << 0x10;
    } else {
        signed_sample = 0 - (sample << 0x10);
    }
    return signed_sample >> 0x10;
}
