#include "sound_engine.h"
void ConfigureSoundSampleRate(s32) asm("func_080EBB9C");
void DisableSoundVSync(void) asm("func_080EBD2C");
void CallFunctionR1(u8, s32) asm("func_80ECD60");

void ConfigureSoundMode(s32 sound_mode_flags) asm("func_080EBC40");

void ConfigureSoundMode(s32 sound_mode_flags) {
    void *sound_state = *(void **)0x03007FF0;
    s32 signature = SOUND_ENGINE_WORD(sound_state, signature);
    if (signature == SOUND_ENGINE_SIGNATURE) {
        u32 mode_value;
        SOUND_ENGINE_WORD(sound_state, signature) = signature + 1;
        mode_value = SOUND_MODE_REVERB_MASK & sound_mode_flags;
        if (mode_value != 0) {
            SOUND_ENGINE_SIGNED_BYTE(sound_state, reverb) = mode_value & SOUND_MODE_REVERB_VALUE_MASK;
        }
        mode_value = SOUND_MODE_PCM_CHANNEL_COUNT_MASK & sound_mode_flags;
        if (mode_value != 0) {
            u8 *channel;
            SOUND_ENGINE_SIGNED_BYTE(sound_state, max_pcm_channels) = mode_value >> 8;
            mode_value = SOUND_ENGINE_PCM_CHANNEL_COUNT;
            channel = (u8 *)sound_state + SOUND_ENGINE_OFFSET(pcm_channels);
            do {
                *channel = 0;
                mode_value--;
                channel += sizeof(struct AudioChannelState);
            } while (mode_value != 0);
        }
        mode_value = SOUND_MODE_MASTER_VOLUME_MASK & sound_mode_flags;
        if (mode_value != 0) {
            SOUND_ENGINE_SIGNED_BYTE(sound_state, master_volume) = mode_value >> 0xC;
        }
        mode_value = SOUND_MODE_PWM_SETTING_MASK & sound_mode_flags;
        if (mode_value != 0) {
            u32 pwm_bits = SOUND_MODE_PWM_VALUE_MASK;
            pwm_bits &= mode_value;
            mode_value = pwm_bits >> 0xE;
            *(u8 *)0x04000089 = (SOUND_BIAS_LOW_BITS_MASK & *(u8 *)0x04000089) | mode_value;
        }
        mode_value = SOUND_MODE_SAMPLE_FREQUENCY_MASK & sound_mode_flags;
        if (mode_value != 0) {
            DisableSoundVSync();
            ConfigureSoundSampleRate(mode_value);
        }
        SOUND_ENGINE_WORD(sound_state, signature) = SOUND_ENGINE_SIGNATURE;
    }
}

void StopAllSoundChannels(void) asm("func_080EBCD8");

void StopAllSoundChannels(void) {
    void *sound_state = *(void **)0x03007FF0;
    s32 signature = SOUND_ENGINE_WORD(sound_state, signature);
    if (signature == SOUND_ENGINE_SIGNATURE) {
        s32 channel_counter;
        u8 *channel;
        SOUND_ENGINE_WORD(sound_state, signature) = signature + 1;
        channel_counter = SOUND_ENGINE_PCM_CHANNEL_COUNT;
        channel = (u8 *)sound_state + SOUND_ENGINE_OFFSET(pcm_channels);
        do {
            *channel = 0;
            channel_counter--;
            channel += sizeof(struct AudioChannelState);
        } while (channel_counter > 0);
        channel = SOUND_ENGINE_FIELD(sound_state, u8 **, psg_channels);
        if (channel != 0) {
            channel_counter = 1;
            do {
                CallFunctionR1((u8)channel_counter, SOUND_ENGINE_WORD(sound_state, stop_psg_channel));
                *channel = 0;
                channel_counter++;
                channel += sizeof(struct AudioChannelState);
            } while (channel_counter <= SOUND_ENGINE_PSG_CHANNEL_COUNT);
        }
        SOUND_ENGINE_WORD(sound_state, signature) = SOUND_ENGINE_SIGNATURE;
    }
}
