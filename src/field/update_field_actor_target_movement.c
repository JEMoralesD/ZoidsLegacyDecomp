#include "field_actor.h"

void SetSpriteAnimation(void *, u16) asm("func_08094564");
u16 GetFieldActorAnimationIndex(void *) asm("func_080A9A54");

void UpdateFieldActorTargetMovement(struct FieldActor *actor) asm("func_080AAF80");

void UpdateFieldActorTargetMovement(struct FieldActor *actor)
{
    register u8 *sprite asm("r4");

    asm volatile("" : "=r"(sprite));
    {
        u32 value = FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.x_accumulator) + FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_distance_pixels);
        FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.x_accumulator) = value;
    }
    FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.y_accumulator) += FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_distance_pixels);

    while (FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.x_accumulator) >= FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.period_ticks)) {
        u8 *offset_view;

        if (FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_decreases) == 0) {
            offset_view = FIELD_ACTOR_FIELD(actor, u8 **, sprite);
            (FIELD_ACTOR_SPRITE_FIELD(offset_view, u16 *, offset_x))++;
            offset_view = FIELD_ACTOR_FIELD(actor, u8 **, secondary_sprite);
            if (offset_view != 0) {
                (FIELD_ACTOR_SPRITE_FIELD(offset_view, u16 *, offset_x))++;
            }
        } else {
            offset_view = FIELD_ACTOR_FIELD(actor, u8 **, sprite);
            (FIELD_ACTOR_SPRITE_FIELD(offset_view, u16 *, offset_x))--;
            offset_view = FIELD_ACTOR_FIELD(actor, u8 **, secondary_sprite);
            if (offset_view != 0) {
                (FIELD_ACTOR_SPRITE_FIELD(offset_view, u16 *, offset_x))--;
            }
        }
        FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.x_accumulator) =
            FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.x_accumulator) - FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.period_ticks);
    }

    {
        register u32 y_accumulator_or_elapsed_ticks asm("r0") = FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.y_accumulator);
        register u32 period_ticks asm("r1") = FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.period_ticks);

        asm volatile("" : "+r"(y_accumulator_or_elapsed_ticks), "+r"(period_ticks));
        while (y_accumulator_or_elapsed_ticks >= period_ticks) {
            u8 *offset_view;

            if (FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_decreases) == 0) {
                offset_view = FIELD_ACTOR_FIELD(actor, u8 **, sprite);
                (FIELD_ACTOR_SPRITE_FIELD(offset_view, u16 *, offset_y))++;
                offset_view = FIELD_ACTOR_FIELD(actor, u8 **, secondary_sprite);
                if (offset_view != 0) {
                    (FIELD_ACTOR_SPRITE_FIELD(offset_view, u16 *, offset_y))++;
                }
            } else {
                offset_view = FIELD_ACTOR_FIELD(actor, u8 **, sprite);
                (FIELD_ACTOR_SPRITE_FIELD(offset_view, u16 *, offset_y))--;
                offset_view = FIELD_ACTOR_FIELD(actor, u8 **, secondary_sprite);
                if (offset_view != 0) {
                    (FIELD_ACTOR_SPRITE_FIELD(offset_view, u16 *, offset_y))--;
                }
            }
            y_accumulator_or_elapsed_ticks = FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.y_accumulator);
            period_ticks = FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.period_ticks);
            asm volatile("" : "+r"(y_accumulator_or_elapsed_ticks), "+r"(period_ticks));
            y_accumulator_or_elapsed_ticks -= period_ticks;
            FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.y_accumulator) = y_accumulator_or_elapsed_ticks;
        }

        y_accumulator_or_elapsed_ticks = FIELD_ACTOR_FIELD(actor, volatile u32 *, behavior_state.target_movement.elapsed_ticks) + 1;
        FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.elapsed_ticks) = y_accumulator_or_elapsed_ticks;
        if (y_accumulator_or_elapsed_ticks != period_ticks) {
            return;
        }
    }
    {
        register u32 zero asm("r6");
        register u32 clear asm("r2");

        FIELD_ACTOR_FIELD(actor, u32 *, flags) &= ~FIELD_ACTOR_TARGET_MOVEMENT_ACTIVE;
        {
            register u8 *offset_view asm("r0") = FIELD_ACTOR_FIELD(actor, u8 **, sprite);

            clear = 0;
            zero = 0;
            FIELD_ACTOR_SPRITE_FIELD(offset_view, u16 *, offset_x) = zero;
            asm volatile("" ::: "memory");
        }
        {
            register u8 *offset_view asm("r1") = FIELD_ACTOR_FIELD(actor, u8 **, sprite);
            register u32 value asm("r0") = 0xFFFE;

            FIELD_ACTOR_SPRITE_FIELD(offset_view, u16 *, offset_y) = value;
        }
        {
            register u8 *offset_view asm("r0") = FIELD_ACTOR_FIELD(actor, u8 **, secondary_sprite);

            if (offset_view != 0) {
                FIELD_ACTOR_SPRITE_FIELD(offset_view, u16 *, offset_x) = zero;
                asm volatile("" ::: "memory");
                {
                    register u8 *secondary_offset_view asm("r1") =
                        FIELD_ACTOR_FIELD(actor, u8 **, secondary_sprite);
                    register u32 value asm("r0") = 0xE;

                    FIELD_ACTOR_SPRITE_FIELD(secondary_offset_view, u16 *, offset_y) = value;
                }
            }
        }
        FIELD_ACTOR_FIELD(actor, u8 *, previous_movement_mode) = clear;
        FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = clear;
        sprite = FIELD_ACTOR_FIELD(actor, u8 **, sprite);
        SetSpriteAnimation(sprite, GetFieldActorAnimationIndex(actor));
        FIELD_ACTOR_FIELD(actor, u32 *, velocity_y_fixed8) = zero;
        FIELD_ACTOR_FIELD(actor, u32 *, velocity_x_fixed8) = zero;
    }
}
