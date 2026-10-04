#include "../sound_engine.h"
void MusicCommandJump(MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAF00");
void MusicCommandCallPattern(MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAF1E");
void MusicCommandEndTrack(MusicPlayerInfo *player, MusicPlayerTrack *track) asm("func_080EAE90");
u32 ReadMusicByteIntoR3WithAddressGuard(u32 preserved, MusicPlayerTrack *track, u8 *address) asm("func_080EAED8");

void MusicCommandJump(MusicPlayerInfo *player, MusicPlayerTrack *track) {
    register MusicPlayerTrack *track_r1 asm("r1") = track;
    register u8 *command asm("r2") = track_r1->command;
    register u32 target asm("r0");
    register u32 byte asm("r3");

    target = command[3];
    target <<= 8;
    byte = command[2];
    target = (target | byte) << 8;
    byte = command[1];
    target = (target | byte) << 8;
    target = ReadMusicByteIntoR3WithAddressGuard(target, track_r1, command);
    track_r1->command = (u8 *)(target | byte);
}

void MusicCommandCallPattern(MusicPlayerInfo *player, MusicPlayerTrack *track) {
    register MusicPlayerInfo *player_r0 asm("r0") = player;
    register MusicPlayerTrack *track_r1 asm("r1") = track;
    register u32 level asm("r2") = track_r1->patternLevel;

    if (level < 3) {
        register u8 **stack asm("r3") = (u8 **)((u8 *)track_r1 + level * 4);

        level = (u32)track_r1->command + 4;
        stack[MUSIC_TRACK_OFFSET(patternStack) / 4] = (u8 *)level;
        level = track_r1->patternLevel + 1;
        track_r1->patternLevel = level;
        MusicCommandJump(player_r0, track_r1);
    } else {
        MusicCommandEndTrack(player_r0, track_r1);
    }
}
