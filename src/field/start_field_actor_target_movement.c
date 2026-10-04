#include "field_actor.h"

s32 SetSpriteAnimation(void *, u16) asm("func_8094564");
u16 GetFieldActorAnimationIndex(void *) asm("func_080A9A54");
u16 BiosArcTan2(s16, s16) asm("func_80ECD24");
u16 BiosSqrt(s32) asm("func_80ECD3C");

#define ACTOR_VOLATILE_FIELD(off, ty) (*(volatile ty *)((s8 *)actor + (off)))
#define ACTOR_VOLATILE_POINTER(off) ((void *) *(volatile s32 *)((s8 *)actor + (off)))

void StartFieldActorTargetMovement(struct FieldActor *actor) asm("func_080AAE80");

void StartFieldActorTargetMovement(struct FieldActor *actor) {
    s32 signed_x_distance;
    s32 signed_y_distance;
    s32 x_distance_squared;
    s32 x_distance_pixels;
    u32 target_x_fixed8;
    s32 target_y_fixed8;
    s32 x_delta_fixed8;
    s32 y_delta_fixed8;
    s32 x_decreases;
    s32 y_decreases;
    s32 direction;
    u16 distance_root;
    u16 distance_pixels;
    u32 period_ticks;
    u32 distance_shifted16;
    void *main_x_sprite;
    void *main_y_sprite;
    void *secondary_x_sprite;
    void *secondary_y_sprite;
    void *animation_sprite;

    FIELD_ACTOR_FIELD(actor, s32 *, flags) = (s32) ((FIELD_ACTOR_FIELD(actor, s32 *, flags) & ~FIELD_ACTOR_TARGET_MOVEMENT_PENDING) | FIELD_ACTOR_TARGET_MOVEMENT_ACTIVE);
    target_x_fixed8 = ACTOR_VOLATILE_FIELD(FIELD_ACTOR_OFFSET(behavior_state.pending_target.world_x_fixed8), u32);
    target_y_fixed8 = ACTOR_VOLATILE_FIELD(FIELD_ACTOR_OFFSET(behavior_state.pending_target.world_y_fixed8), s32);
    x_delta_fixed8 = target_x_fixed8 - FIELD_ACTOR_FIELD(actor, u32 *, world_x_fixed8);
    if (x_delta_fixed8 < 0) {
        x_delta_fixed8 += 0xFF;
    }
    x_distance_pixels = x_delta_fixed8 >> 8;
    FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_distance_pixels) = x_distance_pixels;
    y_delta_fixed8 = target_y_fixed8 - FIELD_ACTOR_FIELD(actor, s32 *, world_y_fixed8);
    if (y_delta_fixed8 < 0) {
        y_delta_fixed8 += 0xFF;
    }
    FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_distance_pixels) = (s32) (y_delta_fixed8 >> 8);
    main_x_sprite = ACTOR_VOLATILE_POINTER(FIELD_ACTOR_OFFSET(sprite));
    FIELD_ACTOR_SPRITE_FIELD(main_x_sprite, u16 *, offset_x) = (u16) (FIELD_ACTOR_SPRITE_FIELD(main_x_sprite, u16 *, offset_x) - x_distance_pixels);
    main_y_sprite = ACTOR_VOLATILE_POINTER(FIELD_ACTOR_OFFSET(sprite));
    FIELD_ACTOR_SPRITE_FIELD(main_y_sprite, u16 *, offset_y) = (u16) (FIELD_ACTOR_SPRITE_FIELD(main_y_sprite, u16 *, offset_y) - ACTOR_VOLATILE_FIELD(FIELD_ACTOR_OFFSET(behavior_state.target_movement.y_distance_pixels), s32));
    secondary_x_sprite = ACTOR_VOLATILE_POINTER(FIELD_ACTOR_OFFSET(secondary_sprite));
    if (secondary_x_sprite != 0) {
        FIELD_ACTOR_SPRITE_FIELD(secondary_x_sprite, u16 *, offset_x) = (u16) (FIELD_ACTOR_SPRITE_FIELD(secondary_x_sprite, u16 *, offset_x) - ACTOR_VOLATILE_FIELD(FIELD_ACTOR_OFFSET(behavior_state.target_movement.x_distance_pixels), s32));
        secondary_y_sprite = ACTOR_VOLATILE_POINTER(FIELD_ACTOR_OFFSET(secondary_sprite));
        FIELD_ACTOR_SPRITE_FIELD(secondary_y_sprite, u16 *, offset_y) = (u16) (FIELD_ACTOR_SPRITE_FIELD(secondary_y_sprite, u16 *, offset_y) - ACTOR_VOLATILE_FIELD(FIELD_ACTOR_OFFSET(behavior_state.target_movement.y_distance_pixels), s32));
    }
    {
        register u32 raw asm("r0");
        register u32 normalized asm("r1");
        register s32 bias asm("r2");
        register u32 sum asm("r0");
        register s32 quotient asm("r1");
        register s32 remainder asm("r0");

        raw = BiosArcTan2((s16)(0 - ACTOR_VOLATILE_FIELD(FIELD_ACTOR_OFFSET(behavior_state.target_movement.y_distance_pixels), s32)),
                           (s16)FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_distance_pixels));
        raw <<= 16;
        normalized = raw >> 16;
        bias = 0x80;
        bias <<= 5;
        sum = normalized + bias;
        asm volatile("" : "+r"(sum));
        quotient = (s32)sum >> 0xD;
        remainder = quotient;
        asm volatile("" : "+r"(remainder));
        remainder >>= 3;
        remainder <<= 3;
        remainder = quotient - remainder;
        direction = remainder;
    }
    FIELD_ACTOR_FIELD(actor, s8 *, current_direction) = direction;
    FIELD_ACTOR_FIELD(actor, s8 *, requested_direction) = direction;
    signed_x_distance = ACTOR_VOLATILE_FIELD(FIELD_ACTOR_OFFSET(behavior_state.target_movement.x_distance_pixels), s32);
    if (signed_x_distance >= 0) {
        x_decreases = 0;
    } else {
        FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_distance_pixels) = (s32) (0 - signed_x_distance);
        x_decreases = 1;
    }
    FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_decreases) = x_decreases;
    signed_y_distance = ACTOR_VOLATILE_FIELD(FIELD_ACTOR_OFFSET(behavior_state.target_movement.y_distance_pixels), s32);
    if (signed_y_distance >= 0) {
        y_decreases = 0;
    } else {
        FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_distance_pixels) = (s32) (0 - signed_y_distance);
        y_decreases = 1;
    }
    FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_decreases) = y_decreases;
    FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_accumulator) = 0;
    FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_accumulator) = 0;
    x_distance_squared = FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_distance_pixels);
    {
        register s32 product asm("r1");
        register s32 sum asm("r0");

        product = x_distance_squared;
        product *= x_distance_squared;
        asm volatile("" : "+r"(product));
        sum = product;
        asm volatile("" : "+r"(sum));
        x_distance_squared = sum;
    }
    distance_root = BiosSqrt(x_distance_squared + (FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_distance_pixels) * FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_distance_pixels)));
    distance_shifted16 = (u32) distance_root << 0x10;
    distance_pixels = distance_shifted16 >> 0x10;
    FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.period_ticks) = (u32) distance_pixels;
    if (FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) == FIELD_ACTOR_HALF_SPEED) {
        period_ticks = distance_pixels * 2;
    } else {
        FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = FIELD_ACTOR_DOUBLE_SPEED;
        period_ticks = distance_shifted16 >> 0x11;
    }
    FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.period_ticks) = period_ticks;
    animation_sprite = ACTOR_VOLATILE_POINTER(FIELD_ACTOR_OFFSET(sprite));
    SetSpriteAnimation(animation_sprite, GetFieldActorAnimationIndex(actor));
    FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.elapsed_ticks) = 0;
    /* Position changes immediately. Sprite offsets animate the travel. */
    FIELD_ACTOR_FIELD(actor, u32 *, world_x_fixed8) = target_x_fixed8;
    FIELD_ACTOR_FIELD(actor, s32 *, world_y_fixed8) = target_y_fixed8;
}
