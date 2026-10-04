#include "m2c_prelude.h"
#include "../event_script.h"

struct EventMapChangeCommand {
    u8 opcode;
    u8 map_id;
    u8 x;
    u8 y;
};

struct EventMapInfo {
    u8 pad00[0x1D];
    u8 map_group;
    u8 pad1E[2];
};

struct EventWorldMapState {
    u16 map_id;
    u8 pad02[0xA];
    s32 saved_world_map_x_fixed8;
    s32 saved_world_map_y_fixed8;
};

extern struct EventWorldMapState D_0202ECF4;
extern u8 gFieldEventActive asm("D_02030664");
extern struct EventMapInfo D_087C4434[];

void RequestFieldMapChange(u8, s32, s32, s32) asm("func_0809E204");

s32 EventChangeMap(s32 script_slot, struct EventMapChangeCommand **cursor) {
    register struct EventMapChangeCommand **source_slot asm("r4");
    register struct EventMapChangeCommand *initial_input asm("r1");
    s32 initial_map_id;
    struct EventMapChangeCommand *input;
    register s32 scale asm("r5");
    u32 map_group;

    source_slot = cursor;
    initial_input = *source_slot;
    initial_map_id = initial_input->map_id;
    scale = 1;
    if (initial_map_id == 0) {
        scale = 2;
    }

    map_group = D_087C4434[initial_input->map_id].map_group;
    if (map_group != 0) {
        struct EventWorldMapState *state;

        state = &D_0202ECF4;
        if (D_087C4434[state->map_id].map_group != map_group) {
            register u8 *position_base asm("r2");
            register s32 kind_index asm("r0");
            register s32 offset asm("r1");
            register s32 address asm("r0");
            register s32 opcode asm("r0");

            position_base = (u8 *)0x087AFBB4;
            asm volatile("" : "+r"(position_base));
            kind_index = map_group - 1;
            offset = kind_index << 1;
            offset += kind_index;
            offset <<= 1;
            address = (s32)position_base + 2;
            address = offset + address;
            opcode = *(s16 *)address;
            opcode <<= 11;
            state->saved_world_map_x_fixed8 = opcode;
            position_base += 4;
            offset += (s32)position_base;
            opcode = *(s16 *)offset;
            opcode <<= 11;
            state->saved_world_map_y_fixed8 = opcode;
        }
    }

    input = *source_slot;
    RequestFieldMapChange(input->map_id,
                  (input->x * scale) << 11,
                  (input->y * scale) << 11,
                  0);
    *source_slot = 0;
    gFieldEventActive = 0;
    return EVENT_YIELD;
}
