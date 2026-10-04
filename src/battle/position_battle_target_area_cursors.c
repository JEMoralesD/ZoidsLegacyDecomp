#include "m2c_prelude.h"
#include "battle_display.h"

M2C_UNK MoveBattleSelectionCursorTo(s32, s16, s16) asm("func_080CC400");
void *AcquireEquipmentStatBuffer() asm("func_080E669C");
M2C_UNK ReleaseEquipmentStatBuffer() asm("func_080E66B8");
void BuildBattleEquipmentStats(u32, u32, u32, u32, void *) asm("func_080E8C90");
s32 DivideSigned32(s32, s32) asm("func_080ECD98");
s32 ModuloSigned32(s32, s32) asm("func_080ECE30");
u8 DivideUnsigned32(s32, s32) asm("func_080ECF00");
s32 ModuloUnsigned32(s32, s32) asm("func_080ECF78");

extern u8 gBattleState[];
extern s32 *gBattleSelectionCursorSprites[] asm("D_02033F58");

#define SET_TARGET_Y_FROM_COLUMN(column) target_choice_or_y_pixels = (u16) (((2 - (column)) << 5) + 0x18)
#define SET_TARGET_POSITION_FROM_CHOICE() do { target_x_pixels = ((1 - DivideSigned32(target_choice_or_y_pixels, 3)) << 5) + 0x18; SET_TARGET_Y_FROM_COLUMN(ModuloSigned32(target_choice_or_y_pixels, 3)); } while (0)
#define LOAD_SELECTED_TARGET_CHOICE() ({ register u8 *battle_state asm("r0"); register u32 selection_offset asm("r2"); register u8 *action_count_address asm("r1"); battle_state = gBattleState; selection_offset = BATTLE_SELECTION_OFFSET(action_count); action_count_address = battle_state + selection_offset; selection_offset += 4; battle_state += selection_offset; battle_state[*action_count_address]; })
void PositionBattleTargetAreaCursors(u8 target_side) asm("func_080CC49C");

