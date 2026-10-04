#include "m2c_prelude.h"
#include "battle_turn_order.h"

extern u8 gBattleState[];
extern u8 gBattleSetup;
extern u8 gLinkBattleSideZeroTiePriority asm("D_0300603C");

u32 IsBattleUnitActive(u32, u32) asm("func_080E9D88");
u8 FindBattleEffect(u32, u32, s32) asm("func_080BF464");
void InsertBattleUnitTurnOrderEntries(u32, u32) asm("func_080C00B0");

void BuildBattleTurnOrder(u8 order_mode) asm("func_080C030C");

void BuildBattleTurnOrder(u8 order_mode)
{
    volatile struct BattleTurnBuildFrame {
        s32 data00;
        s32 highest_priority;
    } frame;
    register s32 side asm("r5");
    register u8 *battle_state_scan asm("sl");

    {
        register u8 *battle_state asm("r1");
        register u8 *mode_address asm("r2");
        register s32 mode_offset asm("r3");
        register s32 zero asm("r3");
        register s32 count_offset asm("r4");
        register s32 current_entry_offset asm("r2");
        register u8 *count_address asm("r0");
        register u8 *current_entry_address asm("r0");

        battle_state = gBattleState;
        mode_offset = BATTLE_TURN_ORDER_OFFSET(mode);
        mode_address = battle_state + mode_offset;
        zero = 0;
        *mode_address = order_mode;
        count_offset = BATTLE_TURN_ORDER_OFFSET(entry_count);
        asm volatile("" : : "r"(count_offset));
        count_address = battle_state + count_offset;
        *count_address = zero;
        current_entry_offset = BATTLE_TURN_ORDER_OFFSET(current_entry);
        current_entry_address = battle_state + current_entry_offset;
        *current_entry_address = zero;
        frame.highest_priority = zero;
        side = 0;
        battle_state_scan = battle_state;
    }

    do {
        register u16 unit_slot asm("r6");
        register u32 side_byte asm("r8");
        register s32 side_shifted asm("r9");
        s32 side_signed;
        register u32 side_byte_shifted asm("r0");
        register s32 side_halfword_shifted asm("r4");

        unit_slot = 0;
        side_byte_shifted = side << 24;
        side_halfword_shifted = side << 16;
        side_shifted = side_halfword_shifted;
        side_byte = side_byte_shifted >> 24;
        side_signed = side_halfword_shifted >> 16;

        do {
            register u32 unit_slot_byte asm("r4");
            register u32 unit_slot_byte_shifted asm("r0");

            unit_slot_byte_shifted = unit_slot << 24;
            unit_slot_byte = unit_slot_byte_shifted >> 24;
            if ((IsBattleUnitActive(side_byte, unit_slot_byte) << 24) != 0 &&
                FindBattleEffect(side_byte, unit_slot_byte, BATTLE_EFFECT_FREEZE) == BATTLE_EFFECT_NOT_FOUND &&
                FindBattleEffect(side_byte, unit_slot_byte, BATTLE_EFFECT_TURN_MARKER) == BATTLE_EFFECT_NOT_FOUND) {
                register s32 unit_slot_shifted asm("r2");
                register s32 record_side asm("r3");
                register s32 unit_slot_signed asm("r0");
                register s32 unit_record_offset asm("r1");
                register s32 highest_priority_raw asm("r4");
                register s32 highest_priority_shifted asm("r0");
                s16 unit_priority;
                s16 highest_priority;
                register u8 *unit_record asm("r1");

                asm volatile("" : "+r"(unit_slot));
                unit_slot_shifted = unit_slot << 16;
                unit_slot_signed = unit_slot_shifted >> 16;
                unit_record_offset = unit_slot_signed * 0x270;
                record_side = side_shifted >> 16;
                unit_record_offset += record_side * 0x1380;
                unit_record = battle_state_scan + unit_record_offset;
                highest_priority_raw = frame.highest_priority;
                highest_priority_shifted = highest_priority_raw << 16;
                asm volatile("" : "+r"(highest_priority_shifted));
                highest_priority = highest_priority_shifted >> 16;
                unit_priority = BATTLE_UNIT_FIELD(unit_record, s16, initiative);

                if (highest_priority < unit_priority)
                    goto choose_tie_side;
                if (gBattleSetup != 1)
                    goto next_unit_slot;
                if (highest_priority != unit_priority)
                    goto next_unit_slot;
                if (gLinkBattleSideZeroTiePriority != 0) {
                    if (record_side == 0)
                        goto choose_tie_side;
                    goto next_unit_slot;
                }
                if (record_side == 0)
                    goto next_unit_slot;

choose_tie_side:
                unit_slot_signed = unit_slot_shifted >> 16;
                unit_record_offset = unit_slot_signed * 0x270;
                unit_record_offset += side_signed * 0x1380;
                unit_record = battle_state_scan + unit_record_offset;
                {
                    register u32 chosen_priority asm("r1");
                    register u8 *chosen_side_address asm("r1");

                    chosen_priority = BATTLE_UNIT_FIELD(unit_record, u16, initiative);
                    frame.highest_priority = chosen_priority;
                    chosen_side_address = (u8 *)0x0203725E;
                    *chosen_side_address = (u8)side;
                }
            }

next_unit_slot:
            {
                register s32 next asm("r0");
                register s32 step asm("r2");

                next = unit_slot << 16;
                step = 0x10000;
                next += step;
                unit_slot = next >> 16;
                if ((next >> 16) <= BATTLE_ACTIVE_UNIT_COUNT - 1)
                    continue;

                next = side << 16;
                next += step;
                side = (u16)(next >> 16);
                if ((next >> 16) <= BATTLE_SIDE_COUNT - 1)
                    break;
                goto tie_side_chosen;
            }
        } while (1);
    } while (1);

tie_side_chosen:
    side = 0;
    do {
        register u16 unit_slot asm("r6");
        register s32 side_shifted asm("r9");
        register u32 side_byte_shifted asm("r0");

        unit_slot = 0;
        side_byte_shifted = side << 24;
        side <<= 16;
        side_shifted = side;
        side = side_byte_shifted >> 24;

        do {
            register u32 unit_slot_byte asm("r4");
            register u32 unit_slot_byte_shifted asm("r0");

            unit_slot_byte_shifted = unit_slot << 24;
            unit_slot_byte = unit_slot_byte_shifted >> 24;
            if ((IsBattleUnitActive(side, unit_slot_byte) << 24) != 0 &&
                FindBattleEffect(side, unit_slot_byte, BATTLE_EFFECT_FREEZE) == BATTLE_EFFECT_NOT_FOUND &&
                FindBattleEffect(side, unit_slot_byte, BATTLE_EFFECT_TURN_MARKER) == BATTLE_EFFECT_NOT_FOUND) {
                InsertBattleUnitTurnOrderEntries(side, unit_slot_byte);
            }

            {
                register s32 next asm("r0");
                register s32 step asm("r3");

                asm volatile("" : "+r"(unit_slot));
                next = unit_slot << 16;
                step = 0x10000;
                next += step;
                unit_slot = next >> 16;
                if ((next >> 16) <= BATTLE_ACTIVE_UNIT_COUNT - 1)
                    continue;

                next = step;
                asm volatile("" : "+r"(next));
                next += side_shifted;
                side = (u16)(next >> 16);
                if ((next >> 16) <= BATTLE_SIDE_COUNT - 1)
                    break;
                goto entries_inserted;
            }
        } while (1);
    } while (1);

entries_inserted:
    {
        register u8 *battle_state_tail asm("r1");
        register s32 tail_entry_index asm("r5");
        register s32 tail_index_shifted asm("r2");
        s32 entry_count_offset;
        register u8 *entry_count_address asm("r0");
        register s32 count_load_scratch asm("r0");
        register s32 count_load_scratch2 asm("r2");
        register s32 count_load_scratch3 asm("r3");

        battle_state_tail = gBattleState;
        entry_count_offset = BATTLE_TURN_ORDER_OFFSET(entry_count);
        asm volatile("" : "=&r"(count_load_scratch), "=&r"(count_load_scratch2),
                       "=&r"(count_load_scratch3)
                     : "r"(entry_count_offset));
        entry_count_address = entry_count_offset + battle_state_tail;
        tail_entry_index = *entry_count_address;
        tail_index_shifted = tail_entry_index << 16;
        if (tail_entry_index <= BATTLE_TURN_ORDER_ENTRY_COUNT - 1) {
            u8 *entry_sides;
            u8 *entry_units;
            register u32 empty_entry_mask asm("r4");
            s32 entry_side_offset;
            register s32 entry_unit_offset asm("r3");

            entry_side_offset = BATTLE_TURN_ORDER_OFFSET(entries[0].side);
            asm volatile("" : : "r"(entry_side_offset));
            entry_sides = entry_side_offset + battle_state_tail;
            entry_unit_offset = BATTLE_TURN_ORDER_OFFSET(entries[0].unit_slot);
            entry_units = entry_unit_offset + battle_state_tail;
            empty_entry_mask = BATTLE_TURN_ORDER_EMPTY_ENTRY;
            do {
                s32 entry_byte_offset;
                u8 *entry_side_address;
                u8 *entry_unit_address;

                tail_index_shifted >>= 16;
                entry_byte_offset = tail_index_shifted << 1;
                entry_side_address = (u8 *)((u32)entry_byte_offset + (u32)entry_sides);
                entry_unit_address = (u8 *)((u32)entry_byte_offset + (u32)entry_units);
                *entry_unit_address |= empty_entry_mask;
                *entry_side_address |= empty_entry_mask;
                tail_index_shifted++;
                tail_index_shifted <<= 16;
            } while ((tail_index_shifted >> 16) <= BATTLE_TURN_ORDER_ENTRY_COUNT - 1);
        }
    }
}
