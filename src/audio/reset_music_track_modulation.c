#include "sound_engine.h"
void ResetMusicTrackModulation(void *player, void *track) asm("func_080EB5DC");

void ResetMusicTrackModulation(void *player, void *track)
{
    register s32 update_flags asm("r2") = 0;
    MUSIC_TRACK_FIELD(track, s8 *, modulationCalculated) = update_flags;
    MUSIC_TRACK_FIELD(track, s8 *, lfoSpeedCounter) = update_flags;
    update_flags = MUSIC_TRACK_FIELD(track, u8 *, modulationType);
    if (update_flags == MUSIC_MODULATION_PITCH) {
        update_flags = MUSIC_TRACK_PITCH_UPDATE_FLAGS;
    } else {
        update_flags = MUSIC_TRACK_VOLUME_UPDATE_FLAGS;
    }
    {
        register u8 track_flags asm("r3");
        track_flags = MUSIC_TRACK_FIELD(track, u8 *, flags);
        track_flags |= update_flags;
        MUSIC_TRACK_FIELD(track, u8 *, flags) = track_flags;
    }
}
