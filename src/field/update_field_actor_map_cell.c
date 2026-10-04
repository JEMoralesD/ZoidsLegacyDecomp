#include "field_actor.h"

extern s32 UsesUnshiftedFieldActorCollisionOrigin(struct FieldActor *) asm("func_080AC098");
extern u16 gCurrentMapId asm("D_0202ECF4");
extern u16 gFieldMapDimensions[] asm("D_020324A4");
extern u8 *gFieldMapCells asm("D_02032E94");

void UpdateFieldActorMapCell(struct FieldActor *actor) asm("func_080A98C4");

void UpdateFieldActorMapCell(struct FieldActor *actor)
{
    register s32 world_y_fixed8 asm("r1");
    register s32 x_coordinate_or_half_cell_limit asm("r2");
    s32 coordinate_work;
    s32 y_coordinate_or_origin_flag;
    s32 sample_row;
    s32 cell_y_or_map_base;
    s32 cell_x;
    u32 width_or_neighbor_index;
    u32 neighbor_row;
    u32 wrapped_neighbor_row;
    u8 first_cell;
    u32 second_cell;
    register u32 candidate_cell asm("r1");
    u8 nearby_cells[4];
    u32 next_x;
    u8 *cell_buffer;
    u16 *dimensions;

    if (gCurrentMapId == FIELD_MAP_PACKED_CELLS) {
        {
        register s32 world_x_fixed8 asm("r0");
        world_x_fixed8 = actor->world_x_fixed8;
        if (world_x_fixed8 < 0) {
            world_x_fixed8 += 0xFFF;
        }
        cell_x = (world_x_fixed8 >> 12) & 0xFF;
        }
        {
        register s32 world_y_fixed8 asm("r0");
        world_y_fixed8 = actor->world_y_fixed8;
        if (world_y_fixed8 < 0) {
            world_y_fixed8 += 0xFFF;
        }
        coordinate_work = (world_y_fixed8 >> 12) & 0xFF;
        }
        actor->map_cell_value = *(u8 *)(gFieldMapCells +
            (cell_x + (((u32)gFieldMapDimensions[0] * coordinate_work) >> 1)));
        return;
    }

    x_coordinate_or_half_cell_limit = actor->world_x_fixed8;
    coordinate_work = x_coordinate_or_half_cell_limit;
    if (x_coordinate_or_half_cell_limit < 0) {
        coordinate_work = x_coordinate_or_half_cell_limit + 0x7FF;
    }
    cell_x = coordinate_work >> 11;
    {
    register s32 remainder asm("r0");
    remainder = x_coordinate_or_half_cell_limit - (cell_x << 11);
    x_coordinate_or_half_cell_limit = 0x3FF;
    if (remainder <= x_coordinate_or_half_cell_limit) {
        cell_x--;
    }
    }
    world_y_fixed8 = actor->world_y_fixed8;
    y_coordinate_or_origin_flag = world_y_fixed8;
    if (world_y_fixed8 < 0) {
        y_coordinate_or_origin_flag = world_y_fixed8 + 0x7FF;
    }
    cell_y_or_map_base = y_coordinate_or_origin_flag >> 11;
    {
    register s32 remainder asm("r0");
    remainder = world_y_fixed8 - (cell_y_or_map_base << 11);
    if (remainder <= x_coordinate_or_half_cell_limit) {
        cell_y_or_map_base--;
    }
    }
    y_coordinate_or_origin_flag = UsesUnshiftedFieldActorCollisionOrigin(actor) << 24;
    sample_row = cell_y_or_map_base;
    asm volatile("" : "+r"(sample_row));
    if (y_coordinate_or_origin_flag == 0) {
        sample_row++;
    }
    cell_buffer = nearby_cells;
    dimensions = gFieldMapDimensions;
    {
    s32 row_offset;
    row_offset = dimensions[0] * sample_row;
    cell_y_or_map_base = (s32)gFieldMapCells;
    cell_buffer[0] = *(u8 *)((u8 *)cell_y_or_map_base + (cell_x + row_offset));
    }
    next_x = cell_x + 1;
    width_or_neighbor_index = dimensions[0];
    if (next_x < (u32)width_or_neighbor_index) {
        u8 *second_cell_buffer;
        second_cell_buffer = nearby_cells;
        width_or_neighbor_index *= sample_row;
        width_or_neighbor_index += cell_x;
        second_cell_buffer[1] = *(u8 *)(width_or_neighbor_index - (0 - (u32)cell_y_or_map_base) + 1);
        neighbor_row = sample_row + 1;
        if (neighbor_row < (u32)dimensions[1]) {
            u8 *lower_cell_buffer;
            lower_cell_buffer = nearby_cells;
            lower_cell_buffer[2] = *(u8 *)((u8 *)cell_y_or_map_base +
                (cell_x + (dimensions[0] * neighbor_row)));
            {
            u32 fourth_offset;
            fourth_offset = dimensions[0] * neighbor_row + cell_x;
            fourth_offset += (u32)cell_y_or_map_base;
            lower_cell_buffer[3] = *(u8 *)(fourth_offset + 1);
            }
        } else {
            nearby_cells[2] = *(u8 *)((u8 *)cell_y_or_map_base + cell_x);
            nearby_cells[3] = *(u8 *)((u8 *)cell_y_or_map_base + cell_x + 1);
        }
    } else {
        nearby_cells[1] = *(u8 *)((u8 *)cell_y_or_map_base + (width_or_neighbor_index * sample_row));
        wrapped_neighbor_row = sample_row + 1;
        if (wrapped_neighbor_row < (u32)dimensions[1]) {
            nearby_cells[2] = *(u8 *)((u8 *)cell_y_or_map_base +
                (cell_x + (dimensions[0] * wrapped_neighbor_row)));
            nearby_cells[3] = *(u8 *)((u8 *)cell_y_or_map_base + (dimensions[0] * wrapped_neighbor_row));
        } else {
            nearby_cells[2] = *(u8 *)((u8 *)cell_y_or_map_base + cell_x);
            nearby_cells[3] = *(u8 *)cell_y_or_map_base;
        }
    }
    {
    u8 *first_cell_buffer;
    u8 *comparison_cell_buffer;
    first_cell_buffer = nearby_cells;
    comparison_cell_buffer = nearby_cells;
    first_cell = first_cell_buffer[0];
    second_cell = comparison_cell_buffer[1];
    }
    candidate_cell = first_cell;
    if ((candidate_cell == second_cell) && (candidate_cell == nearby_cells[2]) &&
        (candidate_cell == nearby_cells[3])) {
        actor->map_cell_value = first_cell;
        return;
    }
    /* Mixed cells retain the previous value when all four have bit 0x40. */
    candidate_cell = nearby_cells[0];
    if (!(FIELD_MAP_NON_PLAYER_COLLISION & candidate_cell) || (candidate_cell = nearby_cells[1], ((FIELD_MAP_NON_PLAYER_COLLISION & candidate_cell) == 0)) ||
        (candidate_cell = nearby_cells[2], ((FIELD_MAP_NON_PLAYER_COLLISION & candidate_cell) == 0)) ||
        (candidate_cell = nearby_cells[3], ((FIELD_MAP_NON_PLAYER_COLLISION & candidate_cell) == 0))) {
        actor->map_cell_value = candidate_cell;
    }
}
