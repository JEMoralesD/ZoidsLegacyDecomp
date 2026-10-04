#include "m2c_prelude.h"
#include "../event_script.h"
M2C_UNK StartBattleCameraTransition(s32, u8, s32, s32) asm("func_080BB224");
M2C_UNK SeekEventCommand(u8, s32, s32) asm("func_80A016C");

s32 EventSetBattleCameraSide(u8 script_slot, u8 **script_cursor) asm("func_080A2EDC");

s32 EventSetBattleCameraSide(u8 script_slot, u8 **script_cursor) {
    if (EVENT_COMMAND_BYTE((*script_cursor), EventBattleCameraSideCommand, side_id) == 0) {
        StartBattleCameraTransition(2, 0, 0, 0);
    } else {
        StartBattleCameraTransition(2, 1, 0, 0);
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
