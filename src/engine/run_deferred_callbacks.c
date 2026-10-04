#include "m2c_prelude.h"
M2C_UNK CallFunctionR1(s32) asm("func_080ECD60");
extern s32 gDeferredCallbacks[];
void RunDeferredCallbacks(void) {
    s32 *entry = gDeferredCallbacks;
    if (*entry != 0) {
        do {
            register s32 callback asm("r1") = entry[1];
            if (callback != -1) {
                u8 delay_frames = M2C_FIELD(entry, u8 *, 12);
                if (delay_frames == 0) {
                    CallFunctionR1(entry[2]);
                    entry[1] = -1;
                } else M2C_FIELD(entry, u8 *, 12) = delay_frames - 1;
            }
            entry += 4;
        } while (*entry != 0);
    }
}
