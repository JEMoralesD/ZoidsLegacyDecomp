#include "m2c_prelude.h"
#include "../../field/field_display.h"
extern void SeekEventCommand(int, int, int) asm("func_080A016C");
extern u8 gFieldTravelState[] asm("D_0202ECF4");

u8 EventClearAdditionalFieldTravelRestriction(u8 script_slot) asm("func_080A60F8");

u8 EventClearAdditionalFieldTravelRestriction(u8 script_slot) {
    gFieldTravelState[FIELD_TRAVEL_RESTRICTIONS_BYTE] &= FIELD_CLEAR_ADDITIONAL_TRAVEL_RESTRICTION_MASK;
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
