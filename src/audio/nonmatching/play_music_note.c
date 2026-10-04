#include "../sound_engine.h"
void PlayMusicNote(u32 clock_index, MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EB39C");
void UnlinkMusicChannelViaCallback(struct AudioChannelState *channel) asm("func_080EBAA8");
void ResetMusicTrackModulation(u32 lfo_delay, MusicPlayerTrack *track) asm("func_080EB5DC");
void UpdateMusicTrackVolumeAndPitch(MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EC02C");
void UpdateMusicChannelVolume(struct AudioChannelState *channel, MusicPlayerTrack *track) asm("func_080EB36C");
u32 CalculatePcmPlaybackFrequency(struct PcmWaveHeader *wave, u32 key, u32 pitch) asm("func_080EB62C");

extern const u8 gMusicClockTable[];

typedef u32 (*PsgFrequencyFunc)(u32 psg_channel, u32 key, u32 pitch);

#define PSG_DEFAULT_SWEEP 8

void PlayMusicNote(u32 clock_index, MusicPlayerInfo *player, MusicPlayerTrack *track) {
    register MusicPlayerTrack *track_r5 asm("r5") = track;
    register struct AudioChannelState *channel asm("r4");
    register ToneData *tone asm("r9");
    struct SoundEngineState *engine = SOUND_ENGINE;
    u32 key;
    u32 priority;
    u32 psg_channel;
    s32 rhythm_pan;
    s32 shifted_key;

    track_r5->gateTime = gMusicClockTable[clock_index];
    {
        register u8 *command asm("r3") = track_r5->command;
        register u32 value asm("r0") = *command;

        if (value < 0x80) {
            track_r5->key = value;
            value = *++command;
            if (value < 0x80) {
                track_r5->velocity = value;
                value = *++command;
                if (value < 0x80) {
                    track_r5->gateTime += value;
                    command++;
                }
            }
            track_r5->command = command;
        }
    }

    rhythm_pan = 0;
    {
        register ToneData *track_tone asm("r4") = &track_r5->tone;
        register u32 type asm("r2") = track_tone->fields.type;

        if (type & TONE_TYPE_MULTI) {
            register u32 note asm("r3") = track_r5->key;
            register u32 mapped asm("r0");
            register ToneData *instrument asm("r6");

            if (type & TONE_TYPE_SPLIT)
                mapped = ((u8 *)track_tone->words[2])[note];
            else
                mapped = note;
            tone = (ToneData *)track_tone->fields.wave + mapped;
            instrument = tone;
            if (instrument->fields.type & TONE_TYPE_MULTI)
                return;
            if (type & TONE_TYPE_RHYTHM) {
                register u32 pan_sweep asm("r1") = instrument->fields.panSweep;

                if (pan_sweep & 0x80)
                    rhythm_pan = (pan_sweep - 0xC0) * 2;
                note = instrument->fields.key;
            }
            key = note;
        } else {
            tone = track_tone;
            key = track_r5->key;
        }
    }

    priority = player->priority + track_r5->priority;
    if (priority > 0xFF)
        priority = 0xFF;

    psg_channel = tone->fields.type & TONE_TYPE_PSG_MASK;
    if (psg_channel != 0) {
        channel = (struct AudioChannelState *)engine->psg_channels;
        if (channel == NULL)
            return;
        channel = (struct AudioChannelState *)((struct PsgChannelState *)channel + (psg_channel - 1));
        if ((channel->flags & SOUND_CHANNEL_ON) && !(channel->flags & SOUND_CHANNEL_STOP)) {
            if (channel->priority > priority)
                return;
            if (channel->priority == priority && channel->owner_track < track_r5)
                return;
        }
    } else {
        register u32 best_priority asm("r6") = priority;
        MusicPlayerTrack *best_track = track_r5;
        register u32 found_stopping asm("r2") = 0;
        register struct AudioChannelState *best asm("r8") = NULL;
        register s32 channels_left asm("r3") = engine->max_pcm_channels;

        channel = engine->pcm_channels;
    search:
        {
            register u32 flags asm("r1") = channel->flags;

            if (!(flags & SOUND_CHANNEL_ON))
                goto take_channel;
            if (flags & SOUND_CHANNEL_STOP) {
                if (found_stopping == 0) {
                    found_stopping++;
                    best_priority = channel->priority;
                    best_track = channel->owner_track;
                    goto select;
                }
            } else if (found_stopping != 0) {
                goto next_candidate;
            }

            {
                register u32 candidate_priority asm("r0") = channel->priority;

                if (candidate_priority < best_priority) {
                    best_priority = candidate_priority;
                    best_track = channel->owner_track;
                } else if (candidate_priority > best_priority) {
                    goto next_candidate;
                } else {
                    register MusicPlayerTrack *candidate_track asm("r0") = channel->owner_track;

                    if (candidate_track > best_track)
                        best_track = candidate_track;
                    else if (candidate_track < best_track)
                        goto next_candidate;
                }
            }
        select:
            best = channel;
        next_candidate:
            channel++;
            if (--channels_left > 0)
                goto search;
        }

        channel = best;
        if (channel == NULL)
            return;
    }

take_channel:
    UnlinkMusicChannelViaCallback(channel);
    {
        register u32 zero asm("r1") = 0;
        register struct AudioChannelState *next asm("r3");
        register u32 lfo_delay asm("r0");

        channel->previous = (struct AudioChannelState *)zero;
        next = track_r5->channel;
        channel->next = next;
        if (next != NULL)
            next->previous = channel;
        track_r5->channel = channel;
        channel->owner_track = track_r5;

        lfo_delay = track_r5->lfoDelay;
        track_r5->lfoDelayCounter = lfo_delay;
        if (lfo_delay != zero)
            ResetMusicTrackModulation(lfo_delay, track_r5);
    }
    UpdateMusicTrackVolumeAndPitch(player, track_r5);

    *(u32 *)&channel->gate_time = *(u32 *)&track_r5->gateTime;
    channel->priority = priority;
    channel->key = key;
    channel->rhythm_pan = rhythm_pan;
    {
        register ToneData *instrument asm("r6") = tone;

        channel->type = instrument->fields.type;
        channel->wave = instrument->fields.wave;
        *(u32 *)&channel->attack = instrument->words[2];
    }
    *(u16 *)&channel->echo_volume = *(u16 *)&track_r5->pseudoEchoVolume;
    UpdateMusicChannelVolume(channel, track_r5);

    {
        register u32 base_key asm("r1") = channel->key;
        register s32 key_shift asm("r0") = (s8)track_r5->keyMapped;

        shifted_key = base_key + key_shift;
        if (shifted_key < 0)
            shifted_key = 0;
    }

    if (psg_channel != 0) {
        register ToneData *instrument asm("r6") = tone;
        struct PsgChannelState *psg = (struct PsgChannelState *)channel;
        register u32 sweep asm("r1");

        psg->length = instrument->fields.length;
        sweep = instrument->fields.panSweep;
        if ((sweep & 0x80) || !(sweep & 0x70))
            sweep = PSG_DEFAULT_SWEEP;
        psg->sweep = sweep;
        channel->frequency = ((PsgFrequencyFunc)engine->calculate_psg_frequency)(
            psg_channel, shifted_key, track_r5->pitchMapped);
    } else {
        channel->frequency = CalculatePcmPlaybackFrequency(tone->fields.wave, shifted_key, track_r5->pitchMapped);
    }

    channel->flags = SOUND_CHANNEL_START;
    track_r5->flags &= 0xF0;
}
