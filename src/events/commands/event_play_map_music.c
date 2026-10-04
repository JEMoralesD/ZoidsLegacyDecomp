#include "m2c_prelude.h"
#include "../event_script.h"
M2C_UNK PlayOrContinueSong(u16) asm("func_8092E74");
M2C_UNK SeekEventCommand(u8, s32, s32) asm("func_80A016C");
extern u8 gCurrentFieldSongId asm("D_02030667");
extern u16 gCurrentMapId asm("D_0202ECF4");

typedef struct {
    u8 reserved00[0x1E];
    u8 song_id;
    u8 reserved1F;
} MapMusicDefinition;
extern MapMusicDefinition gMapMusicDefinitions[] asm("D_087C4434");

int EventPlayMapMusic(u8 script_slot) asm("func_080A24A4");

int EventPlayMapMusic(u8 script_slot) {
    gCurrentFieldSongId = gMapMusicDefinitions[gCurrentMapId].song_id;
    PlayOrContinueSong(gCurrentFieldSongId);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
