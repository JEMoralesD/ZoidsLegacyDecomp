#include "m2c_prelude.h"
#include "../../field/field_display.h"
#include "../../game/game_state.h"
extern void SeekEventCommand() asm("func_80A016C");
extern void BiosLz77ToVram() asm("func_80ECD34");
extern u8 gFieldTravelState[] asm("D_0202ECF4");

s32 EventShowWorldMapBg1Overlay(u8 script_slot) asm("func_080A6034");

s32 EventShowWorldMapBg1Overlay(u8 script_slot) {
    u8 t;
    s32 *p;
    if (*(s32 *)0x02021690 == GAME_MODE_FIELD) {
        t = *(u8 *)0x020316F4;
        if (t == 0) {
            BiosLz77ToVram(WORLD_MAP_BG1_SLIDE_GRAPHICS_ROM, 0x0600C000);
            BiosLz77ToVram(WORLD_MAP_BG1_SLIDE_TILEMAP_ROM, 0x06000800);
            p = (s32 *)0x03000054;
            p[2] = t;
            p[3] = 0xFFFFC000;
            *(u8 *)0x020324B0 = (0xFC & *(u8 *)0x020324B0) | 2;
        }
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}

s32 EventClearFieldTravelRestriction(u8 script_slot) asm("func_080A60A8");

s32 EventClearFieldTravelRestriction(u8 script_slot) {
    gFieldTravelState[FIELD_TRAVEL_RESTRICTIONS_BYTE] &= FIELD_CLEAR_TRAVEL_RESTRICTION_MASK;
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
