#include "field_actor.h"
extern u8 gFieldMapModeFlags asm("D_020324B0");

s32 IsFieldTerrainBlockedForActorModel(u8 model_id, u16 cell) asm("func_080AB18C");

s32 IsFieldTerrainBlockedForActorModel(u8 model_id, u16 cell) {
    if (gFieldMapModeFlags & FIELD_MAP_ENABLE_CONDITIONAL_COLLISION) {
        if (cell & FIELD_MAP_CONDITIONAL_COLLISION) {
            return 1;
        }
    }
    switch (cell & FIELD_MAP_TERRAIN_TYPE_MASK) {
        case 4:
        case 6:
        case 7:
        case 9:
            if ((u8)(model_id - 0x69) <= 2) {
                return 1;
            }
            return 0;
        case 8:
        case 10:
            if (model_id == 0x69) {
                return 1;
            }
            return 0;
        case 11:
            if ((u8)(model_id - 0x69) <= 1) {
                return 1;
            }
            return 0;
        case 0:
        case 12:
        default:
            return 0;
    }
}
