#include "sound_engine.h"

s32 DivideSigned32(s32, s32) asm("func_080ECD98");
void EnableSoundVSync(void) asm("func_080EBD90");

void ConfigureSoundSampleRate(s32 sound_mode_flags) asm("func_080EBB9C");

void ConfigureSoundSampleRate(s32 sound_mode_flags)
{
    register s32 mode_flags asm("r2") = sound_mode_flags;
    struct SoundEngineState *state;
    u32 frequency_mask;
    register u32 frequency_index asm("r2");
    register s32 zero asm("r6");
    u32 samples_per_vblank;
    s32 sample_frequency_hz;

    asm volatile("" : "+r"(mode_flags));
    state = *(struct SoundEngineState **)0x03007FF0;
    frequency_mask = SOUND_MODE_SAMPLE_FREQUENCY_BYTE_MASK;
    frequency_mask <<= 12;
    frequency_mask &= mode_flags;
    frequency_index = frequency_mask >> 16;
    asm volatile("" : "+r"(frequency_index));
    zero = 0;
    state->sample_frequency_index = frequency_index;
    {
        register u16 *sample_count_table asm("r1") = (u16 *)SOUND_SAMPLE_RATE_TABLE;
        register u32 sample_count_address asm("r0");
        asm volatile("" : "+r"(sample_count_table));
        sample_count_address = frequency_index - 1;
        asm volatile("" : "+r"(sample_count_address));
        sample_count_address <<= 1;
        sample_count_address += (u32)sample_count_table;
        samples_per_vblank = *(u16 *)sample_count_address;
    }
    state->samples_per_vblank = samples_per_vblank;
    state->dma_reset_period_vblanks = DivideSigned32(SOUND_ENGINE_PCM_BUFFER_SIZE, samples_per_vblank);
    sample_frequency_hz = DivideSigned32((SOUND_SAMPLE_FREQUENCY_NUMERATOR * samples_per_vblank) + SOUND_SAMPLE_FREQUENCY_ROUNDING, SOUND_SAMPLE_FREQUENCY_DENOMINATOR);
    state->sample_frequency_hz = sample_frequency_hz;
    state->sample_step_factor = (DivideSigned32(SOUND_SAMPLE_STEP_NUMERATOR, sample_frequency_hz) + 1) >> 1;

    *(volatile u16 *)0x04000102 = zero;
    {
        register volatile u16 *timer_reload asm("r4") =
            (volatile u16 *)0x04000100;
        *timer_reload = -DivideSigned32(SOUND_CPU_CYCLES_PER_FRAME, samples_per_vblank);
    }
    EnableSoundVSync();

    {
        volatile u8 *vcount = (volatile u8 *)0x04000006;
        do {
        } while (*vcount == SOUND_TIMER_START_SCANLINE);
    }
    {
        volatile u8 *vcount = (volatile u8 *)0x04000006;
        do {
        } while (*vcount != SOUND_TIMER_START_SCANLINE);
    }
    *(volatile u16 *)0x04000102 = SOUND_TIMER_ENABLED;
}
