#include "m2c_prelude.h"
struct SongEntry { s32 song_header; u16 player_index; };
struct MusicPlayerEntry { s32 *player; u8 pad[8]; };
extern struct SongEntry gSongTable[];
extern struct MusicPlayerEntry gMusicPlayerTable[];
void StopMusicPlayer(s32 *) asm("func_80EBF24");
void ContinueMusicPlayer(s32 *) asm("func_80EB694");

void StopSongById(s32 song_id) {
    s32 *player;
    u16 song_index = (u16)song_id;
    register struct MusicPlayerEntry *players asm("r2") = gMusicPlayerTable;
    player = players[gSongTable[song_index].player_index].player;
    if (*player == gSongTable[song_index].song_header) {
        StopMusicPlayer(player);
    }
}

void ContinueSongById(s32 song_id) {
    s32 *player;
    u16 song_index = (u16)song_id;
    register struct MusicPlayerEntry *players asm("r2") = gMusicPlayerTable;
    player = players[gSongTable[song_index].player_index].player;
    if (*player == gSongTable[song_index].song_header) {
        ContinueMusicPlayer(player);
    }
}
