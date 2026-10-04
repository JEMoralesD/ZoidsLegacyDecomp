#include "m2c_prelude.h"
#include "battle_combination.h"

extern u8 gBattleState[];
extern u8 gZoidBaseStatTable[];
extern u8 gBattleCombinationPresentation[] asm("D_02032F7C");

void RemoveBattleUnitFromTurnOrder(u8, u8) asm("func_080C02B4");
void ClearBattleUnitEffects(u8, u8) asm("func_080BE560");
void ApplyBattlePassiveEquipmentEffects(u8, u8) asm("func_080E90AC");
void RecalculateBattleUnitStats(u8, u8) asm("func_080E8B08");
void InsertBattleUnitTurnOrderEntries(u8, u8) asm("func_080C00B0");

void SetBattleUnitCombinedModel(u8 side, u8 unit_slot, u8 combined_model_id) asm("func_080C34A4");

void SetBattleUnitCombinedModel(u8 side, u8 unit_slot, u8 combined_model_id)
{
    u8 *battle_unit;
    u8 *form_defense_bonuses;
    u8 *form_ep_regen_bonuses;
    u8 *form_weapon_power_bonuses;
    register u8 *equipment_slots asm("r10");
    u8 *base_equipment_slots;
    register u8 *base_model_record asm("r1");
    s32 side_offset;
    s32 unit_address;
    u32 form_weapon_row_offset;
    u8 form_or_equipment_slot, weapon_kind;
    u8 original_party_slot;
    u8 zero;
    register u16 empty_item_id asm("r2");
    register u16 unit_flags asm("r2");
    register u16 combined_flags asm("r0");

    side_offset = side * 0x1380;
    unit_address = unit_slot * 0x270 + (s32)gBattleState;
    battle_unit = (u8 *)(side_offset + unit_address);
    base_model_record = (u8 *)(combined_model_id * sizeof(struct ZoidBaseRecordView) + (s32)gZoidBaseStatTable);
    battle_unit[0] = combined_model_id;
    unit_flags = *(u16 *)(battle_unit + 4);
    asm volatile("" : : "r"(unit_flags));
    combined_flags = BATTLE_COMBINATION_MODEL_FLAG;
    combined_flags |= unit_flags;
    *(u16 *)(battle_unit + 4) = combined_flags;

    form_or_equipment_slot = 0;
    zero = 0;
    form_weapon_power_bonuses = battle_unit + BATTLE_UNIT_OFFSET(form_weapon_power_bonuses);
    asm volatile("" : : "r"(form_weapon_power_bonuses));
    equipment_slots = (u8 *)BATTLE_UNIT_OFFSET(equipment);
    equipment_slots += (s32)battle_unit;
    base_equipment_slots = base_model_record + ZOID_BASE_RECORD_OFFSET(equipment);
    form_ep_regen_bonuses = (u8 *)BATTLE_UNIT_OFFSET(form_ep_regen_bonuses);
    form_ep_regen_bonuses += (s32)battle_unit;
    {
        register u32 guard0 asm("r0");
        register u32 guard1 asm("r1");
        asm volatile("" : "=r"(guard0), "=r"(guard1));
        form_defense_bonuses = (u8 *)BATTLE_UNIT_OFFSET(form_defense_bonuses);
        form_defense_bonuses += (s32)battle_unit;
        asm volatile("" : : "r"(guard0), "r"(guard1));
    }

    do {
        form_ep_regen_bonuses[form_or_equipment_slot] = zero;
        form_defense_bonuses[form_or_equipment_slot] = zero;
        weapon_kind = 0;
        form_weapon_row_offset = form_or_equipment_slot * 4;
        asm volatile("" : : "r"(form_weapon_row_offset), "r"(form_weapon_row_offset), "r"(form_weapon_row_offset));
        do {
            form_weapon_power_bonuses[weapon_kind + form_weapon_row_offset] = zero;
            weapon_kind++;
        } while (weapon_kind <= 3);
        form_or_equipment_slot++;
    } while (form_or_equipment_slot <= 5);

    {
        u32 equipment_offset;
        u32 equipment_address;

        form_or_equipment_slot = 0;
        empty_item_id = 0;
        do {
            equipment_offset = form_or_equipment_slot * 4;
            equipment_address = (u32)battle_unit;
            equipment_address += equipment_offset;
            *(u16 *)(equipment_address + BATTLE_UNIT_OFFSET(equipment[0].item_id)) = empty_item_id;
            form_or_equipment_slot++;
        } while (form_or_equipment_slot <= 3);
    }

    form_or_equipment_slot = 4;
    {
        u8 *copy_dst = equipment_slots;
        u8 *copy_src = base_equipment_slots;
        u32 copy_off;
        u32 copy_dst_addr;
        u32 copy_src_addr;
        do {
            copy_off = form_or_equipment_slot * 4;
            copy_dst_addr = (u32)copy_dst;
            copy_dst_addr += copy_off;
            copy_src_addr = (u32)copy_src;
            copy_src_addr += copy_off;
            *(u32 *)copy_dst_addr = *(u32 *)copy_src_addr;
            form_or_equipment_slot++;
        } while (form_or_equipment_slot <= 7);
    }

    RemoveBattleUnitFromTurnOrder(side, unit_slot);
    ClearBattleUnitEffects(side, unit_slot);
    ApplyBattlePassiveEquipmentEffects(side, unit_slot);
    RecalculateBattleUnitStats(side, unit_slot);
    InsertBattleUnitTurnOrderEntries(side, unit_slot);

    {
        register u8 *base asm("r1") = gBattleState;
        register u32 original_party_slots_offset asm("r2") = BATTLE_COMBINATION_OFFSET(current_unit_original_party_slots);
        original_party_slot = *(u8 *)(unit_slot + (s32)(base + original_party_slots_offset));
        original_party_slots_offset += 6;
        *(u8 *)((u32)original_party_slot + (u32)(base + original_party_slots_offset)) = original_party_slot;
        gBattleCombinationPresentation[unit_slot] = BATTLE_COMBINATION_CHANGED_MODEL;
    }
}
