#include "m2c_prelude.h"
#include "../event_script.h"
extern void ClearEventFlag(int) asm("func_809F7F0");
extern int SeekEventCommand(int, int, int) asm("func_80A016C");
extern u8 gEventFlagConditionEnabled[];

int EventClearFlag(u8 script_slot, u8 **cursor) {
    u8 i;
    for (i = 0; i <= 0x45; i++) {
        gEventFlagConditionEnabled[i] = 1;
    }
    ClearEventFlag((*cursor)[1]);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
