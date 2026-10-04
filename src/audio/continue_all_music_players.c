#include "sound_engine.h"
void ContinueMusicPlayer(MusicPlayerInfo *player) asm("func_80EB694");
void ContinueAllMusicPlayers(void) asm("func_080EB8C0");

extern const struct MusicPlayerDefinition gMusicPlayerTable[];
extern u8 gMusicPlayerCount[];

void ContinueAllMusicPlayers(void) {
    s32 i;

    for (i = 0; i < (u16)(s32)gMusicPlayerCount; i++)
        ContinueMusicPlayer(gMusicPlayerTable[i].player);
}
