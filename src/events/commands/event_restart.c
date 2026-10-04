#include "m2c_prelude.h"
#include "../../game/game_state.h"
#include "../event_script.h"
extern void UnlockEventInteractionActors(void) asm("func_080A0114");
extern u32 gGameMode;
extern u32 gEventScriptStarts[];
extern u8 gFieldEventActive asm("D_02030664");

int EventRestart(u8 script_slot, u32 *cursor) {
    if (gGameMode == GAME_MODE_FIELD) {
        UnlockEventInteractionActors();
        *cursor = gEventScriptStarts[script_slot];
        gFieldEventActive = 0;
        return EVENT_CONTINUE;
    }
    return EVENT_YIELD;
}
