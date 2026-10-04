#include "sound_engine.h"
M2C_UNK ContinueMusicPlayer() asm("func_080EB694");

void ResumeMusicPlayerPlayback(void) asm("func_080EB8B4");

void ResumeMusicPlayerPlayback(void) {
    ContinueMusicPlayer();
}
