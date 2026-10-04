#include "sound_engine.h"
void SetMusicPlayerTempo(struct MusicPlayerControlState *music_player, u16 tempo) asm("func_080EC68C");

void SetMusicPlayerTempo(struct MusicPlayerControlState *music_player, u16 tempo) {
    register u32 signature asm("r3") = music_player->signature;

    if (signature == SOUND_ENGINE_SIGNATURE) {
        register u16 base_tempo asm("r4");

        music_player->player.tempoScale = tempo;
        base_tempo = music_player->player.tempo;
        music_player->player.tempoInterval = (tempo * base_tempo) >> 8;
    }
}
