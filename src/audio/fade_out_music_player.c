#include "m2c_prelude.h"
M2C_UNK StartMusicPlayerFadeOut(M2C_UNK, u16) asm("func_080EB6B0");                /* extern */

void FadeOutMusicPlayer(M2C_UNK player, u16 fade_interval) {
    StartMusicPlayerFadeOut(player, fade_interval);
}
