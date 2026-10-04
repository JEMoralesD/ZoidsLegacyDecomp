#include "m2c_prelude.h"
M2C_UNK WaitForFrameUpdates(u8) asm("func_0809223C");                              /* extern */
M2C_UNK ReadKeys() asm("func_8092604");                                /* extern */
M2C_UNK ServiceLinkControlRequests() asm("func_0809279C");                                /* extern */
M2C_UNK UpdateSpritesAndBuildOam() asm("func_80946A0");                                /* extern */
M2C_UNK UpdateSpriteGroups() asm("func_8095154");                                /* extern */
M2C_UNK BuildScanlineWindowEffects() asm("func_08096278");                                /* extern */
M2C_UNK BuildScreenTransitionFrame() asm("func_08096774");                                /* extern */
M2C_UNK RefreshWindowTilemaps() asm("func_80972D4");                                /* extern */
M2C_UNK BiosSoftReset(s32) asm("func_80ECD40");                             /* extern */
M2C_UNK RunScheduledTasks() asm("func_080ED174");                                /* extern */

void RunMainLoop(void) asm("func_080921D4");

void RunMainLoop(void) {
    s32 temp_r1;

loop_1:
    ReadKeys();
    RunScheduledTasks();
    RefreshWindowTilemaps();
    UpdateSpriteGroups();
    UpdateSpritesAndBuildOam();
    BuildScreenTransitionFrame();
    BuildScanlineWindowEffects();
    ServiceLinkControlRequests();
    WaitForFrameUpdates(*(u8 *)0x03000075);
    temp_r1 = 0x3FF & *(u16 *)0x0300000C;
    if (temp_r1 == 0x30C) {
        temp_r1 &= *(u16 *)0x0300000E;
        if (temp_r1) {
            BiosSoftReset(0xFF);
        }
    }
    *(s32 *)0x03000078 += 1;
    goto loop_1;
}
