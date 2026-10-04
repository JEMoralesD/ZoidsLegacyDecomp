#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../graphics/screen_effects.h"
#include "../../game/game_state.h"
extern void StartScreenTransition(int, int) asm("func_08096308");
extern void YieldTaskForUpdates(int) asm("func_080ED17C");
extern int SeekEventCommand(int, int, int) asm("func_080A016C");

extern u8 gFieldEventActive asm("D_02030664");
extern u32 gGameMode;

int EventOpenZoidsLab(u8 script_slot) asm("func_080A242C");

int EventOpenZoidsLab(u8 script_slot)
{
    gFieldEventActive = 1;
    gGameMode = GAME_MODE_ZOIDS_LAB;
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0);
    gFieldEventActive = 2;
    do {
        YieldTaskForUpdates(1);
    } while (gFieldEventActive != 1);
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
