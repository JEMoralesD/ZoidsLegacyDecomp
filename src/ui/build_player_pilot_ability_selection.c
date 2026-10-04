#include "player_selection.h"

void BuildPlayerPilotAbilitySelection(s32 pilot_record_address) asm("func_080B67B8");

void BuildPlayerPilotAbilitySelection(s32 pilot_record_address) {
    register u8 *pilot_record asm("r12");
    register u8 *selected_count asm("r4");
    u8 *selected_kinds;
    u8 *selected_values;
    u8 *ability_kinds;
    register u32 ability_index asm("r3");
    register u8 *selected_count_address asm("r1");
    register s32 count_or_index_bits asm("r0");

    pilot_record = (u8 *)pilot_record_address;
    selected_count_address = (u8 *)PLAYER_ABILITY_SELECTION_COUNT_RAM;
    count_or_index_bits = 0;
    *selected_count_address = count_or_index_bits;
    ability_index = 0;
    selected_kinds = (u8 *)PLAYER_ABILITY_SELECTION_KINDS_RAM;
    selected_count = selected_count_address;
    selected_values = (u8 *)PLAYER_ABILITY_SELECTION_VALUES_RAM;
    ability_kinds = pilot_record;
    ability_kinds += PLAYER_PILOT_OFFSET(ability_kinds);

    do {
        register u8 *ability_kind_address asm("r0");
        s32 ability_kind;

        ability_kind_address = ability_kinds + ability_index;
        ability_kind = *ability_kind_address;
        if (ability_kind != 0) {
            register s32 kind_output_address asm("r0");
            register u8 *kind_output asm("r0");
            register s32 value_output_address asm("r1");
            register u8 *value_output asm("r1");
            register s32 ability_value_offset asm("r2");
            register u8 *ability_value_address asm("r0");
            register s32 ability_value_low_byte asm("r0");

            kind_output_address = *selected_count;
            kind_output_address += (s32)selected_kinds;
            kind_output = (u8 *)kind_output_address;
            *kind_output = ability_kind;
            value_output_address = *selected_count;
            value_output_address += (s32)selected_values;
            value_output = (u8 *)value_output_address;
            ability_value_offset = ability_index << 1;
            ability_value_address = pilot_record + PLAYER_PILOT_OFFSET(ability_values);
            ability_value_address += ability_value_offset;
            ability_value_low_byte = *(u16 *)ability_value_address;
            *value_output = ability_value_low_byte;
            count_or_index_bits = *selected_count;
            count_or_index_bits += 1;
            *selected_count = count_or_index_bits;
        }
        count_or_index_bits = ability_index + 1;
        count_or_index_bits <<= 24;
        ability_index = (u32)count_or_index_bits >> 24;
    } while (ability_index <= 9);
}
