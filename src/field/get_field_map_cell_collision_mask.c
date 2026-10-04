#include "field_actor.h"
extern u16 IsFieldTerrainBlockedForActorModel(u8, u8) asm("func_080AB18C");
extern u16 gCurrentMapId asm("D_0202ECF4");
extern u16 gFieldMapDimensions[] asm("D_020324A4");
extern u8 *gFieldMapCells asm("D_02032E94");

u16 GetFieldMapCellCollisionMask(u8 model_id, u16 behavior, s32 cell_x, s32 cell_y) asm("func_080AB268");

u16 GetFieldMapCellCollisionMask(u8 model_id, u16 behavior, s32 cell_x, s32 cell_y) {
    if (gCurrentMapId != FIELD_MAP_ACTOR_MODEL_TERRAIN) {
        u16 width;
        if (cell_x < 0 || cell_y < 0) {
            return 1;
        }
        width = gFieldMapDimensions[0];
        if (cell_x >= width || cell_y >= gFieldMapDimensions[1]) {
            return 1;
        }
        {
            int collision_mask = FIELD_MAP_ALL_COLLISION;
            if (behavior == FIELD_ACTOR_PLAYER_CONTROLLED) {
                collision_mask = FIELD_MAP_COLLISION;
            }
            collision_mask &= gFieldMapCells[cell_x + width * cell_y];
            return collision_mask;
        }
    }
    return IsFieldTerrainBlockedForActorModel(model_id, gFieldMapCells[cell_x + cell_y * gFieldMapDimensions[0]]);
}
