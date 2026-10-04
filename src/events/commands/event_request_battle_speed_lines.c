#include "m2c_prelude.h"
#include "../event_script.h"
extern u8 gBattleSpeedLinesPending asm("D_020317D9");
extern int SeekEventCommand() asm("func_80A016C");

int EventRequestBattleSpeedLines(u8 script_slot) asm("func_080A3A20");

int EventRequestBattleSpeedLines(u8 script_slot) {
    gBattleSpeedLinesPending = 1;
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
