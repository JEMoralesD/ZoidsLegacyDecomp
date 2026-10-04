#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../battle/battle_display.h"
extern int StartBattleCameraTransition(int, int, int, int) asm("func_080BB224");
extern int SeekEventCommand(int, int, int) asm("func_80A016C");

int EventCenterBattleCamera(u8 script_slot) asm("func_080A2F18");

int EventCenterBattleCamera(u8 script_slot) {
    StartBattleCameraTransition(BATTLE_CAMERA_CENTERED_ALTERNATE, 0, 0, 0);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
