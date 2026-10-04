#include "../sound_engine.h"
void UpdateMusicPlayer(struct MusicPlayerControlState *music_player) asm("func_080EB0C0");
void UnlinkMusicChannelViaCallback(struct AudioChannelState *channel) asm("func_080EBAA8");
void ClearMusicStatePrefixViaCallback(MusicPlayerTrack *track) asm("func_080EBABC");
void UpdateMusicPlayerFade(MusicPlayerInfo *player) asm("func_080EBF64");
void UpdateMusicTrackVolumeAndPitch(MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EC02C");
void UpdateMusicChannelVolume(struct AudioChannelState *channel, MusicPlayerTrack *track) asm("func_080EB36C");
u32 CalculatePcmPlaybackFrequency(struct PcmWaveHeader *wave, u32 key, u32 pitch) asm("func_080EB62C");

extern const u8 gMusicClockTable[];

typedef void (*MusicPlayerCallback)(void *);
typedef void (*PlayNoteFunc)(u32 clock_index, MusicPlayerInfo *player, MusicPlayerTrack *track);
typedef void (*MusicCommandFunc)(MusicPlayerInfo *player, MusicPlayerTrack *track);
typedef u32 (*PsgFrequencyFunc)(u32 psg_channel, u32 key, u32 pitch);

#define MUSIC_TEMPO_STEP 150
#define PLAYER_CONTROL(player) ((struct MusicPlayerControlState *)(player))

void UpdateMusicPlayer(struct MusicPlayerControlState *music_player) {
    MusicPlayerInfo *player;
    register struct SoundEngineState *engine asm("r8");
    register MusicPlayerTrack *track asm("r5");
    register s32 tracks_left asm("r6");
    register u32 track_bit asm("r3");
    register u32 active_tracks asm("r4");
    register u32 saved_track_bit asm("sl");
    u32 saved_active_tracks;
    register struct AudioChannelState *channel asm("r4");
    register u32 counter asm("r0");

    player = &music_player->player;
    if (PLAYER_CONTROL(player)->signature != SOUND_ENGINE_SIGNATURE)
        return;
    PLAYER_CONTROL(player)->signature++;

    if (PLAYER_CONTROL(player)->next_update_callback != NULL) {
        register MusicPlayerCallback callback asm("r3") = PLAYER_CONTROL(player)->next_update_callback;

        callback(PLAYER_CONTROL(player)->next_player);
    }

    if ((s32)player->status < 0)
        goto unlock;
    engine = SOUND_ENGINE;
    UpdateMusicPlayerFade(player);
    if ((s32)player->status < 0)
        goto unlock;

    for (counter = player->tempoCounter + player->tempoInterval; player->tempoCounter = counter, counter >= MUSIC_TEMPO_STEP;
         counter = player->tempoCounter - MUSIC_TEMPO_STEP) {
        tracks_left = player->trackCount;
        track = player->tracks;
        track_bit = 1;
        active_tracks = 0;
    first_pass_track:
        {
            u32 wait;

            if (!(track->flags & MUSIC_TRACK_ACTIVE))
                goto advance;

            saved_track_bit = track_bit;
            active_tracks |= track_bit;
            saved_active_tracks = active_tracks;

            for (channel = track->channel; channel != NULL; channel = channel->next) {
                register u32 flags asm("r1") = channel->flags;

                if (flags & SOUND_CHANNEL_ON) {
                    u32 gate_time = channel->gate_time;

                    if (gate_time != 0) {
                        gate_time--;
                        channel->gate_time = gate_time;
                        if (gate_time == 0)
                            channel->flags = flags | SOUND_CHANNEL_STOP;
                    }
                } else {
                    UnlinkMusicChannelViaCallback(channel);
                }
            }

            if (track->flags & MUSIC_TRACK_INITIALIZATION_PENDING) {
                ClearMusicStatePrefixViaCallback(track);
                track->flags = MUSIC_TRACK_ACTIVE;
                track->bendRange = MUSIC_TRACK_DEFAULT_BEND_RANGE;
                track->volumeExtra = MUSIC_TRACK_DEFAULT_VOLUME_EXTRA;
                track->lfoSpeed = MUSIC_TRACK_DEFAULT_LFO_SPEED;
                track->tone.fields.type = MUSIC_TRACK_DEFAULT_TONE_TYPE;
            }

            while ((wait = track->wait) == 0) {
                register u8 *command asm("r2") = track->command;
                register u32 value asm("r1") = *command;

                if (value < 0x80) {
                    value = track->runningStatus;
                } else {
                    track->command = ++command;
                    if (value >= 0xBD)
                        track->runningStatus = value;
                }

                if (value >= 0xCF) {
                    register PlayNoteFunc play_note asm("r3") = engine->play_note;

                    play_note(value - 0xCF, player, track);
                } else if (value > 0xB0) {
                    register u32 index asm("r0") = value - 0xB1;
                    register MusicCommandFunc *handlers asm("r3");
                    register MusicCommandFunc handler asm("r3");

                    player->command = index;
                    handlers = engine->command_handlers;
                    handler = handlers[index];
                    handler(player, track);
                    if (track->flags == 0)
                        goto next_track;
                } else {
                    track->wait = gMusicClockTable[value - 0x80];
                }
            }

            track->wait = wait - 1;
            {
                register u32 lfo_speed asm("r1") = track->lfoSpeed;

                if (lfo_speed != 0 && track->modulationDepth != 0) {
                    u32 delay = track->lfoDelayCounter;

                    if (delay != 0) {
                        track->lfoDelayCounter = delay - 1;
                    } else {
                        register u32 sum asm("r0") = track->lfoSpeedCounter + lfo_speed;
                        register u32 phase asm("r1");
                        register s32 modulation asm("r2");
                        register s32 product asm("r0");

                        track->lfoSpeedCounter = sum;
                        phase = sum;
                        if ((s8)(sum - 0x40) < 0)
                            modulation = (s8)phase;
                        else
                            modulation = 0x80 - phase;
                        product = track->modulationDepth * modulation;
                        modulation = product >> 6;
                        if ((u8)(modulation ^ track->modulationCalculated) != 0) {
                            track->modulationCalculated = modulation;
                            track->flags |= track->modulationType == 0 ? MUSIC_TRACK_PITCH_UPDATE_FLAGS
                                                                       : MUSIC_TRACK_VOLUME_UPDATE_FLAGS;
                        }
                    }
                }
            }

        next_track:
            track_bit = saved_track_bit;
            active_tracks = saved_active_tracks;
        advance:
            if (--tracks_left > 0) {
                track++;
                track_bit <<= 1;
                goto first_pass_track;
            }
        }

        player->clock++;
        if (active_tracks == 0) {
            player->status = MUSIC_PLAYER_STOPPED_STATUS;
            goto unlock;
        }
        player->status = active_tracks;
    }

    {
        register s32 tracks_remaining asm("r2") = player->trackCount;
        register s32 saved_tracks_remaining asm("r9");
        register u32 psg_channel asm("r6");

        track = player->tracks;
    second_pass_track:
        {
            if ((track->flags & MUSIC_TRACK_ACTIVE) && (track->flags & MUSIC_TRACK_VOLUME_PITCH_UPDATE_FLAGS)) {
                saved_tracks_remaining = tracks_remaining;
                UpdateMusicTrackVolumeAndPitch(player, track);
                for (channel = track->channel; channel != NULL; channel = channel->next) {
                    if (!(channel->flags & SOUND_CHANNEL_ON)) {
                        UnlinkMusicChannelViaCallback(channel);
                        continue;
                    }

                    psg_channel = channel->type & TONE_TYPE_PSG_MASK;
                    if (track->flags & MUSIC_TRACK_VOLUME_UPDATE_FLAGS) {
                        UpdateMusicChannelVolume(channel, track);
                        if (psg_channel != 0)
                            ((struct PsgChannelState *)channel)->modify |= PSG_MODIFY_VOLUME;
                    }
                    if (track->flags & MUSIC_TRACK_PITCH_UPDATE_FLAGS) {
                        register u32 base_key asm("r1") = channel->key;
                        register s32 key_shift asm("r0") = (s8)track->keyMapped;
                        register s32 key asm("r2") = base_key + key_shift;
                        register u32 key_arg asm("r1");
                        register u32 pitch asm("r2");

                        if (key < 0)
                            key = 0;
                        if (psg_channel != 0) {
                            register PsgFrequencyFunc frequency asm("r3") = engine->calculate_psg_frequency;

                            key_arg = key;
                            pitch = track->pitchMapped;
                            channel->frequency = frequency(psg_channel, key_arg, pitch);
                            ((struct PsgChannelState *)channel)->modify |= PSG_MODIFY_PITCH;
                        } else {
                            key_arg = key;
                            pitch = track->pitchMapped;
                            channel->frequency = CalculatePcmPlaybackFrequency(channel->wave, key_arg, pitch);
                        }
                    }
                }
                track->flags &= 0xF0;
                tracks_remaining = saved_tracks_remaining;
            }
            if (--tracks_remaining > 0) {
                track++;
                goto second_pass_track;
            }
        }
    }

unlock:
    PLAYER_CONTROL(player)->signature = SOUND_ENGINE_SIGNATURE;
}
