#include "player_selection.h"
void UpdateZoidSelectionStatusSprites(s32 visible_rows, s32 first_visible_index) asm("func_080ACBDC");

void UpdateZoidSelectionStatusSprites(s32 visible_rows, s32 first_visible_index) {
    volatile u32 visible_row_count;
    u8 visible_row;
    register u16 first_index_unsigned asm("r0");
    register s32 selection_index asm("r0");
    u8 stored_zoid_slot;
    s32 player_record_offset;
    u8 *zoid_bytes;
    struct PlayerSelectionSpriteFlagsView **new_marker_sprites;
    register s32 hidden_mask asm("r5");
    register s32 first_selection_index asm("r9");
    register struct PlayerSelectionSpriteFlagsView **team_marker_sprites asm("r12");
    register u8 *player_records_base asm("r10");
    register s32 visible_mask asm("r8");

    visible_rows <<= 24;
    visible_row_count = (u32)visible_rows >> 24;
    first_visible_index <<= 16;
    first_index_unsigned = (u32)first_visible_index >> 16;
    visible_row = 0;
    if (visible_row < visible_row_count) {
        first_selection_index = (s16)first_index_unsigned;
        {
            register struct PlayerSelectionSpriteFlagsView **team_marker_table asm("r2");

            team_marker_table = (struct PlayerSelectionSpriteFlagsView **)PLAYER_SELECTION_TEAM_SPRITES_RAM;
            asm volatile("" : "+r"(team_marker_table));
            team_marker_sprites = team_marker_table;
        }
        hidden_mask = PLAYER_SELECTION_SPRITE_HIDDEN;
        {
            register struct PlayerSelectionSpriteFlagsView **new_marker_table asm("r7");

            new_marker_table = (struct PlayerSelectionSpriteFlagsView **)PLAYER_SELECTION_NEW_SPRITES_RAM;
            new_marker_sprites = new_marker_table;
        }
        {
            register u8 *player_record_table asm("r6");

            player_record_table = (u8 *)PLAYER_ZOID_RECORDS_RAM;
            asm volatile("" : "+r"(player_record_table));
            player_records_base = player_record_table;
        }
        {
            register s32 visible_mask_value asm("r0");

            visible_mask_value = PLAYER_SELECTION_SPRITE_VISIBLE_MASK;
            visible_mask = visible_mask_value;
        }
        do {
            register s32 first_index_view asm("r1");

            first_index_view = first_selection_index;
            selection_index = first_index_view + visible_row;
            if (selection_index >= 0 && selection_index < ({
                register u8 *selection_count_ptr asm("r2");
                register s32 selection_count asm("r2");

                selection_count_ptr = (u8 *)PLAYER_ZOID_SELECTION_COUNT_RAM;
                selection_count = *selection_count_ptr;
                selection_count;
            })) {
                stored_zoid_slot = ({
                    register u8 *selection_slots asm("r6");
                    register u8 *selection_slot_address asm("r0");

                    selection_slots = (u8 *)PLAYER_ZOID_SELECTION_SLOTS_RAM;
                    selection_slot_address = (u8 *)selection_index;
                    asm volatile("" : "+r"(selection_slot_address));
                    selection_slot_address += (s32)selection_slots;
                    *selection_slot_address;
                });
                player_record_offset = stored_zoid_slot * 0x70;
                {
                    register u8 *player_records_address asm("r1");

                    player_records_address = player_records_base;
                    asm volatile("" : "+r"(player_records_address));
                    zoid_bytes = (u8 *)(player_record_offset + (s32)player_records_address);
                }
                {
                    register u16 record_flags asm("r1");

                    record_flags = M2C_FIELD(zoid_bytes, u16 *, PLAYER_ZOID_OFFSET(flags));
                    asm volatile("" : "+r"(record_flags));
                    if (record_flags & PLAYER_RECORD_IN_TEAM) {
                        struct PlayerSelectionSpriteFlagsView *marker_flags;
                        register struct PlayerSelectionSpriteFlagsView **marker_flags_address asm("r0");
                        s32 sprite_flags;
                        s32 marker_slot_offset;
                        register struct PlayerSelectionSpriteFlagsView **marker_table_view asm("r6");

                        marker_slot_offset = visible_row << 2;
                        marker_table_view = team_marker_sprites;
                        marker_flags_address = (struct PlayerSelectionSpriteFlagsView **)(marker_slot_offset + (s32)marker_table_view);
                        marker_flags = *marker_flags_address;
                        sprite_flags = marker_flags->flags;
                        sprite_flags &= visible_mask;
                        marker_flags->flags = sprite_flags;
                    } else {
                        struct PlayerSelectionSpriteFlagsView *marker_flags;
                        s32 sprite_flags;

                        marker_flags = team_marker_sprites[visible_row];
                        sprite_flags = marker_flags->flags;
                        sprite_flags |= hidden_mask;
                        marker_flags->flags = sprite_flags;
                    }
                }
                {
                    register u16 record_flags asm("r1");

                    record_flags = M2C_FIELD(zoid_bytes, u16 *, PLAYER_ZOID_OFFSET(flags));
                    asm volatile("" : "+r"(record_flags));
                    if (record_flags & PLAYER_RECORD_NEW) {
                        struct PlayerSelectionSpriteFlagsView *marker_flags;
                        s32 sprite_flags;

                        marker_flags = new_marker_sprites[visible_row];
                        sprite_flags = marker_flags->flags;
                        sprite_flags &= visible_mask;
                        marker_flags->flags = sprite_flags;
                    } else {
                        struct PlayerSelectionSpriteFlagsView *marker_flags;
                        s32 sprite_flags;

                        marker_flags = new_marker_sprites[visible_row];
                        sprite_flags = marker_flags->flags;
                        sprite_flags |= hidden_mask;
                        marker_flags->flags = sprite_flags;
                    }
                }
            } else {
                struct PlayerSelectionSpriteFlagsView *team_marker_flags;
                struct PlayerSelectionSpriteFlagsView *new_marker_flags;

                team_marker_flags = team_marker_sprites[visible_row];
                team_marker_flags->flags |= hidden_mask;
                new_marker_flags = new_marker_sprites[visible_row];
                new_marker_flags->flags |= hidden_mask;
            }
            visible_row++;
        } while (visible_row < visible_row_count);
    }
}
