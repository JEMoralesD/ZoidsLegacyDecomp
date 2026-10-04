#include "sound_engine.h"
M2C_UNK StopMusicPlayer(s32) asm("func_080EBF24");
extern u32 D_00000004;

void StopAllMusicPlayers(void) asm("func_080EB888");

void StopAllMusicPlayers(void) {
    s32 *player_definition;
    u32 player_count;
    u32 players_remaining;

    player_count = (u32)&D_00000004;
    player_count = player_count << 0x10;
    player_count = player_count >> 0x10;
    if (player_count != 0) {
        player_definition = (s32 *)MUSIC_PLAYER_TABLE_ROM;
        players_remaining = player_count;
        do {
            StopMusicPlayer(*player_definition);
            player_definition += MUSIC_PLAYER_TABLE_WORD_STRIDE;
            players_remaining -= 1;
        } while (players_remaining != 0);
    }
}
