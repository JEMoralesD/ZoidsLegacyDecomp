#include "m2c_prelude.h"
#include "link_transfer.h"

u8 GetLinkConnectionStatus(s32) asm("func_0809ACC4");
void ResetLinkTransferState(void) asm("func_0809AC98");

extern u8 gLinkReceiveSequence asm("D_03006059");
extern s32 gLinkReceiveBytesRemaining asm("D_03006054");
extern u8 gLinkIncomingPacketIndex asm("D_0300603C");
extern u8 gLinkIncomingPacket1SendSequence asm("D_030009B9");
extern u8 gLinkIncomingPacket0SendSequence[] asm("D_030009A9");
extern u8 gLinkIncomingPacket1[] asm("D_030009B8");
extern u8 gLinkIncomingPacket0[] asm("D_030009A8");
extern u8 *gLinkReceiveTag asm("D_03006044");
extern u8 *gLinkReceiveBuffer asm("D_0300604C");
extern u8 gLinkOutgoingReceiveSequence[] asm("D_03000998");
extern u16 gLinkControlFlags asm("D_030009EC");

s32 PollLinkReceive(void) asm("func_0809B040");

s32 PollLinkReceive(void) {
    u8 *packet_cursor;
    u8 *tag_cursor;
    s32 next_sequence;
    u8 copied_bytes;
    s32 bytes_remaining;

    if (GetLinkConnectionStatus(0) != LINK_CONNECTION_READY) {
        return 0;
    }
    {
        u8 receive_sequence = gLinkReceiveSequence;
        u8 other_packet_receive_sequence;

        if (receive_sequence == LINK_TRANSFER_IDLE_SEQUENCE) {
            return 1;
        }
        if (receive_sequence > LINK_TRANSFER_TAG_SEQUENCE && gLinkReceiveBytesRemaining <= 0) {
            if (gLinkIncomingPacketIndex != 0) {
                if (gLinkIncomingPacket1SendSequence != 0) {
                    return 0;
                }
                other_packet_receive_sequence = ((u8 *)&gLinkIncomingPacket1SendSequence)[-(LINK_PACKET_BYTES + LINK_PACKET_OFFSET(send_sequence))];
            } else {
                if (gLinkIncomingPacket0SendSequence[0] != 0) {
                    return 0;
                }
                other_packet_receive_sequence = gLinkIncomingPacket0SendSequence[LINK_PACKET_BYTES - LINK_PACKET_OFFSET(send_sequence)];
            }
            if (other_packet_receive_sequence == LINK_TRANSFER_IDLE_SEQUENCE) {
                gLinkReceiveSequence = other_packet_receive_sequence;
                return 1;
            }
            return 0;
        }
    }
    if (gLinkIncomingPacketIndex != 0) {
        packet_cursor = gLinkIncomingPacket1;
        if (packet_cursor[LINK_PACKET_OFFSET(send_sequence)] != gLinkReceiveSequence) {
            return 0;
        }
    } else {
        packet_cursor = gLinkIncomingPacket0;
        if (packet_cursor[LINK_PACKET_OFFSET(send_sequence)] != gLinkReceiveSequence) {
            return 0;
        }
    }
    if (gLinkReceiveSequence == LINK_TRANSFER_TAG_SEQUENCE) {
        tag_cursor = gLinkReceiveTag;
        packet_cursor += LINK_PACKET_OFFSET(payload);
        copied_bytes = 0;
        while (*tag_cursor != 0) {
            if (*packet_cursor != *tag_cursor) {
                ResetLinkTransferState();
                gLinkControlFlags |= LINK_CONTROL_REQUEST_STOP;
                return 0;
            }
            tag_cursor++;
            packet_cursor++;
            copied_bytes++;
            if (copied_bytes >= LINK_PACKET_PAYLOAD_BYTES) {
                break;
            }
        }
    } else {
        packet_cursor += LINK_PACKET_OFFSET(payload);
        copied_bytes = 0;
        if (gLinkReceiveBytesRemaining != 0) {
            do {
                *gLinkReceiveBuffer = *packet_cursor;
                gLinkReceiveBuffer++;
                packet_cursor++;
                bytes_remaining = gLinkReceiveBytesRemaining - 1;
                gLinkReceiveBytesRemaining = bytes_remaining;
                copied_bytes++;
            } while (copied_bytes < LINK_PACKET_PAYLOAD_BYTES && bytes_remaining != 0);
        }
    }
    next_sequence = gLinkReceiveSequence + 1;
    gLinkReceiveSequence = next_sequence;
    bytes_remaining = gLinkReceiveBytesRemaining;
    if (bytes_remaining == 0) {
        gLinkOutgoingReceiveSequence[0] = bytes_remaining;
    } else {
        gLinkOutgoingReceiveSequence[0] = next_sequence;
    }
    return 0;
}
