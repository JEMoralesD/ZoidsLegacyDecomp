#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
extern u8 gFieldSaveState[] asm("D_0202ECF4");
extern void *gWorldMapStructureSprite asm("D_020314A0");
extern void DestroySprite(void *) asm("func_8094554");
extern void SeekEventCommand(int, int, int) asm("func_080A016C");

u8 EventRemoveWorldMapStructure(u8 script_slot) asm("func_080A6490");

u8 EventRemoveWorldMapStructure(u8 script_slot) {
    gFieldSaveState[EVENT_COMMAND_OFFSET(EventFieldMapStateView, world_structure_kind)] = 0;
    if (gWorldMapStructureSprite != 0) {
        DestroySprite(gWorldMapStructureSprite);
        gWorldMapStructureSprite = 0;
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
