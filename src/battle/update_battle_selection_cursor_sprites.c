#include "m2c_prelude.h"
#include "battle_display.h"

void UpdateBattleSelectionCursorSprites(void) asm("func_080CC8B0");

void UpdateBattleSelectionCursorSprites(void)
{
    u8 *movement_progress;
    struct BattleSelectionCursorSignedPositionView **cursor_sprites;
    s16 *start_positions;
    s16 *target_positions;
    u8 cursor_mode;
    s32 scaled_delta;
    u8 cursor_slot;
    struct BattleSelectionCursorSignedPositionView *cursor_sprite_for_x;

    {
        u8 *lead_progress_address = (u8 *)BATTLE_SELECTION_CURSOR_PROGRESS_RAM;
        u8 progress = lead_progress_address[0];

        movement_progress = lead_progress_address;
        if (progress < BATTLE_SELECTION_CURSOR_MOVE_UPDATES)
            movement_progress[0] = progress + 1;
    }

    {
        struct BattleSelectionCursorSignedPositionView **cursor_sprite_table = (struct BattleSelectionCursorSignedPositionView **)BATTLE_SELECTION_CURSOR_SPRITES_RAM;
        struct BattleSelectionCursorSignedPositionView *lead_sprite_for_x = cursor_sprite_table[0];
        s16 *start_position_table;
        s16 *target_position_table;

        asm volatile("" : "+r"(lead_sprite_for_x));
        start_position_table = (s16 *)BATTLE_SELECTION_CURSOR_START_POSITIONS_RAM;
        target_position_table = (s16 *)BATTLE_SELECTION_CURSOR_TARGET_POSITIONS_RAM;

        scaled_delta = (target_position_table[0] - start_position_table[0]) * movement_progress[0];
        asm volatile("" :: "r"(start_position_table), "r"(target_position_table));
        cursor_sprites = cursor_sprite_table;
        start_positions = start_position_table;
        target_positions = target_position_table;
        asm volatile("" :: "r"(start_positions));
        asm volatile("" :: "r"(target_positions));
        asm volatile("" :: "r"(target_positions));
        if (scaled_delta < 0)
            scaled_delta += 7;
        lead_sprite_for_x->x = start_positions[0] + (scaled_delta >> 3);
    }
    {
        register struct BattleSelectionCursorSignedPositionView *lead_sprite_for_y asm("r5") =
            *(struct BattleSelectionCursorSignedPositionView *volatile *)cursor_sprites;

        scaled_delta = (target_positions[1] - start_positions[1]) * movement_progress[0];
        if (scaled_delta < 0)
            scaled_delta += 7;
        {
            s32 delta_pixels = scaled_delta >> 3;
            register u32 start_positions_address_or_y_bits asm("r1") = (u32)start_positions;
            start_positions_address_or_y_bits = *(u16 *)(start_positions_address_or_y_bits + 2);
            lead_sprite_for_y->y = (u32)((u8 *)delta_pixels + start_positions_address_or_y_bits);
            asm volatile("" :: "r"(start_positions_address_or_y_bits));
        }
    }

    cursor_mode = *(u8 *)BATTLE_SELECTION_CURSOR_MODE_RAM;
    switch (cursor_mode) {
    case BATTLE_SELECTION_CURSOR_TRAIL: {
        register struct BattleSelectionCursorSignedPositionView **trail_sprites asm("r6");

        cursor_slot = BATTLE_SELECTION_CURSOR_LAST;
        trail_sprites = cursor_sprites;
        asm volatile("" : "+r"(trail_sprites));
        do {
            register s32 previous_cursor_slot asm("r3");
            register struct BattleSelectionCursorSignedPositionView **current_cursor_ref asm("r2");
            register struct BattleSelectionCursorSignedPositionView **previous_cursor_ref asm("r1");
            struct BattleSelectionCursorSignedPositionView *current_cursor;

            current_cursor_ref = (struct BattleSelectionCursorSignedPositionView **)(cursor_slot * 4 + (u32)trail_sprites);
            current_cursor = *(struct BattleSelectionCursorSignedPositionView *volatile *)current_cursor_ref;
            previous_cursor_slot = cursor_slot - 1;
            asm volatile("" : "+r"(previous_cursor_slot));
            previous_cursor_ref =
                (struct BattleSelectionCursorSignedPositionView **)(previous_cursor_slot * 4 + (u32)trail_sprites);
            current_cursor->x = (*(struct BattleSelectionCursorSignedPositionView *volatile *)previous_cursor_ref)->x;
            (*(struct BattleSelectionCursorSignedPositionView *volatile *)current_cursor_ref)->y =
                (*(struct BattleSelectionCursorSignedPositionView *volatile *)previous_cursor_ref)->y;
            cursor_slot = previous_cursor_slot;
        } while (cursor_slot != 0);
        asm volatile("" :: "r"(cursor_slot), "r"(cursor_slot), "r"(cursor_slot), "r"(cursor_slot));
        break;
    }
    case BATTLE_SELECTION_CURSOR_TARGET_AREA:
        for (cursor_slot = 1; cursor_slot <= BATTLE_SELECTION_CURSOR_LAST; cursor_slot++) {
            register u8 *progress_address asm("r4");
            u8 progress;
            {
                register u8 *progress_table asm("r2") = movement_progress;
                asm volatile("" : "+r"(progress_table));
                progress_address = (u8 *)((u32)cursor_slot + (u32)progress_table);
            }
            progress = *progress_address;
            if (progress < BATTLE_SELECTION_CURSOR_MOVE_UPDATES)
                *progress_address = progress + 1;
            {
                register u32 position_offset asm("r2");
                register struct BattleSelectionCursorSignedPositionView **cursor_sprite_ref asm("r6");
                struct BattleSelectionCursorSignedPositionView *cursor_sprite_for_y;

                position_offset = cursor_slot * 4;
                asm volatile("" : "+r"(position_offset));
                {
                    register u32 r0_allocation_guard asm("r0");
                    register u32 r1_allocation_guard asm("r1");
                    asm volatile("" : "=r"(r0_allocation_guard), "=r"(r1_allocation_guard));
                    cursor_sprite_ref = (struct BattleSelectionCursorSignedPositionView **)(position_offset + (u32)cursor_sprites);
                    asm volatile("" :: "r"(r0_allocation_guard), "r"(r1_allocation_guard));
                }
                asm volatile("" : "+r"(cursor_sprite_ref));
                cursor_sprite_for_x = *(struct BattleSelectionCursorSignedPositionView *volatile *)cursor_sprite_ref;
                {
                    register s16 *start_coordinate asm("r3");
                    register s16 *target_coordinate asm("r0");
                    register s32 coordinate_delta asm("r1");

                    start_coordinate = (s16 *)(position_offset + (u32)start_positions);
                    target_coordinate = (s16 *)(position_offset + (u32)target_positions);
                    coordinate_delta = *target_coordinate;
                    asm volatile("" : "+r"(coordinate_delta));
                    coordinate_delta -= *start_coordinate;
                    scaled_delta = coordinate_delta * *progress_address;
                    if (scaled_delta < 0)
                        scaled_delta += 7;
                    cursor_sprite_for_x->x = *start_coordinate + (scaled_delta >> 3);
                }
                cursor_sprite_for_y = *(struct BattleSelectionCursorSignedPositionView *volatile *)cursor_sprite_ref;
                {
                    register s16 *start_coordinate asm("r3");
                    register u32 position_table_address asm("r0");

                    position_table_address = (u32)start_positions;
                    asm volatile("" : "+r"(position_table_address));
                    position_table_address += 2;
                    asm volatile("" : "+r"(position_table_address));
                    start_coordinate = (s16 *)(position_offset + position_table_address);
                    position_table_address = (u32)target_positions;
                    asm volatile("" : "+r"(position_table_address));
                    position_table_address += 2;
                    asm volatile("" : "+r"(position_table_address));
                    position_table_address = position_offset + position_table_address;
                    asm volatile("" : "+r"(position_table_address));
                    scaled_delta = (*(s16 *)position_table_address - *start_coordinate) * *progress_address;
                    if (scaled_delta < 0)
                        scaled_delta += 7;
                    cursor_sprite_for_y->y = *start_coordinate + (scaled_delta >> 3);
                }
            }
        }
        break;
    default:
        break;
    }
}
