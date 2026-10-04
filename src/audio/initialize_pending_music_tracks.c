#include "sound_engine.h"
M2C_UNK ClearMusicStatePrefixViaCallback(void *) asm("func_080EBABC");

void InitializePendingMusicTracks(void *player) asm("func_080EB944");

void InitializePendingMusicTracks(void *player) {
    u8 track_flags;
    u8 active_mask;
    register s32 pending_mask asm("r0");
    s32 tracks_remaining;
    void *track;
    u8 active_flag;
    register u8 pending_flag asm("r6");
    tracks_remaining = MUSIC_PLAYER_FIELD(player, u8 *, trackCount);
    track = MUSIC_PLAYER_FIELD(player, void **, tracks);
    if ((s32)tracks_remaining > 0) {
        active_flag = MUSIC_TRACK_ACTIVE;
        do {
            track_flags = MUSIC_TRACK_FIELD(track, u8 *, flags);
            active_mask = active_flag;
            active_mask &= track_flags;
            if (active_mask != 0) {
                pending_flag = MUSIC_TRACK_INITIALIZATION_PENDING;
                pending_mask = pending_flag;
                asm volatile("" : "+r"(pending_mask));
                pending_mask &= track_flags;
                asm volatile("" : "+r"(pending_mask));
                if (pending_mask != 0) {
                    ClearMusicStatePrefixViaCallback(track);
                    MUSIC_TRACK_FIELD(track, u8 *, flags) = active_flag;
                    MUSIC_TRACK_FIELD(track, s8 *, bendRange) = MUSIC_TRACK_DEFAULT_BEND_RANGE;
                    MUSIC_TRACK_FIELD(track, s8 *, volumeExtra) = pending_flag;
                    MUSIC_TRACK_FIELD(track, s8 *, lfoSpeed) = MUSIC_TRACK_DEFAULT_LFO_SPEED;
                    MUSIC_TRACK_FIELD(track, s8 *, tone.fields.type) = MUSIC_TRACK_DEFAULT_TONE_TYPE;
                }
            }
            tracks_remaining -= 1;
            track += sizeof(MusicPlayerTrack);
        } while ((s32)tracks_remaining > 0);
    }
}
