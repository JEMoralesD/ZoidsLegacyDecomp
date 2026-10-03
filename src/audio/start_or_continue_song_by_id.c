#include "m2c_prelude.h"
extern void ContinueMusicPlayer(void *) asm("func_80EB694");
extern void StartMusicPlayer(void *, s32) asm("func_80EBE40");

struct MusicPlayerState { s32 song_header; s32 status; };
struct SongEntry { s32 song_header; u16 player_index; u16 unused; };
struct MusicPlayerEntry { struct MusicPlayerState *player; u8 pad[8]; };

extern struct SongEntry gSongTable[];
extern struct MusicPlayerEntry gMusicPlayerTable[];

void StartOrContinueSongById(u16 song_id) {
    register struct MusicPlayerEntry *players asm("r2") = gMusicPlayerTable;
    struct SongEntry *songs = gSongTable;
    struct SongEntry *song = &songs[song_id];
    struct MusicPlayerState *player = players[song->player_index].player;
    s32 current_song = player->song_header;
    s32 requested_song = song->song_header;
    if (current_song != requested_song) {
        StartMusicPlayer(player, requested_song);
        return;
    }
    if ((u16)player->status == 0) {
        StartMusicPlayer(player, current_song);
        return;
    }
    if (player->status < 0) {
        ContinueMusicPlayer(player);
    }
}
