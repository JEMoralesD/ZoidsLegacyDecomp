#include "m2c_prelude.h"
struct SongEntry { s32 song_header; u16 player_index; };
struct MusicPlayerState { s32 song_header; s32 status; };
struct MusicPlayerEntry { struct MusicPlayerState *player; u8 pad[8]; };
extern struct SongEntry gSongTable[];
extern struct MusicPlayerEntry gMusicPlayerTable[];
void SetMusicTrackVolume(s32, u16, u16) asm("func_80EC6B4");
s32 func_80ECD98(s32, s32);
void FadeOutMusicPlayer(s32, u16) asm("func_80EB8EC");
void PlaySong(u16) asm("func_08092E84");
void func_80ED17C(s32);

s32 IsSongPlayerStopped(s32 song_id) {
    u16 song_index = (u16)song_id;
    if (gMusicPlayerTable[gSongTable[song_index].player_index].player->status < 0) {
        return 1;
    }
    return 0;
}

s32 IsMusicPlayerStopped(s32 player_id) {
    u16 player_index = (u16)player_id;
    if (gMusicPlayerTable[player_index].player->status < 0) {
        return 1;
    }
    return 0;
}

void SetSongVolume(s32 song_id, s32 volume) {
    u16 song_index = (u16)song_id;
    u16 track_volume = (u16)volume;
    struct MusicPlayerState *player = gMusicPlayerTable[gSongTable[song_index].player_index].player;
    SetMusicTrackVolume((s32) player, (u16) player->status, track_volume);
}

void FadeOutSong(s32 song_id, s32 duration) {
    u16 song_index = (u16)song_id;
    u32 duration_bits = (u32) (duration << 16);
    struct MusicPlayerState *player = gMusicPlayerTable[gSongTable[song_index].player_index].player;
    u16 fade_interval = (u16) func_80ECD98((s32) (duration_bits >> 12), 60);
    FadeOutMusicPlayer((s32) player, fade_interval);
}

void PlaySongAndWait(s32 song_id) {
    u16 song_index = (u16)song_id;
    PlaySong(song_index);
    do {
        func_80ED17C(1);
    } while (IsSongPlayerStopped(song_index) == 0);
}
