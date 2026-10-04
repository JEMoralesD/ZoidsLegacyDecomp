#include "m2c_prelude.h"
#include "battle_display.h"

void BuildBattleSelectedTargetList(void) asm("func_080CD5CC");

void BuildBattleSelectedTargetList(void)
{
    u32 action_count_offset;
    u32 target_list_offset;
    u8 *battle_state;
    register u8 *target_sides asm("r4");
    register u8 *first_target_unit_slot asm("ip");
    u8 *target_side_address;
    register u32 list_index_or_empty_marker asm("r3");
    s32 side_marker;
    register u32 action_index asm("r2");
    register u32 target_side asm("r6");
    register u32 target_unit_slot asm("r5");
    u32 outcome;
    register s32 action_outcomes_offset asm("sl");
    register s32 side_outcomes_offset asm("r8");
    volatile s32 next_action_index;
    register s32 next_side asm("r9");
    s32 next_unit_slot;
    u32 list_pair_offset;
    u32 next_pair_offset;

    action_count_offset = BATTLE_SELECTION_OFFSET(action_count);
    target_list_offset = BATTLE_SELECTED_TARGET_LIST_OFFSET;
    action_index = 0;
    {
        register u8 *battle_state_r0 asm("r0") = (u8 *)0x02034B4C;
        asm volatile("" : "+r"(battle_state_r0));
        target_sides = battle_state_r0 + BATTLE_SELECTED_TARGET_LIST_OFFSET;
    }
    list_index_or_empty_marker = 0xFF;
    asm volatile("" : "+r"(list_index_or_empty_marker));
    do {
        u8 *side_address = (u8 *)((u32)(action_index * 2) + (u32)target_sides);
        side_marker = *side_address;
        side_marker = side_marker | list_index_or_empty_marker;
        *side_address = side_marker;
        {
            register s32 next_action_or_list_index asm("r0");
            next_action_or_list_index = action_index + 1;
            action_index = (u8)next_action_or_list_index;
        }
    } while (action_index <= 11);
    action_index = 0;
    {
        register u8 *battle_state_r3 asm("r3") = (u8 *)0x02034B4C;
        asm volatile("" : "+r"(battle_state_r3));
        if (action_index < (u32)*(battle_state_r3 + action_count_offset)) {
            {
                u32 first_unit_slot_offset = BATTLE_SELECTED_TARGET_FIRST_SLOT_OFFSET;
                asm volatile("" : "+r"(first_unit_slot_offset));
                first_target_unit_slot = (u8 *)((u32)battle_state_r3 + first_unit_slot_offset);
            }
scan_action:
            battle_state = (u8 *)0x02034B4C;
            target_side = 0;
            {
                s32 action_stride_partial = action_index * 2;
                next_action_index = action_index + 1;
                action_outcomes_offset = (action_stride_partial + action_index) * 4;
            }
scan_side:
            target_unit_slot = 0;
            {
                s32 side_stride_partial = target_side * 2;
                next_side = target_side + 1;
                side_outcomes_offset = (side_stride_partial + target_side) * 2;
            }
scan_unit:
            {
                register s32 side_offset_r3 asm("r3");
                register s32 outcome_offset asm("r0");
                side_offset_r3 = side_outcomes_offset;
                asm volatile("" : "+r"(side_offset_r3));
                outcome_offset = target_unit_slot + side_offset_r3 + action_outcomes_offset;
                outcome = *(u8 *)(outcome_offset + BATTLE_SELECTED_TARGET_OUTCOMES_RAM);
            }
            next_unit_slot = target_unit_slot + 1;
            if (outcome != 0) {
                list_index_or_empty_marker = 0;
                if (({ register u8 *first_target_side_address asm("r2") = (u8 *)BATTLE_SELECTED_TARGET_FIRST_SIDE_RAM; *first_target_side_address; }) == target_side) {
                    register u8 *first_target_unit_slot_address asm("r1") = first_target_unit_slot;
                    asm volatile("" : "+r"(first_target_unit_slot_address));
                    if (*first_target_unit_slot_address == target_unit_slot) goto next_target;
                }
scan_target_list:
                list_pair_offset = list_index_or_empty_marker * 2;
                target_sides = (u8 *)(target_list_offset + (u32)battle_state);
                target_side_address = (u8 *)(list_pair_offset + (u32)target_sides);
                if (*target_side_address == 0xFF) {
                    *target_side_address = target_side;
                    {
                        register u8 *unit_slots_base asm("r3");
                        register u8 *target_unit_slot_address asm("r0");
                        unit_slots_base = first_target_unit_slot;
                        asm volatile("" : "+r"(unit_slots_base));
                        target_unit_slot_address = (u8 *)(list_pair_offset + (u32)unit_slots_base);
                        *target_unit_slot_address = target_unit_slot;
                    }
                    goto next_target;
                }
                {
                    register s32 next_list_index asm("r0");
                    next_list_index = list_index_or_empty_marker + 1;
                    list_index_or_empty_marker = (u8)next_list_index;
                }
                if (list_index_or_empty_marker > 11) goto next_target;
                next_pair_offset = list_index_or_empty_marker * 2;
                {
                    register u8 *next_target_side_address asm("r0") = (u8 *)(next_pair_offset + (u32)target_sides);
                    if (*next_target_side_address != target_side) goto scan_target_list;
                }
                if (*(u8 *)(next_pair_offset + (u32)first_target_unit_slot) != target_unit_slot) goto scan_target_list;
            }
next_target:
            {
                register s32 unit_slot_byte_bits asm("r0");
                unit_slot_byte_bits = next_unit_slot << 24;
                target_unit_slot = (u32)unit_slot_byte_bits >> 24;
            }
            if (target_unit_slot <= 5) goto scan_unit;
            {
                register s32 next_side_r3 asm("r3") = next_side;
                asm volatile("" : "+r"(next_side_r3));
                {
                    register s32 side_byte_bits asm("r0");
                    side_byte_bits = next_side_r3 << 24;
                    target_side = (u32)side_byte_bits >> 24;
                }
            }
            if (target_side <= 1) goto scan_side;
            {
                register s32 next_action_r1 asm("r1");
                register s32 action_byte_bits asm("r0");
                next_action_r1 = next_action_index;
                action_byte_bits = next_action_r1 << 24;
                action_index = (u32)action_byte_bits >> 24;
            }
            {
                register u8 *battle_state_count_r3 asm("r3") = (u8 *)0x02034B4C;
                asm volatile("" : "+r"(battle_state_count_r3));
                {
                    register u8 *action_count_address asm("r0") = battle_state_count_r3 + BATTLE_SELECTION_OFFSET(action_count);
                    if (action_index < (u32)*action_count_address) goto scan_action;
                }
            }
        }
    }
}
