#include "sound_engine.h"
void MusicCommandSetTrackPseudoEchoVolume(int player_address, void *track) asm("func_080ECB38");

void MusicCommandSetTrackPseudoEchoVolume(int player_address, void *track) {
    u8 *command_cursor;

    command_cursor = MUSIC_TRACK_FIELD(track, u8 **, command);
    MUSIC_TRACK_FIELD(track, u8 *, pseudoEchoVolume) = *command_cursor;
    MUSIC_TRACK_FIELD(track, u8 **, command) = command_cursor + 1;
}
