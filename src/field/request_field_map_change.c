#include "m2c_prelude.h"
#include "field_display.h"
extern struct FieldMapChangePositionView gFieldMapChangePosition asm("D_0202ECF4");
extern s8 gFieldMapChangeRequest asm("D_020324B2");
void RequestFieldMapChange(s16 map_id, s32 center_x_fixed8, s32 center_y_fixed8, u8 request_kind) asm("func_0809E204");

void RequestFieldMapChange(s16 map_id, s32 center_x_fixed8, s32 center_y_fixed8, u8 request_kind) {
    gFieldMapChangeRequest = request_kind + 1;
    *(s16 *)FIELD_REQUESTED_MAP_ID_RAM = map_id;
    gFieldMapChangePosition.center_x_fixed8 = center_x_fixed8;
    gFieldMapChangePosition.center_y_fixed8 = center_y_fixed8;
}
