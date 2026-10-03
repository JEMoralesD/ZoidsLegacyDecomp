#include "m2c_prelude.h"
#include "../../game/game_state.h"
#include "../event_script.h"
extern void func_80A0114(void);
extern u32 gGameMode;
extern u32 gEventScriptStarts[];
extern u8 D_02030664;

int EventRestart(u8 script_slot, u32 *cursor) {
    if (gGameMode == GAME_MODE_FIELD) {
        func_80A0114();
        *cursor = gEventScriptStarts[script_slot];
        D_02030664 = 0;
        return EVENT_CONTINUE;
    }
    return EVENT_YIELD;
}
