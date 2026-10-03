#include "m2c_prelude.h"
#include "../../game/game_state.h"
#include "../event_script.h"
M2C_UNK func_08096308(s32, s32);                    /* extern */
M2C_UNK SeekEventCommand(u8, s32, s32) asm("func_080A016C");                /* extern */
M2C_UNK func_080ED17C(s32);                         /* extern */

s32 EventOpenArmorShop(u8 script_slot, void **cursor) {
    *(u8 *)0x02030664 = 1;
    *(u8 *)0x02032B98 = M2C_FIELD(*cursor, u8 *, 1);
    *(s32 *)0x02021690 = GAME_MODE_ARMOR_SHOP;
    func_08096308(0x10, 0);
    *(u8 *)0x02030664 = 2;
    do {
        func_080ED17C(1);
    } while (*(u8 *)0x02030664 != 1);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