void PositionBattleTargetAreaCursors(u8 target_side) {
    s32 *cursor_flags;
    s32 cursor_slot;
    register s32 target_column asm("r0");
    s32 target_choice_or_y_pixels;
    s16 target_x_pixels;
    u32 area_cursor_index;
    u8 area_type;
    void *equipment_stats;

    equipment_stats = AcquireEquipmentStatBuffer();
    {
        register u8 *battle_state asm("r2");
        register u32 selection_offset asm("r6");
        u32 action_index;

        BuildBattleEquipmentStats(*(u8 *)BATTLE_SCENE_SIDE_RAM, *(u8 *)BATTLE_SCENE_UNIT_SLOT_RAM,
                      ({ battle_state = gBattleState; selection_offset = BATTLE_SELECTION_OFFSET(action_count); action_index = *(battle_state + selection_offset); selection_offset += 1; battle_state += selection_offset; *(u8 *)(action_index + (u32) battle_state); }),
                      action_index, equipment_stats);
    }
    cursor_slot = 1;
position_next_area_cursor:
    cursor_flags = gBattleSelectionCursorSprites[cursor_slot];
    *cursor_flags &= ~BATTLE_SPRITE_BACKGROUND_RELATIVE;
    area_type = EQUIPMENT_RECORD_FIELD(equipment_stats, u8, area_type);
    if ((u32) area_type <= EQUIPMENT_AREA_SIDE) {
        goto select_area_shape;
    }
    goto mirror_player_side;
select_area_shape:
    switch (area_type) {
case EQUIPMENT_AREA_SINGLE:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_x_pixels = ((1 - DivideUnsigned32(target_choice_or_y_pixels, 3)) << 5) + 0x18;
    target_column = (u8) ModuloUnsigned32(target_choice_or_y_pixels, 3);
    asm("@ e1");
    SET_TARGET_Y_FROM_COLUMN(target_column);
    goto mirror_player_side;
case EQUIPMENT_AREA_COLUMN:
    if ((u8) cursor_slot != 1) {
        goto position_opposite_row;
    }
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_x_pixels = ((cursor_slot - DivideUnsigned32(target_choice_or_y_pixels, 3)) << 5) + 0x18;
    target_column = (u8) ModuloUnsigned32(target_choice_or_y_pixels, 3);
    asm("@ e2");
    SET_TARGET_Y_FROM_COLUMN(target_column);
    goto mirror_player_side;
position_opposite_row:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_choice_or_y_pixels += 3;
    SET_TARGET_POSITION_FROM_CHOICE();
    goto mirror_player_side;
case EQUIPMENT_AREA_ROW_PAIR:
    if ((u8) cursor_slot != 1) {
        goto position_second_in_row;
    }
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_x_pixels = ((cursor_slot - DivideUnsigned32(target_choice_or_y_pixels, 3)) << 5) + 0x18;
    target_column = (u8) ModuloUnsigned32(target_choice_or_y_pixels, 3);
    asm("@ e3");
    SET_TARGET_Y_FROM_COLUMN(target_column);
    goto mirror_player_side;
position_second_in_row:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_choice_or_y_pixels += 1;
    SET_TARGET_POSITION_FROM_CHOICE();
    goto mirror_player_side;
case EQUIPMENT_AREA_ROW:
    if ((u8) cursor_slot == 1) {
        goto position_first_in_row;
    }
    if (cursor_slot == 2) {
        goto position_second_column_cursor;
    }
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_choice_or_y_pixels += 2;
    SET_TARGET_POSITION_FROM_CHOICE();
    goto mirror_player_side;
position_first_in_row:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_x_pixels = ((cursor_slot - DivideUnsigned32(target_choice_or_y_pixels, 3)) << 5) + 0x18;
    target_column = (u8) ModuloUnsigned32(target_choice_or_y_pixels, 3);
    asm("@ e4");
    SET_TARGET_Y_FROM_COLUMN(target_column);
    goto mirror_player_side;
case EQUIPMENT_AREA_TWO_COLUMNS:
    if (cursor_slot == 2) {
        goto position_second_column_cursor;
    }
    if (cursor_slot > 2) {
        goto select_last_column_cursor;
    }
    if ((u8) cursor_slot == 1) {
        goto position_first_column_cursor;
    }
    goto position_opposite_row_second;
select_last_column_cursor:
    if (cursor_slot == 3) {
        goto position_opposite_row_first;
    }
    goto position_opposite_row_second;
position_first_column_cursor:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_x_pixels = ((cursor_slot - DivideUnsigned32(target_choice_or_y_pixels, 3)) << 5) + 0x18;
    target_column = (u8) ModuloUnsigned32(target_choice_or_y_pixels, 3);
    asm("@ e5");
    SET_TARGET_Y_FROM_COLUMN(target_column);
    goto mirror_player_side;
position_second_column_cursor:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_choice_or_y_pixels += 1;
    target_x_pixels = ((1 - DivideSigned32(target_choice_or_y_pixels, 3)) << 5) + 0x18;
    target_choice_or_y_pixels = (u16) (((cursor_slot - ModuloSigned32(target_choice_or_y_pixels, 3)) << 5) + 0x18);
    goto mirror_player_side;
position_opposite_row_first:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_choice_or_y_pixels += 3;
    SET_TARGET_POSITION_FROM_CHOICE();
    goto mirror_player_side;
position_opposite_row_second:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_choice_or_y_pixels += 4;
    SET_TARGET_POSITION_FROM_CHOICE();
    goto mirror_player_side;
case EQUIPMENT_AREA_SIDE:
    area_cursor_index = cursor_slot - 1;
    if (area_cursor_index > 4U) {
        goto position_sixth_side_cursor;
    }
    switch (area_cursor_index) {
case 0:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_x_pixels = ((1 - DivideUnsigned32(target_choice_or_y_pixels, 3)) << 5) + 0x18;
    target_column = (u8) ModuloUnsigned32(target_choice_or_y_pixels, 3);
    asm("@ e6");
    SET_TARGET_Y_FROM_COLUMN(target_column);
    goto mirror_player_side;
case 1:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_choice_or_y_pixels += 1;
    SET_TARGET_POSITION_FROM_CHOICE();
    goto mirror_player_side;
case 2:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_choice_or_y_pixels += 2;
    SET_TARGET_POSITION_FROM_CHOICE();
    goto mirror_player_side;
case 3:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_choice_or_y_pixels += 3;
    SET_TARGET_POSITION_FROM_CHOICE();
    goto mirror_player_side;
case 4:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_choice_or_y_pixels += 4;
    SET_TARGET_POSITION_FROM_CHOICE();
    goto mirror_player_side;
    }
position_sixth_side_cursor:
    target_choice_or_y_pixels = LOAD_SELECTED_TARGET_CHOICE();
    target_choice_or_y_pixels += 5;
    }
    SET_TARGET_POSITION_FROM_CHOICE();
mirror_player_side:
    if (target_side != 0) {
        goto set_cursor_destination;
    }
    target_x_pixels = 0x50 - (s16) target_x_pixels;
    { s32 mirrored_y_pixels = 0x70; mirrored_y_pixels -= (s16) target_choice_or_y_pixels; target_choice_or_y_pixels = (u16) mirrored_y_pixels; }
set_cursor_destination:
    MoveBattleSelectionCursorTo(cursor_slot, (s16) target_x_pixels, (s16) target_choice_or_y_pixels);
    cursor_slot = (s32) (u8) (cursor_slot + 1);
    if ((u32) cursor_slot > BATTLE_SELECTION_CURSOR_LAST) {
        goto release_equipment_stats;
    }
    goto position_next_area_cursor;
release_equipment_stats:
    ReleaseEquipmentStatBuffer();
    return;
}
