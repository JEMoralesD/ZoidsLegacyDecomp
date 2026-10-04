#include "m2c_prelude.h"
#include "serial_link.h"
extern u16 gLinkControlFlags asm("D_030009EC");
extern s32 gLinkConnectionStatusBits asm("D_030009E8");
extern s32 gInterruptCallbacks[] asm("D_03000014");
extern volatile s32 gDma3Registers[] asm("D_040000D4");
extern void InitializeSerialLinkDriver(s32) asm("func_08093008");
extern void StopSerialLinkDriver(void) asm("func_080930E8");
void ServiceLinkControlRequests(void) asm("func_0809279C");

void ServiceLinkControlRequests(void) {
    u16 zero_halfword;
    u16 control_flags = gLinkControlFlags;
    s32 start_request = LINK_CONTROL_REQUEST_START & control_flags;
    if (start_request != 0) {
        gInterruptCallbacks[3] = SERIAL_LINK_INTERRUPT_IWRAM_THUMB_ENTRY;
        gLinkConnectionStatusBits = 0;
        zero_halfword = 0;
        gDma3Registers[0] = (s32)&zero_halfword;
        gDma3Registers[1] = 0x03000998;
        gDma3Registers[2] = 0x81000008;
        gDma3Registers[2];
        zero_halfword = 0;
        gDma3Registers[0] = (s32)&zero_halfword;
        gDma3Registers[1] = 0x030009A8;
        gDma3Registers[2] = 0x81000020;
        InitializeSerialLinkDriver(gDma3Registers[2]);
        gLinkControlFlags = (~LINK_CONTROL_REQUEST_START & gLinkControlFlags) | LINK_CONTROL_ACTIVE;
        return;
    }
    {
        s32 stop_request = LINK_CONTROL_REQUEST_STOP;
        stop_request &= control_flags;
        if (stop_request != 0) {
            s32 *connection_status_bits;
            StopSerialLinkDriver();
            connection_status_bits = &gLinkConnectionStatusBits;
            gLinkControlFlags = start_request;
            *connection_status_bits = start_request;
        }
    }
}
