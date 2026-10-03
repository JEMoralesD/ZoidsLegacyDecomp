#include "m2c_prelude.h"
#include "../../game/game_state.h"
#include "../event_script.h"
extern void func_0809534C();
extern void func_08092E0C();
extern void SeekEventCommand() asm("func_80A016C");
extern void func_80ED17C();
extern u8 gBattleSetup[];

s32 EventEnterField(u8 script_slot) {
    s32 temp_r0;
    *(s8 *)0x02030664 = 1;
    temp_r0 = *(s32 *)0x02021690;
    if (temp_r0 != 3) {
        if (temp_r0 == 0xA && gBattleSetup[2] == 0xFF) {
            func_0809534C();
            func_08092E0C(7);
        }
        *(s8 *)0x02030666 = 0;
        *(s32 *)0x02031744 = 0;
        *(s32 *)0x02021690 = GAME_MODE_FIELD;
        func_80ED17C(1);
        *(s8 *)0x02030666 = 0;
        *(s32 *)0x02031744 = 0;
    }
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
