#include "sound_engine.h"

void SetMusicPlayerTrackPitchOffset(void *player, s32 track_mask, s32 pitch_offset) asm("func_080EC71C");

void SetMusicPlayerTrackPitchOffset(void *player, s32 track_mask, s32 pitch_offset) {
    register void *state asm("r4");
    register s32 selected_tracks asm("r12");
    register s32 packed_pitch asm("r6");
    register s32 tracks_remaining asm("r2");
    register u8 *signature_or_track asm("r3");
    register s32 track_bit asm("r5");
    s32 semitone_offset;
    register s32 active_flag asm("r9");
    register s32 update_flags asm("r8");

    asm volatile("" ::: "r10");
    state = player;
    track_mask <<= 16;
    track_mask = (u32)track_mask >> 16;
    selected_tracks = track_mask;
    pitch_offset <<= 16;
    packed_pitch = (u32)pitch_offset >> 16;
    signature_or_track = (u8 *)MUSIC_PLAYER_CONTROL_FIELD(state, s32 *, signature);
    if ((s32)signature_or_track == SOUND_ENGINE_SIGNATURE) {
        {
            register s32 next asm("r0");
            next = (s32)signature_or_track + 1;
            asm volatile("" : "+r"(next));
            MUSIC_PLAYER_CONTROL_FIELD(state, s32 *, signature) = next;
        }
        tracks_remaining = MUSIC_PLAYER_FIELD(state, u8 *, trackCount);
        signature_or_track = MUSIC_PLAYER_FIELD(state, u8 **, tracks);
        track_bit = 1;
        if (tracks_remaining > 0) {
            active_flag = MUSIC_TRACK_ACTIVE;
            semitone_offset = (s32)(packed_pitch << 16) >> 24;
            update_flags = MUSIC_TRACK_PITCH_UPDATE_FLAGS;
            do {
                {
                    register s32 test asm("r0");
                    register s32 flags asm("r1");
                    test = selected_tracks;
                    asm volatile("" : "+r"(test));
                    test &= track_bit;
                    if (test) {
                        flags = MUSIC_TRACK_FIELD(signature_or_track, u8 *, flags);
                        test = active_flag;
                        asm volatile("" : "+r"(test));
                        test &= flags;
                        if (test) {
                            MUSIC_TRACK_FIELD(signature_or_track, u8 *, keyShiftExtra) = semitone_offset;
                            MUSIC_TRACK_FIELD(signature_or_track, u8 *, pitchExtra) = packed_pitch;
                            test = update_flags;
                            asm volatile("" : "+r"(test));
                            test |= flags;
                            MUSIC_TRACK_FIELD(signature_or_track, u8 *, flags) = test;
                        }
                    }
                }
                tracks_remaining--;
                signature_or_track += sizeof(MusicPlayerTrack);
                track_bit <<= 1;
            } while (tracks_remaining > 0);
        }
        MUSIC_PLAYER_CONTROL_FIELD(state, s32 *, signature) = SOUND_ENGINE_SIGNATURE;
    }
}
