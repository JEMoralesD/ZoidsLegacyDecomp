#include "m2c_prelude.h"
#include "event_script.h"

extern unsigned char gEventMapId;
extern unsigned char gLoadedEventMapId;
extern int gEventScriptCursors;
extern int gEventScriptStarts;

void StartMapEventScript(int map_id, int script_start)
{
    unsigned char *map_id_slot = &gEventMapId;
    unsigned char *loaded_map_id = &gLoadedEventMapId;
    *loaded_map_id = map_id;
    *map_id_slot = map_id;
    {
        int *cursor = &gEventScriptCursors;
        int *restart = &gEventScriptStarts;
        *restart = script_start;
        *cursor = script_start;
    }
}
