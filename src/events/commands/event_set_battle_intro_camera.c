#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../battle/battle_display.h"
extern int StartBattleCameraTransition(int camera_mode, int side_id,
    int unit_id, int immediate) asm("func_080BB224");
extern int SeekEventCommand(int script_slot, int target_opcode,
    int choice_id) asm("func_80A016C");

int EventSetBattleIntroCamera(u8 script_slot) asm("func_080A2F40");

int EventSetBattleIntroCamera(u8 script_slot) {
    StartBattleCameraTransition(BATTLE_CAMERA_INTRO_CENTERED, 0, 0, 1);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
