#include "sound_engine.h"
void MusicCommandSetTrackToneAttack(int player_address, void *track) asm("func_080ECAE8");

void MusicCommandSetTrackToneAttack(int player_address, void *track) {
    register u8 *byte_address asm("r0");
    register u8 command_value asm("r2");

    byte_address = MUSIC_TRACK_FIELD(track, u8 **, command);
    command_value = *byte_address;
    byte_address = track;
    byte_address += MUSIC_TRACK_OFFSET(tone.fields.attack);
    *byte_address = command_value;
    byte_address = MUSIC_TRACK_FIELD(track, u8 **, command);
    MUSIC_TRACK_FIELD(track, u8 **, command) = byte_address + 1;
}
