#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
extern u8 gFieldSaveState[] asm("D_0202ECF4");
extern u8 gEventMapId;
extern void CreateWorldMapStructureSprite(void) asm("func_080A6148");
extern int SeekEventCommand(int, int, int) asm("func_80A016C");

s32 EventSetWorldMapStructure(u8 script_slot, u8 **script_cursor) asm("func_080A63DC");

s32 EventSetWorldMapStructure(u8 script_slot, u8 **script_cursor) {
    gFieldSaveState[EVENT_COMMAND_OFFSET(EventFieldMapStateView, world_structure_kind)] = (*script_cursor)[1] + 1;
    gFieldSaveState[EVENT_COMMAND_OFFSET(EventFieldMapStateView, world_structure_cell_x)] = (*script_cursor)[2];
    gFieldSaveState[EVENT_COMMAND_OFFSET(EventFieldMapStateView, world_structure_cell_y)] = (*script_cursor)[3];
    if (gEventMapId == 0)
        CreateWorldMapStructureSprite();
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
