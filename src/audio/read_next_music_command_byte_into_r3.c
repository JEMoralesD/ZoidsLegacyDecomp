#include "sound_engine.h"
void ReadNextMusicCommandByteIntoR3(int player_address, void *track) asm("func_080EB5F8");

void ReadNextMusicCommandByteIntoR3(int player_address, void *track) {
    register u8 *command asm("r2");

    command = MUSIC_TRACK_FIELD(track, u8 **, command);
    {
        register u8 *next_command asm("r3");

        next_command = command + 1;
        MUSIC_TRACK_FIELD(track, u8 **, command) = next_command;
    }
    {
        register u8 command_byte asm("r3");

        command_byte = *(volatile u8 *)command;
    }
}
