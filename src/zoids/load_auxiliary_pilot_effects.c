#include "m2c_prelude.h"
#include "../game/player_state.h"

void UnlockDeckCommand(s32) asm("func_080E5EBC");

void LoadAuxiliaryPilotEffects(struct PlayerAuxiliaryPilotRecordView *auxiliary_pilot) asm("func_080E77FC");

void LoadAuxiliaryPilotEffects(struct PlayerAuxiliaryPilotRecordView *auxiliary_pilot) {
    u8 effect_slot_index;
    u8 *effect_value_table;
    register u8 *effect_kind_table asm("r6");

    effect_slot_index = 0;
    effect_kind_table = (u8 *)0x087B924C;
    effect_value_table = effect_kind_table + 2;
    do {
        register s32 effect_slot_offset asm("r3");
        register s32 auxiliary_pilot_id asm("r1");
        register s32 effect_kind_offset asm("r0");
        register u8 *effect_kind_address asm("r0");
        register s32 effect_kind asm("r1");
        register s32 effect_kind_signed asm("r0");
        register s32 zero_offset asm("r2");

        effect_slot_offset = effect_slot_index << 2;
        auxiliary_pilot_id = auxiliary_pilot->auxiliary_pilot_id;
        effect_kind_offset = auxiliary_pilot_id << 2;
        effect_kind_offset += auxiliary_pilot_id;
        effect_kind_offset <<= 3;
        effect_kind_offset = effect_slot_offset + effect_kind_offset;
        effect_kind_address = (u8 *)(effect_kind_offset + (s32)effect_kind_table);
        effect_kind = *(u16 *)effect_kind_address;
        zero_offset = 0;
        effect_kind_signed = *(s16 *)(effect_kind_address + zero_offset);
        if (effect_kind_signed == 0x1A) {
            UnlockDeckCommand(0x26);
        } else if (effect_kind_signed == 0x1B) {
            UnlockDeckCommand(0x25);
        } else {
            register u8 *effect_kind_output asm("r0");
            register s32 effect_value_offset asm("r0");
            register u8 *effect_value_output asm("r2");
            register s32 auxiliary_pilot_id_copy asm("r1");
            register s32 effect_value_table_offset asm("r0");

            effect_kind_output = (u8 *)auxiliary_pilot;
            effect_kind_output += PLAYER_AUXILIARY_PILOT_OFFSET(effect_kinds);
            effect_kind_output += effect_slot_index;
            *effect_kind_output = effect_kind;

            effect_value_offset = effect_slot_index << 1;
            effect_value_output = (u8 *)auxiliary_pilot;
            effect_value_output += PLAYER_AUXILIARY_PILOT_OFFSET(effect_values);
            effect_value_output += effect_value_offset;
            auxiliary_pilot_id_copy = auxiliary_pilot->auxiliary_pilot_id;
            effect_value_table_offset = auxiliary_pilot_id_copy << 2;
            effect_value_table_offset += auxiliary_pilot_id_copy;
            effect_value_table_offset <<= 3;
            effect_value_table_offset = effect_slot_offset + effect_value_table_offset;
            effect_value_table_offset += (s32)effect_value_table;
            *(u16 *)effect_value_output = *(u16 *)effect_value_table_offset;
        }
        effect_slot_index = (u8)(effect_slot_index + 1);
    } while (effect_slot_index < PLAYER_PILOT_ABILITY_COUNT);
}
