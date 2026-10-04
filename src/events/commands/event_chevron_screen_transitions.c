#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../graphics/screen_effects.h"
void StartScreenTransition(s32, s32) asm("func_08096308");
s32 IsScreenTransitionComplete(void) asm("func_0809669C");
void SeekEventCommand(s32, s32, s32) asm("func_080A016C");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventRevealFieldWithRightwardChevron(u8 script_slot) asm("func_080A1934");

s32 EventRevealFieldWithRightwardChevron(u8 script_slot) {
    *(s8 *)FIELD_EVENT_ACTIVE_RAM = 1;
    if ((IsScreenTransitionComplete() << 0x18) != 0) {
        StartScreenTransition(SCREEN_TRANSITION_RIGHTWARD_CHEVRON_REVEAL, 0);
        SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    } else {
        YieldTaskForUpdates(1);
    }
    return EVENT_CONTINUE;
}

s32 EventConcealFieldWithLeftwardChevron(u8 script_slot) asm("func_080A1974");

s32 EventConcealFieldWithLeftwardChevron(u8 script_slot) {
    *(s8 *)FIELD_EVENT_ACTIVE_RAM = 1;
    if ((IsScreenTransitionComplete() << 0x18) != 0) {
        StartScreenTransition(SCREEN_TRANSITION_LEFTWARD_CHEVRON_CONCEAL, 0);
        SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    } else {
        YieldTaskForUpdates(1);
    }
    return EVENT_CONTINUE;
}

s32 EventConcealFieldWithRightwardChevron(u8 script_slot) asm("func_080A19B4");

s32 EventConcealFieldWithRightwardChevron(u8 script_slot) {
    *(s8 *)FIELD_EVENT_ACTIVE_RAM = 1;
    if ((IsScreenTransitionComplete() << 0x18) != 0) {
        StartScreenTransition(SCREEN_TRANSITION_RIGHTWARD_CHEVRON_CONCEAL, 0);
        SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    } else {
        YieldTaskForUpdates(1);
    }
    return EVENT_CONTINUE;
}
