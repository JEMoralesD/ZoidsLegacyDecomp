#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/game_state.h"

s32 IsScreenTransitionComplete(void) asm("func_0809669C");
void SeekEventCommand(u8, s32, s32) asm("func_80A016C");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventOpenPauseMenu(u8 script_slot, void **script_cursor) asm("func_080A5F24");

s32 EventOpenPauseMenu(u8 script_slot, void **script_cursor) {
    s32 previous_game_mode;

    *(u8 *)0x02030664 = 1;
    previous_game_mode = *(s32 *)0x02021690;
    *(s32 *)0x02021690 = GAME_MODE_PAUSE_MENU;
    *(u8 *)0x02032A85 = EVENT_COMMAND_BYTE(*script_cursor, EventPauseMenuCommand, menu_entry_mode);
    do {
        YieldTaskForUpdates(1);
    } while (*(s32 *)0x02021690 != GAME_MODE_FIELD);

    goto check_transition;
wait_for_transition:
    YieldTaskForUpdates(1);
check_transition:
    if ((IsScreenTransitionComplete() << 24) == 0) {
        goto wait_for_transition;
    }
    *(s32 *)0x02021690 = previous_game_mode;
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
