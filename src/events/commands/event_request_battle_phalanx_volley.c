#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
extern u8 gEventBattlePhalanxOpcode asm("D_020317D8");
extern void SeekEventCommand(int, int, int) asm("func_080A016C");

u8 EventRequestBattlePhalanxVolley(u8 script_slot, u8 **script_cursor) asm("func_080A39FC");

u8 EventRequestBattlePhalanxVolley(u8 script_slot, u8 **script_cursor) {
    gEventBattlePhalanxOpcode = **script_cursor;
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
