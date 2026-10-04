#include "../../field/field_actor.h"
#include "../event_script.h"

extern u8 FindFieldActorSlot(u8) asm("func_080A9EF0");
extern void StartTask(s32, s32) asm("func_08092D8C");
extern void SeekEventCommand(u8, s32, s32) asm("func_080A016C");

extern s8 gFieldEventActive asm("D_02030664");
extern s32 gFieldCameraMoveTargetFixed8[] asm("D_0203174C");
extern struct FieldActor gFieldActors[] asm("D_020325A0");
extern u16 gFieldMapDimensions[] asm("D_020324A4");
extern u8 gFieldCameraMoveDurationUpdates asm("D_02031754");
extern s8 gFieldCameraMoveActive asm("D_02031749");

s32 EventFocusFieldCameraOnActor(u8 script_slot, struct EventFieldCameraActorCommand **cursor) asm("func_080A1DE4");

s32 EventFocusFieldCameraOnActor(u8 script_slot, struct EventFieldCameraActorCommand **cursor) {
    s8 active;
    s32 target_x_fixed8;
    s32 camera_limit_fixed8;
    u8 actor_slot;

    gFieldEventActive = active = 1;
    actor_slot = FindFieldActorSlot((*cursor)->actor_id);
    if (actor_slot != FIELD_ACTOR_SLOT_NOT_FOUND) {
        gFieldCameraMoveTargetFixed8[0] = target_x_fixed8 = gFieldActors[actor_slot].world_x_fixed8 + 0xFFFF8800;
        gFieldCameraMoveTargetFixed8[1] = gFieldActors[actor_slot].world_y_fixed8 + 0xFFFFB000;
        if (target_x_fixed8 < 0) gFieldCameraMoveTargetFixed8[0] = 0;
        camera_limit_fixed8 = (gFieldMapDimensions[0] << 11) + 0xFFFF1000;
        if (gFieldCameraMoveTargetFixed8[0] > camera_limit_fixed8) gFieldCameraMoveTargetFixed8[0] = camera_limit_fixed8;
        if (gFieldCameraMoveTargetFixed8[1] < 0) gFieldCameraMoveTargetFixed8[1] = 0;
        camera_limit_fixed8 = (gFieldMapDimensions[1] << 11) + 0xFFFF6000;
        if (gFieldCameraMoveTargetFixed8[1] > camera_limit_fixed8) gFieldCameraMoveTargetFixed8[1] = camera_limit_fixed8;
        gFieldCameraMoveDurationUpdates = (*cursor)->duration_updates;
        gFieldCameraMoveActive = active;
        StartTask(FIELD_CAMERA_MOVE_TASK, FIELD_CAMERA_MOVE_TASK_THUMB);
    }
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
