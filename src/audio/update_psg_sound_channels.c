#include "sound_engine.h"
void UpdatePsgSoundChannels(void) asm("func_080EC240");
void StopPsgChannel(u8 channel_id) asm("func_080EC188");
void UpdatePsgChannelVolumeAndPan(struct PsgChannelState *channel) asm("func_080EC1D8");

extern const u8 gPsgWaveChannelVolumes[];

#define REG_NR51 (*(volatile u8 *)0x04000081)
#define REG_SOUNDBIAS_H (*(volatile u8 *)0x04000089)
#define REG_WAVE_RAM0 (*(volatile u32 *)0x04000090)
#define REG_WAVE_RAM1 (*(volatile u32 *)0x04000094)
#define REG_WAVE_RAM2 (*(volatile u32 *)0x04000098)
#define REG_WAVE_RAM3 (*(volatile u32 *)0x0400009C)

#define PSG_ENVELOPE_INCREASE 0x08

void UpdatePsgSoundChannels(void) {
    s32 channel_number;
    struct PsgChannelState *channel;
    s32 envelope_frame;
    s32 envelope_control;
    struct SoundEngineState *engine = SOUND_ENGINE;
    volatile u8 *nrx0;
    volatile u8 *nrx1;
    volatile u8 *nrx2;
    volatile u8 *nrx3;
    volatile u8 *nrx4;
    int mask = 0xFF;

    if (engine->psg_envelope_frame)
        engine->psg_envelope_frame--;
    else
        engine->psg_envelope_frame = 14;

    for (channel_number = 1, channel = engine->psg_channels; channel_number <= 4; channel_number++, channel++) {
        if (!(channel->flags & SOUND_CHANNEL_ON))
            continue;

        switch (channel_number) {
        case 1:
            nrx0 = (volatile u8 *)0x04000060;
            nrx1 = (volatile u8 *)0x04000062;
            nrx2 = (volatile u8 *)0x04000063;
            nrx3 = (volatile u8 *)0x04000064;
            nrx4 = (volatile u8 *)0x04000065;
            break;
        case 2:
            nrx0 = (volatile u8 *)0x04000061;
            nrx1 = (volatile u8 *)0x04000068;
            nrx2 = (volatile u8 *)0x04000069;
            nrx3 = (volatile u8 *)0x0400006C;
            nrx4 = (volatile u8 *)0x0400006D;
            break;
        case 3:
            nrx0 = (volatile u8 *)0x04000070;
            nrx1 = (volatile u8 *)0x04000072;
            nrx2 = (volatile u8 *)0x04000073;
            nrx3 = (volatile u8 *)0x04000074;
            nrx4 = (volatile u8 *)0x04000075;
            break;
        default:
            nrx0 = (volatile u8 *)0x04000071;
            nrx1 = (volatile u8 *)0x04000078;
            nrx2 = (volatile u8 *)0x04000079;
            nrx3 = (volatile u8 *)0x0400007C;
            nrx4 = (volatile u8 *)0x0400007D;
            break;
        }

        envelope_frame = engine->psg_envelope_frame;
        envelope_control = *nrx2;

        if (channel->flags & SOUND_CHANNEL_START) {
            if (!(channel->flags & SOUND_CHANNEL_STOP)) {
                channel->flags = SOUND_CHANNEL_ENVELOPE_ATTACK;
                channel->modify = PSG_MODIFY_PITCH | PSG_MODIFY_VOLUME;
                UpdatePsgChannelVolumeAndPan(channel);
                switch (channel_number) {
                case 1:
                    *nrx0 = channel->sweep;
                case 2:
                    *nrx1 = ((u32)channel->wave << 6) + channel->length;
                    goto set_attack_envelope;
                case 3:
                    if (channel->wave != channel->loaded_wave) {
                        *nrx0 = 0x40;
                        REG_WAVE_RAM0 = channel->wave[0];
                        REG_WAVE_RAM1 = channel->wave[1];
                        REG_WAVE_RAM2 = channel->wave[2];
                        REG_WAVE_RAM3 = channel->wave[3];
                        channel->loaded_wave = channel->wave;
                    }
                    *nrx0 = 0;
                    *nrx1 = channel->length;
                    if (channel->length)
                        channel->frequency_control = 0xC0;
                    else
                        channel->frequency_control = 0x80;
                    break;
                default:
                    *nrx1 = channel->length;
                    *nrx3 = (u32)channel->wave << 3;
                set_attack_envelope:
                    envelope_control = channel->attack + PSG_ENVELOPE_INCREASE;
                    if (channel->length)
                        channel->frequency_control = 0x40;
                    else
                        channel->frequency_control = 0x00;
                    break;
                }
                channel->envelope_counter = channel->attack;
                if ((s8)(channel->attack & mask)) {
                    channel->envelope_volume = 0;
                    goto envelope_step_complete;
                } else {
                    goto start_decay;
                }
            } else {
                goto stop_channel;
            }
        } else if (channel->flags & SOUND_CHANNEL_ECHO) {
            channel->echo_length--;
            if ((s8)(channel->echo_length & mask) <= 0) {
            stop_channel:
                StopPsgChannel(channel_number);
                channel->flags = 0;
                goto channel_complete;
            }
            goto envelope_complete;
        } else if ((channel->flags & SOUND_CHANNEL_STOP) && (channel->flags & SOUND_CHANNEL_ENVELOPE_MASK)) {
            channel->flags &= ~SOUND_CHANNEL_ENVELOPE_MASK;
            channel->envelope_counter = channel->release;
            if ((s8)(channel->release & mask)) {
                channel->modify |= PSG_MODIFY_VOLUME;
                if (channel_number != 3)
                    envelope_control = channel->release;
                goto envelope_step_complete;
            } else {
                goto start_echo;
            }
        } else {
        envelope_step:
            if (channel->envelope_counter == 0) {
                if (channel_number == 3)
                    channel->modify |= PSG_MODIFY_VOLUME;

                UpdatePsgChannelVolumeAndPan(channel);
                if ((channel->flags & SOUND_CHANNEL_ENVELOPE_MASK) == SOUND_CHANNEL_ENVELOPE_RELEASE) {
                    channel->envelope_volume--;
                    if ((s8)(channel->envelope_volume & mask) <= 0) {
                    start_echo:
                        channel->envelope_volume = ((channel->amplitude * channel->echo_volume) + 0xFF) >> 8;
                        if (channel->envelope_volume) {
                            channel->flags |= SOUND_CHANNEL_ECHO;
                            channel->modify |= PSG_MODIFY_VOLUME;
                            if (channel_number != 3)
                                envelope_control = PSG_ENVELOPE_INCREASE;
                            goto envelope_complete;
                        } else {
                            goto stop_channel;
                        }
                    } else {
                        channel->envelope_counter = channel->release;
                    }
                } else if ((channel->flags & SOUND_CHANNEL_ENVELOPE_MASK) == SOUND_CHANNEL_ENVELOPE_SUSTAIN) {
                hold_sustain:
                    channel->envelope_volume = channel->sustain_level;
                    channel->envelope_counter = 7;
                } else if ((channel->flags & SOUND_CHANNEL_ENVELOPE_MASK) == SOUND_CHANNEL_ENVELOPE_DECAY) {
                    int envelope_volume, sustain_level;

                    channel->envelope_volume--;
                    envelope_volume = (s8)(channel->envelope_volume & mask);
                    sustain_level = (s8)(channel->sustain_level);
                    if (envelope_volume <= sustain_level) {
                    start_sustain:
                        if (channel->sustain == 0) {
                            channel->flags &= ~SOUND_CHANNEL_ENVELOPE_MASK;
                            goto start_echo;
                        } else {
                            channel->flags--;
                            channel->modify |= PSG_MODIFY_VOLUME;
                            if (channel_number != 3)
                                envelope_control = PSG_ENVELOPE_INCREASE;
                            goto hold_sustain;
                        }
                    } else {
                        channel->envelope_counter = channel->decay;
                    }
                } else {
                    channel->envelope_volume++;
                    if ((u8)(channel->envelope_volume & mask) >= channel->amplitude) {
                    start_decay:
                        channel->flags--;
                        channel->envelope_counter = channel->decay;
                        if ((u8)(channel->envelope_counter & mask)) {
                            channel->modify |= PSG_MODIFY_VOLUME;
                            channel->envelope_volume = channel->amplitude;
                            if (channel_number != 3)
                                envelope_control = channel->decay;
                        } else {
                            goto start_sustain;
                        }
                    } else {
                        channel->envelope_counter = channel->attack;
                    }
                }
            }
        }

    envelope_step_complete:
        channel->envelope_counter--;
        if (envelope_frame == 0) {
            envelope_frame--;
            goto envelope_step;
        }

    envelope_complete:
        if (channel->modify & PSG_MODIFY_PITCH) {
            if (channel_number < 4 && (channel->channel_id & TONE_TYPE_FIXED)) {
                int pwm_rate = REG_SOUNDBIAS_H;

                if (pwm_rate < 0x40)
                    channel->frequency = (channel->frequency + 2) & 0x7FC;
                else if (pwm_rate < 0x80)
                    channel->frequency = (channel->frequency + 1) & 0x7FE;
            }

            if (channel_number != 4)
                *nrx3 = channel->frequency;
            else
                *nrx3 = (*nrx3 & 0x08) | channel->frequency;
            channel->frequency_control = (channel->frequency_control & 0xC0) + (*((u8 *)(&channel->frequency) + 1));
            *nrx4 = (s8)(channel->frequency_control & mask);
        }

        if (channel->modify & PSG_MODIFY_VOLUME) {
            REG_NR51 = (REG_NR51 & ~channel->output_pan_mask) | channel->output_routing;
            if (channel_number == 3) {
                *nrx2 = gPsgWaveChannelVolumes[channel->envelope_volume];
                if (channel->frequency_control & 0x80) {
                    *nrx0 = 0x80;
                    *nrx4 = channel->frequency_control;
                    channel->frequency_control &= 0x7F;
                }
            } else {
                envelope_control &= 0xF;
                *nrx2 = (channel->envelope_volume << 4) + envelope_control;
                *nrx4 = channel->frequency_control | 0x80;
                if (channel_number == 1 && !(*nrx0 & 0x08))
                    *nrx4 = channel->frequency_control | 0x80;
            }
        }

    channel_complete:
        channel->modify = 0;
    }
}
