#include "m2c_prelude.h"
#include "serial_link.h"
extern u8 gSerialLinkDriverBytes asm("D_030032D4");
void BiosCpuSet(int, void *, int) asm("func_80ECD2C");

void PrepareSerialLinkPacket(int transfer_packet_address) asm("func_0809324C");

void PrepareSerialLinkPacket(int transfer_packet_address) {
    s8 *driver_bytes;
    s32 checksum_sum;
    u16 *halfword_cursor;
    u32 halfword_index;

    checksum_sum = 0;
    driver_bytes = (s8 *)&gSerialLinkDriverBytes;
    SERIAL_LINK_PACKET_FIELD(SERIAL_LINK_STATE_FIELD(driver_bytes, u8 * volatile, prepared_send_packet), u8, sequence) = (u8) SERIAL_LINK_STATE_FIELD(driver_bytes, u8, packet_sequence);
    SERIAL_LINK_PACKET_FIELD(SERIAL_LINK_STATE_FIELD(driver_bytes, u8 * volatile, prepared_send_packet), s8, missing_slot_mask) = (s8) (SERIAL_LINK_STATE_FIELD(driver_bytes, u8, seen_slot_mask) ^ SERIAL_LINK_STATE_FIELD(driver_bytes, u8, valid_received_slot_mask));
    SERIAL_LINK_PACKET_FIELD(SERIAL_LINK_STATE_FIELD(driver_bytes, u8 * volatile, prepared_send_packet), s16, checksum) = checksum_sum;
    BiosCpuSet(transfer_packet_address, SERIAL_LINK_STATE_FIELD(driver_bytes, u8 * volatile, prepared_send_packet) + SERIAL_LINK_PACKET_OFFSET(transfer_packet), 0x04000004);
    halfword_index = 0;
    halfword_cursor = (u16 *) SERIAL_LINK_STATE_FIELD(driver_bytes, u8 * volatile, prepared_send_packet);
    do {
        checksum_sum += *halfword_cursor;
        halfword_cursor += 1;
        halfword_index += 1;
    } while (halfword_index < SERIAL_LINK_PACKET_HALFWORDS);
    SERIAL_LINK_PACKET_FIELD(SERIAL_LINK_STATE_FIELD(driver_bytes, u8 * volatile, prepared_send_packet), s16, checksum) = (s16) (~checksum_sum - SERIAL_LINK_CHECKSUM_BIAS);
    SERIAL_LINK_STATE_FIELD(driver_bytes, s8, send_packet_ready) = 1;
}
