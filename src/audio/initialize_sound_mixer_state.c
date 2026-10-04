#include "sound_engine.h"
extern void BiosCpuSet(void *, void *, s32) asm("func_80ECD2C");
extern void InitializeMusicCommandHandlers(s32) asm("func_080EAEC0");
extern void ConfigureSoundSampleRate(s32) asm("func_080EBB9C");

void InitializeSoundMixerState(void *sound_state) asm("func_080EBAD0");

void InitializeSoundMixerState(void *sound_state) {
    s32 clear_source;
    s32 zero = 0;
    SOUND_ENGINE_WORD(sound_state, signature) = zero;
    if (*(s32 *)0x040000C4 & SOUND_DMA_REPEAT) {
        *(s32 *)0x040000C4 = SOUND_DMA_STOP_WORD;
    }
    *(s16 *)0x040000C6 = SOUND_DMA_DISABLED;
    *(s16 *)0x04000084 = SOUND_INITIAL_MASTER_CONTROL;
    *(s16 *)0x04000082 = SOUND_INITIAL_MIXER_CONTROL;
    *(u8 *)0x04000089 = (SOUND_BIAS_LOW_BITS_MASK & *(u8 *)0x04000089) | SOUND_INITIAL_PWM_BITS;
    M2C_FIELD((void *)0x040000BC, void **, 0) = (void *) ((s8 *)sound_state + SOUND_ENGINE_OFFSET(pcm_buffer));
    M2C_FIELD((void *)0x040000BC, s32 *, 4) = 0x040000A0;
    *(void **)0x03007FF0 = sound_state;
    clear_source = zero;
    BiosCpuSet(&clear_source, sound_state, SOUND_ENGINE_STATE_CLEAR_CONTROL);
    SOUND_ENGINE_SIGNED_BYTE(sound_state, max_pcm_channels) = SOUND_INITIAL_MAX_PCM_CHANNELS;
    SOUND_ENGINE_SIGNED_BYTE(sound_state, master_volume) = SOUND_INITIAL_MASTER_VOLUME;
    SOUND_ENGINE_WORD(sound_state, play_note) = SOUND_PLAY_NOTE_THUMB_ENTRY;
    SOUND_ENGINE_WORD(sound_state, update_psg_channels) = SOUND_NOOP_CALLBACK_THUMB_ENTRY;
    SOUND_ENGINE_WORD(sound_state, stop_psg_channel) = SOUND_NOOP_CALLBACK_THUMB_ENTRY;
    SOUND_ENGINE_WORD(sound_state, calculate_psg_frequency) = SOUND_NOOP_CALLBACK_THUMB_ENTRY;
    SOUND_ENGINE_WORD(sound_state, reserved_callback) = SOUND_NOOP_CALLBACK_THUMB_ENTRY;
    InitializeMusicCommandHandlers(MUSIC_COMMAND_TABLE_RAM);
    SOUND_ENGINE_WORD(sound_state, command_handlers) = MUSIC_COMMAND_TABLE_RAM;
    ConfigureSoundSampleRate(SOUND_INITIAL_SAMPLE_RATE_MODE);
    SOUND_ENGINE_WORD(sound_state, signature) = SOUND_ENGINE_SIGNATURE;
}
