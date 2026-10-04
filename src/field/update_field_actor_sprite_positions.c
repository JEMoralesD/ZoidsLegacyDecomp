#include "field_actor.h"

void UpdateFieldActorSpritePositions(void) asm("func_080ABFCC");

void UpdateFieldActorSpritePositions(void)
{
    register u32 slot_index asm("r4") = 0;
    register struct FieldActor *base asm("r8") =
        (struct FieldActor *)0x020325A0;
    register s32 *camera asm("r12");
    s32 wrap_threshold_fixed8;
    register s32 wrap_offset_fixed8 asm("r6");
    register s32 wrap_span_pixels asm("r5");

    {
        register s32 *camera_init asm("r1") = (s32 *)0x03000054;
        register s32 wrap_span_seed asm("r0");

        camera = camera_init;
        asm volatile("" : "+r"(camera));
        wrap_threshold_fixed8 = 0xEFFFF;
        asm volatile("" : "+r"(wrap_threshold_fixed8));
        wrap_offset_fixed8 = -0x10000;
        asm volatile("" : "+r"(wrap_offset_fixed8));
        wrap_span_seed = 0x80;
        wrap_span_seed <<= 5;
        asm volatile("" : "+r"(wrap_span_seed));
        wrap_span_pixels = wrap_span_seed;
    }

    do {
        register u32 offset asm("r0") = slot_index << 3;
        register struct FieldActor *base_view asm("r1");
        register struct FieldActor *actor asm("r3");

        offset += slot_index;
        offset <<= 3;
        base_view = base;
        actor = (struct FieldActor *)(offset + (u32)base_view);

        if ((actor->flags & FIELD_ACTOR_ACTIVE) != 0) {
            {
                register s32 *camera_view asm("r1") = camera;
                register s32 camera_x asm("r0");
                register s32 world_x_fixed8 asm("r2");
                register s32 x_view asm("r1");

                asm volatile("" : "+r"(camera_view));
                camera_x = camera_view[0];
                world_x_fixed8 = actor->world_x_fixed8;
                if (camera_x <= wrap_threshold_fixed8) {
                    goto x_normal;
                }
                camera_x += wrap_offset_fixed8;
                x_view = world_x_fixed8;
                if (x_view < camera_x) {
                    goto x_wrap;
                }
x_normal:
                {
                    register struct FieldActorSpriteOffsetView *sprite asm("r1") = actor->sprite;
                    register s32 adjusted asm("r0") = world_x_fixed8;

                    if (adjusted < 0) {
                        adjusted += 0xFF;
                    }
                    sprite->position_x_pixels = adjusted >> 8;
                    goto x_done;
                }
x_wrap:
                {
                    register struct FieldActorSpriteOffsetView *sprite asm("r2") = actor->sprite;
                    register s32 adjusted asm("r0") = x_view;

                    if (adjusted < 0) {
                        adjusted += 0xFF;
                    }
                    adjusted >>= 8;
                    adjusted += wrap_span_pixels;
                    sprite->position_x_pixels = adjusted;
                }
x_done:
                ;
            }

            {
                register s32 *camera_view asm("r1") = camera;
                register s32 camera_y asm("r0");
                register s32 world_y_fixed8 asm("r2");
                register s32 y_view asm("r1");

                asm volatile("" : "+r"(camera_view));
                camera_y = camera_view[1];
                world_y_fixed8 = actor->world_y_fixed8;
                if (camera_y <= wrap_threshold_fixed8) {
                    goto y_normal;
                }
                camera_y += wrap_offset_fixed8;
                y_view = world_y_fixed8;
                if (y_view < camera_y) {
                    goto y_wrap;
                }
y_normal:
                {
                    register struct FieldActorSpriteOffsetView *sprite asm("r1") = actor->sprite;
                    register s32 adjusted asm("r0") = world_y_fixed8;

                    if (adjusted < 0) {
                        adjusted += 0xFF;
                    }
                    sprite->position_y_pixels = adjusted >> 8;
                    goto y_done;
                }
y_wrap:
                {
                    register struct FieldActorSpriteOffsetView *sprite asm("r2") = actor->sprite;
                    register s32 adjusted asm("r0") = y_view;

                    if (adjusted < 0) {
                        adjusted += 0xFF;
                    }
                    adjusted >>= 8;
                    adjusted += wrap_span_pixels;
                    sprite->position_y_pixels = adjusted;
                }
y_done:
                ;
            }

            {
                register struct FieldActorSpriteOffsetView *secondary_sprite asm("r1") = actor->secondary_sprite;

                if (secondary_sprite != 0) {
                    secondary_sprite->position_x_pixels = actor->sprite->position_x_pixels;
                    secondary_sprite = actor->secondary_sprite;
                    secondary_sprite->position_y_pixels = actor->sprite->position_y_pixels;
                }
            }
        }
        {
            register u32 next_slot asm("r0") = slot_index + 1;

            next_slot <<= 24;
            slot_index = next_slot >> 24;
        }
    } while (slot_index <= (u32)FIELD_ACTOR_MAX_SLOT);
}
