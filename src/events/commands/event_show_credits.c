#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/game_state.h"
extern void YieldTaskForUpdates() asm("func_080ED17C");
extern void SeekEventCommand() asm("func_80A016C");

int EventShowCredits(u8 script_slot) asm("func_080A6688");

int EventShowCredits(u8 script_slot) {
    *(u8 *)0x02030664 = 1;
    *(s32 *)0x02021690 = GAME_MODE_CREDITS;
    do {
        YieldTaskForUpdates(1);
    } while (*(s32 *)0x02021690 != -1);
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
