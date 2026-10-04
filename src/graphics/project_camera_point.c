#include "m2c_prelude.h"
#include "camera.h"

extern u8 gPerspectiveCamera[] asm("D_030033C4");

s32 TransformVector3Fixed8(void *, s32 *, s32 *) asm("func_0809370C");
s32 BiosDiv(s32, s32) asm("func_80ECD30");

s32 ProjectCameraPoint(s32 *world_position, s16 *screen_position, s8 *visible) asm("func_08093E30");

s32 ProjectCameraPoint(s32 *world_position, s16 *screen_position, s8 *visible) {
    s32 input[3];
    s32 transformed[3];
    u8 *camera;
    register s32 result asm("r0");
    s32 camera_depth;
    s32 focal_length;

    camera = gPerspectiveCamera;
    input[0] = world_position[0] - CAMERA_FIELD(camera, s32, pose.position.world_x);
    input[1] = world_position[1];
    input[2] = CAMERA_FIELD(camera, s32, pose.position.world_z) - world_position[2];
    result = TransformVector3Fixed8(camera + 0x40, input, transformed);
    camera_depth = transformed[2] + CAMERA_FIELD(camera, s32, pose.position.depth_offset);
    /* Invisible points retain the matrix helper's native return register. */
    if (camera_depth < 0 || camera_depth >= CAMERA_FIELD(camera, s32, far_clip_depth) ||
        (focal_length = CAMERA_FIELD(camera, s32, projection.focal_length)) == 0) {
        *visible = 0;
        goto done;
    }

    screen_position[0] = CAMERA_FIELD(camera, s32, projection.screen_center_x) +
        BiosDiv(transformed[0] * focal_length, camera_depth);
    screen_position[1] = CAMERA_FIELD(camera, s32, projection.screen_center_y) +
        BiosDiv(transformed[1] * CAMERA_FIELD(camera, s32, projection.focal_length), camera_depth);
    *visible = 1;
    result = (s16)BiosDiv(CAMERA_FIELD(camera, s32, projection.focal_length) << 16, camera_depth);

done:
    return result;
}
