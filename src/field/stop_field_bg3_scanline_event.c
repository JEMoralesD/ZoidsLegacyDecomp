#include "m2c_prelude.h"
#include "field_display.h"
M2C_UNK ResetScanlineEvents() asm("func_809258C");                                /* extern */

void StopFieldBg3ScanlineEvent(void) asm("func_0809EB24");

void StopFieldBg3ScanlineEvent(void) {
    ResetScanlineEvents();
    *(s8 *)FIELD_BG3_SCANLINE_STATE_RAM = FIELD_BG3_SCANLINE_STOPPING;
}
