#include "m2c_prelude.h"
extern s32 gHBlankCallbacks[];

void SetHBlankCallback(u8 callback_index, s32 callback) {
    if (gHBlankCallbacks[0] == 0x08092555) {
        if ((gHBlankCallbacks[1] == gHBlankCallbacks[0]) && (gHBlankCallbacks[2] == gHBlankCallbacks[1])) {
            *(u16 *)0x04000200 |= 2;
            *(u16 *)0x04000004 |= 0x10;
        }
    }
    gHBlankCallbacks[callback_index] = callback;
}
