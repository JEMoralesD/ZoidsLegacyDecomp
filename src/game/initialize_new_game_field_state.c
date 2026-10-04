#include "../field/field_actor.h"
#include "../events/event_script.h"
extern void RequestFieldMapChange(s32, s32, s32, s32) asm("func_0809E204");
extern void BiosCpuSet(void *, void *, s32) asm("func_80ECD2C");
extern s32 gPlayerCatalogFlags[] asm("D_020217B4");
extern u8 gFieldSaveState[] asm("D_0202ECF4");

void InitializeNewGameFieldState(void) asm("func_0809A048");

void InitializeNewGameFieldState(void) {
    s32 zero_fill = 0;
    u8 *field_state = gFieldSaveState;
    register s32 unset_map_id asm("r0");
    BiosCpuSet(&zero_fill, field_state, 0x05000073);
    field_state[FIELD_TRANSPORT_STATE_OFFSET(actor_model_id)] = 0x69;
    gPlayerCatalogFlags[4] |= 0x80000;
    M2C_FIELD(field_state, s32 *, EVENT_COMMAND_OFFSET(EventFieldMapStateView, saved_world_map_x_fixed8)) = 0xCB000;
    M2C_FIELD(field_state, s32 *, EVENT_COMMAND_OFFSET(EventFieldMapStateView, saved_world_map_y_fixed8)) = 0xF1000;
    RequestFieldMapChange(0, 0, 0, 0);
    unset_map_id = 0xFFFF;
    *(u16 *)(field_state + 0x26) = unset_map_id;
}
