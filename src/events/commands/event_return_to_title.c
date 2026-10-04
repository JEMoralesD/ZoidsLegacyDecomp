#include "m2c_prelude.h"
#include "../../game/game_state.h"
#include "../event_script.h"
extern s32 gGameMode;
extern s8 D_02021698;
extern s8 gFieldEventActive asm("D_02030664");

s32 EventReturnToTitle(int script_slot, s32 *cursor) {
    gGameMode = GAME_MODE_TITLE;
    D_02021698 = 0;
    *cursor = 0;
    gFieldEventActive = 0;
    return EVENT_YIELD;
}
