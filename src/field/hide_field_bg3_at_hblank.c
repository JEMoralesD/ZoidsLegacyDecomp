#include "m2c_prelude.h"
#include "field_display.h"
extern u8 gFieldBg3ScanlineHideRequested asm("D_020324B9");

void HideFieldBg3AtHBlank(void) asm("func_0809EA8C");

void HideFieldBg3AtHBlank(void) {
    register volatile u16 *bg3_control asm("r1");
    if (gFieldBg3ScanlineHideRequested != 0) {
        while ((*(volatile u16*)0x04000004 & 2) == 0)
            ;
        bg3_control = (volatile u16*)0x0400000E;
        *bg3_control = 0x300;
        bg3_control += 7;
        *(volatile u32*)bg3_control = 0;
        *(volatile u16*)0x04000050 = *(volatile u16*)0x04000050 & 0xf7f7;
    }
}
