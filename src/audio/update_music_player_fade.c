#include "sound_engine.h"

void func_080EB328(MusicPlayerInfo *, MusicPlayerTrack *);

void UpdateMusicPlayerFade(MusicPlayerInfo *player) asm("func_080EBF64");

void UpdateMusicPlayerFade(MusicPlayerInfo *player)
{
    register MusicPlayerInfo *state asm("r6") = player;
    register u32 fade_interval asm("r1") = state->fadeInterval;

    if (fade_interval != 0) {
        register s32 fade_counter asm("r0") = state->fadeCounter - 1;
        register u32 mask_init asm("r3");
        register u32 mask asm("r2");
        register u32 normalized_counter asm("r3");

        state->fadeCounter = fade_counter;
        mask_init = 0xFFFF;
        asm volatile("" : "+r"(mask_init));
        mask = mask_init;
        normalized_counter = (u16)fade_counter;
        if (normalized_counter == 0) {
            u16 fade_volume;

            state->fadeCounter = fade_interval;
            fade_volume = state->fadeVolume;
            if ((fade_volume & MUSIC_FADE_INCREASE_VOLUME) != 0) {
                s32 next = fade_volume + MUSIC_FADE_VOLUME_STEP;

                state->fadeVolume = next;
                next &= mask;
                if (next > (u32)MUSIC_FADE_LAST_PARTIAL_VOLUME) {
                    state->fadeVolume = MUSIC_FADE_FULL_VOLUME;
                    state->fadeInterval = normalized_counter;
                }
            } else {
                s32 next = fade_volume - MUSIC_FADE_VOLUME_STEP;

                state->fadeVolume = next;
                next &= mask;
                if ((s32)(next << 16) <= 0) {
                    s32 track_count = state->trackCount;
                    MusicPlayerTrack *track = state->tracks;

                    if (track_count > 0) {
                        do {
                            register u32 clear asm("r0");
                            register u32 volume_view asm("r7");

                            func_080EB328(state, track);
                            clear = MUSIC_FADE_KEEP_TRACK_FLAGS;
                            asm volatile("" : "+r"(clear));
                            volume_view = state->fadeVolume;
                            clear &= volume_view;
                            if (clear == 0) {
                                track->flags = clear;
                            }
                            track_count -= 1;
                            track += 1;
                        } while (track_count > 0);
                    }
                    {
                        register u32 test asm("r0") = MUSIC_FADE_KEEP_TRACK_FLAGS;
                        register u32 volume_view asm("r1");

                        asm volatile("" : "+r"(test));
                        volume_view = state->fadeVolume;
                        test &= volume_view;
                        if (test != 0) {
                            state->status |= MUSIC_PLAYER_STOPPED_STATUS;
                        } else {
                            state->status = MUSIC_PLAYER_STOPPED_STATUS;
                        }
                    }
                    state->fadeInterval = 0;
                    return;
                }
            }

            {
                register s32 track_count asm("r5") = state->trackCount;
                register MusicPlayerTrack *track asm("r4") = state->tracks;

                if (track_count > 0) {
                    register u32 flag_mask asm("r3") = MUSIC_TRACK_ACTIVE;
                    u32 volume_value = 0;
                    register u32 set_bits asm("r2");

                    asm volatile("" : "+r"(volume_value));
                    set_bits = MUSIC_TRACK_VOLUME_UPDATE_FLAGS;
                    do {
                        register u32 flags asm("r1") = track->flags;
                        register u32 test asm("r0") = flag_mask;

                        test &= flags;
                        if (test != 0) {
                            volume_value = state->fadeVolume;
                            test = volume_value >> 2;
                            track->volumeExtra = test;
                            test = flags;
                            test |= set_bits;
                            track->flags = test;
                        }
                        asm volatile("" : "+r"(volume_value));
                        track_count -= 1;
                        track += 1;
                    } while (track_count > 0);
                }
            }
        }
    }
}
