#include "m2c_prelude.h"
void BlankFieldDisplayAtHBlank(void) asm("func_080A0098");

void BlankFieldDisplayAtHBlank(void) {
    register volatile u16 *display_status asm("r2") = (volatile u16 *)0x04000004;
    register volatile u16 *saved_backdrop_color asm("r4");
    u16 hblank_active;
    hblank_active = *display_status & 2;
    saved_backdrop_color = (volatile u16 *)0x02031988;
    while (hblank_active == 0)
        hblank_active = *display_status & 2;
    *(volatile u16 *)0x04000000 = 0;
    *saved_backdrop_color = *(volatile u16 *)0x05000000;
    *(volatile u16 *)0x05000000 = 0;
}
