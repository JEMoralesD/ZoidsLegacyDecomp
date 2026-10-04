#include "sound_engine.h"
s32 MultiplyUnsigned32High(s32, s32) asm("func_080EAA94");

extern u8 gPcmKeyFrequencyCodes[] asm("D_086A3200");
extern u32 gPcmFrequencyRatios[] asm("D_086A32B4");

s32 CalculatePcmPlaybackFrequency(void *wave_header, u8 key, s32 fine_pitch) asm("func_080EB62C");

s32 CalculatePcmPlaybackFrequency(void *wave_header, u8 key, s32 fine_pitch) {
    s32 base_frequency;
    s32 packed_fine_pitch;
    u32 frequency_ratio;
    u32 next_frequency_ratio;
    u8 next_key_code;
    u8 clamped_key;

    clamped_key = key;
    packed_fine_pitch = fine_pitch << 0x18;
    if ((u32) clamped_key > PCM_MAX_KEY) {
        clamped_key = PCM_MAX_KEY;
        packed_fine_pitch = 0xFF000000;
    }
    frequency_ratio = gPcmKeyFrequencyCodes[clamped_key];
    frequency_ratio = gPcmFrequencyRatios[frequency_ratio & 0xF] >> (frequency_ratio >> 4);
    next_key_code = gPcmKeyFrequencyCodes[clamped_key + 1];
    next_frequency_ratio = gPcmFrequencyRatios[next_key_code & 0xF] >> (next_key_code >> 4);
    base_frequency = M2C_FIELD(wave_header, s32 *, PCM_WAVE_OFFSET(frequency_fixed10));
    {
        register s32 fractional_ratio asm("r1");
        fractional_ratio = MultiplyUnsigned32High(next_frequency_ratio - frequency_ratio, packed_fine_pitch);
        asm volatile("" : "+r"(fractional_ratio));
        return MultiplyUnsigned32High(base_frequency, frequency_ratio + fractional_ratio);
    }
}

void SoundDriverNoOp(void) asm("func_080EB690");

void SoundDriverNoOp(void) {
}
