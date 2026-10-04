#include "m2c_prelude.h"
#include "field_display.h"

extern u8 gFieldMapDefinitions[] asm("D_087C4434");
extern u8 gFieldMapModeFlags[] asm("D_020324B0");
extern u8 gFieldMapDimensions[] asm("D_020324A4");
extern volatile u16 gFieldMapState asm("D_0202ECF4");

u32 SelectFieldRandomEncounterTable(void) asm("func_0809E6E0");

u32 SelectFieldRandomEncounterTable(void) {
    register u32 encounter_table_index asm("r2");
    register s32 map_id_or_offset asm("r1");
    register u8 *encounter_table_bytes asm("r4");
    register u8 *selected_table_index_address asm("r5");
    register u32 *movement_counters_address asm("r6");
    register volatile u16 *current_map_id_address asm("r3");
    u32 current_map_id;

    asm volatile(""
                 : "=l"(current_map_id)
                 :
                 : "r0", "r1", "r2", "r3", "r4", "r5", "r6");
    encounter_table_index = 0;
    encounter_table_bytes = gFieldMapDefinitions - 0xBB0;
    map_id_or_offset = *(u16 *)encounter_table_bytes;
    selected_table_index_address = gFieldMapModeFlags + 7;
    movement_counters_address = (u32 *)(gFieldMapDimensions + 4);
    if (map_id_or_offset != 0) {
        current_map_id_address = &gFieldMapState;
        goto compare;
loop:
        map_id_or_offset = encounter_table_index + 1;
        map_id_or_offset <<= 24;
        encounter_table_index = (u32)map_id_or_offset >> 24;
        map_id_or_offset = encounter_table_index << 1;
        map_id_or_offset += encounter_table_index;
        map_id_or_offset <<= 1;
        map_id_or_offset += (s32)encounter_table_bytes;
        map_id_or_offset = *(u16 *)map_id_or_offset;
        if (map_id_or_offset == 0) {
            goto done;
        }
compare:
        asm volatile("ldrh %0, [%1, #0]"
                     : "+r"(current_map_id)
                     : "r"(current_map_id_address));
        if (map_id_or_offset != current_map_id) {
            goto loop;
        }
    }
done:
    *selected_table_index_address = encounter_table_index;
    map_id_or_offset = 0;
    *movement_counters_address = map_id_or_offset;
}
