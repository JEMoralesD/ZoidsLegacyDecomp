#include "m2c_prelude.h"
#include "battle_turn_order.h"
extern u8 gBattleState[];
extern u8 gBattleTurnOrderSideOffset[] asm("D_off_2713");
extern u8 gBattleTurnOrderUnitOffset[] asm("D_off_2714");
void SetBattleTurnOrderEntry(s32 entry_index, s8 side, s8 unit_slot, s16 priority) asm("func_080C007C");

void SetBattleTurnOrderEntry(s32 entry_index, s8 side, s8 unit_slot, s16 priority) {
    register s32 battle_state_address asm("r5");
    register s32 side_field_offset asm("r6");
    u32 entry_index_shifted, entry_byte_offset;
    s32 entry_field_address, unit_field_address;
    entry_index_shifted = entry_index << 0x18;
    battle_state_address = (s32)gBattleState;
    entry_byte_offset = (u32)entry_index_shifted >> 0x17;
    side_field_offset = (s32)gBattleTurnOrderSideOffset;
    entry_field_address = battle_state_address + side_field_offset;
    *(s8 *)(entry_byte_offset + entry_field_address) = side;
    entry_field_address = (s32)gBattleTurnOrderUnitOffset;
    unit_field_address = battle_state_address + entry_field_address;
    *(s8 *)(entry_byte_offset + unit_field_address) = unit_slot;
    side_field_offset += BATTLE_TURN_ORDER_PRIORITIES_FROM_SIDE;
    battle_state_address += side_field_offset;
    *(s16 *)(entry_byte_offset + battle_state_address) = priority;
}
