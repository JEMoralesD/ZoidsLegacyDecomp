#ifndef SERIAL_LINK_H
#define SERIAL_LINK_H

#include "link_transfer.h"

enum SerialLinkPhase {
    SERIAL_LINK_PHASE_INITIALIZE = 0,
    SERIAL_LINK_PHASE_WAIT_FOR_PACKETS = 1,
    SERIAL_LINK_PHASE_POLL_PACKETS = 2
};

enum SerialLinkMode {
    SERIAL_LINK_MODE_TIMER_DRIVEN = 8
};

enum SerialLinkLimits {
    SERIAL_LINK_SLOT_COUNT = 2,
    SERIAL_LINK_PACKET_HALFWORDS = 10,
    SERIAL_LINK_PACKET_BYTES = 20,
    SERIAL_LINK_PACKET_STRIDE = 24,
    SERIAL_LINK_STARTUP_UPDATES = 8,
    SERIAL_LINK_INTERRUPT_WAIT_UPDATES = 4,
    SERIAL_LINK_INITIAL_HALFWORD_INDEX = 12,
    SERIAL_LINK_VALID_PACKET_SUM = -13,
    SERIAL_LINK_CHECKSUM_BIAS = 12,
    SERIAL_LINK_SYNC_HALFWORD = 0xFEFE,
    SERIAL_LINK_STATE_CLEAR_WORDS = 0x3C,
    SERIAL_LINK_RECEIVE_SWAP_COPY_WORDS = 0x10,
    SERIAL_LINK_INTERRUPT_COPY_WORDS = 0x48
};

enum SerialLinkNativeCode {
    SERIAL_LINK_RECEIVE_SWAP_ROM_ENTRY = 0x080005F8,
    SERIAL_LINK_RECEIVE_SWAP_IWRAM_ENTRY = 0x03003174,
    SERIAL_LINK_INTERRUPT_ROM_THUMB_ENTRY = 0x080933E1,
    SERIAL_LINK_INTERRUPT_IWRAM_ENTRY = 0x030031B4,
    SERIAL_LINK_INTERRUPT_IWRAM_THUMB_ENTRY = 0x030031B5
};

enum SerialLinkStatusFlags {
    SERIAL_LINK_STATUS_TIMER_DRIVEN = 0x80,
    SERIAL_LINK_STATUS_HARDWARE_ERROR = 0x1000,
    SERIAL_LINK_STATUS_UNSUPPORTED_SLOT = 0x2000,
    SERIAL_LINK_STATUS_SEND_PACKET_PENDING = 0x80000000
};

enum SerialLinkHardware {
    SERIAL_LINK_PLAYER_ID_MASK = 0x30,
    SERIAL_LINK_READY_AND_BUSY_MASK = 0x88,
    SERIAL_LINK_READY = 8,
    SERIAL_LINK_SLAVE_SIGNAL = 4,
    SERIAL_LINK_BUSY = 0x80,
    SERIAL_LINK_IRQ_SERIAL = 0x80,
    SERIAL_LINK_IRQ_TIMER3 = 0x40,
    SERIAL_LINK_IRQ_MASK = 0xC0,
    SERIAL_LINK_TIMER_RELOAD = 0xABFB,
    SERIAL_LINK_TIMER_ENABLE_AND_IRQ = 0xC0,
    SERIAL_LINK_CONTROL_MULTIPLAYER = 0x2000,
    SERIAL_LINK_CONTROL_IRQ_AND_BAUD = 0x4003,
    SERIAL_LINK_CONTROL_MULTIPLAYER_AND_BAUD = 0x2003
};

#define SERIAL_LINK_IRQ_CLEAR_MASK 0xFF3F
#define SERIAL_LINK_SERIAL_IRQ_CLEAR_MASK 0xFF7F
#define SERIAL_LINK_STATUS_SEND_PACKET_CLEAR_MASK 0x7FFFFFFF

struct SerialLinkPacket {
    u8 sequence;
    u8 missing_slot_mask;
    u16 checksum;
    struct LinkPacket transfer_packet;
    u16 trailing_halfwords[2];
};

struct SerialLinkDriverState {
    u8 mode;
    u8 phase;
    u8 seen_slot_mask;
    u8 valid_received_slot_mask;
    u8 send_packet_ready;
    u8 receive_packet_ready;
    u8 transfer_requested;
    u8 serial_error;
    u8 startup_update_count;
    u8 interrupt_wait_count;
    u8 data0A;
    u8 packet_sequence;
    u8 data0C[8];
    s32 send_halfword_index;
    s32 receive_halfword_index;
    struct SerialLinkPacket *prepared_send_packet;
    struct SerialLinkPacket *active_send_packet;
    struct SerialLinkPacket *active_receive_packets;
    struct SerialLinkPacket *pending_receive_packets;
    struct SerialLinkPacket *completed_receive_packets;
    struct SerialLinkPacket send_buffers[2];
    struct SerialLinkPacket receive_buffers[3][SERIAL_LINK_SLOT_COUNT];
};

#define SERIAL_LINK_STATE_FIELD(state, type, field) \
    M2C_FIELD(state, type *, (s32)&((struct SerialLinkDriverState *)0)->field)

#define SERIAL_LINK_STATE_OFFSET(field) \
    ((s32)&((struct SerialLinkDriverState *)0)->field)

/* Indexed access preserves the driver base address in native literal pools. */
#define SERIAL_LINK_STATE_BYTE(state, field) \
    ((state)[SERIAL_LINK_STATE_OFFSET(field)])

#define SERIAL_LINK_PACKET_OFFSET(field) \
    ((s32)&((struct SerialLinkPacket *)0)->field)

#define SERIAL_LINK_PACKET_FIELD(packet, type, field) \
    M2C_FIELD(packet, type *, SERIAL_LINK_PACKET_OFFSET(field))

#endif
