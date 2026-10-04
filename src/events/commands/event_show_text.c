#include "m2c_prelude.h"
#include "../../game/game_state.h"
#include "../event_script.h"
extern void RunMenuScript(int) asm("func_08098BB4");
extern void SeekEventCommand(int, int, int) asm("func_080A016C");

s32 EventShowText(u8 script_slot, s32 *cursor) {
    u8 temp_r1;
    s32 one;
    *(s8 *)0x02030664 = (one = 1);
    if ((*(s32 *)0x02021690 != GAME_MODE_BATTLE_SCENE) || (*(u8 *)0x02031748 != 0)) {
        u8 *p = (u8 *)0x02030666;
        temp_r1 = *p;
        if (temp_r1 == 0) {
            if (*(s32 *)0x02031744 != 0) {
                goto block_8;
            }
            goto block_11;
        }
        if (*(s32 *)0x02031744 != 0) {
            if (temp_r1 == 2) {
                RunMenuScript(0x080177FA);
block_8:
                RunMenuScript(0x080177DA);
                *p = one;
            }
        } else if (temp_r1 == 1) {
            RunMenuScript(0x080177F5);
block_11:
            RunMenuScript(0x080177ED);
            *p = 2;
        }
    } else {
        *(u8 *)0x02030666 = one;
    }
    *(s32 *)0x0200A888 = *cursor + 1;
    if (*(u8 *)0x02030666 == 1) {
        RunMenuScript(0x080177D0);
    } else {
        RunMenuScript(0x080177D5);
    }
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
