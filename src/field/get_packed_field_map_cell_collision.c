#include "field_actor.h"
extern s32 gFieldMapCells asm("D_02032E94");
extern u16 gFieldMapDimensions asm("D_020324A4");
u16 IsFieldTerrainBlockedForActorModel(u8) asm("func_080AB18C");
u16 GetPackedFieldMapCellCollision(u8 model_id, s32 cell_x, s32 cell_y) asm("func_080AB224");

u16 GetPackedFieldMapCellCollision(u8 model_id, s32 cell_x, s32 cell_y) {
    s32 row_offset, map_base, cell_index, coordinate_mask;
    coordinate_mask = 0xFF;
    row_offset = cell_y & coordinate_mask;
    row_offset *= gFieldMapDimensions;
    row_offset = (s32)row_offset >> 1;
    map_base = gFieldMapCells;
    coordinate_mask = coordinate_mask & cell_x;
    cell_index = coordinate_mask + row_offset;
    if (FIELD_PACKED_MAP_COLLISION_BYPASS & *(u8 *)(map_base + cell_index)) {
        return 0;
    }
    /* R1 carries the sampled cell through the native one-argument call. */
    return IsFieldTerrainBlockedForActorModel(model_id);
}
