#include "m2c_prelude.h"
#include "serial_link.h"
extern s8 gSerialLinkDriverBytes[] asm("D_030032D4");
void StopSerialLinkDriver(void) asm("func_080930E8");

void StopSerialLinkDriver(void) {
    *(volatile s16 *)0x04000208 = 0;
    *(u16 *)0x04000200 &= SERIAL_LINK_IRQ_CLEAR_MASK;
    *(volatile s16 *)0x04000208 = 1;
    *(s16 *)0x04000128 = SERIAL_LINK_CONTROL_MULTIPLAYER_AND_BAUD;
    M2C_FIELD((void *)0x0400010C, s32 *, 0) = SERIAL_LINK_TIMER_RELOAD;
    M2C_FIELD((void *)0x0400010C, s16 *, 0xF6) = SERIAL_LINK_IRQ_MASK;
    SERIAL_LINK_STATE_BYTE(gSerialLinkDriverBytes, transfer_requested) = 0;
}
