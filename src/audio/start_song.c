#include "m2c_prelude.h"
struct SongEntry { s32 song_header; u16 player_index; };
struct MusicPlayerState { s32 song_header; s32 status; };
struct MusicPlayerEntry { struct MusicPlayerState *player; u8 pad[8]; };
extern struct SongEntry gSongTable[];
extern struct MusicPlayerEntry gMusicPlayerTable[];
void StartMusicPlayer(s32, s32) asm("func_80EBE40");

void StartSongById(s32 song_id) {
    u16 song_index = (u16)song_id;
    register struct MusicPlayerEntry *players asm("r2") = gMusicPlayerTable;
    StartMusicPlayer((s32) players[gSongTable[song_index].player_index].player, gSongTable[song_index].song_header);
}

void StartSongIfChanged(s32 song_id) {
    u16 song_index = (u16)song_id;
    register struct MusicPlayerEntry *players asm("r2") = gMusicPlayerTable;
    struct MusicPlayerState *player = players[gSongTable[song_index].player_index].player;
    s32 current_song = player->song_header;
    s32 requested_song = gSongTable[song_index].song_header;
    if (current_song != requested_song) {
        StartMusicPlayer((s32) player, requested_song);
    } else {
        s32 player_status = *(s32 *)((s8 *)player + 4);
        u16 active_tracks = *(u16 *)((s8 *)player + 4);
        if (active_tracks == 0 || player_status < 0) {
            StartMusicPlayer((s32) player, current_song);
        }
    }
}
