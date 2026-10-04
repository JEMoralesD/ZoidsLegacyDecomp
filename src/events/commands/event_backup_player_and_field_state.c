#include "m2c_prelude.h"
#include "../event_script.h"
extern int CopyBytes() asm("func_80ED038");
extern int SeekEventCommand() asm("func_80A016C");
extern u8 gPlayerStateBackup[] asm("D_020282EC");
extern u8 gPlayerState[] asm("D_020218E4");
extern u8 gFieldStateBackup[] asm("D_0202EEC0");
extern u8 gFieldSaveState[] asm("D_0202ECF4");

int EventBackupPlayerAndFieldState(u8 script_slot) asm("func_080A6604");

int EventBackupPlayerAndFieldState(u8 script_slot) {
    CopyBytes(gPlayerStateBackup, gPlayerState, 0x6a08);
    CopyBytes(gFieldStateBackup, gFieldSaveState, 460);
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
