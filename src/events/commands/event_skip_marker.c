#include "m2c_prelude.h"
#include "../event_script.h"
extern int SeekEventCommand(int, int, int) asm("func_80A016C");

int EventSkipMarker(u8 arg) {
    SeekEventCommand(arg, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
