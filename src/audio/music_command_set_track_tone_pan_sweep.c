#include "sound_engine.h"
void MusicCommandSetTrackTonePanSweep(int player_address, void *track) asm("func_080ECB64");

void MusicCommandSetTrackTonePanSweep(int player_address, void *track) {
    u8 command_value;
    u8 *tone_field;

    command_value = *MUSIC_TRACK_FIELD(track, u8 **, command);
    tone_field = track;
    tone_field += MUSIC_TRACK_OFFSET(tone.fields.panSweep);
    *tone_field = command_value;
    command_value = 0;
    MUSIC_TRACK_FIELD(track, u8 **, command) = MUSIC_TRACK_FIELD(track, u8 **, command) + 1;
}
