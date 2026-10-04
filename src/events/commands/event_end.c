#include "m2c_prelude.h"
#include "../../game/game_state.h"
#include "../event_script.h"
extern u32 gGameMode;
extern u8 gFieldEventActive asm("D_02030664");
extern u16 D_0202ECF4;
extern u8 gLoadedEventMapId;
extern u32 gMapEventScriptTable[];

extern void UnlockEventInteractionActors(void) asm("func_080A0114");
extern void ResetEventScripts(void) asm("func_0809FCB0");
extern void StartMapEventScript(int, int) asm("func_0809FD3C");

int EventEnd(int script_slot, int *cursor) {
    if (gGameMode == GAME_MODE_FIELD) {
        UnlockEventInteractionActors();
        *cursor = 0;
        gFieldEventActive = 0;
        if (D_0202ECF4 != gLoadedEventMapId) {
            ResetEventScripts();
            StartMapEventScript(D_0202ECF4, gMapEventScriptTable[D_0202ECF4]);
        }
    }
    return EVENT_YIELD;
}
