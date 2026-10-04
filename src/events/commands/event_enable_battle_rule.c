#include "m2c_prelude.h"
#include "../event_script.h"
extern int EnableBattleRule(int) asm("func_080E65B4");
extern int SeekEventCommand(int, int, int) asm("func_80A016C");

int EventEnableBattleRule(u8 script_slot, u8 **cursor) asm("func_080A1344");

int EventEnableBattleRule(u8 script_slot, u8 **cursor) {
    EnableBattleRule(EVENT_COMMAND_BYTE(*cursor, EventBattleRuleCommand, rule_id));
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
