#include "player_selection.h"
void UpdateBattleUnitSelectionStatusSprites(s32 row_count, s32 first_visible_row) asm("func_080B43B4");

void UpdateBattleUnitSelectionStatusSprites(s32 row_count, s32 first_visible_row) {
    volatile u32 visible_row_count;
    u8 index;
    register u16 start_input asm("r0");
    register s32 position asm("r0");
    u8 unit_index;
    s32 unit_offset;
    u8 *unit_record;
    s32 **new_markers;
    register s32 hidden_flag asm("r5");
    register s32 start asm("r9");
    register s32 **team_markers asm("r12");
    register u8 *battle_units asm("r10");
    register s32 visible_mask asm("r8");

    row_count <<= 24;
    visible_row_count = (u32)row_count >> 24;
    first_visible_row <<= 16;
    start_input = (u32)first_visible_row >> 16;
    index = 0;
    if (index < visible_row_count) {
        start = (s16)start_input;
        {
            register s32 **first_input asm("r2");

            first_input = (s32 **)PLAYER_SELECTION_TEAM_SPRITES_RAM;
            asm volatile("" : "+r"(first_input));
            team_markers = first_input;
        }
        hidden_flag = PLAYER_SELECTION_SPRITE_HIDDEN;
        {
            register s32 **second_input asm("r7");

            second_input = (s32 **)PLAYER_SELECTION_NEW_SPRITES_RAM;
            new_markers = second_input;
        }
        {
            register u8 *record_input asm("r6");

            record_input = (u8 *)0x02034B4C;
            asm volatile("" : "+r"(record_input));
            battle_units = record_input;
        }
        {
            register s32 clear_input asm("r0");

            clear_input = PLAYER_SELECTION_SPRITE_VISIBLE_MASK;
            visible_mask = clear_input;
        }
        do {
            register s32 start_view asm("r1");

            start_view = start;
            position = start_view + index;
            if (position >= 0 && position < ({
                register u8 *bound_ptr asm("r2");
                register s32 bound asm("r2");

                bound_ptr = (u8 *)PLAYER_ZOID_SELECTION_COUNT_RAM;
                bound = *bound_ptr;
                bound;
            })) {
                unit_index = ({
                    register u8 *lookup asm("r6");
                    register u8 *lookup_address asm("r0");

                    lookup = (u8 *)PLAYER_ZOID_SELECTION_SLOTS_RAM;
                    lookup_address = (u8 *)position;
                    asm volatile("" : "+r"(lookup_address));
                    lookup_address += (s32)lookup;
                    *lookup_address;
                });
                unit_offset = unit_index * sizeof(struct BattleUnit);
                {
                    register u8 *base_view asm("r1");

                    base_view = battle_units;
                    asm volatile("" : "+r"(base_view));
                    unit_record = (u8 *)(unit_offset + (s32)base_view);
                }
                {
                    register u16 flags asm("r1");

                    flags = BATTLE_UNIT_FIELD(unit_record, u16, flags);
                    asm volatile("" : "+r"(flags));
                    if (flags & PLAYER_RECORD_IN_TEAM) {
                        s32 *slot;
                        register s32 **slot_address asm("r0");
                        s32 value;
                        s32 offset;
                        register s32 **table_view asm("r6");

                        offset = index << 2;
                        table_view = team_markers;
                        slot_address = (s32 **)(offset + (s32)table_view);
                        slot = *slot_address;
                        value = *slot;
                        value &= visible_mask;
                        *slot = value;
                    } else {
                        s32 *slot;
                        s32 value;

                        slot = team_markers[index];
                        value = *slot;
                        value |= hidden_flag;
                        *slot = value;
                    }
                }
                {
                    register u16 flags asm("r1");

                    flags = BATTLE_UNIT_FIELD(unit_record, u16, flags);
                    asm volatile("" : "+r"(flags));
                    if (flags & PLAYER_RECORD_NEW) {
                        s32 *slot;
                        s32 value;

                        slot = new_markers[index];
                        value = *slot;
                        value &= visible_mask;
                        *slot = value;
                    } else {
                        s32 *slot;
                        s32 value;

                        slot = new_markers[index];
                        value = *slot;
                        value |= hidden_flag;
                        *slot = value;
                    }
                }
            } else {
                s32 *first_slot;
                s32 *second_slot;

                first_slot = team_markers[index];
                *first_slot |= hidden_flag;
                second_slot = new_markers[index];
                *second_slot |= hidden_flag;
            }
            index++;
        } while (index < visible_row_count);
    }
}
