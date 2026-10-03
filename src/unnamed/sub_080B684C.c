#include "m2c_prelude.h"
M2C_UNK PlaySong(s32) asm("func_08092E84");

extern u16 gRepeatedKeys_A[];
extern u16 gRepeatedKeys_B[];
extern u16 gRepeatedKeys_C[];
extern u16 gRepeatedKeys_D[];

s32 sub_080B684C(s32 arg0, s32 arg1) {
    register s32 value asm("r4");
    register s32 bound asm("r5");

    value = arg0;
    bound = arg1;
    if (gRepeatedKeys_A[0] & 0x20) {
        if (value > 1) {
            value--;
        } else {
            value = bound;
        }
        PlaySong(0x40);
    }
    if (gRepeatedKeys_B[0] & 0x10) {
        if (value < bound) {
            value++;
        } else {
            value = 1;
        }
        PlaySong(0x40);
    }
    if (gRepeatedKeys_C[0] & 0x200) {
        if (value > 10) {
            value -= 10;
        } else if (value > 1) {
            value = 1;
        } else {
            value = bound;
        }
        PlaySong(0x40);
    }
    if (gRepeatedKeys_D[0] & 0x100) {
        if (value < bound - 10) {
            value += 10;
        } else if (value < bound) {
            value = bound;
        } else {
            value = 1;
        }
        PlaySong(0x40);
    }
    return value;
}
