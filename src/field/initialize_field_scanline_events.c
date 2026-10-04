#include "m2c_prelude.h"
M2C_UNK InitScanlineEvents() asm("func_809256C");

void InitializeFieldScanlineEvents(void) asm("func_0809EB38");

void InitializeFieldScanlineEvents(void) {
    *(u16 *)0x04000200 &= 0xFFFB;
    *(u16 *)0x04000004 &= 0xFFDF;
    InitScanlineEvents();
}
