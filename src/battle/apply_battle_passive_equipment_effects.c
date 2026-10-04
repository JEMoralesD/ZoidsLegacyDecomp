#include "m2c_prelude.h"
#include "battle.h"

s32 ApplyBattleEquipmentEffects(void *, s32, s32, s32) asm("func_080BEE04");

void ApplyBattlePassiveEquipmentEffects(s32 side, s32 unit_index) asm("func_080E90AC");

void ApplyBattlePassiveEquipmentEffects(s32 side, s32 unit_index) {
    register u32 side_index asm("r6");
    register u32 unit_slot asm("r5");
    u8 *unit;
    u8 equipment_slot;
    register s32 side_offset asm("r1");
    register s32 unit_offset asm("r0");
    register u8 *battle_units asm("r2");

    side <<= 24;
    side_index = (u32)side >> 24;
    unit_index <<= 24;
    unit_slot = (u32)unit_index >> 24;

    side_offset = side_index << 2;
    side_offset += side_index;
    side_offset <<= 3;
    side_offset -= side_index;
    side_offset <<= 7;
    unit_offset = unit_slot << 2;
    unit_offset += unit_slot;
    unit_offset <<= 3;
    unit_offset -= unit_slot;
    unit_offset <<= 4;
    battle_units = (u8 *)0x02034B4C;
    unit_offset += (s32)battle_units;
    unit = (u8 *)(side_offset + unit_offset);

    equipment_slot = 0;
    do {
        register s32 slot_offset asm("r0");
        register u8 *slot asm("r0");
        register u8 *slot_value asm("r1");
        register s32 item_id asm("r0");

        slot_offset = equipment_slot << 2;
        slot = (u8 *)((s32)unit + slot_offset);
        asm volatile("" : "+r"(slot));
        slot_value = slot;
        slot_value += 0x52;
        item_id = *(u16 *)slot_value;
        if (item_id != 0) {
            register s32 item_copy asm("r1");
            register s32 item_offset asm("r0");
            register struct EquipmentRecord *item_base asm("r1");
            register struct EquipmentRecord *item asm("r2");
            register s32 flags asm("r1");
            register s32 masked asm("r0");

            item_copy = item_id;
            item_offset = item_copy << 1;
            item_offset += item_copy;
            item_offset <<= 3;
            item_base = (struct EquipmentRecord *)0x087B2524;
            item = (struct EquipmentRecord *)(item_offset + (s32)item_base);
            flags = item->flags;
            masked = EQUIPMENT_KIND_MASK;
            masked &= flags;
            if (masked == (EQUIPMENT_COMMAND | EQUIPMENT_PASSIVE) && item->area_type == EQUIPMENT_AREA_SELF) {
                /* This local occupies the callback's fifth ABI argument slot. */
                volatile s32 callback_type;
                register void *call_item asm("r0");
                register u32 call_side asm("r1");
                register u32 call_unit asm("r2");
                register u32 call_equipment_slot asm("r3");

                callback_type = 1;
                call_item = item;
                asm volatile("" : "+r"(call_item));
                call_side = side_index;
                asm volatile("" : "+r"(call_side));
                call_unit = unit_slot;
                asm volatile("" : "+r"(call_unit));
                call_equipment_slot = equipment_slot;
                asm volatile("" : "+r"(call_equipment_slot));
                ApplyBattleEquipmentEffects(call_item, call_side, call_unit, call_equipment_slot);
            }
        }
        equipment_slot = (u8)(equipment_slot + 1);
    } while (equipment_slot <= 7);
}
