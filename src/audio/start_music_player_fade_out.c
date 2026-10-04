#include "m2c_prelude.h"

typedef struct {
    u8 pad00[0x24];
    u16 fade_interval;
    u16 fade_counter;
    u16 fade_volume;
    u8 pad2A[10];
    u32 signature;
} MusicPlayerFadeState;

void StartMusicPlayerFadeOut(MusicPlayerFadeState *state, u16 fade_interval)
{
    register u32 signature asm("r3") = state->signature;

    if (signature == 0x68736D53) {
        state->fade_counter = fade_interval;
        state->fade_interval = fade_interval;
        state->fade_volume = 0x100;
    }
}
