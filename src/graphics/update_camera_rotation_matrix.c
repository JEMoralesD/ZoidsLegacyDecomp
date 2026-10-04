#include "m2c_prelude.h"
#include "camera.h"


void BuildRotationMatrixXYZ(s16 *, s16 *) asm("func_0809378C");

void UpdateCameraRotationMatrix(void) asm("func_080BB764");

void UpdateCameraRotationMatrix(void) {
    struct PerspectiveCamera *state;

    state = (struct PerspectiveCamera *)0x030033C4;
    asm volatile("" : "+r"(state));
    if (state->pose.orientation.words.pitch_yaw != state->previous_pose.orientation.words.pitch_yaw) {
        goto update_matrix;
    }
    {
        register s32 angle_offset asm("r0");
        register s32 angle asm("r1");
        register s32 saved_offset asm("r2");
        register s32 saved_angle asm("r0");

        angle_offset = 0x10;
        angle = *(s16 *)((u8 *)state + angle_offset);
        saved_offset = 0x30;
        saved_angle = *(s16 *)((u8 *)state + saved_offset);
        if (angle == saved_angle) {
            goto matrix_done;
        }
    }
update_matrix:
    {
        u32 angles[2];
        register s32 upper asm("r1");
        register s32 lower asm("r0");
        register s32 mask asm("r2");
        register s16 *output asm("r1");

        mask = 0xFFFF0000;
        upper = *(u16 *)((u8 *)state + 0xE);
        upper = -upper;
        upper <<= 16;
        lower = *(u16 *)((u8 *)state + 0xC);
        lower |= upper;
        angles[0] = lower;
        /* The native camera tracks roll changes but builds the matrix with zero roll. */
        angles[1] &= mask;
        output = state->rotation_matrix;
        BuildRotationMatrixXYZ((s16 *)angles, output);
        {
            register s32 packed asm("r0");
            register s32 angle asm("r1");

            packed = state->pose.orientation.words.pitch_yaw;
            angle = state->pose.orientation.words.roll_padding;
            state->previous_pose.orientation.words.pitch_yaw = packed;
            state->previous_pose.orientation.words.roll_padding = angle;
        }
    }
matrix_done:
    {
        register struct PerspectiveCamera *base asm("r0");
        register s32 head_c asm("r2");
        register s32 saved_head_c asm("r1");
        struct PerspectiveCamera *current;

        base = (struct PerspectiveCamera *)0x030033C4;
        head_c = base->pose.position.depth_offset;
        saved_head_c = base->previous_pose.position.depth_offset;
        current = base;
        asm volatile("" : "+&r"(current) : "r"(base));
        if (head_c != saved_head_c ||
            current->projection.screen_center_x != current->previous_projection.screen_center_x ||
            current->projection.screen_center_y != current->previous_projection.screen_center_y ||
            current->projection.focal_length != current->previous_projection.focal_length) {
            current->previous_pose.position = current->pose.position;
            current->previous_projection = current->projection;
        }
    }
}
