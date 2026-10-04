#include "m2c_prelude.h"
#include "../event_script.h"
extern u8 gFieldSaveState[] asm("D_0202ECF4");
extern int CreateFieldActor() asm("func_080A9D78");
extern int SeekEventCommand() asm("func_080A016C");

s32 EventSpawnTransportActor(u8 script_slot, u8 **script_cursor) asm("func_080A4860");

s32 EventSpawnTransportActor(u8 script_slot, u8 **script_cursor) {
    u8 *command = *script_cursor;
    if (EVENT_COMMAND_BYTE(command, EventTransportActorCommand, actor_id) <= 0xC) {
        u8 cell_size_mode = *(u8 *)0x020316F4;
        int cell_pixels = 8;
        int actor_address;
        if (cell_size_mode == 0) cell_pixels = 16;
        actor_address = CreateFieldActor(gFieldSaveState[2], EVENT_COMMAND_BYTE(command, EventTransportActorCommand, actor_id), (EVENT_COMMAND_BYTE(command, EventTransportActorCommand, map_cell_x) * cell_pixels) << 8,
                            (EVENT_COMMAND_BYTE(command, EventTransportActorCommand, map_cell_y) * cell_pixels) << 8, EVENT_COMMAND_BYTE(command, EventTransportActorCommand, direction), 0, EVENT_COMMAND_BYTE(command, EventTransportActorCommand, behavior), 0);
        if (actor_address != 0 && EVENT_COMMAND_BYTE(*script_cursor, EventTransportActorCommand, behavior) == 0)
            *(s32 *)0x02032990 = actor_address;
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
