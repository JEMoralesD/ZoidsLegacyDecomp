#include "m2c_prelude.h"
#include "../ui/name_entry.h"
#include "../graphics/screen_effects.h"
#include "../game/game_state.h"

extern void StopTask(s32) asm("func_08092E0C");
extern void DisableDisplayWindows(void) asm("func_0809534C");
extern void StartScreenTransition(s32, s32) asm("func_08096308");
extern s32 IsScreenTransitionComplete(void) asm("func_0809669C");
extern void ResetEventScripts(void) asm("func_0809FCB0");
extern void StopBattleAnimation(void) asm("func_080D120C");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");
extern u8 gTitleResumeFlag asm("D_02021698");

void RunStartButtonSceneSkipTask(void) asm("func_0809B51C");

void RunStartButtonSceneSkipTask(void)
{
    s32 skip_complete;
    u32 transition_kind_minus_two;

    skip_complete = 0;
    goto outer_test;

outer_body:
    if ((*(u16 *)NAME_ENTRY_PRESSED_KEYS_RAM & NAME_ENTRY_KEY_START) == 0) {
        goto tick;
    }
    if ((IsScreenTransitionComplete() << 24) != 0) {
        if ((*(u16 *)0x0300004E == 0xFF) &&
            (*(u16 *)0x03000052 == 0x10)) {
            goto complete;
        }
        StartScreenTransition(SCREEN_TRANSITION_DITHER_CONCEAL, 0);
        {
            u8 *transition_update_address = (u8 *)0x03005F72;
            u8 *transition_end_update_address = (u8 *)0x03005F71;
            goto poll_first_test;
poll_first_body:
            if ((*(s32 *)0x02021690 == GAME_MODE_BATTLE_SCENE) &&
                (*transition_update_address == *transition_end_update_address)) {
                DisableDisplayWindows();
                StopBattleAnimation();
            }
            YieldTaskForUpdates(1);
poll_first_test:
            if ((IsScreenTransitionComplete() << 24) == 0) {
                goto poll_first_body;
            }
        }
        goto complete;
    }

    transition_kind_minus_two = (*(u8 *)0x03005F70 & 0x3F) - 2;
    switch (transition_kind_minus_two) {
    case 0:
    case 2:
    case 4:
    case 6:
    case 9:
    case 10:
    case 12:
    case 16:
        {
            u8 *transition_update_address = (u8 *)0x03005F72;
            u8 *transition_end_update_address = (u8 *)0x03005F71;
            goto poll_selected_test;
poll_selected_body:
            if ((*(s32 *)0x02021690 == GAME_MODE_BATTLE_SCENE) &&
                (*transition_update_address == *transition_end_update_address)) {
                DisableDisplayWindows();
                StopBattleAnimation();
            }
            YieldTaskForUpdates(1);
poll_selected_test:
            if ((IsScreenTransitionComplete() << 24) == 0) {
                goto poll_selected_body;
            }
        }
        goto complete;
    default:
        goto wait_input_test;
wait_input_body:
        YieldTaskForUpdates(1);
wait_input_test:
        if ((IsScreenTransitionComplete() << 24) == 0) {
            goto wait_input_body;
        }
        StartScreenTransition(SCREEN_TRANSITION_DITHER_CONCEAL, 0);
        {
            u8 *transition_update_address = (u8 *)0x03005F72;
            u8 *transition_end_update_address = (u8 *)0x03005F71;
            goto poll_default_test;
poll_default_body:
            if ((*(s32 *)0x02021690 == GAME_MODE_BATTLE_SCENE) &&
                (*transition_update_address == *transition_end_update_address)) {
                DisableDisplayWindows();
                StopBattleAnimation();
            }
            YieldTaskForUpdates(1);
poll_default_test:
            if ((IsScreenTransitionComplete() << 24) == 0) {
                goto poll_default_body;
            }
        }
        goto complete;
    }

complete:
    ResetEventScripts();
    StopTask(3);
    *(u8 *)0x03000075 = 1;
    *(s32 *)0x02021690 = GAME_MODE_TITLE;
    gTitleResumeFlag = 0;
    skip_complete = 1;

tick:
    YieldTaskForUpdates(1);
    if (skip_complete == 0) {
outer_test:
        if (*(u8 *)0x02030664 != 0) {
            goto outer_body;
        }
    }
}
