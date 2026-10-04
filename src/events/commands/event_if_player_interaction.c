#include "m2c_prelude.h"
#include "../event_script.h"
struct S { u8 unk0; u8 unk1; u8 unk2; u8 unk3; u8 unk4; u8 pad[24]; u8 unk1D; };
extern struct S *D_02032990;
extern u32 gEventScriptStarts[];
extern int SeekEventCommand(int, int, int) asm("func_080A016C");
s32 EventIfPlayerInteraction(u8 script_slot, struct S **cursor) {
    struct S *p = D_02032990;
    if (p != 0 && p->unk4 != 0x6C) {
        register u8 t asm("r2") = p->unk1D;
        if ((0x40 & t) && ((0x3F & t) == (*cursor)->unk1)) {
            register int neg asm("r5") = -1;
            SeekEventCommand(script_slot, neg, 0);
            if ((*cursor)->unk0 != 0x18) goto end;
        loop:
            SeekEventCommand(script_slot, neg, 0);
            SeekEventCommand(script_slot, neg, 0);
            if ((*cursor)->unk0 == 0x18) goto loop;
            goto end;
        }
    }
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    if ((*cursor)->unk0 != 0x18) {
        *cursor = (struct S *)gEventScriptStarts[script_slot];
        return EVENT_YIELD;
    }
end:
    return EVENT_CONTINUE;
}
