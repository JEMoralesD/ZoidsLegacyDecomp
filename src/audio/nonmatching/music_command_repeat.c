#include "../sound_engine.h"
void MusicCommandRepeat(MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAF50");
void MusicCommandJump(MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAF00");

void MusicCommandRepeat(MusicPlayerInfo *player, MusicPlayerTrack *track) {
    register MusicPlayerInfo *player_r0 asm("r0") = player;
    register MusicPlayerTrack *track_r1 asm("r1") = track;
    register u8 *command asm("r2") = track_r1->command;
    register u32 value asm("r3") = *command;

    if (value == 0) {
        track_r1->command = command + 1;
        MusicCommandJump(player_r0, track_r1);
        return;
    }

    value = track_r1->repeatCount + 1;
    track_r1->repeatCount = value;
    ReadNextMusicCommandByteIntoR3WithAddressGuard(player_r0, track_r1);
    if (track_r1->repeatCount < value) {
        MusicCommandJump(player_r0, track_r1);
        return;
    }
    value = 0;
    track_r1->repeatCount = value;
    track_r1->command += 4;
}
