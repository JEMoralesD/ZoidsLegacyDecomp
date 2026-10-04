#include "m2c_prelude.h"
#include "../../field/field_display.h"
extern int SeekEventCommand(int, int, int) asm("func_80A016C");
extern u8 gFieldTravelState[] asm("D_0202ECF4");

int EventSetFieldTravelRestriction(u8 script_slot) asm("func_080A60D0");

int EventSetFieldTravelRestriction(u8 script_slot) {
    gFieldTravelState[FIELD_TRAVEL_RESTRICTIONS_BYTE] |= FIELD_TRAVEL_RESTRICTED;
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
