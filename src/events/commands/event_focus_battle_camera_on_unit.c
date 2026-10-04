#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../battle/battle_display.h"
extern int StartBattleCameraTransition(int, int, int, int) asm("func_080BB224");
extern int SeekEventCommand(int, int, int) asm("func_80A016C");

int EventFocusBattleCameraOnUnit(u8 script_slot, u8 **cursor) asm("func_080A2EB0");

int EventFocusBattleCameraOnUnit(u8 script_slot, u8 **cursor) {
    u8 *command = *cursor;
    StartBattleCameraTransition(BATTLE_CAMERA_FOCUS_UNIT,
        EVENT_COMMAND_BYTE(command, EventBattleCameraUnitCommand, side_id),
        EVENT_COMMAND_BYTE(command, EventBattleCameraUnitCommand, unit_id), 0);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
