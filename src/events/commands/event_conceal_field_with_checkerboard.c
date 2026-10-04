#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../graphics/screen_effects.h"
extern void StartScreenTransition(s32, s32) asm("func_08096308");
extern s32 IsScreenTransitionComplete(void) asm("func_0809669C");
extern void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventConcealFieldWithCheckerboard(u8 script_slot) asm("func_080A1C98");

s32 EventConcealFieldWithCheckerboard(u8 script_slot) {
    *(s8 *)FIELD_EVENT_ACTIVE_RAM = 1;
    if ((IsScreenTransitionComplete() << 0x18) != 0) {
        StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0);
        SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    } else {
        YieldTaskForUpdates(1);
    }
    return EVENT_CONTINUE;
}
