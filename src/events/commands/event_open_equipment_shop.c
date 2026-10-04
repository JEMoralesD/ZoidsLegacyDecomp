#include "m2c_prelude.h"
#include "../../graphics/screen_effects.h"
#include "../../game/game_state.h"
#include "../event_script.h"
extern void StartScreenTransition(s32, s32) asm("func_08096308");
extern void SeekEventCommand(s32, s32, s32) asm("func_080A016C");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventOpenWeaponShop(u8 script_slot, u8 **cursor) {
    u8 *p = (u8 *)0x02030664;
    *p = 1;
    *(u8 *)0x02032B98 = (*cursor)[1];
    *(s32 *)0x02021690 = GAME_MODE_WEAPON_SHOP;
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0);
    *p = 2;
    do {
        YieldTaskForUpdates(1);
    } while (*p != 1);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}

s32 EventOpenArmorShop(u8 script_slot, u8 **cursor) {
    u8 *p = (u8 *)0x02030664;
    *p = 1;
    *(u8 *)0x02032B98 = (*cursor)[1];
    *(s32 *)0x02021690 = GAME_MODE_ARMOR_SHOP;
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0);
    *p = 2;
    do {
        YieldTaskForUpdates(1);
    } while (*p != 1);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
