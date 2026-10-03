#include "m2c_prelude.h"
#include "../event_script.h"
struct EventPartyRestrictionCommand { u8 opcode; u8 restriction_id; };

s32 func_080E60B0(u8);
void func_080E65B4(u8);
void func_080E6684(void);
void SeekEventCommand(s32, s32, s32) asm("func_080A016C");

s32 EventIfPartyRestriction(u8 script_slot, struct EventPartyRestrictionCommand **cursor) {
    u8 temp_r5;

    temp_r5 = script_slot;
    if ((func_080E60B0((*cursor)->restriction_id) << 0x18) != 0) {
        func_080E65B4((*cursor)->restriction_id);
        SeekEventCommand(temp_r5, EVENT_BRANCH_TRUE, 0);
    } else {
        func_080E6684();
        SeekEventCommand(temp_r5, EVENT_BRANCH_FALSE, 0);
    }
    return EVENT_CONTINUE;
}
