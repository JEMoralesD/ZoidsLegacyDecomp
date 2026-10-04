#include "sound_engine.h"
void SetMusicTrackToneWave(MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080ECA8C");

#define READ_WAVE_BYTE(wave, n)                \
    {                                          \
        u32 byte = track->command[(n)];        \
        byte <<= (n) * 8;                      \
        (wave) &= ~(0xFF << ((n) * 8));        \
        (wave) |= byte;                        \
    }

void SetMusicTrackToneWave(MusicPlayerInfo *player, MusicPlayerTrack *track) {
    u32 wave;

    READ_WAVE_BYTE(wave, 0)
    READ_WAVE_BYTE(wave, 1)
    READ_WAVE_BYTE(wave, 2)
    READ_WAVE_BYTE(wave, 3)
    track->tone.fields.wave = (void *)wave;
    track->command += 4;
}
