#include "m2c_prelude.h"
#include "title_menu.h"
s32 CallFunctionR0(s32) asm("func_080ECD5C");

void UpdateTitleMenuParticle(void *sprite, s32 unused_callback_argument, s32 callback_r2_word_index) asm("func_0809B970");

void UpdateTitleMenuParticle(void *sprite, s32 unused_callback_argument, s32 callback_r2_word_index) {
    u8 *user_word_address;
    s32 phase;
    s32 word_offset_or_value;
    u32 next_y_fixed8;

    phase = TITLE_PARTICLE_FIELD(sprite, s32, phase);
    switch (phase) {
    case TITLE_PARTICLE_DESCENDING:
        next_y_fixed8 = TITLE_PARTICLE_FIELD(sprite, u32, y_fixed8) + 0x80;
        TITLE_PARTICLE_FIELD(sprite, u32, y_fixed8) = next_y_fixed8;
        if (next_y_fixed8 > 0x67FFU) {
            TITLE_PARTICLE_FIELD(sprite, s32, drift_x_fixed8) = (s32) (((u32) (CallFunctionR0(*(s32 *)0x03000010) * 0x81) >> 0xF) - 0x40);
            TITLE_PARTICLE_FIELD(sprite, s32, phase) = (s32) (TITLE_PARTICLE_FIELD(sprite, s32, phase) + 1);
        }
        break;
    case TITLE_PARTICLE_DRIFTING:
        TITLE_PARTICLE_FIELD(sprite, u32, x_fixed8) = (u32) (TITLE_PARTICLE_FIELD(sprite, u32, x_fixed8) + TITLE_PARTICLE_FIELD(sprite, s32, drift_x_fixed8));
        TITLE_PARTICLE_FIELD(sprite, u32, y_fixed8) = (u32) (TITLE_PARTICLE_FIELD(sprite, u32, y_fixed8) + 0x40);
        word_offset_or_value = callback_r2_word_index + 3;
        word_offset_or_value *= 4;
        user_word_address = sprite + 0x28;
        user_word_address += word_offset_or_value;
        word_offset_or_value = *(s32 *) user_word_address;
        if (word_offset_or_value != 0) {
            *(s32 *) user_word_address = word_offset_or_value - 0x10;
        }
        break;
    }
    M2C_FIELD(sprite, s16 *, 4) = (s16) ((u32) TITLE_PARTICLE_FIELD(sprite, u32, x_fixed8) >> 8);
    M2C_FIELD(sprite, s16 *, 6) = (s16) ((u32) TITLE_PARTICLE_FIELD(sprite, u32, y_fixed8) >> 8);
}
