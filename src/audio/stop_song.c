#include "m2c_prelude.h"
M2C_UNK StopSongById(u16) asm("func_80EB820");                             /* extern */

void StopSong(u16 song_id) {
    StopSongById(song_id);
}
