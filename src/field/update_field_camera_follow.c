#include "field_actor.h"

extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern u16 gFieldMapDimensions[] asm("D_020324A4");
extern s32 gFieldBackgroundTilemaps[] asm("D_02032E88");
extern u16 gCurrentMapId asm("D_0202ECF4");

void UpdateFieldCameraFollow(struct FieldActor *actor_in) asm("func_080ABE70");

void UpdateFieldCameraFollow(struct FieldActor *actor_in) {
    register struct FieldActor *actor asm("r2") = actor_in;
    s32 *camera;
    register s32 camera_x_fixed8 asm("r6");
    register s32 unclamped_x_step asm("r5");
    register s32 x_step_fixed8 asm("r3");
    register s32 y_step_fixed8 asm("r4");
    register s32 coordinate_work asm("r0");
    s32 *camera_base;

    if (actor->behavior != FIELD_ACTOR_PLAYER_CONTROLLED && actor->behavior != FIELD_ACTOR_SCRIPTED_CAMERA_FOLLOW) {
        return;
    }

    coordinate_work = actor->world_x_fixed8 + 0xFFFF8800;
    camera_base = gFieldCameraScrollOffsets;
    camera_x_fixed8 = camera_base[0];
    coordinate_work -= camera_x_fixed8;
    camera = camera_base;
    if (coordinate_work < 0) {
        coordinate_work += 7;
    }
    unclamped_x_step = coordinate_work >> 3;
    x_step_fixed8 = unclamped_x_step;
    asm volatile("" : "+r"(camera));

    coordinate_work = actor->world_y_fixed8 + 0xFFFFB000;
    coordinate_work -= camera[1];
    if (coordinate_work < 0) {
        coordinate_work += 7;
    }
    y_step_fixed8 = coordinate_work >> 3;

    if (gCurrentMapId == FIELD_MAP_PACKED_CELLS) {
        if (x_step_fixed8 < -0x800) {
            x_step_fixed8 += gFieldMapDimensions[0] << 8;
        } else if (x_step_fixed8 > 0x800) {
            x_step_fixed8 -= gFieldMapDimensions[0] << 8;
        }

        if (y_step_fixed8 < -0x800) {
            y_step_fixed8 += gFieldMapDimensions[1] << 8;
        } else if (y_step_fixed8 > 0x800) {
            y_step_fixed8 -= gFieldMapDimensions[1] << 8;
        }
    }

    else {
        if (unclamped_x_step < 0) {
            register s32 bound asm("r1") = -camera_x_fixed8;
            if (bound < 0) {
                bound += 7;
            }
            coordinate_work = bound >> 3;
            if (x_step_fixed8 < coordinate_work) {
                x_step_fixed8 = coordinate_work;
            }
        } else if (unclamped_x_step > 0) {
            coordinate_work = (gFieldMapDimensions[0] - 30) << 11;
            coordinate_work -= camera_x_fixed8;
            if (coordinate_work < 0) {
                coordinate_work += 7;
            }
            coordinate_work >>= 3;
            if (x_step_fixed8 > coordinate_work) {
                x_step_fixed8 = coordinate_work;
            }
        }

        if (y_step_fixed8 < 0) {
            coordinate_work = -camera[1];
            if (coordinate_work < 0) {
                coordinate_work += 7;
            }
            coordinate_work >>= 3;
            if (y_step_fixed8 < coordinate_work) {
                y_step_fixed8 = coordinate_work;
            }
        } else if (y_step_fixed8 > 0) {
            coordinate_work = (gFieldMapDimensions[1] - 20) << 11;
            coordinate_work -= camera[1];
            if (coordinate_work < 0) {
                coordinate_work += 7;
            }
            coordinate_work >>= 3;
            if (y_step_fixed8 > coordinate_work) {
                y_step_fixed8 = coordinate_work;
            }
        }

    }

    if (gFieldBackgroundTilemaps[0] != 0) {
        s32 camera_x = camera[0] + x_step_fixed8;
        s32 camera_y = camera[1] + y_step_fixed8;
        camera[0] = camera_x & 0xFFFFF;
        camera[1] = camera_y & 0xFFFFF;
    }
    if (gFieldBackgroundTilemaps[1] != 0) {
        s32 camera_x = camera[2] + x_step_fixed8;
        s32 camera_y = camera[3] + y_step_fixed8;
        camera[2] = camera_x & 0xFFFFF;
        camera[3] = camera_y & 0xFFFFF;
    }
}
