#include "m2c_prelude.h"
#include "../event_script.h"
extern u8 gFieldEventActive asm("D_02030664");
extern u8 gFrameStep;
void YieldTaskForUpdates(int) asm("func_080ED17C");
int SeekEventCommand(int, int, int) asm("func_80A016C");

int EventWaitFrames(u8 script_slot, u8 **cursor) {
    u32 total;
    gFieldEventActive = 1;
    total = 0;
    while (total < (*cursor)[1]) {
        YieldTaskForUpdates(1);
        total += gFrameStep;
    }
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
