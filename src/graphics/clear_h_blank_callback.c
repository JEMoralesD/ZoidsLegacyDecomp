#include "m2c_prelude.h"
extern s32 gHBlankCallbacks[];
void ClearHBlankCallback(s32 callback_index) {
    s32 *callbacks;
    s32 first_callback, second_callback, index_bits;
    index_bits = callback_index << 0x18;
    callbacks = gHBlankCallbacks;
    *(s32 *)(((u32)index_bits >> 0x16) + (s32)callbacks) = 0x08092555;
    first_callback = callbacks[0];
    if (first_callback == 0x08092555) {
        second_callback = callbacks[1];
        if (second_callback == first_callback && callbacks[2] == second_callback) {
            *(u16 *)0x04000200 &= 0xFFFD;
            *(u16 *)0x04000004 &= 0xFFEF;
        }
    }
}
