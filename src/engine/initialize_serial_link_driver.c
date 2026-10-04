#include "m2c_prelude.h"
#include "serial_link.h"
extern volatile s16 gInterruptMasterEnable asm("D_04000208");
extern volatile u16 gInterruptEnable asm("D_04000200");
extern volatile s16 gSerialModeControl asm("D_04000134");
extern volatile u16 gSerialControl asm("D_04000128");
extern s32 BiosCpuSet() asm("func_080ECD2C");

void InitializeSerialLinkDriver(void) asm("func_08093008");

void InitializeSerialLinkDriver(void) {
    s32 zero_word;
    struct SerialLinkDriverState *driver;
    char *buffer_cursor;
    gInterruptMasterEnable = 0;
    gInterruptEnable &= SERIAL_LINK_IRQ_CLEAR_MASK;
    gInterruptMasterEnable = 1;
    gSerialModeControl = 0;
    gSerialControl = SERIAL_LINK_CONTROL_MULTIPLAYER;
    gSerialControl |= SERIAL_LINK_CONTROL_IRQ_AND_BAUD;
    zero_word = 0;
    driver = (struct SerialLinkDriverState *)0x030032D4;
    BiosCpuSet(&zero_word, driver, 0x05000000 | SERIAL_LINK_STATE_CLEAR_WORDS);
    BiosCpuSet((void *)SERIAL_LINK_RECEIVE_SWAP_ROM_ENTRY, (void *)SERIAL_LINK_RECEIVE_SWAP_IWRAM_ENTRY, 0x04000000 | SERIAL_LINK_RECEIVE_SWAP_COPY_WORDS);
    BiosCpuSet((void *)SERIAL_LINK_INTERRUPT_ROM_THUMB_ENTRY, (void *)SERIAL_LINK_INTERRUPT_IWRAM_ENTRY, 0x04000000 | SERIAL_LINK_INTERRUPT_COPY_WORDS);
    driver->send_halfword_index = SERIAL_LINK_INITIAL_HALFWORD_INDEX;
    driver->receive_halfword_index = SERIAL_LINK_INITIAL_HALFWORD_INDEX;
    buffer_cursor = (char *)driver + SERIAL_LINK_STATE_OFFSET(send_buffers);
    driver->prepared_send_packet = (struct SerialLinkPacket *)buffer_cursor;
    buffer_cursor += sizeof(struct SerialLinkPacket);
    driver->active_send_packet = (struct SerialLinkPacket *)buffer_cursor;
    buffer_cursor += sizeof(struct SerialLinkPacket);
    driver->active_receive_packets = (struct SerialLinkPacket *)buffer_cursor;
    buffer_cursor += sizeof(struct SerialLinkPacket) * SERIAL_LINK_SLOT_COUNT;
    driver->pending_receive_packets = (struct SerialLinkPacket *)buffer_cursor;
    buffer_cursor += sizeof(struct SerialLinkPacket) * SERIAL_LINK_SLOT_COUNT;
    driver->completed_receive_packets = (struct SerialLinkPacket *)buffer_cursor;
    gInterruptMasterEnable = 0;
    gInterruptEnable |= SERIAL_LINK_IRQ_SERIAL;
    gInterruptMasterEnable = 1;
}
