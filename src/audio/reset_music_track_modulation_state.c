#include "sound_engine.h"

void ResetMusicTrackModulationState(MusicPlayerTrack *track) asm("func_080EC7F8");

void ResetMusicTrackModulationState(MusicPlayerTrack *track)
{
    register MusicPlayerTrack *state asm("r1") = track;
    register s32 unused_zero asm("r2") = 0;
    register s32 value asm("r0") = 0;

    asm volatile("" : "+r"(unused_zero));
    state->lfoSpeedCounter = value;
    state->modulationCalculated = value;
    value = state->modulationType;
    if (value == MUSIC_MODULATION_PITCH) {
        value = MUSIC_TRACK_PITCH_UPDATE_FLAGS;
    } else {
        value = MUSIC_TRACK_VOLUME_UPDATE_FLAGS;
    }
    {
        register u8 flags asm("r2") = state->flags;

        value |= flags;
        state->flags = value;
    }
}
