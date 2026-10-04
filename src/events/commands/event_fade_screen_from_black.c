#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../graphics/screen_effects.h"
extern void StartScreenTransition(int, int) asm("func_08096308");
extern int IsScreenTransitionComplete(void) asm("func_0809669C");
extern void SeekEventCommand(int, int, int) asm("func_080A016C");
extern void YieldTaskForUpdates(int) asm("func_080ED17C");

s32 EventFadeScreenFromBlack(u8 script_slot, u8 **command_cursor) asm("func_080A16E4");

s32 EventFadeScreenFromBlack(u8 script_slot, u8 **command_cursor) {
    *(s8 *)FIELD_EVENT_ACTIVE_RAM = 1;
    if ((IsScreenTransitionComplete() << 0x18) != 0) {
        StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, EVENT_COMMAND_BYTE(*command_cursor, EventScreenFadeCommand, duration_updates));
        SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    } else {
        YieldTaskForUpdates(1);
    }
    return EVENT_CONTINUE;
}
