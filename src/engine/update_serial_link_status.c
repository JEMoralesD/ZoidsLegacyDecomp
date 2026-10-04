#include "m2c_prelude.h"
#include "serial_link.h"

extern u8 gSerialLinkDriverBytes[] asm("D_030032D4");

u8 ConsumeSerialLinkPackets(s32) asm("func_0809329C");

s32 UpdateSerialLinkStatus(s32 received_packets_address) asm("func_08093138");

s32 UpdateSerialLinkStatus(s32 received_packets_address) {
    register s32 saved_received_packets_address asm("r12");
    register volatile u32 *serial_control asm("r6");
    register u32 serial_status asm("r5");
    u8 *driver_bytes;
    u8 *initial_driver_bytes;
    register s32 phase asm("r2");

    saved_received_packets_address = received_packets_address;
    asm volatile("" : "+r"(saved_received_packets_address));
    serial_control = (volatile u32 *)0x04000128;
    serial_status = *serial_control;
    initial_driver_bytes = gSerialLinkDriverBytes;
    phase = SERIAL_LINK_STATE_FIELD(initial_driver_bytes, u8, phase);
    driver_bytes = initial_driver_bytes;
    asm volatile("" : "+r"(serial_control), "+r"(serial_status), "+r"(driver_bytes), "+r"(phase));
    switch (phase) {
    case SERIAL_LINK_PHASE_INITIALIZE: {
        register u32 ready_and_busy_bits asm("r4");
        register u32 player_id_bits asm("r0");
        register u32 player_id_mask asm("r1");

        player_id_mask = SERIAL_LINK_PLAYER_ID_MASK;
        player_id_bits = serial_status;
        asm volatile("" : "+r"(player_id_bits), "+r"(player_id_mask));
        player_id_bits &= player_id_mask;
        if (player_id_bits == 0) {
            {
                register u32 ready_and_busy_mask asm("r0");

                ready_and_busy_mask = SERIAL_LINK_READY_AND_BUSY_MASK;
                ready_and_busy_bits = serial_status;
                asm volatile("" : "+r"(ready_and_busy_bits), "+r"(ready_and_busy_mask));
                ready_and_busy_bits &= ready_and_busy_mask;
            }
            if (ready_and_busy_bits != SERIAL_LINK_READY) {
                break;
            }
            {
                register s32 slave_signal_bits asm("r0");
                register s32 slave_signal_mask asm("r1");
                u8 slave_signal;

                slave_signal_mask = SERIAL_LINK_SLAVE_SIGNAL;
                slave_signal_bits = serial_status;
                asm volatile("" : "+r"(slave_signal_bits), "+r"(slave_signal_mask));
                slave_signal_bits &= slave_signal_mask;
                slave_signal = slave_signal_bits;
                if (slave_signal == 0 && SERIAL_LINK_STATE_FIELD(driver_bytes, s32, send_halfword_index) == SERIAL_LINK_INITIAL_HALFWORD_INDEX) {
                    volatile u16 *interrupt_master_enable;
                    volatile u16 *interrupt_enable;
                    volatile u8 *serial_control_bytes;
                    volatile u32 *timer3;

                    interrupt_master_enable = (volatile u16 *)0x04000208;
                    *interrupt_master_enable = slave_signal;
                    interrupt_enable = (volatile u16 *)0x04000200;
                    *interrupt_enable &= SERIAL_LINK_SERIAL_IRQ_CLEAR_MASK;
                    *interrupt_enable |= SERIAL_LINK_IRQ_TIMER3;
                    *interrupt_master_enable = 1;
                    serial_control_bytes = (volatile u8 *)serial_control;
                    {
                        s32 serial_irq_clear_mask;
                        u8 serial_control_high;

                        serial_control_high = serial_control_bytes[1];
                        serial_irq_clear_mask = 0x41;
                        serial_irq_clear_mask = -serial_irq_clear_mask;
                        serial_control_high &= serial_irq_clear_mask;
                        serial_control_bytes[1] = serial_control_high;
                    }
                    {
                        register volatile u32 *timer3_registers asm("r1");
                        register s32 timer_reload asm("r0");

                        timer3_registers = (volatile u32 *)0x0400010C;
                        timer_reload = SERIAL_LINK_TIMER_RELOAD;
                        asm volatile("" : "+r"(timer3_registers), "+r"(timer_reload));
                        *timer3_registers = timer_reload;
                        timer3 = timer3_registers;
                    }
                    *(volatile u16 *)((u8 *)timer3 + 0xF6) = SERIAL_LINK_IRQ_MASK;
                    SERIAL_LINK_STATE_FIELD(driver_bytes, u8, mode) = ready_and_busy_bits;
                }
            }
        }
        SERIAL_LINK_STATE_FIELD(driver_bytes, u8, phase) = SERIAL_LINK_PHASE_WAIT_FOR_PACKETS;
    }
        /* fallthrough */
    case SERIAL_LINK_PHASE_WAIT_FOR_PACKETS: {
        u8 *startup_driver_bytes;

        startup_driver_bytes = gSerialLinkDriverBytes;
        if (SERIAL_LINK_STATE_FIELD(startup_driver_bytes, u8, seen_slot_mask) != 0) {
            u8 startup_updates;

            startup_updates = SERIAL_LINK_STATE_FIELD(startup_driver_bytes, u8, startup_update_count);
            if ((u32)startup_updates < SERIAL_LINK_STARTUP_UPDATES) {
                SERIAL_LINK_STATE_FIELD(startup_driver_bytes, u8, startup_update_count) = startup_updates + 1;
            } else {
                SERIAL_LINK_STATE_FIELD(startup_driver_bytes, u8, phase) = SERIAL_LINK_PHASE_POLL_PACKETS;
            }
        }
    }
        /* fallthrough */
    case SERIAL_LINK_PHASE_POLL_PACKETS:
        ConsumeSerialLinkPackets(saved_received_packets_address);
        break;
    }

    {
        u8 *status_driver_bytes;
        register s32 valid_received_slot_mask asm("r3");
        register s32 driver_status asm("r2");
        register s32 seen_slot_mask asm("r0");
        register s32 returned_status asm("r0");
        s32 startup_status_bits;
        s32 combined_status;
        s32 mode;

        status_driver_bytes = gSerialLinkDriverBytes;
        asm volatile("" : "+r"(status_driver_bytes));
        SERIAL_LINK_STATE_FIELD(status_driver_bytes, u8, packet_sequence)++;
        valid_received_slot_mask = SERIAL_LINK_STATE_FIELD(status_driver_bytes, u8, valid_received_slot_mask);
        seen_slot_mask = SERIAL_LINK_STATE_FIELD(status_driver_bytes, u8, seen_slot_mask);
        driver_status = seen_slot_mask << 8;
        mode = SERIAL_LINK_STATE_FIELD(status_driver_bytes, u8, mode);
        driver_bytes = status_driver_bytes;
        asm volatile("" : "+r"(valid_received_slot_mask), "+r"(driver_status), "+r"(driver_bytes));
        if (mode == SERIAL_LINK_MODE_TIMER_DRIVEN) {
            combined_status = SERIAL_LINK_STATUS_TIMER_DRIVEN;
            combined_status |= driver_status;
            combined_status |= valid_received_slot_mask;
        } else {
            combined_status = valid_received_slot_mask;
            combined_status |= driver_status;
        }
        driver_status = combined_status;
        if (SERIAL_LINK_STATE_FIELD(driver_bytes, u8, serial_error) != 0) {
            driver_status |= SERIAL_LINK_STATUS_HARDWARE_ERROR;
        }
        startup_status_bits = (SERIAL_LINK_STATE_FIELD(driver_bytes, u8, startup_update_count) >> 3) << 15;
        asm volatile("" : "+r"(startup_status_bits), "+r"(driver_status));
        {
            u32 player_id;

            player_id = (serial_status << 26) >> 30;
            asm volatile("" : : "r"(serial_status));
            if (player_id <= 1) {
                goto supported_player_id;
            }
        }
        returned_status = SERIAL_LINK_STATUS_TIMER_DRIVEN;
        returned_status <<= 6;
        returned_status |= startup_status_bits;
        returned_status |= driver_status;
        goto return_driver_status;

supported_player_id:
        returned_status = driver_status;
        returned_status |= startup_status_bits;
return_driver_status:
        return returned_status;
    }
}
