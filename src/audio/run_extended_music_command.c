#include "sound_engine.h"
void RunExtendedMusicCommand(MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080ECA58");

typedef void (*MusicCommandHandler)(MusicPlayerInfo *, MusicPlayerTrack *);
extern const MusicCommandHandler gExtendedMusicCommandHandlers[];

void RunExtendedMusicCommand(MusicPlayerInfo *player, MusicPlayerTrack *track) {
    u32 command = *track->command;

    track->command++;
    gExtendedMusicCommandHandlers[command](player, track);
}
