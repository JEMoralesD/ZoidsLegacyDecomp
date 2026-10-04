#include "sound_engine.h"
void InitializeSoundEngine(void) asm("func_080EB6D0");
void ConfigureSoundMode(int) asm("func_080EBC40");
void InitializeAudio(void) asm("func_08092E2C");

void InitializeAudio(void) {
    *(s8 *)0x03003170 = AUDIO_VSYNC_ENABLED;
    InitializeSoundEngine();
    ConfigureSoundMode(SOUND_MODE_GAME);
}
