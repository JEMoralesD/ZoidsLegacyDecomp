#include "m2c_prelude.h"
void InitScanlineEvents(void) asm("func_809256C");

void ArmScanlineEvents(void) {
    u8 *first_event;
    first_event = *(u8 **)0x03000880;
    if (first_event != (u8 *)-1) {
        *(u8 **)0x03000884 = first_event;
        if (*(u8 *)0x03000888 != 0) {
            *(volatile u16 *)0x04000200 |= 4;
            *(volatile u16 *)0x04000004 |= 0x20;
            *(u8 *)0x03000888 = 0;
        }
        *(volatile u16 *)0x04000004 = (**(u8 **)0x03000884 << 8) | (0xFF & *(volatile u16 *)0x04000004);
        return;
    }
    if (*(u8 *)0x03000888 != 0) {
        *(volatile u16 *)0x04000200 = 0xFFFB & *(volatile u16 *)0x04000200;
        *(volatile u16 *)0x04000004 = 0xFFDF & *(volatile u16 *)0x04000004;
        InitScanlineEvents();
    }
}
