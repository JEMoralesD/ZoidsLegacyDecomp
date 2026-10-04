#include "m2c_prelude.h"
#include "../game/player_state.h"

void RecalculateZoidStats(void *, s32) asm("func_080E5880");
void AddEquipmentToInventory(u16, s32) asm("func_080E5CE4");
void SubtractEquipmentFromInventory(u8, s32) asm("func_080E5D38");

void EquipPlayerZoid(u8 equipment_id, u8 stored_zoid_slot, u8 equipment_slot_index) asm("func_080E5EFC");

void EquipPlayerZoid(u8 equipment_id, u8 stored_zoid_slot, u8 equipment_slot_index)
{
    register u8 *zoid_records_base asm("r1");
    register struct PlayerZoidRecordView *zoid asm("r6");
    register s32 pilot_address;
    register struct EquipmentSlot *equipment_slot asm("r5");

    if (equipment_id != 0) {
        register u8 *inventory_quantity_address asm("r0") = (u8 *)0x020218E4;
        register u32 equipment_inventory_offset asm("r3") = PLAYER_STATE_OFFSET(equipment_quantities);
        asm volatile("" : "+r"(inventory_quantity_address));
        asm volatile("" : "+r"(equipment_inventory_offset));
        inventory_quantity_address += equipment_inventory_offset;
        inventory_quantity_address = (u8 *)((u32)equipment_id + (u32)inventory_quantity_address);
        if (*inventory_quantity_address == 0) {
            return;
        }
    }

    {
        register s32 zoid_record_offset asm("r0");
        zoid_record_offset = stored_zoid_slot << 3;
        zoid_record_offset -= stored_zoid_slot;
        zoid_record_offset <<= 4;
        zoid_records_base = (u8 *)0x020218E8;
        asm volatile("" : "+r"(zoid_records_base));
        zoid = (struct PlayerZoidRecordView *)(zoid_record_offset + (s32)zoid_records_base);
    }
    {
        register u32 pilot_slot asm("r0") = zoid->pilot_slot;
        if (pilot_slot != 0) {
            pilot_slot <<= 6;
            {
                register u32 pilot_records_offset asm("r3") = PLAYER_STATE_OFFSET(pilots) - PLAYER_STATE_OFFSET(zoids);
                asm volatile("" : "+r"(pilot_records_offset));
                zoid_records_base += pilot_records_offset;
                pilot_address = pilot_slot + (s32)zoid_records_base;
            }
        } else {
            pilot_address = 0;
        }
    }
    {
        register u32 equipment_slot_offset asm("r0") = equipment_slot_index << 2;
        equipment_slot_offset += PLAYER_ZOID_OFFSET(equipment);
        equipment_slot = (struct EquipmentSlot *)((u8 *)zoid + equipment_slot_offset);
    }

    if (equipment_id == 0) {
        goto replace_equipment;
    }
    {
        register struct EquipmentRecord *equipment_catalog asm("r1") =
            (struct EquipmentRecord *)0x087B2524;
        register u32 equipment_record_offset asm("r0");
        register u32 equipment_flags asm("r1");
        register u32 command_or_weapon_bit asm("r2");
        register u32 required_slot_flag asm("r0");
        register u32 available_slot_flags asm("r1");

        asm volatile("" : "+r"(equipment_catalog));
        equipment_record_offset = equipment_id << 1;
        equipment_record_offset += equipment_id;
        equipment_record_offset <<= 3;
        equipment_record_offset += (u32)equipment_catalog;
        equipment_flags = ((struct EquipmentRecord *)equipment_record_offset)->flags;
        asm volatile("" : "+r"(equipment_flags));
        command_or_weapon_bit = EQUIPMENT_COMMAND;
        required_slot_flag = command_or_weapon_bit;
        required_slot_flag &= equipment_flags;
        if (required_slot_flag != 0) {
            goto command_slot_required;
        }
        available_slot_flags = equipment_slot->flags;
        required_slot_flag = command_or_weapon_bit;
        goto test_slot_compatibility;
command_slot_required:
        available_slot_flags = equipment_slot->flags;
        required_slot_flag = EQUIPMENT_SLOT_COMMAND;
test_slot_compatibility:
        required_slot_flag &= available_slot_flags;
        if (required_slot_flag == 0) {
            return;
        }
    }
    asm volatile("" : "+r"(equipment_id));
    if (equipment_id == 0) {
        goto replace_equipment;
    }
    SubtractEquipmentFromInventory(equipment_id, 1);

replace_equipment:
    if (equipment_slot->item_id != 0) {
        AddEquipmentToInventory(equipment_slot->item_id, 1);
    }
    equipment_slot->item_id = equipment_id;
    RecalculateZoidStats(zoid, pilot_address);
}
