#include "m2c_prelude.h"
#include "battle_turn_order.h"

extern u8 gBattleState[];

#define BATTLE_TURN_ORDER_COUNT (*(u8 *)0x0203725D)

extern u8 FindBattleEffect(u32, u32, s32) asm("func_080BF464");
extern void SetBattleTurnOrderEntry(s32, s32, s32, s32) asm("func_080C007C");

void InsertBattleUnitTurnOrderEntries(u32 side, u32 unit_slot) asm("func_080C00B0");

void InsertBattleUnitTurnOrderEntries(u32 side, u32 unit_slot)
{
    register u32 side_byte asm("r8");
    register u32 extra_turn_effect_slot asm("r5");
    register u32 unit_slot_times_four asm("r3");
    register u32 side_times_four asm("r2");
    register u32 priority asm("r1");
    register u32 turn_number asm("r0");
    struct BattleTurnInsertionFrame {
        u32 unit_slot;
        u32 turn_count;
        u32 order_mode;
        u32 next_turn_number;
    } frame;

    side_byte = (u8)side;
    frame.unit_slot = (u8)unit_slot;
    extra_turn_effect_slot = (u8)FindBattleEffect(side_byte, frame.unit_slot, BATTLE_EFFECT_EXTRA_TURNS);
    if (extra_turn_effect_slot == BATTLE_EFFECT_NOT_FOUND) {
        register u32 side_copy asm("r4");
        register u32 unit_slot_copy asm("r1");

        frame.turn_count = 1;
        unit_slot_copy = frame.unit_slot;
        unit_slot_times_four = unit_slot_copy << 2;
        side_copy = side_byte;
        asm("" : "+r"(side_copy));
        side_times_four = side_copy << 2;
    } else {
        register u8 *battle_state_base asm("r4");
        register u32 offset asm("r1");
        register u32 unit_slot_value asm("r5");
        register u32 side_copy asm("r7");
        u32 unit_record_offset;
        u32 side_record_offset;

        battle_state_base = gBattleState;
        offset = extra_turn_effect_slot * 12;
        unit_slot_value = frame.unit_slot;
        unit_slot_times_four = unit_slot_value << 2;
        unit_record_offset = ((unit_slot_times_four + unit_slot_value) * 8 - unit_slot_value) << 4;
        offset += unit_record_offset;
        side_copy = side_byte;
        side_times_four = side_copy << 2;
        side_record_offset = ((side_times_four + side_copy) * 8 - side_copy) << 7;
        offset += side_record_offset;
        offset += (u32)battle_state_base;
        frame.turn_count = (u8)(*(u8 *)(offset + BATTLE_UNIT_OFFSET(effects[0].value)) + 1);
    }
    {
        register u32 unit_slot_value asm("r0");
        register u32 address asm("r1");
        register u32 side_copy asm("r3");
        register u32 side_record_offset asm("r0");
        register u8 *battle_state_base asm("r4");

        unit_slot_value = frame.unit_slot;
        address = ((unit_slot_times_four + unit_slot_value) * 8 - unit_slot_value) << 4;
        side_copy = side_byte;
        side_record_offset = ((side_times_four + side_copy) * 8 - side_copy) << 7;
        address += side_record_offset;
        battle_state_base = gBattleState;
        address += (u32)battle_state_base;
        priority = *(u16 *)(address + BATTLE_UNIT_OFFSET(initiative));
    }
    turn_number = 0;
    goto pass_test;

pass_loop:
    {
        register u32 index asm("r5");
        register s32 priority_shifted asm("r9");
        register u8 *battle_state asm("r6");
        register u8 *order_mode_address asm("sl");
        register u8 *entry_sides asm("ip");
        register u32 write_index asm("r0");
        u8 *entry_units_or_count;

        index = 0;
        priority_shifted = priority << 16;
        frame.next_turn_number = turn_number + 1;
        entry_units_or_count = (u8 *)0x0203725D;
        entry_units_or_count = (u8 *)(u32)*entry_units_or_count;
        if (index >= (u32)entry_units_or_count)
            goto found;

        battle_state = gBattleState;
        {
            register u32 order_mode_offset asm("r0");
            register u32 order_mode asm("r1");
            register u32 entry_side_offset asm("r2");
            register u32 entry_unit_offset asm("r3");

            order_mode_offset = BATTLE_TURN_ORDER_OFFSET(mode);
            order_mode_address = battle_state + order_mode_offset;
            order_mode = *order_mode_address;
            frame.order_mode = order_mode;
            entry_side_offset = BATTLE_TURN_ORDER_OFFSET(entries[0].side);
            asm("" : "+r"(entry_side_offset));
            entry_side_offset += (u32)battle_state;
            entry_sides = (u8 *)entry_side_offset;
            entry_unit_offset = BATTLE_TURN_ORDER_OFFSET(entries[0].unit_slot);
            asm("" : "+r"(entry_unit_offset));
            entry_units_or_count = (u8 *)((u32)battle_state - (0 - entry_unit_offset));
        }

scan:
        {
            u32 offset = index << 1;
            register u32 previous_priority asm("r4");
            register u32 priority_offset asm("r4");
            register u8 *priority_address asm("r0");

            priority_offset = BATTLE_TURN_ORDER_OFFSET(priorities);
            asm("" : "+r"(priority_offset));
            priority_address = battle_state + priority_offset;
            priority_address = (u8 *)(offset - (0 - (u32)priority_address));
            previous_priority = *(u16 *)priority_address;

            if (frame.order_mode == BATTLE_TURN_ORDER_DESCENDING_INITIATIVE) {
                register u32 entry_side asm("r3");

                entry_side = *(u8 *)(offset - (0 - (u32)entry_sides));

                if (entry_side != BATTLE_TURN_ORDER_EMPTY_ENTRY ||
                    *(u8 *)(offset - (0 - (u32)entry_units_or_count)) != BATTLE_TURN_ORDER_EMPTY_ENTRY) {
                    register s32 signed_priority asm("r2");
                    register s32 previous_signed_priority asm("r0");

                    signed_priority = priority_shifted >> 16;
                    previous_signed_priority = (s16)previous_priority;
                    if (signed_priority > previous_signed_priority)
                        goto found;
                    if (signed_priority == previous_signed_priority &&
                        side_byte == battle_state[BATTLE_TURN_ORDER_OFFSET(tie_preferred_side)] && entry_side != side_byte)
                        goto found;
                }
            }
            {
                register u8 *order_mode_copy asm("r3");

                order_mode_copy = order_mode_address;
                if (*order_mode_copy == BATTLE_TURN_ORDER_ASCENDING_INITIATIVE) {
                    register u32 entry_side asm("r3");

                    entry_side = *(u8 *)(offset - (0 - (u32)entry_sides));

                    if (entry_side != BATTLE_TURN_ORDER_EMPTY_ENTRY ||
                        *(u8 *)(offset - (0 - (u32)entry_units_or_count)) != BATTLE_TURN_ORDER_EMPTY_ENTRY) {
                        register s32 signed_priority asm("r2");
                        register s32 previous_signed_priority asm("r0");

                        signed_priority = priority_shifted >> 16;
                        previous_signed_priority = (s16)previous_priority;
                        if (signed_priority < previous_signed_priority)
                            goto found;
                        if (signed_priority == previous_signed_priority) {
                            register u8 *tie_state_base asm("r2");
                            register u32 tie_side_offset asm("r4");
                            register u32 tie_side asm("r0");
                            register u8 *tie_side_address asm("r0");

                            tie_state_base = gBattleState;
                            tie_side_offset = BATTLE_TURN_ORDER_OFFSET(tie_preferred_side);
                            asm("" : "+r"(tie_side_offset));
                            tie_side_address = (u8 *)((u32)tie_state_base -
                                (0 - tie_side_offset));
                            tie_side = *tie_side_address;
                            if (side_byte != tie_side && entry_side == tie_side)
                                goto found;
                        }
                    }
                }
            }
            {
                register u8 *order_mode_copy asm("r2");

                order_mode_copy = order_mode_address;
                if (*order_mode_copy == BATTLE_TURN_ORDER_FIRST_EMPTY_ENTRY) {
                    register u8 *entry_sides_copy asm("r3");
                    register u8 *entry_side_address asm("r0");
                    register u32 entry_side asm("r0");

                    entry_sides_copy = entry_sides;
                    asm("" : "+r"(entry_sides_copy));
                    entry_side_address = (u8 *)(offset + (u32)entry_sides_copy);
                    entry_side = *entry_side_address;

                    if (entry_side == BATTLE_TURN_ORDER_EMPTY_ENTRY &&
                        *(u8 *)(offset - (0 - (u32)entry_units_or_count)) == BATTLE_TURN_ORDER_EMPTY_ENTRY)
                        goto found;
                }
            }
        }
        {
            register u32 next_index asm("r0");

            next_index = index + 1;
            index = (u8)next_index;
        }
        {
            register u32 next_count asm("r4");

            next_count = 0x0203725D;
            next_count = *(u8 *)next_count;
            if (index < next_count)
                goto scan;
        }

found:
        asm("" : "+r"(index));
        {
            register u8 *insertion_state asm("r7");
            register u32 insertion_mode_offset asm("r0");
            register u8 *insertion_mode_address asm("r1");
            register u8 *count_address asm("r2");
            register u32 shift_count asm("r0");

            insertion_state = gBattleState;
            insertion_mode_offset = BATTLE_TURN_ORDER_OFFSET(mode);
            asm("" : "+r"(insertion_mode_offset));
            insertion_mode_address = (u8 *)((u32)insertion_state -
                (0 - insertion_mode_offset));
            if (*insertion_mode_address != BATTLE_TURN_ORDER_FIRST_EMPTY_ENTRY) {
                count_address = (u8 *)0x0203725D;
                shift_count = *count_address;
                write_index = shift_count;
                if (shift_count <= index)
                    goto write_entry;
                {
                    register u8 *shift_sides asm("ip");
                    register u8 *shift_units asm("sl");
                    register u8 *shift_priorities asm("r6");
                    register u8 *entry_sides_seed asm("r3");
                    register u8 *entry_units_seed asm("r4");
                    u32 cursor;

                    entry_sides_seed = insertion_mode_address + 4;
                    asm("" : "+r"(entry_sides_seed));
                    shift_sides = entry_sides_seed;
                    entry_units_seed = insertion_mode_address + 5;
                    asm("" : "+r"(entry_units_seed));
                    shift_units = entry_units_seed;
                    shift_priorities = insertion_mode_address;
                    shift_priorities += BATTLE_TURN_ORDER_PRIORITIES_FROM_MODE;
                    cursor = shift_count;
shift:
                    {
                        register u32 destination asm("r2");
                        register u32 previous asm("r4");
                        register u32 source asm("r1");

                        destination = cursor << 1;
                        {
                            register u8 *entry_sides_copy asm("r7");
                            register u8 *destination_address asm("r3");
                            register u8 *source_address asm("r0");

                            entry_sides_copy = shift_sides;
                            destination_address = (u8 *)(destination +
                                (u32)entry_sides_copy);
                            asm("" : "+r"(destination_address));
                            previous = cursor - 1;
                            source = previous << 1;
                            source_address = (u8 *)(source -
                                (0 - (u32)entry_sides_copy));
                            *destination_address = *source_address;
                        }
                        {
                            register u8 *entry_units_copy asm("r0");
                            register u8 *destination_address asm("r3");

                            entry_units_copy = shift_units;
                            destination_address = (u8 *)(destination -
                                (0 - (u32)entry_units_copy));
                            entry_units_copy = (u8 *)(source -
                                (0 - (u32)entry_units_copy));
                            *destination_address = *entry_units_copy;
                        }
                        destination += (u32)shift_priorities;
                        source += (u32)shift_priorities;
                        *(u16 *)destination = *(u16 *)source;
                        cursor = (u8)previous;
                    }
                    if (cursor > index)
                        goto shift;
                    write_index = cursor;
                    goto write_entry;
                }
            }
            write_index = index;
        }

write_entry:
        asm("" : "+r"(priority_shifted));
        {
            register s32 signed_priority asm("r4");
            register u32 call_side asm("r1");
            register u32 call_unit_slot asm("r2");
            register u32 sign_copy asm("r3");
            register u32 sign asm("r0");

            signed_priority = priority_shifted >> 16;
            call_side = side_byte;
            asm("" : "+r"(call_side));
            call_unit_slot = frame.unit_slot;
            SetBattleTurnOrderEntry(write_index, call_side, call_unit_slot, signed_priority);
            {
                register u8 *count_ptr asm("r2");
                register u32 count asm("r0");

                count_ptr = (u8 *)0x0203725D;
                count = *count_ptr;
                count++;
                *count_ptr = count;
            }
            sign_copy = priority_shifted;
            asm("" : "+r"(sign_copy));
            sign = sign_copy >> 31;
            signed_priority += sign;
            signed_priority = (u32)signed_priority << 15;
            priority = (u32)signed_priority >> 16;
        }
        {
            register u32 next_turn_number asm("r4");

            next_turn_number = frame.next_turn_number;
            turn_number = (u8)next_turn_number;
        }
    }

pass_test:
    {
        register u32 turn_count asm("r5");

        turn_count = frame.turn_count;
        if (turn_number < turn_count)
            goto pass_loop;
    }
}
