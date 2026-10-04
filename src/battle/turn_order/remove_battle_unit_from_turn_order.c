#include "m2c_prelude.h"
#include "battle_turn_order.h"
extern u8 gBattleTurnOrderEntries[] asm("D_0203725F");

void RemoveBattleUnitFromTurnOrder(u8 side, u8 unit_slot) asm("func_080C02B4");

void RemoveBattleUnitFromTurnOrder(u8 side, u8 unit_slot) {
    register int empty_entry_mask asm("r5");
    u8 *entry_sides;
    u8 *entry_units;
    s32 entry_index;

    entry_index = 0;
    entry_sides = gBattleTurnOrderEntries;
    entry_units = entry_sides + 1;
    empty_entry_mask = BATTLE_TURN_ORDER_EMPTY_ENTRY;
    do {
        s32 entry_byte_offset = entry_index * 2;
        u8 *entry_side = (u8 *)(entry_byte_offset + (int)entry_sides);
        if (*entry_side == side) {
            u8 *entry_unit = (u8 *)(entry_byte_offset + (int)entry_units);
            if (*entry_unit == unit_slot) {
                *entry_side = *entry_side | empty_entry_mask;
                *entry_unit = *entry_unit | empty_entry_mask;
            }
        }
        entry_index = (u8)(entry_index + 1);
    } while ((u32)entry_index <= BATTLE_TURN_ORDER_ENTRY_COUNT - 1);
}
