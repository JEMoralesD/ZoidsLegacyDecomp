#ifndef CAMERA_H
#define CAMERA_H

#include "../engine/math3d.h"

enum PerspectiveScanlineLimits {
    PERSPECTIVE_SCANLINE_COUNT = 160,
    PERSPECTIVE_SCANLINE_BANK_BYTES = 0xA00,
    PERSPECTIVE_FAR_CLIP_ROW_NONE = 0xFF
};

struct CameraWorldOffset {
    s32 world_x;
    s32 world_z;
    s32 depth_offset;
};

union CameraOrientation {
    struct {
        u16 pitch;
        u16 yaw;
        u16 roll;
        u16 data06;
    } angles;
    struct {
        s32 pitch_yaw;
        s32 roll_padding;
    } words;
};

struct CameraPose {
    struct CameraWorldOffset position;
    union CameraOrientation orientation;
};

struct PerspectiveView {
    s32 screen_center_x;
    s32 screen_center_y;
    s32 focal_length;
};

struct PerspectiveCamera {
    struct CameraPose pose;
    struct PerspectiveView projection;
    struct CameraPose previous_pose;
    struct PerspectiveView previous_projection;
    s16 rotation_matrix[9];
    u8 data52[14];
    s32 far_clip_depth;
};

struct PerspectiveScanline {
    s16 bg2_pa;
    s16 bg2_pb;
    s16 bg2_pc;
    s16 bg2_pd;
    s32 bg2_x;
    s32 bg2_y;
};

struct PointProjectionTransform {
    s16 rotation_matrix[9];
    u8 data12[2];
    struct Vector3 translation;
};

/* Explicit access types preserve the native signed loads. */
#define CAMERA_FIELD(camera, type, field) \
    M2C_FIELD(camera, type *, (s32)&((struct PerspectiveCamera *)0)->field)

#endif
