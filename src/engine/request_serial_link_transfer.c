#include "m2c_prelude.h"
#include "serial_link.h"
extern u8 gSerialLinkDriverBytes[] asm("D_030032D4");
void RequestSerialLinkTransfer(void) asm("func_080930D0");

void RequestSerialLinkTransfer(void) {
    if ((SERIAL_LINK_STATE_FIELD(gSerialLinkDriverBytes, u8, mode)) != 0) {
        (SERIAL_LINK_STATE_FIELD(gSerialLinkDriverBytes, s8, transfer_requested)) = 1;
    }
}
