#include "sound_engine.h"

extern void ReadNextMusicCommandByteIntoR3(s32 player_address, MusicPlayerTrack *track) asm("func_080EB5F8");
extern void ResetMusicTrackModulation(s32 player_address, MusicPlayerTrack *track) asm("func_080EB5DC");

#define SAVE_MUSIC_COMMAND_LINK_REGISTER() asm volatile("mov ip, lr")
#define RETURN_THROUGH_MUSIC_COMMAND_LINK_REGISTER() asm volatile("bx ip")

void SetMusicTrackLfoSpeed(s32 player_address, MusicPlayerTrack *track) asm("func_080EB604");

__attribute__((naked)) void SetMusicTrackLfoSpeed(s32 player_address, MusicPlayerTrack *track)
{
    register s32 live_player asm("r0") = player_address;
    register MusicPlayerTrack *live_track asm("r1") = track;
    register u32 value asm("r3");

    SAVE_MUSIC_COMMAND_LINK_REGISTER();
    ReadNextMusicCommandByteIntoR3(live_player, live_track);
    asm volatile("" : "=r"(live_player), "=r"(live_track), "=r"(value));
    live_track->lfoSpeed = value;
    if (value == 0) {
        ResetMusicTrackModulation(live_player, live_track);
    }
    RETURN_THROUGH_MUSIC_COMMAND_LINK_REGISTER();
}
