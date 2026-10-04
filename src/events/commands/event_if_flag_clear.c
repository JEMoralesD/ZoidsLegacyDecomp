#include "m2c_prelude.h"
#include "../event_script.h"
extern u8 gEventFlagConditionEnabled[];
extern u32 gEventScriptStarts[];
extern int TestEventFlag(u8) asm("func_0809F818");
extern int SeekEventCommand(int, int, int) asm("func_080A016C");
s32 EventIfFlagClear(u8 script_slot, struct EventFlagCommand **cursor) {
    if (gEventFlagConditionEnabled[script_slot] != 0 && (TestEventFlag((*cursor)->flag_id) << 24) == 0) {
        register int neg asm("r5") = -1;
        SeekEventCommand(script_slot, neg, 0);
        if ((*cursor)->opcode != EVENT_ALTERNATIVE) goto end;
    loop:
        SeekEventCommand(script_slot, neg, 0);
        SeekEventCommand(script_slot, neg, 0);
        if ((*cursor)->opcode == EVENT_ALTERNATIVE) goto loop;
        goto end;
    }
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    if ((*cursor)->opcode != EVENT_ALTERNATIVE) {
        *cursor = (struct EventFlagCommand *)gEventScriptStarts[script_slot];
        gEventFlagConditionEnabled[script_slot] = 0;
        return EVENT_YIELD;
    }
end:
    return EVENT_CONTINUE;
}
