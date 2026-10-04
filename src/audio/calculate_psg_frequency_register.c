#include "sound_engine.h"

s32 CalculatePsgFrequencyRegister(s32 channel_id, s32 key, s32 fine_pitch) asm("func_080EC0E0");

s32 CalculatePsgFrequencyRegister(s32 channel_id, s32 key, s32 fine_pitch) {
    register s32 work asm("r0");
    register s32 value asm("r1");
    register s32 key_index asm("r5");
    register s32 pitch_fraction asm("r12");
    register u8 *key_codes asm("r3");
    register u8 *frequency_curve asm("r4");
    register s32 current_frequency_or_code asm("r6");
    register s32 mask asm("r2");
    s32 offset;

    channel_id <<= 24;
    channel_id = (u32)channel_id >> 24;
    key <<= 24;
    key_index = (u32)key >> 24;
    fine_pitch <<= 24;
    fine_pitch = (u32)fine_pitch >> 24;
    pitch_fraction = fine_pitch;

    if (channel_id == PSG_NOISE_CHANNEL) {
        if ((u32)key_index <= 0x14) {
            key_index = 0;
        } else {
            work = key_index;
            work -= PSG_NOISE_KEY_START;
            work <<= 24;
            key_index = (u32)work >> 24;
            if ((u32)key_index > PSG_NOISE_MAX_INDEX) {
                key_index = PSG_NOISE_MAX_INDEX;
            }
        }
        work = PSG_NOISE_FREQUENCIES_ROM;
        asm volatile("" : "+r"(work));
        work = key_index + work;
        work = *(u8 *)work;
        return work;
    }

    if ((u32)key_index <= 0x23) {
        work = 0;
        asm volatile("" : "+r"(work));
        pitch_fraction = work;
        key_index = 0;
    } else {
        work = key_index;
        work -= PSG_TONE_KEY_START;
        work <<= 24;
        key_index = (u32)work >> 24;
        if ((u32)key_index > PSG_TONE_MAX_INDEX) {
            key_index = PSG_TONE_MAX_INDEX;
            value = 0xFF;
            asm volatile("" : "+r"(value));
            pitch_fraction = value;
        }
    }

    key_codes = (u8 *)PSG_KEY_FREQUENCY_CODES_ROM;
    work = key_index + (s32)key_codes;
    current_frequency_or_code = *(u8 *)work;
    asm volatile("" : "+r"(current_frequency_or_code));
    frequency_curve = (u8 *)PSG_FREQUENCY_CURVE_ROM;
    asm volatile("" : "+r"(frequency_curve));
    mask = 0x0F;
    work = current_frequency_or_code;
    work &= mask;
    work <<= 1;
    work = work + (s32)frequency_curve;
    asm volatile("" : "+r"(work));
    offset = 0;
    value = M2C_FIELD(work, s16 *, offset);
    asm volatile("" : "+r"(value));
    work = current_frequency_or_code;
    work >>= 4;
    current_frequency_or_code = value;
    current_frequency_or_code >>= work;

    work = key_index + 1;
    work = work + (s32)key_codes;
    value = *(u8 *)work;
    asm volatile("" : "+r"(value));
    work = value;
    work &= mask;
    work <<= 1;
    work = work + (s32)frequency_curve;
    asm volatile("" : "+r"(work));
    mask = 0;
    work = M2C_FIELD(work, s16 *, mask);
    value >>= 4;
    work >>= value;
    work -= current_frequency_or_code;
    {
        register s32 interpolation_product asm("r7");
        interpolation_product = pitch_fraction;
        interpolation_product *= work;
        work = interpolation_product;
        asm volatile("" : "+&r"(work) : "r"(interpolation_product));
        work >>= 8;
        work = current_frequency_or_code + work;
        value = 0x80;
        value <<= 4;
        work += value;
        return work;
    }
}
