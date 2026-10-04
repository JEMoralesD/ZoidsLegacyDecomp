#include "m2c_prelude.h"
#include "serial_link.h"
extern u16 gLinkControlFlags asm("D_030009EC");
extern u8 gLinkIncomingPackets[] asm("D_030009A8");
extern s32 gLinkConnectionStatusBits asm("D_030009E8");
extern u8 gSerialLinkDriverBytes[] asm("D_030032D4");
extern u8 gLinkOutgoingPacket[] asm("D_03000998");

s32 UpdateSerialLinkStatus(void *) asm("func_08093138");
void PrepareSerialLinkPacket(void *) asm("func_0809324C");
void RequestSerialLinkTransfer(void) asm("func_080930D0");

void UpdateLinkDriverPackets(void) asm("func_08092834");

void UpdateLinkDriverPackets(void) {
    register s32 driver_status asm("r1");
    register s32 updated_status asm("r0");
    s32 *connection_status_bits;

    if (LINK_CONTROL_ACTIVE & gLinkControlFlags) {
        driver_status = UpdateSerialLinkStatus(gLinkIncomingPackets);
        connection_status_bits = &gLinkConnectionStatusBits;
        *connection_status_bits = driver_status;
        if (!(SERIAL_LINK_STATUS_UNSUPPORTED_SLOT & driver_status)) {
            if (SERIAL_LINK_STATE_BYTE(gSerialLinkDriverBytes, send_packet_ready) == 0) {
                PrepareSerialLinkPacket(gLinkOutgoingPacket);
                updated_status = *connection_status_bits & SERIAL_LINK_STATUS_SEND_PACKET_CLEAR_MASK;
            } else {
                updated_status = SERIAL_LINK_STATUS_SEND_PACKET_PENDING | driver_status;
            }
            *connection_status_bits = updated_status;
        }
        RequestSerialLinkTransfer();
    }
}
