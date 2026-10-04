#include "m2c_prelude.h"
#include "../event_script.h"
extern void DisableDisplayWindows(void) asm("func_0809534C");
extern void StopTask(s32) asm("func_08092E0C");
extern void SeekEventCommand(s32, s32, s32) asm("func_80A016C");
extern u8 gBattleSetup[];

s32 EventSetBattleSceneTerrain(u8 script_slot, u8 **script_cursor) asm("func_080A3998");

s32 EventSetBattleSceneTerrain(u8 script_slot, u8 **script_cursor) {
    if (gBattleSetup[2] == 0xFF) {
        DisableDisplayWindows();
        StopTask(7);
        *(u16 *)0x0300004C = 0x1D40;
        *(u16 *)0x04000008 = 0x4085;
        *(u16 *)0x0400000C = 0x028F;
    }
    gBattleSetup[2] = EVENT_COMMAND_BYTE((*script_cursor), EventBattleTerrainCommand, terrain_id);
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
