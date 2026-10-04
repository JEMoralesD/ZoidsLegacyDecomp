#include "sound_engine.h"

M2C_UNK ResetMusicTrackModulationState(void *) asm("func_080EC7F8");

void SetMusicPlayerTrackModulationDepth(void *player, s32 track_mask, s32 modulation_depth) asm("func_080EC818");

void SetMusicPlayerTrackModulationDepth(void *player, s32 track_mask, s32 modulation_depth) {
    register void *state asm("r6");
    register s32 selected_tracks asm("r10");
    register u8 value asm("r8");
    register s32 tracks_remaining asm("r5");
    register u8 *track asm("r4");
    u32 track_bit;
    register u8 retained_value asm("r9");
    s32 signature;

    state = player;
    track_mask <<= 16;
    track_mask = (u32)track_mask >> 16;
    selected_tracks = track_mask;
    modulation_depth <<= 24;
    value = (u32)modulation_depth >> 24;
    signature = MUSIC_PLAYER_CONTROL_FIELD(state, s32 *, signature);
    if (signature == SOUND_ENGINE_SIGNATURE) {
        MUSIC_PLAYER_CONTROL_FIELD(state, s32 *, signature) = signature + 1;
        tracks_remaining = MUSIC_PLAYER_FIELD(state, u8 *, trackCount);
        track = MUSIC_PLAYER_FIELD(state, u8 **, tracks);
        track_bit = 1;
        if (tracks_remaining > 0) {
            retained_value = value;
            do {
                {
                    register s32 test asm("r0");
                    register s32 scratch asm("r1");
                    test = selected_tracks;
                    asm volatile("" : "+r"(test));
                    test &= track_bit;
                    if (test) {
                        test = MUSIC_TRACK_ACTIVE;
                        asm volatile("" : "+r"(test));
                        scratch = MUSIC_TRACK_FIELD(track, u8 *, flags);
                        test &= scratch;
                        if (test) {
                            MUSIC_TRACK_FIELD(track, u8 *, modulationDepth) = value;
                            scratch = retained_value;
                            asm volatile("" : "+r"(scratch));
                            if (scratch == 0) {
                                ResetMusicTrackModulationState(track);
                            }
                        }
                    }
                }
                tracks_remaining--;
                track += sizeof(MusicPlayerTrack);
                track_bit <<= 1;
            } while (tracks_remaining > 0);
        }
        MUSIC_PLAYER_CONTROL_FIELD(state, s32 *, signature) = SOUND_ENGINE_SIGNATURE;
    }
}
