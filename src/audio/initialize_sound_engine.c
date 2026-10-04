#include "sound_engine.h"
extern void BiosCpuSet() asm("func_80ECD2C");
extern void InitializeSoundMixerState() asm("func_080EBAD0");
extern void InitializePsgChannels() asm("func_080EB98C");
extern void ConfigureSoundMode() asm("func_080EBC40");
extern void InitializeMusicPlayer() asm("func_080EBDC8");
extern u8 gMusicPlayerTable[];
extern u8 D_080EAB29[];
extern u8 D_00000004[];

void InitializeSoundEngine(void) asm("func_080EB6D0");

void InitializeSoundEngine(void) {
    u8 *player_definition;
    u16 player_count;
    u32 players_remaining;
    void *player;
    BiosCpuSet((s32)D_080EAB29 & ~1, SOUND_ENGINE_MIXER_RAM, SOUND_ENGINE_MIXER_COPY_CONTROL);
    InitializeSoundMixerState(SOUND_ENGINE_STATE_RAM);
    InitializePsgChannels(SOUND_ENGINE_PSG_CHANNELS_RAM);
    ConfigureSoundMode(SOUND_MODE_DEFAULT);
    player_count = (u16)(u32)D_00000004;
    if (player_count != 0) {
        player_definition = gMusicPlayerTable;
        players_remaining = player_count;
        do {
            player = MUSIC_PLAYER_DEFINITION_FIELD(player_definition, void **, player);
            InitializeMusicPlayer(player, MUSIC_PLAYER_DEFINITION_FIELD(player_definition, s32 *, tracks), MUSIC_PLAYER_DEFINITION_FIELD(player_definition, u8 *, track_count));
            MUSIC_PLAYER_FIELD(player, s8 *, priorityCheckEnabled) = MUSIC_PLAYER_DEFINITION_FIELD(player_definition, u16 *, priority_check_enabled);
            MUSIC_PLAYER_FIELD(player, s32 *, memory) = SOUND_ENGINE_PLAYER_MEMORY;
            player_definition += sizeof(struct MusicPlayerDefinition);
            players_remaining -= 1;
        } while (players_remaining != 0);
    }
}
