#include "m2c_prelude.h"
void func_80EB328(void *, s32);

void StopMusicPlayer(void *player) {
    s32 lock;
    s32 remaining_tracks;
    s32 track_address;

    lock = M2C_FIELD(player, s32 *, 0x34);
    if (lock == 0x68736D53) {
        M2C_FIELD(player, s32 *, 0x34) = lock + 1;
        M2C_FIELD(player, s32 *, 4) = M2C_FIELD(player, s32 *, 4) | 0x80000000;
        remaining_tracks = M2C_FIELD(player, u8 *, 8);
        track_address = M2C_FIELD(player, s32 *, 0x2C);
        if (remaining_tracks > 0) {
            do {
                func_80EB328(player, track_address);
                remaining_tracks -= 1;
                track_address += 0x50;
            } while (remaining_tracks > 0);
        }
        M2C_FIELD(player, s32 *, 0x34) = 0x68736D53;
    }
}
