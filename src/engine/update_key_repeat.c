#include "m2c_prelude.h"
extern u16 gHeldKeys;
extern u16 gPressedKeys;
extern u16 gRepeatedKeys;
extern u16 gPreviousRepeatKeys;
extern u8 gKeyRepeatTimer;
extern u8 gFrameStep;

void UpdateKeyRepeat(void) {
    u16 *held_keys_address = &gHeldKeys;
    u16 *previous_keys_address = &gPreviousRepeatKeys;
    register u16 held_keys asm("r1") = *held_keys_address;
    if (held_keys == *previous_keys_address) {
        register u8 *repeat_timer asm("r2") = &gKeyRepeatTimer;
        s32 elapsed_frames = gFrameStep + *repeat_timer;
        *repeat_timer = elapsed_frames;
        if ((u8)elapsed_frames > 7) {
            gRepeatedKeys = held_keys;
            *repeat_timer = 0;
        } else {
            gRepeatedKeys = 0;
        }
    } else {
        *previous_keys_address = held_keys;
        gRepeatedKeys = gPressedKeys;
        gKeyRepeatTimer = 0;
    }
}
