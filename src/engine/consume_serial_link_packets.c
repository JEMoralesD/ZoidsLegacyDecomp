#include "m2c_prelude.h"
#include "serial_link.h"

s32 CallFunctionR0(s32) asm("func_080ECD5C");
void BiosCpuSet(void *, void *, s32) asm("func_080ECD2C");

u8 ConsumeSerialLinkPackets(s32 received_packets_address) asm("func_0809329C");

u8 ConsumeSerialLinkPackets(s32 received_packets_address) {
    register s32 saved_received_packets_address asm("r9");
    register s32 *zero_fill_word asm("r8");
    register s32 valid_packet_sum asm("r10");
    volatile s32 zero_fill_scratch[2];
    u8 *initial_driver_bytes;
    u8 *final_driver_bytes;
    s32 receive_ready_bits;

    saved_received_packets_address = received_packets_address;
    asm volatile("" : "+r"(saved_received_packets_address));
    {
        s32 receive_swap_entry;

        receive_swap_entry = SERIAL_LINK_RECEIVE_SWAP_IWRAM_ENTRY;
        asm volatile("" : "+r"(receive_swap_entry));
        zero_fill_scratch[0] = 0;
        /* The copied ARM helper swaps receive buffers even when no packet is ready. */
        receive_ready_bits = CallFunctionR0(receive_swap_entry) << 24;
    }
    initial_driver_bytes = (u8 *)0x030032D4;
    SERIAL_LINK_STATE_FIELD(initial_driver_bytes, u8, valid_received_slot_mask) = 0;
    if (receive_ready_bits != 0) {
        u8 *driver_bytes;
        s32 *zero_fill_address;
        s32 checksum_target;
        s32 slot_index;

        slot_index = 0;
        zero_fill_address = (s32 *)&zero_fill_scratch[1];
        asm volatile("" : "+r"(zero_fill_address));
        zero_fill_word = zero_fill_address;
        asm volatile("" : "+r"(zero_fill_word));
        driver_bytes = initial_driver_bytes;
        checksum_target = SERIAL_LINK_VALID_PACKET_SUM;
        asm volatile("" : "+r"(checksum_target));
        valid_packet_sum = checksum_target;
        asm volatile("" : "+r"(valid_packet_sum));
        do {
            u8 *received_packet;
            u16 *halfword_cursor;
            s32 checksum_sum;
            s32 halfword_count;
            s32 next_slot_index;
            s32 checksum_low16;
            u8 *payload_source;

            received_packet = SERIAL_LINK_STATE_FIELD(driver_bytes, u8 *, completed_receive_packets) + slot_index * sizeof(struct SerialLinkPacket);
            checksum_sum = 0;
            halfword_count = 0;
            next_slot_index = slot_index + 1;
            halfword_cursor = (u16 *)received_packet;
            do {
                checksum_sum += *halfword_cursor;
                halfword_cursor++;
                halfword_count++;
            } while ((u32)halfword_count < SERIAL_LINK_PACKET_HALFWORDS);
            checksum_low16 = (s16)checksum_sum;
            asm volatile("" : "+r"(checksum_low16));
            payload_source = received_packet + SERIAL_LINK_PACKET_OFFSET(transfer_packet);
            if (checksum_low16 == valid_packet_sum) {
                register s32 payload_destination asm("r1");

                payload_destination = slot_index << 4;
                asm volatile("" : "+r"(payload_destination));
                payload_destination += saved_received_packets_address;
                BiosCpuSet(payload_source, (void *)payload_destination, 0x04000004);
                SERIAL_LINK_STATE_FIELD(driver_bytes, u8, valid_received_slot_mask) |= 1 << slot_index;
            }
            zero_fill_scratch[1] = 0;
            BiosCpuSet((void *)zero_fill_word, payload_source, 0x05000004);
            slot_index = next_slot_index;
        } while (slot_index < SERIAL_LINK_SLOT_COUNT);
    }
    final_driver_bytes = (u8 *)0x030032D4;
    SERIAL_LINK_STATE_FIELD(final_driver_bytes, u8, seen_slot_mask) |= SERIAL_LINK_STATE_FIELD(final_driver_bytes, u8, valid_received_slot_mask);
    return SERIAL_LINK_STATE_FIELD(final_driver_bytes, u8, valid_received_slot_mask);
}
