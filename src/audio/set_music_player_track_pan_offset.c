#include "sound_engine.h"

void SetMusicPlayerTrackPanOffset(void *player, s32 track_mask, s32 pan_offset) asm("func_080EC790");

void SetMusicPlayerTrackPanOffset(void *player, s32 track_mask, s32 pan_offset) {
    register void *state asm("r4");
    s32 selected_tracks;
    register s32 packed_pan asm("r6");
    register s32 tracks_remaining asm("r2");
    register u8 *track asm("r1");
    register s32 track_bit asm("r5");
    register s32 active_flag asm("r8");
    register s32 update_flags asm("r12");
    s32 signature_or_flags;

    asm volatile("" ::: "r9");
    state = player;
    track_mask <<= 16;
    selected_tracks = (u32)track_mask >> 16;
    pan_offset <<= 24;
    packed_pan = (u32)pan_offset >> 24;
    signature_or_flags = MUSIC_PLAYER_CONTROL_FIELD(state, s32 *, signature);
    if (signature_or_flags == SOUND_ENGINE_SIGNATURE) {
        {
            register s32 next asm("r0");
            next = signature_or_flags + 1;
            asm volatile("" : "+r"(next));
            MUSIC_PLAYER_CONTROL_FIELD(state, s32 *, signature) = next;
        }
        tracks_remaining = MUSIC_PLAYER_FIELD(state, u8 *, trackCount);
        track = MUSIC_PLAYER_FIELD(state, u8 **, tracks);
        track_bit = 1;
        if (tracks_remaining > 0) {
            active_flag = MUSIC_TRACK_ACTIVE;
            update_flags = MUSIC_TRACK_VOLUME_UPDATE_FLAGS;
            do {
                {
                    register s32 test asm("r0");
                    test = selected_tracks;
                    asm volatile("" : "+r"(test));
                    test &= track_bit;
                    if (test) {
                        signature_or_flags = MUSIC_TRACK_FIELD(track, u8 *, flags);
                        test = active_flag;
                        asm volatile("" : "+r"(test));
                        test &= signature_or_flags;
                        if (test) {
                            MUSIC_TRACK_FIELD(track, u8 *, panExtra) = packed_pan;
                            test = update_flags;
                            asm volatile("" : "+r"(test));
                            test |= signature_or_flags;
                            MUSIC_TRACK_FIELD(track, u8 *, flags) = test;
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
