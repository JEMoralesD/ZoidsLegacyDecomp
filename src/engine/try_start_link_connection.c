#include "m2c_prelude.h"
#include "link_transfer.h"
extern u16 gLinkControlFlags asm("D_030009EC");
extern u8 gLinkConnectionRequested asm("D_0300605B");
extern s32 gLinkConnectionStatusBits asm("D_030009E8");
extern u8 gLinkIncomingPacketIndex asm("D_0300603C");
extern s32 StartTask(s32, s32) asm("func_08092D8C");
extern u8 GetLinkConnectionStatus(s32) asm("func_0809ACC4");

s32 TryStartLinkConnection(void) asm("func_0809AE38");

s32 TryStartLinkConnection(void) {
    u8 connection_status;
    s32 packet_index_bit;

    if (*(u8 *)&gLinkConnectionRequested == 0) {
        *(u16 *)&gLinkControlFlags |= LINK_CONTROL_REQUEST_START;
        *(u8 *)&gLinkConnectionRequested = 1;
    }
    connection_status = GetLinkConnectionStatus(0);
    if (connection_status == LINK_CONNECTION_READY) {
        packet_index_bit = *(s32 *)&gLinkConnectionStatusBits & LINK_CONNECTION_PACKET_INDEX_BIT;
        if (packet_index_bit != 0) {
            *(u8 *)&gLinkIncomingPacketIndex = connection_status;
        } else {
            *(u8 *)&gLinkIncomingPacketIndex = packet_index_bit;
        }
        StartTask(LINK_CONNECTION_MONITOR_TASK_SLOT, LINK_CONNECTION_MONITOR_TASK_ENTRY);
        return 1;
    }
    return 0;
}
