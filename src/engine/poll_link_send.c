#include "m2c_prelude.h"
#include "link_transfer.h"

u8 GetLinkConnectionStatus(s32) asm("func_0809ACC4");

extern u8 gLinkSendSequence asm("D_03006058");
extern s32 gLinkSendBytesRemaining asm("D_03006050");
extern u8 gLinkIncomingPacket0[] asm("D_030009A8");
extern u8 gLinkIncomingPacketIndex asm("D_0300603C");
extern u8 gLinkIncomingPacket1 asm("D_030009B8");
extern u8 gLinkOutgoingSendSequence asm("D_03000999");
extern u8 gLinkOutgoingPayload[] asm("D_0300099A");
extern u8 *gLinkSendTag asm("D_03006040");
extern u8 *gLinkSendBuffer asm("D_03006048");

u8 PollLinkSend(void) asm("func_0809AEF4");

u8 PollLinkSend(void) {
    u8 *send_sequence;
    u8 *send_sequence_load;
    u8 sequence_or_ack;
    u8 *tag_cursor;
    u8 *payload_cursor;
    u8 *outgoing_send_sequence;
    u8 *other_packet_send_sequence;
    u8 copied_bytes;

    if (GetLinkConnectionStatus(0) != LINK_CONNECTION_READY) goto waiting;

    send_sequence_load = &gLinkSendSequence;
    sequence_or_ack = *send_sequence_load;
    send_sequence = send_sequence_load;
    asm volatile("" : "+&r"(send_sequence) : "r"(send_sequence_load));
    if (sequence_or_ack == LINK_TRANSFER_IDLE_SEQUENCE) {
        return 1;
    }

    if (sequence_or_ack > LINK_TRANSFER_TAG_SEQUENCE && gLinkSendBytesRemaining <= 0) {
        if (gLinkIncomingPacketIndex != 0) {
            sequence_or_ack = gLinkIncomingPacket1;
            if (sequence_or_ack != 0) goto waiting;
            other_packet_send_sequence = &gLinkIncomingPacket1;
            other_packet_send_sequence -= LINK_PACKET_BYTES - LINK_PACKET_OFFSET(send_sequence);
            if (*other_packet_send_sequence == 0) {
                goto finish_send;
            }
            goto clear_outgoing_sequence;
        }
        sequence_or_ack = gLinkIncomingPacket0[0];
        if (sequence_or_ack != 0) goto waiting;
        if (gLinkIncomingPacket0[LINK_PACKET_BYTES + LINK_PACKET_OFFSET(send_sequence)] != 0) goto clear_outgoing_sequence;
finish_send:
        *send_sequence = sequence_or_ack;
        return 1;
clear_outgoing_sequence:
        gLinkOutgoingSendSequence = sequence_or_ack;
        goto waiting;
    }

    if (gLinkIncomingPacketIndex != 0) {
        {
            register u8 ack_sequence asm("r0");
            ack_sequence = gLinkIncomingPacket1;
            if (ack_sequence == *send_sequence) goto copy_next_packet;
        }
        goto waiting;
    }
    {
        register u8 ack_sequence asm("r0");
        ack_sequence = gLinkIncomingPacket0[0];
        if (ack_sequence != *send_sequence) goto waiting;
    }

copy_next_packet:
    {
        register u8 sequence asm("r0");
        sequence = *send_sequence;
        if (sequence == LINK_TRANSFER_TAG_SEQUENCE) goto copy_tag_packet;
    }
    goto copy_payload_packet;
copy_tag_packet:
    {
        tag_cursor = gLinkSendTag;
        payload_cursor = gLinkOutgoingPayload;
        copied_bytes = 0;
        outgoing_send_sequence = payload_cursor - 1;
        do {
            *payload_cursor = *tag_cursor;
            tag_cursor++;
            payload_cursor++;
            copied_bytes++;
        } while (copied_bytes < LINK_PACKET_PAYLOAD_BYTES);
    }
    goto advance_send_sequence;
copy_payload_packet:
    {
        payload_cursor = gLinkOutgoingPayload;
        copied_bytes = 0;
        outgoing_send_sequence = payload_cursor - 1;
        do {
            /* The native sender copies a full packet, even when fewer bytes remain. */
            *payload_cursor = *gLinkSendBuffer;
            gLinkSendBuffer += 1;
            payload_cursor++;
            gLinkSendBytesRemaining--;
            copied_bytes++;
        } while (copied_bytes < LINK_PACKET_PAYLOAD_BYTES);
    }
advance_send_sequence:
    {
        register u8 sequence asm("r0");
        sequence = *send_sequence;
        *outgoing_send_sequence = sequence;
        sequence++;
        *send_sequence = sequence;
    }
waiting:
    return 0;
}
