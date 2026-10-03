#include "m2c_prelude.h"
#include "../event_script.h"
extern u8 D_02030664;
extern int ShowEventChoices(int) asm("func_809FBA8");
extern void SeekEventCommand(u8, int, u8) asm("func_80A016C");

int EventChoice(u8 script_slot, int *cursor) {
    int r;
    D_02030664 = 1;
    r = ShowEventChoices(*cursor + 2);
    SeekEventCommand(script_slot, EVENT_CHOICE_CASE, r);
    return EVENT_CONTINUE;
}
