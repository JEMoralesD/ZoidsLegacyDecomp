#include "m2c_prelude.h"
#include "../event_script.h"
int SeekEventCommand(int, int, int) asm("func_80A016C");
extern u8 gPlayerState[] asm("D_020218E4");
extern u8 gPlayerDeckCommandFlagsOffset[] asm("D_off_69FC");

int EventIfPlayerDeckCommandUnlocked(u8 script_slot, u8 **script_cursor) asm("func_080A0FEC");

int EventIfPlayerDeckCommandUnlocked(u8 script_slot, u8 **script_cursor) {
    s32 deck_flags_address;
    u32 command_id_or_bit_index;
    u32 *flags_word;

    deck_flags_address = (s32)gPlayerState;
    command_id_or_bit_index = EVENT_COMMAND_BYTE((*script_cursor), EventPlayerDeckCommand, deck_command_id);
    flags_word = (u32 *)((command_id_or_bit_index >> 5) * 4);
    deck_flags_address += (s32)gPlayerDeckCommandFlagsOffset;
    flags_word = (u32 *)((s32)flags_word + deck_flags_address);
    command_id_or_bit_index &= 0x1F;
    if (*flags_word & (1 << command_id_or_bit_index)) {
        SeekEventCommand(script_slot, 15, 0);
    } else {
        SeekEventCommand(script_slot, 16, 0);
    }
    return 0;
}
