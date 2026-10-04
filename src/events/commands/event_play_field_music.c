#include "m2c_prelude.h"
#include "../event_script.h"
M2C_UNK PlayOrContinueSong(u16) asm("func_8092E74");
M2C_UNK SeekEventCommand(u8, s32, s32) asm("func_80A016C");
extern u8 gCurrentFieldSongId asm("D_02030667");

int EventPlayFieldMusic(u8 script_slot, u8 **cursor) asm("func_080A2474");

int EventPlayFieldMusic(u8 script_slot, u8 **cursor) {
    gCurrentFieldSongId = EVENT_COMMAND_BYTE(*cursor, EventSongCommand, song_id);
    PlayOrContinueSong(gCurrentFieldSongId);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
