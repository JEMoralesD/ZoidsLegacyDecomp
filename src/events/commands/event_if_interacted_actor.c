#include "m2c_prelude.h"
#include "../event_script.h"
extern u8 D_02032998;
extern u32 gEventScriptStarts[];
extern int SeekEventCommand(int, int, int) asm("func_080A016C");

s32 EventIfInteractedActor(u8 script_slot, struct EventActorConditionCommand **cursor) {
    register u8 temp_r2 asm("r2");
    register s32 saved asm("r5");

    temp_r2 = script_slot;
    saved = temp_r2;

    if (D_02032998 == (*cursor)->actor_id) {
        register int neg asm("r4") = -1;
        SeekEventCommand(temp_r2, neg, 0);
        if ((*cursor)->opcode != EVENT_ALTERNATIVE) goto end;
loop:
        SeekEventCommand(saved, neg, 0);
        SeekEventCommand(saved, neg, 0);
        if ((*cursor)->opcode == EVENT_ALTERNATIVE) goto loop;
        goto end;
    }
    SeekEventCommand(saved, EVENT_SCAN_NEXT, 0);
    if ((*cursor)->opcode != EVENT_ALTERNATIVE) {
        register u32 *base asm("r0");
        register s32 offset asm("r1");

        base = gEventScriptStarts;
        asm volatile("" : "+r"(base));
        offset = saved << 2;
        asm volatile("" : "+r"(offset));
        offset += (s32)base;
        asm volatile("" : "+r"(offset));
        *cursor = *(struct EventActorConditionCommand **)offset;
        return EVENT_YIELD;
    }
end:
    return EVENT_CONTINUE;
}
