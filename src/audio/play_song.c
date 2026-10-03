#include "m2c_prelude.h"
M2C_UNK StartSongById(u16) asm("func_80EB754");
M2C_UNK StartMusicPlayer(void) asm("func_80EBE40");
void PlaySong(u16 song_id) { StartSongById(song_id); }
void sub_08092E90(void) { StartMusicPlayer(); }
