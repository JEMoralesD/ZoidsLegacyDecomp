#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/game_state.h"
extern u8 gFieldEventActive asm("D_02030664");
extern u32 gGameMode;
extern void YieldTaskForUpdates(int) asm("func_080ED17C");
extern void SeekEventCommand(int, int, int) asm("func_80A016C");

int EventSaveClearData(u8 script_slot) asm("func_080A664C");

int EventSaveClearData(u8 script_slot) {
    gFieldEventActive = 1;
    gGameMode = GAME_MODE_CLEAR_DATA_SAVE;
    do {
        YieldTaskForUpdates(1);
    } while (gGameMode == GAME_MODE_CLEAR_DATA_SAVE);
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
