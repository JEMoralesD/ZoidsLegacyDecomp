#include "m2c_prelude.h"
#include "../../field/field_display.h"
#include "../../game/game_state.h"
M2C_UNK SeekEventCommand() asm("func_80A016C");
extern u32 gGameMode;
extern u8 gEventMapId;
extern u8 gFieldDecorationsEnabled asm("D_020324B1");

s32 EventDisableWorldMapDecorations(u8 script_slot) asm("func_080A5FF4");

s32 EventDisableWorldMapDecorations(u8 script_slot) {
    u8 v;
    if (gGameMode == GAME_MODE_FIELD) {
        v = gEventMapId;
        if (v == 0 || v == 0x40) {
            gFieldDecorationsEnabled = 0;
        }
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
