#include "m2c_prelude.h"
#include "player_state.h"
extern u8 gPlayerStateBytes[] asm("D_020218E4");
extern u8 gPlayerDeckCommandFlagsOffset[] asm("D_off_69FC");
s32 UnlockDeckCommand(s32 deck_command_id) asm("func_080E5EBC");

s32 UnlockDeckCommand(s32 deck_command_id) {
    u32 command_id_bits = deck_command_id << 0x18;
    s32 unlocked_commands, command_bit, deck_command_words_address, command_word_index, command_word_offset;
    s32 *command_word;
    u32 command_bit_index;
    command_word_index = command_id_bits >> 0x1D;
    command_bit_index = 0x1F000000;
    command_bit_index &= command_id_bits;
    command_bit_index >>= 0x18;
    command_bit = 1 << command_bit_index;
    deck_command_words_address = (s32)gPlayerStateBytes;
    command_word_offset = command_word_index * 4;
    deck_command_words_address += (s32)gPlayerDeckCommandFlagsOffset;
    command_word = (s32 *)(command_word_offset + deck_command_words_address);
    unlocked_commands = *command_word;
    if (unlocked_commands & command_bit) {
        return PLAYER_DATA_ALREADY_UNLOCKED;
    }
    *command_word = unlocked_commands | command_bit;
    return PLAYER_DATA_NEWLY_UNLOCKED;
}
