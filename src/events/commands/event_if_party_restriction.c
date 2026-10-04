#include "m2c_prelude.h"
#include "../event_script.h"
struct EventPartyRestrictionCommand { u8 opcode; u8 restriction_id; };

s32 DoesPlayerTeamMeetBattleRule(u8) asm("func_080E60B0");
void EnableBattleRule(u8) asm("func_080E65B4");
void ClearBattleRules(void) asm("func_080E6684");
void SeekEventCommand(s32, s32, s32) asm("func_080A016C");

s32 EventIfPartyRestriction(u8 script_slot, struct EventPartyRestrictionCommand **cursor) {
    u8 temp_r5;

    temp_r5 = script_slot;
    if ((DoesPlayerTeamMeetBattleRule((*cursor)->restriction_id) << 0x18) != 0) {
        EnableBattleRule((*cursor)->restriction_id);
        SeekEventCommand(temp_r5, EVENT_BRANCH_TRUE, 0);
    } else {
        ClearBattleRules();
        SeekEventCommand(temp_r5, EVENT_BRANCH_FALSE, 0);
    }
    return EVENT_CONTINUE;
}
