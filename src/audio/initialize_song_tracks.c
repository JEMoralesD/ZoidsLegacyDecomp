#include "sound_engine.h"
M2C_UNK InitializePendingMusicTracks(s32) asm("func_080EB944");
extern u8 gSongTable[];
extern s32 gMusicPlayerTable[];

void InitializeSongTracks(s32 song_id) asm("func_08092E4C");

void InitializeSongTracks(s32 song_id) {
    s32 *music_player_table;
    u8 *song_table;
    song_id <<= 16;
    music_player_table = gMusicPlayerTable;
    song_table = gSongTable;
    InitializePendingMusicTracks(music_player_table[*(u16 *)(song_table + ((u32)song_id >> 13) + MUSIC_SONG_OFFSET(player_index)) * MUSIC_PLAYER_TABLE_WORD_STRIDE]);
}
