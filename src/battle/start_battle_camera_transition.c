#include "m2c_prelude.h"
#include "battle_display.h"
#include "../graphics/camera.h"


extern u8 gBattleCameraMode asm("D_02032EF9");
extern struct CameraPose gBattleCameraTarget asm("D_02032EFC");
extern u8 gBattleCameraTransitionComplete asm("D_02032F62");
extern struct CameraPose gPerspectiveCamera asm("D_030033C4");
extern s32 gBattleUnitWorldPositions[][3] asm("D_087A2790");

void StartBattleCameraTransition(u8 camera_mode, u8 side_id, u8 unit_id, u8 immediate) asm("func_080BB224");

void StartBattleCameraTransition(u8 camera_mode, u8 side_id, u8 unit_id, u8 immediate)
{
    u8 *mode_address;
    u8 *side;
    u8 *index;
    u8 mode;

    mode_address = &gBattleCameraMode;
    *mode_address = camera_mode;
    *(u8 *)0x02032F60 = side_id;
    *(u8 *)0x02032F61 = unit_id;
    mode = *mode_address;
    side = (u8 *)0x02032F60;
    index = (u8 *)0x02032F61;

    switch (mode) {
    case BATTLE_CAMERA_INTRO_OFFSET:
    {
        struct CameraPose *state;

        state = &gBattleCameraTarget;
        state->position.world_x = 0xFFFF3000;
        state->position.world_z = 0x20000;
        state->position.depth_offset = 0x8000;
        state->orientation.angles.pitch = 4;
        state->orientation.angles.roll = 0;
        state->orientation.angles.yaw = 0;
        break;
    }
    case BATTLE_CAMERA_INTRO_CENTERED:
    {
        struct CameraPose *state;

        state = &gBattleCameraTarget;
        state->position.world_x = 0;
        state->position.world_z = 0x20000;
        state->position.depth_offset = 0x8000;
        state->orientation.angles.pitch = 4;
        state->orientation.angles.roll = 0;
        state->orientation.angles.yaw = 0;
        break;
    }
    case 2:
    {
        u8 side_value;

        side_value = *side;
        if (side_value == 0) {
            struct CameraPose *state;

            state = &gBattleCameraTarget;
            state->position.world_x = 0xD000;
            state->position.world_z = side_value;
            state->position.depth_offset = 0x8000;
            state->orientation.angles.pitch = 0x20;
            state->orientation.angles.roll = side_value;
            state->orientation.angles.yaw = side_value;
        } else {
            struct CameraPose *state;

            state = &gBattleCameraTarget;
            state->position.world_x = 0xFFFF3000;
            state->position.world_z = 0;
            state->position.depth_offset = 0x8000;
            state->orientation.angles.pitch = 0x20;
            state->orientation.angles.roll = 0;
            state->orientation.angles.yaw = 0;
        }
        break;
    }
    case 3:
    {
        u8 side_value;

        side_value = *side;
        if (side_value == 0) {
            struct CameraPose *state;

            state = &gBattleCameraTarget;
            state->position.world_x = 0xB000;
            state->position.world_z = side_value;
            state->position.depth_offset = 0x8000;
            state->orientation.angles.pitch = 0x20;
            state->orientation.angles.roll = side_value;
            state->orientation.angles.yaw = side_value;
        } else {
            struct CameraPose *state;

            state = &gBattleCameraTarget;
            state->position.world_x = 0xFFFF1000;
            state->position.world_z = 0;
            state->position.depth_offset = 0x8000;
            state->orientation.angles.pitch = 0x20;
            state->orientation.angles.roll = 0;
            state->orientation.angles.yaw = 0;
        }
        break;
    }
    case 4:
    {
        u8 side_value;

        side_value = *side;
        if (side_value == 0) {
            struct CameraPose *state;

            state = &gBattleCameraTarget;
            state->position.world_x = 0xC000;
            state->position.world_z = side_value;
            state->position.depth_offset = 0x8000;
            state->orientation.angles.pitch = 0x20;
            state->orientation.angles.roll = side_value;
            state->orientation.angles.yaw = side_value;
        } else {
            struct CameraPose *state;

            state = &gBattleCameraTarget;
            state->position.world_x = 0xFFFF0000;
            state->position.world_z = 0;
            state->position.depth_offset = 0x8000;
            state->orientation.angles.pitch = 0x20;
            state->orientation.angles.roll = 0;
            state->orientation.angles.yaw = 0;
        }
        break;
    }
    case BATTLE_CAMERA_FOCUS_UNIT:
    {
        register struct CameraPose *state asm("r3");
        register s32 *table asm("r2");
        s32 first_index;

        state = &gBattleCameraTarget;
        table = gBattleUnitWorldPositions[0];
        first_index = (*side * 6) + *index;
        state->position.world_x = table[first_index * 3];
        {
            register s32 second_index asm("r0");
            register s32 second_offset asm("r1");

            second_index = (*side * 6) + *index;
            second_offset = second_index * 3;
            second_offset <<= 2;
            table += 2;
            state->position.world_z = *(s32 *)((u32)second_offset + (u32)table);
        }
        state->position.depth_offset = 0x8000;
        state->orientation.angles.pitch = 0x20;
        state->orientation.angles.roll = 0;
        state->orientation.angles.yaw = 0;
        break;
    }
    case BATTLE_CAMERA_CENTERED:
    case BATTLE_CAMERA_CENTERED_ALTERNATE:
    {
        struct CameraPose *state;

        state = &gBattleCameraTarget;
        state->position.world_x = 0;
        state->position.world_z = 0;
        state->position.depth_offset = 0x8000;
        state->orientation.angles.pitch = 0x20;
        state->orientation.angles.roll = 0;
        state->orientation.angles.yaw = 0;
        break;
    }
    case BATTLE_CAMERA_KEEP_TARGET:
        break;
    case BATTLE_CAMERA_FOCUS_UNIT_CLOSE:
    {
        register struct CameraPose *state asm("r3");
        register s32 *table asm("r2");
        s32 first_index;

        state = &gBattleCameraTarget;
        table = gBattleUnitWorldPositions[0];
        first_index = (*side * 6) + *index;
        state->position.world_x = table[first_index * 3];
        {
            register s32 second_index asm("r0");
            register s32 second_offset asm("r1");

            second_index = (*side * 6) + *index;
            second_offset = second_index * 3;
            second_offset <<= 2;
            table += 2;
            state->position.world_z = *(s32 *)((u32)second_offset + (u32)table);
        }
        state->position.depth_offset = 0x4000;
        state->orientation.angles.pitch = 0x20;
        state->orientation.angles.roll = 0;
        state->orientation.angles.yaw = 0;
        break;
    }
    default:
        break;
    }

    if (immediate == 1) {
        s32 x;
        s32 y;

        gPerspectiveCamera.position = gBattleCameraTarget.position;
        x = gBattleCameraTarget.orientation.words.pitch_yaw;
        y = gBattleCameraTarget.orientation.words.roll_padding;
        gPerspectiveCamera.orientation.words.pitch_yaw = x;
        gPerspectiveCamera.orientation.words.roll_padding = y;
    }
    gBattleCameraTransitionComplete = 0;
}
