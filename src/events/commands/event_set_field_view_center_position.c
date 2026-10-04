#include "m2c_prelude.h"
#include "../event_script.h"
extern u8 gEventMapId;
extern u32 gFieldViewCenterFixed8[] asm("D_02032494");

void SeekEventCommand(s32, s32, s32) asm("func_80A016C");

s32 EventSetFieldViewCenterPosition(u8 script_slot, struct EventFieldViewCenterCommand **cursor) asm("func_080A21F4");

s32 EventSetFieldViewCenterPosition(u8 script_slot, struct EventFieldViewCenterCommand **cursor) {
    s32 cell_size_pixels;
    struct EventFieldViewCenterCommand *command;
    u8 map_id;
    u32 *view_center_fixed8;

    map_id = gEventMapId;
    cell_size_pixels = FIELD_MAP_CELL_PIXELS;
    if (map_id == 0)
        cell_size_pixels = FIELD_PACKED_MAP_CELL_PIXELS;
    view_center_fixed8 = gFieldViewCenterFixed8;
    command = *cursor;
    view_center_fixed8[0] = (command->map_cell_x * cell_size_pixels) << 8;
    view_center_fixed8[1] = (command->map_cell_y * cell_size_pixels) << 8;
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
