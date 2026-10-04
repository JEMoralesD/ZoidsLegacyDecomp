#include "m2c_prelude.h"
M2C_UNK StartOrContinueSongById(u16) asm("func_80EB7CC");                             /* extern */

void PlayOrContinueSong(u16 song_id) {
    StartOrContinueSongById(song_id);
}
