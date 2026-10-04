#include "m2c_prelude.h"
#include "battle_display.h"
#include "../graphics/camera.h"


extern u8 gBattleCameraMode asm("D_02032EF9");
extern struct CameraPose gBattleCameraTarget asm("D_02032EFC");
extern u8 gBattleCameraTransitionComplete asm("D_02032F62");
extern struct CameraPose gPerspectiveCamera asm("D_030033C4");

void UpdateBattleCameraTransition(void) asm("func_080BB474");

void UpdateBattleCameraTransition(void)
{
    u8 *done;
    u8 done_value;

    {
        register u8 *done_seed asm("r0");

        done_seed = &gBattleCameraTransitionComplete;
        done_value = *done_seed;
        done = done_seed;
    }
    if (done_value != 0) {
        return;
    }
    if (gBattleCameraMode == BATTLE_CAMERA_INTRO_OFFSET || gBattleCameraMode == BATTLE_CAMERA_INTRO_CENTERED) {
        register struct CameraPose *state_seed asm("r0");
        register struct CameraPose *state asm("r5");
        register s32 value asm("r2");
        register s32 limit asm("r1");

        state_seed = &gPerspectiveCamera;
        value = state_seed->position.world_z;
        limit = 0x7FFF;
        state = state_seed;
        if (value <= limit) {
            s32 rounded;
            s32 next_value;
            u32 angle;

            rounded = value;
            if (value < 0) {
                rounded = value + 7;
            }
            next_value = value - (rounded >> 3);
            asm volatile("" : : "r"(value));
            state->position.world_z = next_value;
            angle = state->orientation.angles.pitch;
            if ((s16)state->orientation.angles.pitch <= 0x1F) {
                state->orientation.angles.pitch = angle + 1;
            }
        } else {
            s32 next_value;

            next_value = value + 0xFFFFF000;
            asm volatile("" : : "r"(value));
            state->position.world_z = next_value;
        }
        if ((s16)state->orientation.angles.pitch != 0x20) {
            return;
        }
        if (state->position.world_z > 0x3F) {
            return;
        }
        state->position.world_z = 0;
        goto complete;
    }

    {
        register struct CameraPose *state asm("r5");
        register struct CameraPose *target asm("r6");

        {
            register struct CameraPose *state_seed asm("r0");
            register struct CameraPose *target_seed asm("r1");
            register s32 target_value asm("r3");
            register s32 current_value asm("r4");
            register s32 delta asm("r2");
            s32 next;

            state_seed = &gPerspectiveCamera;
            target_seed = &gBattleCameraTarget;
            target_value = target_seed->position.world_x;
            current_value = state_seed->position.world_x;
            delta = target_value - current_value;
            state = state_seed;
            target = target_seed;
            if (delta < 0) {
                delta += 3;
            }
            next = current_value + (delta >> 2);
            asm volatile("" : : "r"(delta), "r"(current_value));
            state->position.world_x = next;
            if ((target_value > next && target_value - next <= 0x7F) ||
                (target_value < next && target_value - next > -0x80)) {
                state->position.world_x = target->position.world_x;
            }
            asm volatile("" : : "r"(target_value));
        }

        {
            s32 target_value;
            s32 current_value;
            s32 delta;
            s32 next;

            target_value = target->position.world_z;
            current_value = state->position.world_z;
            delta = target_value - current_value;
            if (delta < 0) {
                delta += 3;
            }
            next = current_value + (delta >> 2);
            state->position.world_z = next;
            if ((target_value > next && target_value - next <= 0x7F) ||
                (target_value < next && target_value - next > -0x80)) {
                state->position.world_z = target->position.world_z;
            }
        }

        {
            s32 target_value;
            s32 current_value;
            s32 delta;
            s32 next;

            target_value = target->position.depth_offset;
            current_value = state->position.depth_offset;
            delta = target_value - current_value;
            if (delta < 0) {
                delta += 3;
            }
            next = current_value + (delta >> 2);
            state->position.depth_offset = next;
            if ((target_value > next && target_value - next <= 0x7F) ||
                (target_value < next && target_value - next > -0x80)) {
                state->position.depth_offset = target->position.depth_offset;
            }
        }
        {
            u32 current_raw;
            register u32 updated asm("r0");
            s16 current_short;
            s16 target_short;

            current_raw = state->orientation.angles.pitch;
            current_short = (s16)state->orientation.angles.pitch;
            target_short = (s16)target->orientation.angles.pitch;
            if (current_short != target_short) {
                if (current_short < target_short) {
                    if (target_short - current_short > 0x80) {
                        updated = current_raw - 1;
                    } else {
                        goto increment_d;
                    }
                } else if ((s16)state->orientation.angles.pitch -
                           (s16)target->orientation.angles.pitch <= 0x80) {
                    updated = current_raw - 1;
                    asm volatile("" : "+r"(updated));
                } else {
increment_d:
                    updated = current_raw + 1;
                }
                state->orientation.angles.pitch = updated;
            }
        }

        {
            u32 current_raw;
            register u32 updated asm("r0");
            s16 current_short;
            s16 target_short;

            current_raw = state->orientation.angles.yaw;
            current_short = (s16)state->orientation.angles.yaw;
            target_short = (s16)target->orientation.angles.yaw;
            if (current_short != target_short) {
                if (current_short < target_short) {
                    if (target_short - current_short > 0x80) {
                        updated = current_raw - 1;
                    } else {
                        goto increment_e;
                    }
                } else if ((s16)state->orientation.angles.yaw -
                           (s16)target->orientation.angles.yaw <= 0x80) {
                    updated = current_raw - 1;
                    asm volatile("" : "+r"(updated));
                } else {
increment_e:
                    updated = current_raw + 1;
                }
                state->orientation.angles.yaw = updated;
            }
        }

        {
            u32 current_raw;
            register u32 updated asm("r0");
            s16 current_short;
            s16 target_short;

            current_raw = state->orientation.angles.roll;
            current_short = (s16)state->orientation.angles.roll;
            target_short = (s16)target->orientation.angles.roll;
            if (current_short != target_short) {
                if (current_short < target_short) {
                    if (target_short - current_short > 0x80) {
                        updated = current_raw - 1;
                    } else {
                        goto increment_f;
                    }
                } else if ((s16)state->orientation.angles.roll -
                           (s16)target->orientation.angles.roll <= 0x80) {
                    updated = current_raw - 1;
                    asm volatile("" : "+r"(updated));
                } else {
increment_f:
                    updated = current_raw + 1;
                }
                state->orientation.angles.roll = updated;
            }
        }

        if (*(volatile s32 *)&state->position.world_x !=
                *(volatile s32 *)&target->position.world_x ||
            *(volatile s32 *)&state->position.world_z !=
                *(volatile s32 *)&target->position.world_z ||
            *(volatile s32 *)&state->position.depth_offset !=
                *(volatile s32 *)&target->position.depth_offset ||
            *(volatile s32 *)&state->orientation.words.pitch_yaw !=
                *(volatile s32 *)&target->orientation.words.pitch_yaw) {
            return;
        }
        {
            register s32 state_f asm("r1");
            register s32 target_f asm("r0");

            {
                register u32 state_offset asm("r0");

                state_offset = 0x10;
                state_f = *(s16 *)((u8 *)state + state_offset);
            }
            {
                register u32 target_offset asm("r2");

                target_offset = 0x10;
                target_f = *(s16 *)((u8 *)target + target_offset);
            }
            if (state_f != target_f) {
                return;
            }
        }
    }

complete:
    *done = 1;
}
