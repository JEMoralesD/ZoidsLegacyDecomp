#include "m2c_prelude.h"
#include "../event_script.h"
extern u8 D_02030664;
extern u8 gFrameStep;
void func_80ED17C(int);
int SeekEventCommand(int, int, int) asm("func_80A016C");

int EventWaitFrames(u8 script_slot, u8 **cursor) {
    u32 total;
    D_02030664 = 1;
    total = 0;
    while (total < (*cursor)[1]) {
        func_80ED17C(1);
        total += gFrameStep;
    }
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
