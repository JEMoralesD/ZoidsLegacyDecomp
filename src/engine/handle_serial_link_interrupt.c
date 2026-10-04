#include "m2c_prelude.h"
#include "serial_link.h"

extern u8 gSerialLinkDriverBytes[] asm("D_030032D4");

void HandleSerialLinkInterrupt(void) asm("func_080933E0");

void HandleSerialLinkInterrupt(void) {
    u32 received_serial_words[2];
    volatile u32 *serial_receive_registers;
    u32 slots01_word;
    u32 slots23_word;
    u8 *initial_driver_bytes;
    u8 *driver_bytes;
    u32 serial_status;
    u32 serial_error;
    s32 zero;
    s32 sync_halfword;
    u16 slot0_halfword;

    serial_receive_registers = (volatile u32 *)0x04000120;
    slots23_word = serial_receive_registers[1];
    slots01_word = serial_receive_registers[0];
    received_serial_words[0] = slots01_word;
    received_serial_words[1] = slots23_word;

    initial_driver_bytes = gSerialLinkDriverBytes;
    serial_status = *(volatile u32 *)0x04000128;
    serial_error = (serial_status << 25) >> 31;
    asm volatile("" : "+r"(serial_error));
    zero = 0;
    SERIAL_LINK_STATE_FIELD(initial_driver_bytes, u8, serial_error) = serial_error;
    slot0_halfword = *(u16 *)received_serial_words;
    sync_halfword = SERIAL_LINK_SYNC_HALFWORD;
    driver_bytes = initial_driver_bytes;
    if (slot0_halfword == sync_halfword && SERIAL_LINK_STATE_FIELD(driver_bytes, volatile s32, receive_halfword_index) >= SERIAL_LINK_PACKET_HALFWORDS) {
        s32 swapped_packet_address;
        volatile u16 *interrupt_master_enable;
        volatile u16 *bios_interrupt_flags;

        SERIAL_LINK_STATE_FIELD(driver_bytes, volatile s32, receive_halfword_index) = -1;
        swapped_packet_address = SERIAL_LINK_STATE_FIELD(driver_bytes, s32, pending_receive_packets);
        SERIAL_LINK_STATE_FIELD(driver_bytes, s32, pending_receive_packets) = SERIAL_LINK_STATE_FIELD(driver_bytes, s32, active_receive_packets);
        SERIAL_LINK_STATE_FIELD(driver_bytes, s32, active_receive_packets) = swapped_packet_address;
        if (SERIAL_LINK_STATE_FIELD(driver_bytes, u8, send_packet_ready) != 0) {
            swapped_packet_address = SERIAL_LINK_STATE_FIELD(driver_bytes, s32, active_send_packet);
            SERIAL_LINK_STATE_FIELD(driver_bytes, s32, active_send_packet) = SERIAL_LINK_STATE_FIELD(driver_bytes, s32, prepared_send_packet);
            SERIAL_LINK_STATE_FIELD(driver_bytes, s32, prepared_send_packet) = swapped_packet_address;
            SERIAL_LINK_STATE_FIELD(driver_bytes, u8, send_packet_ready) = zero;
            SERIAL_LINK_STATE_FIELD(driver_bytes, volatile s32, send_halfword_index) = zero;
        }
        interrupt_master_enable = (volatile u16 *)0x04000208;
        *interrupt_master_enable = zero;
        bios_interrupt_flags = (volatile u16 *)0x03007FF8;
        *bios_interrupt_flags |= SERIAL_LINK_IRQ_SERIAL;
        *interrupt_master_enable = 1;
    }

    {
        register s32 send_halfword_index asm("r0");
        register u8 *active_send_packet_bytes asm("r1");
        u16 outgoing_halfword;
        register volatile u16 *serial_control asm("r2");

        send_halfword_index = SERIAL_LINK_STATE_FIELD(driver_bytes, volatile s32, send_halfword_index);
        if (send_halfword_index < SERIAL_LINK_PACKET_HALFWORDS) {
            serial_control = (volatile u16 *)0x04000128;
            asm volatile("" : "+r"(serial_control));
            active_send_packet_bytes = SERIAL_LINK_STATE_FIELD(driver_bytes, u8 * volatile, active_send_packet);
            asm volatile("" : "+r"(send_halfword_index), "+r"(active_send_packet_bytes));
            outgoing_halfword = *(u16 *)(active_send_packet_bytes + send_halfword_index * 2);
            serial_control[1] = outgoing_halfword;
        }
    }
    {
        s32 send_halfword_index;

        send_halfword_index = SERIAL_LINK_STATE_FIELD(driver_bytes, volatile s32, send_halfword_index);
        if (send_halfword_index <= SERIAL_LINK_PACKET_HALFWORDS) {
            SERIAL_LINK_STATE_FIELD(driver_bytes, volatile s32, send_halfword_index) = send_halfword_index + 1;
        }
    }
    asm volatile("" : : : "memory");

    {
        register s32 receive_halfword_index asm("r0");
        register u16 *incoming_halfword_destination asm("r1");
        register u16 *incoming_halfword_source asm("r2");
        s32 slot_countdown;

        receive_halfword_index = SERIAL_LINK_STATE_FIELD(driver_bytes, volatile s32, receive_halfword_index);
        if (receive_halfword_index >= 0) {
            incoming_halfword_destination = (u16 *)SERIAL_LINK_STATE_FIELD(driver_bytes, u8 * volatile, active_receive_packets);
            incoming_halfword_source = (u16 *)received_serial_words;
            asm volatile("" : "+r"(receive_halfword_index), "+r"(incoming_halfword_destination), "+r"(incoming_halfword_source));
            incoming_halfword_destination = (u16 *)((u8 *)incoming_halfword_destination + receive_halfword_index * 2);
            slot_countdown = SERIAL_LINK_SLOT_COUNT - 1;
            do {
                *incoming_halfword_destination = *incoming_halfword_source;
                incoming_halfword_source++;
                incoming_halfword_destination = (u16 *)((u8 *)incoming_halfword_destination + sizeof(struct SerialLinkPacket));
                slot_countdown--;
            } while (slot_countdown >= 0);
            if (SERIAL_LINK_STATE_FIELD(driver_bytes, volatile s32, receive_halfword_index) == SERIAL_LINK_PACKET_HALFWORDS - 1) {
                SERIAL_LINK_STATE_FIELD(driver_bytes, u8, receive_packet_ready) = 1;
            }
        }
    }
    {
        s32 receive_halfword_index;

        receive_halfword_index = SERIAL_LINK_STATE_FIELD(driver_bytes, volatile s32, receive_halfword_index);
        if (receive_halfword_index <= SERIAL_LINK_PACKET_HALFWORDS) {
            SERIAL_LINK_STATE_FIELD(driver_bytes, volatile s32, receive_halfword_index) = receive_halfword_index + 1;
        }
    }
    asm volatile("" : : : "memory");

    {
        u8 mode;

        mode = SERIAL_LINK_STATE_FIELD(driver_bytes, u8, mode);
        if (mode != 0) {
            *(volatile u16 *)0x0400010E = 0;
        }
        if (SERIAL_LINK_STATE_FIELD(driver_bytes, volatile s32, send_halfword_index) <= SERIAL_LINK_PACKET_HALFWORDS && mode != 0) {
            volatile u16 *serial_control;

            serial_control = (volatile u16 *)0x04000128;
            serial_control[0] |= SERIAL_LINK_BUSY;
            *(volatile u16 *)0x0400010E = SERIAL_LINK_TIMER_ENABLE_AND_IRQ;
        }
    }
    SERIAL_LINK_STATE_FIELD(driver_bytes, u8, interrupt_wait_count) = 0;
}
