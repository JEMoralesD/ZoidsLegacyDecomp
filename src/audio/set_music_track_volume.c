#include "m2c_prelude.h"

void SetMusicTrackVolume(void *player, s32 track_mask, s32 volume) {
    register void *player_state asm("r4");
    s32 selected_tracks;
    register s32 scaled_volume asm("r6");
    register s32 remaining_tracks asm("r2");
    register u8 *track asm("r1");
    register s32 track_bit asm("r5");
    register s32 flag_mask asm("r8");
    register s32 set_mask asm("r12");
    s32 temp_r3;

    asm volatile("" ::: "r9");
    player_state = player;
    track_mask <<= 16;
    selected_tracks = (u32)track_mask >> 16;
    scaled_volume = volume << 16;
    temp_r3 = M2C_FIELD(player_state, s32 *, 0x34);
    if (temp_r3 == 0x68736D53) {
        {
            register s32 next asm("r0");
            next = temp_r3 + 1;
            asm volatile("" : "+r"(next));
            M2C_FIELD(player_state, s32 *, 0x34) = next;
        }
        remaining_tracks = M2C_FIELD(player_state, u8 *, 8);
        track = M2C_FIELD(player_state, u8 **, 0x2C);
        track_bit = 1;
        if (remaining_tracks > 0) {
            flag_mask = 0x80;
            scaled_volume = (u32)scaled_volume >> 18;
            set_mask = 3;
            do {
                {
                    register s32 test asm("r0");
                    test = selected_tracks;
                    asm volatile("" : "+r"(test));
                    test &= track_bit;
                    if (test) {
                        temp_r3 = track[0];
                        test = flag_mask;
                        asm volatile("" : "+r"(test));
                        test &= temp_r3;
                        if (test) {
                            track[0x13] = scaled_volume;
                            test = set_mask;
                            asm volatile("" : "+r"(test));
                            test |= temp_r3;
                            track[0] = test;
                        }
                    }
                }
                remaining_tracks--;
                track += 0x50;
                track_bit <<= 1;
            } while (remaining_tracks > 0);
        }
        M2C_FIELD(player_state, s32 *, 0x34) = 0x68736D53;
    }
}
