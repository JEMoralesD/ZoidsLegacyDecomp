#ifndef LINK_TRANSFER_H
#define LINK_TRANSFER_H

enum LinkConnectionStatus {
    LINK_CONNECTION_WAITING = 0,
    LINK_CONNECTION_READY = 1,
    LINK_CONNECTION_TIMED_OUT = 2
};

enum LinkControlFlags {
    LINK_CONTROL_REQUEST_START = 1,
    LINK_CONTROL_ACTIVE = 2,
    LINK_CONTROL_REQUEST_STOP = 4
};

enum LinkTransferLimits {
    LINK_PACKET_PAYLOAD_BYTES = 14,
    LINK_PACKET_BYTES = 16,
    LINK_DISCONNECT_TIMEOUT_UPDATES = 60,
    LINK_CONNECTION_STATUS_MASK = 0x3F0F,
    LINK_CONNECTION_STATUS_READY = 0x303,
    LINK_CONNECTION_PACKET_INDEX_BIT = 0x80,
    LINK_CONNECTION_MONITOR_TASK_SLOT = 9,
    LINK_CONNECTION_MONITOR_TASK_ENTRY = 0x0809AD2D,
    LINK_TIMEOUT_SPRITE_PRIORITY_MASK = 0xC0
};

enum LinkTransferSequence {
    LINK_TRANSFER_IDLE_SEQUENCE = 0,
    LINK_TRANSFER_TAG_SEQUENCE = 1,
    LINK_TRANSFER_FIRST_PAYLOAD_SEQUENCE = 2
};

struct LinkPacket {
    u8 receive_sequence;
    u8 send_sequence;
    u8 payload[LINK_PACKET_PAYLOAD_BYTES];
};

struct LinkTransferStateView {
    u8 incoming_packet_index;
    u8 saved_frame_update_count;
    u8 data02[2];
    u8 *send_tag;
    u8 *receive_tag;
    u8 *send_buffer;
    u8 *receive_buffer;
    s32 send_bytes_remaining;
    s32 receive_bytes_remaining;
    u8 send_sequence;
    u8 receive_sequence;
    u8 disconnect_timeout_counter;
    u8 connection_requested;
};

#define LINK_PACKET_OFFSET(field) \
    ((s32)&((struct LinkPacket *)0)->field)

#define LINK_TRANSFER_OFFSET(field) \
    ((s32)&((struct LinkTransferStateView *)0)->field)

#endif
