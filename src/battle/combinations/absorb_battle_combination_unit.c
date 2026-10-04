#include "m2c_prelude.h"
#include "battle_combination.h"

M2C_UNK RemoveBattleUnitFromTurnOrder(u8, u8) asm("func_080C02B4");
extern u8 gBattleState[];
extern u8 gBattleCombinationPresentation[] asm("D_02032F7C");

void AbsorbBattleCombinationUnit(u8 side, u8 combined_unit_slot, u8 component_unit_slot) asm("func_080C3440");

void AbsorbBattleCombinationUnit(u8 side, u8 combined_unit_slot, u8 component_unit_slot) {
    u8 side_byte;
    u8 combined_slot_byte;
    u8 component_slot_byte;
    u8 component_original_party_slot;
    u8 *battle_state;
    u8 *original_party_slots;
    u8 *combination_owners;
    u32 component_unit_offset;
    u32 side_offset;

    side_byte = side;
    combined_slot_byte = combined_unit_slot;
    component_slot_byte = component_unit_slot;
    battle_state = gBattleState;
    component_unit_offset = component_slot_byte * 0x270;
    side_offset = side_byte * 0x1380;
    battle_state[component_unit_offset + side_offset] = 0;
    RemoveBattleUnitFromTurnOrder(side_byte, component_slot_byte);
    original_party_slots = battle_state + BATTLE_COMBINATION_OFFSET(current_unit_original_party_slots);
    combination_owners = battle_state + BATTLE_COMBINATION_OFFSET(combination_owner_original_party_slots);
    component_original_party_slot = original_party_slots[component_slot_byte];
    combination_owners[component_original_party_slot] = original_party_slots[combined_slot_byte];
    gBattleCombinationPresentation[component_slot_byte] = combined_slot_byte;
}
