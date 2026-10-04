#include "sound_engine.h"
M2C_UNK CallFunctionR2(M2C_UNK, M2C_UNK, s32) asm("func_080ECD64");
extern s32 gMusicTrackEndCallback asm("D_030074B0");

void MusicCommandEndTrackViaCallback(M2C_UNK player, M2C_UNK track) asm("func_080ECA78");

void MusicCommandEndTrackViaCallback(M2C_UNK player, M2C_UNK track) {
    CallFunctionR2(player, track, gMusicTrackEndCallback);
}
