#include "m2c_prelude.h"
#include "serial_link.h"

extern u8 gSerialLinkDriverBytes[] asm("D_030032D4");

void StartSerialLinkTransfer(void) asm("func_0809334C");

void StartSerialLinkTransfer(void) {
    u8 *driver_bytes;
    register s32 mode asm("r1");

    driver_bytes = gSerialLinkDriverBytes;
    mode = SERIAL_LINK_STATE_FIELD(driver_bytes, u8, mode);
    if (mode != 0) {
        if (SERIAL_LINK_STATE_FIELD(driver_bytes, u8, send_packet_ready) != 0 && SERIAL_LINK_STATE_FIELD(driver_bytes, u8, phase) != 0 && SERIAL_LINK_STATE_FIELD(driver_bytes, u8, transfer_requested) != 0) {
            s32 swapped_packet_address;
            volatile u16 *serial_control;
            volatile u16 *timer3_control;
            u32 serial_status;
            s32 zero;

            SERIAL_LINK_STATE_FIELD(driver_bytes, s32, receive_halfword_index) = -1;
            swapped_packet_address = SERIAL_LINK_STATE_FIELD(driver_bytes, s32, pending_receive_packets);
            SERIAL_LINK_STATE_FIELD(driver_bytes, s32, pending_receive_packets) = SERIAL_LINK_STATE_FIELD(driver_bytes, s32, active_receive_packets);
            SERIAL_LINK_STATE_FIELD(driver_bytes, s32, active_receive_packets) = swapped_packet_address;
            swapped_packet_address = SERIAL_LINK_STATE_FIELD(driver_bytes, s32, active_send_packet);
            SERIAL_LINK_STATE_FIELD(driver_bytes, s32, active_send_packet) = SERIAL_LINK_STATE_FIELD(driver_bytes, s32, prepared_send_packet);
            SERIAL_LINK_STATE_FIELD(driver_bytes, s32, prepared_send_packet) = swapped_packet_address;
            zero = 0;
            SERIAL_LINK_STATE_FIELD(driver_bytes, u8, send_packet_ready) = zero;
            SERIAL_LINK_STATE_FIELD(driver_bytes, s32, send_halfword_index) = zero;

            serial_control = (volatile u16 *)0x04000128;
            serial_status = *(volatile u32 *)serial_control;
            SERIAL_LINK_STATE_FIELD(driver_bytes, u8, serial_error) = (serial_status << 25) >> 31;
            {
                register s32 sync_halfword asm("r0");

                sync_halfword = SERIAL_LINK_SYNC_HALFWORD;
                asm volatile("" : "+r"(sync_halfword));
                serial_control[1] = sync_halfword;
            }
            serial_control[0] |= SERIAL_LINK_BUSY;
            timer3_control = (volatile u16 *)0x0400010E;
            *timer3_control = SERIAL_LINK_TIMER_ENABLE_AND_IRQ;
        }
    } else {
        u8 interrupt_wait_updates;

        interrupt_wait_updates = SERIAL_LINK_STATE_FIELD(driver_bytes, u8, interrupt_wait_count);
        if ((u32)interrupt_wait_updates < SERIAL_LINK_INTERRUPT_WAIT_UPDATES) {
            SERIAL_LINK_STATE_FIELD(driver_bytes, u8, interrupt_wait_count) = interrupt_wait_updates + 1;
        } else {
            volatile u16 *interrupt_master_enable;
            volatile u16 *bios_interrupt_flags;

            interrupt_master_enable = (volatile u16 *)0x04000208;
            *interrupt_master_enable = mode;
            bios_interrupt_flags = (volatile u16 *)0x03007FF8;
            *bios_interrupt_flags |= SERIAL_LINK_IRQ_SERIAL;
            *interrupt_master_enable = 1;
        }
    }
}
