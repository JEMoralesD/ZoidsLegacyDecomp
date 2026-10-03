#include "m2c_prelude.h"
#include "../event_script.h"
extern void func_80A9F40(u8);
extern void SeekEventCommand(u8, s32, s32) asm("func_80A016C");

s32 EventRemoveActor(u8 script_slot, u8 **cursor) {
    func_80A9F40((*cursor)[1]);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
