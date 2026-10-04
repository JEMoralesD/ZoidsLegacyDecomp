#include "m2c_prelude.h"
#include "../event_script.h"
extern s8 gFieldEventActive asm("D_02030664");
extern s32 gFieldCameraMoveTargetFixed8[] asm("D_0203174C");
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern u8 gFieldCameraMoveDurationUpdates asm("D_02031754");
extern s8 gFieldCameraMoveActive asm("D_02031749");
extern s16 Sin256(int) asm("func_08092A90");
extern s16 Cos256(int) asm("func_08092ADC");
extern void StartTask(int, int) asm("func_08092D8C");
extern int SeekEventCommand(int, int, int) asm("func_080A016C");

s32 EventMoveFieldCameraInDirection(u8 script_slot, struct EventFieldCameraMoveCommand **cursor) asm("func_080A1D3C");

s32 EventMoveFieldCameraInDirection(u8 script_slot, struct EventFieldCameraMoveCommand **cursor) {
    struct EventFieldCameraMoveCommand *command;
    gFieldEventActive = 1;
    gFieldCameraMoveTargetFixed8[0] = gFieldCameraScrollOffsets[0] + (Sin256((*cursor)->direction << 5) * (((command = *cursor)->distance_pixels_high << 8) + command->distance_pixels_low));
    gFieldCameraMoveTargetFixed8[1] = gFieldCameraScrollOffsets[1] - (Cos256(command->direction << 5) * (((command = *cursor)->distance_pixels_high << 8) + command->distance_pixels_low));
    gFieldCameraMoveDurationUpdates = command->duration_updates;
    gFieldCameraMoveActive = 1;
    StartTask(FIELD_CAMERA_MOVE_TASK, FIELD_CAMERA_MOVE_TASK_THUMB);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
