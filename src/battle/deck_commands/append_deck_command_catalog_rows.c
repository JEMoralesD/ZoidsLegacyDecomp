#include "m2c_prelude.h"
#include "deck_commands.h"

extern void CopyBytes(u8 *, u8 *, s32) asm("func_080ED038");
extern void CopyString(u8 *, u8 *) asm("func_080ED128");
extern void AppendWindowTextItem(s32, s32) asm("func_080988C8");

void AppendDeckCommandCatalogRows(int battle_deck) asm("func_080C098C");

void AppendDeckCommandCatalogRows(int battle_deck)
{
    register u32 battle_deck_byte asm("r8");
    register u32 catalog_index asm("r4");
    u32 next_catalog_index;
    u8 *row_text;
    register u32 command_id asm("r3");
    register u8 *player_state asm("r6");
    register u8 *row_name asm("r9");
    register u8 *command_names asm("ip");
    register u32 unlock_word_address asm("r2");
    register u32 selected_slot asm("r1");
    register u32 row_prefix asm("r0");

    {
        register u32 unused_register_carrier asm("sl");
        asm volatile("" :: "r"(unused_register_carrier));
    }
    battle_deck_byte = (u8)battle_deck;
    catalog_index = 0;
    row_text = (u8 *)DECK_COMMAND_TEXT_BUFFER_RAM;
    asm volatile("" : "+r"(row_text));
    do {
    {
        register u8 *catalog_entry asm("r0");
        catalog_entry = (u8 *)DECK_COMMAND_CATALOG_ORDER_ROM;
        asm volatile("" : "+r"(catalog_entry));
        catalog_entry = (u8 *)(catalog_index + (u32)catalog_entry);
        command_id = *catalog_entry;
    }
    {
        register u32 battle_deck_copy asm("r1");
        battle_deck_copy = battle_deck_byte;
        asm volatile("" : "+r"(battle_deck_copy));
        if (battle_deck_copy != 0) {
            next_catalog_index = catalog_index + 1;
            if (command_id == 0) goto next_catalog_entry;
        }
    }
    if (command_id == 0) {
        register u32 remove_row_prefix asm("r0");
        remove_row_prefix = DECK_COMMAND_REMOVE_ROW_PREFIX;
        *(u16 *)row_text = remove_row_prefix;
        CopyBytes(row_text + 2, (u8 *)DECK_COMMAND_REMOVE_TEXT_ROM, 13);
        next_catalog_index = catalog_index + 1;
        goto append_catalog_row;
    }
    selected_slot = 0;
    player_state = (u8 *)0x020218E4;
    unlock_word_address = command_id >> 5;
    {
        register u8 *row_name_seed asm("r0");
        row_name_seed = (u8 *)0x02030566;
        row_name = row_name_seed;
    }
    {
        register u8 *name_table_seed asm("r0");
        name_table_seed = (u8 *)DECK_COMMAND_NAME_TABLE_ROM;
        command_names = name_table_seed;
    }
    asm volatile("" : "+r"(catalog_index));
    next_catalog_index = catalog_index + 1;
    goto check_selected_entry;
next_selected_entry:
    {
        register u32 next_selected_slot asm("r0");
        next_selected_slot = selected_slot + 1;
        next_selected_slot <<= 24;
        selected_slot = next_selected_slot >> 24;
    }
check_selected_entry:
    if (selected_slot > PLAYER_SELECTED_DECK_COMMAND_COUNT - 1) goto command_not_selected;
    {
        if (battle_deck_byte == 0) {
            register u32 player_deck_offset asm("r4");
            register u8 *player_selected_entry asm("r0");
            register u32 player_selected_command asm("r0");
            player_deck_offset = PLAYER_STATE_OFFSET(selected_deck_commands);
            player_selected_entry = player_state + player_deck_offset;
            player_selected_entry = (u8 *)(selected_slot + (u32)player_selected_entry);
            player_selected_command = *player_selected_entry;
            if (player_selected_command == command_id) goto selected_command_found;
            goto next_selected_entry;
        } else {
            register u8 *battle_selected_entry asm("r0");
            register u32 battle_deck_offset asm("r4");
            register u32 battle_selected_command asm("r0");
            battle_selected_entry = (u8 *)0x02034B4C;
            battle_deck_offset = BATTLE_DECK_COMMAND_OFFSET(selected_deck_commands);
            asm volatile("" : "+r"(battle_deck_offset));
            battle_selected_entry = battle_selected_entry + battle_deck_offset;
            battle_selected_entry = (u8 *)(selected_slot + (u32)battle_selected_entry);
            battle_selected_command = *battle_selected_entry;
            if (battle_selected_command != command_id) goto next_selected_entry;
        }
    }
selected_command_found:
    if (selected_slot > PLAYER_SELECTED_DECK_COMMAND_COUNT - 1) goto command_not_selected;
    row_prefix = DECK_COMMAND_SELECTED_ROW_PREFIX;
    goto store_row_prefix;
command_not_selected:
    row_prefix = DECK_COMMAND_UNSELECTED_ROW_PREFIX;
store_row_prefix:
    *(u16 *)row_text = row_prefix;
    unlock_word_address <<= 2;
    {
        register u32 unlock_flags_offset asm("r1");
        register u8 *unlock_flags_base asm("r0");
        unlock_flags_offset = PLAYER_STATE_OFFSET(unlocked_deck_commands);
        unlock_flags_base = player_state + unlock_flags_offset;
        unlock_word_address += (u32)unlock_flags_base;
    }
    {
        register u32 unlock_bit_index asm("r0");
        register u32 unlock_bit asm("r1");
        register u32 unlock_word asm("r0");
        unlock_bit_index = 31;
        unlock_bit_index &= command_id;
        unlock_bit = 1;
        unlock_bit <<= unlock_bit_index;
        unlock_word = *(u32 *)unlock_word_address;
        unlock_word &= unlock_bit;
        if (unlock_word != 0) {
            register u32 command_name_address asm("r0");
            register u8 *command_name asm("r1");
            command_name_address = command_id << 2;
            command_name_address += (u32)command_names;
            command_name = *(u8 **)command_name_address;
            CopyString(row_name, command_name);
        } else {
            register u8 *unknown_name_table asm("r4");
            register u8 *unknown_command_name asm("r1");
            unknown_name_table = command_names;
            unknown_command_name = *(u8 **)unknown_name_table;
            CopyString(row_name, unknown_command_name);
        }
    }
append_catalog_row:
    AppendWindowTextItem(DECK_COMMAND_CATALOG_WINDOW, (s32)DECK_COMMAND_TEXT_BUFFER_RAM);
next_catalog_entry:
    {
        register u32 next_catalog_shifted asm("r0");
        next_catalog_shifted = next_catalog_index << 24;
        catalog_index = next_catalog_shifted >> 24;
    }
    } while (catalog_index <= DECK_COMMAND_COUNT - 1);
}
