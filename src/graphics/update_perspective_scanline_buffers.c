#include "m2c_prelude.h"
#include "camera.h"

extern s32 Sin256(s16) asm("func_08092A90");
extern u16 Cos256(s16) asm("func_08092ADC");
extern s32 BiosDiv(s32, s32) asm("func_080ECD30");

#define OBSERVE_SIGNED(value) ({ \
    s32 observed = (value); \
    asm volatile("" : "+r"(observed)); \
    observed; \
})


u8 UpdatePerspectiveScanlineBuffers(void) asm("func_08093B7C");

u8 UpdatePerspectiveScanlineBuffers(void)
{
    register struct PerspectiveCamera *state asm("r6");
    register struct PerspectiveCamera *state_base asm("r8");
    u8 dirty = *(volatile u8 *)0x03005F70;
    register s32 pitch_cosine asm("r10");
    register s32 pitch_sine asm("r4");
    register s32 pitch_sine_shifted asm("r9");
    s32 yaw_cosine;
    s32 yaw_sine;
    struct PerspectiveScanline *record;
    s32 initial_numerator;
    s32 limit;
    s32 line;
    register volatile u8 *phase asm("r1");
    register u32 next_phase asm("r0");

    state = (struct PerspectiveCamera *)0x030033C4;
    if (dirty != 0 ||
        state->pose.position.depth_offset != state->previous_pose.position.depth_offset ||
        state->pose.orientation.words.pitch_yaw != state->previous_pose.orientation.words.pitch_yaw ||
        OBSERVE_SIGNED((s16)state->pose.orientation.angles.roll) !=
            OBSERVE_SIGNED((s16)state->previous_pose.orientation.angles.roll) ||
        state->projection.screen_center_x != state->previous_projection.screen_center_x ||
        state->projection.screen_center_y != state->previous_projection.screen_center_y ||
        state->projection.focal_length != state->previous_projection.focal_length) {
        phase = (volatile u8 *)0x030034A0;
        next_phase = 0;
        goto store_phase;
    }
    phase = (volatile u8 *)0x030034A0;
    next_phase = *phase;
    if (next_phase <= 1) {
        next_phase++;
store_phase:
        *phase = next_phase;
    }

    pitch_cosine = Cos256(-(s16)state->pose.orientation.angles.pitch);
    pitch_sine = Sin256(-(s16)state->pose.orientation.angles.pitch);
    asm volatile("" : "+r"(pitch_sine));
    pitch_sine = (u16)pitch_sine;
    yaw_cosine = Cos256(-(s16)state->pose.orientation.angles.yaw);
    yaw_sine = (u16)Sin256(
        -(s16)state->pose.orientation.angles.yaw);

    record = (struct PerspectiveScanline *)(*(s32 *)0x03003428 +
        ((1 ^ *(u8 *)0x0300342C) * PERSPECTIVE_SCANLINE_BANK_BYTES));
    state_base = state;
    state = (struct PerspectiveCamera *)((u8 *)state + 0x14);
    {
        register struct PerspectiveCamera *limit_base asm("r3") = state_base;

        limit = limit_base->far_clip_depth;
    }
    pitch_sine <<= 16;
    {
        register s32 pitch_sine_value asm("r1") = pitch_sine >> 16;
        register s32 distance_value asm("r0") =
            ((struct PerspectiveView *)state)->focal_length;
        register s32 distance_product asm("r2");
        register struct PerspectiveCamera *depth_base asm("r1");
        register s32 depth_value asm("r0");
        register s32 result asm("r3");

        asm volatile("" : "+r"(pitch_sine_value), "+r"(distance_value));
        distance_product = distance_value;
        distance_product *= pitch_sine_value;
        depth_base = state_base;
        depth_value = depth_base->pose.position.depth_offset;
        depth_value /= 0x100;
        asm volatile("" : "+r"(depth_value));
        result = depth_value;
        result *= distance_product;
        asm volatile("" : "+r"(result));
        initial_numerator = result;
    }

    line = 0;
    pitch_sine_shifted = pitch_sine;
    {
        register s32 pitch_cosine_shifted asm("r0") = pitch_cosine;

        asm volatile("" : "+r"(pitch_cosine_shifted));
        pitch_cosine_shifted <<= 16;
        pitch_cosine = pitch_cosine_shifted;
    }
    for (; line < PERSPECTIVE_SCANLINE_COUNT; record++, line++) {
        register s32 division_denominator asm("r1");
        register s32 pitch_sine_value asm("r4");
        register s32 perspective asm("r2");
        register s32 second_numerator asm("r0");

        if (*(u8 *)0x030034A0 == 2) {
            register s32 pitch_sine_source asm("r2") = pitch_sine_shifted;

            asm volatile("" : "+r"(pitch_sine_source));
            division_denominator = pitch_sine_source >> 16;
            if (division_denominator == 0) {
                goto clear_record;
            }
            second_numerator = (line -
                ((struct PerspectiveView *)state)->screen_center_y) << 8;
            goto project_record;
        } else {
            register s32 pitch_cosine_source asm("r2");
            register s32 pitch_sine_source asm("r3") = pitch_sine_shifted;
            register s32 distance_value asm("r3");
            register s32 denominator asm("r0");
            register s32 pitch_sine_component asm("r12");
            register s32 pitch_cosine_value asm("r1");

            pitch_sine_value = pitch_sine_source >> 16;
            distance_value = ((struct PerspectiveView *)state)->focal_length;
            denominator = distance_value;
            denominator *= pitch_sine_value;
            pitch_sine_component = denominator;
            denominator =
                ((struct PerspectiveView *)state)->screen_center_y - line;
            pitch_cosine_source = pitch_cosine;
            pitch_cosine_value = pitch_cosine_source >> 16;
            denominator *= pitch_cosine_value;
            denominator += pitch_sine_component;
            denominator /= 0x100;
            asm volatile("" : "+r"(denominator));
            division_denominator = denominator;
            division_denominator *= distance_value;
            if (division_denominator == 0) {
                goto clear_record;
            }

            perspective = BiosDiv(initial_numerator,
                division_denominator);
            record->bg2_pa = record->bg2_pd =
                ((s16)yaw_cosine * perspective) / 0x100;
            record->bg2_pb = ((s16)yaw_sine * perspective) / 0x100;
            record->bg2_pc = -record->bg2_pb;
            second_numerator = (line -
                ((struct PerspectiveView *)state)->screen_center_y) << 8;
            division_denominator = pitch_sine_value;
        }

project_record:
        perspective = BiosDiv(second_numerator, division_denominator);
        {
            register s32 negative_scale asm("r0");
            register s32 projected_value asm("r1");
            register s32 projection_term asm("r0");
            register struct PerspectiveCamera *origin_base_x asm("r3");
            register struct PerspectiveCamera *origin_base_y asm("r2");

            negative_scale = -((struct PerspectiveView *)state)->screen_center_x;
            projected_value = record->bg2_pa;
            projected_value *= negative_scale;
            projection_term = record->bg2_pb;
            projection_term *= perspective;
            projected_value -= projection_term;
            origin_base_x = state_base;
            projected_value += origin_base_x->pose.position.world_x;
            record->bg2_x = projected_value;

            negative_scale = -((struct PerspectiveView *)state)->screen_center_x;
            projected_value = record->bg2_pc;
            projected_value *= negative_scale;
            projection_term = record->bg2_pd;
            projection_term *= perspective;
            projected_value -= projection_term;
            origin_base_y = state_base;
            projected_value += origin_base_y->pose.position.world_z;
            record->bg2_y = projected_value;
        }
        continue;

clear_record:
        record->bg2_y = division_denominator;
        record->bg2_x = division_denominator;
        *(s32 *)&record->bg2_pc = division_denominator;
        *(s32 *)&record->bg2_pa = division_denominator;
    }

    {
        register s32 pitch_cosine_source asm("r3") = pitch_cosine;
        register s32 pitch_cosine_value asm("r2");
        register struct PerspectiveCamera *depth_base asm("r1");
        register s32 numerator asm("r0");
        register s32 limit_value asm("r3");
        register s32 pitch_sine_value asm("r1");
        register s32 scaled_numerator asm("r1");
        register s32 numerator_product asm("r4");
        register s32 denominator asm("r0");
        register s32 denominator_limit asm("r1");
        register s32 scaled_denominator asm("r1");
        u32 result;

        asm volatile("" : "+r"(pitch_cosine_source));
        pitch_cosine_value = (s16)(pitch_cosine_source >> 16);
        if (pitch_cosine_value != 0) {
            depth_base = state_base;
            numerator = depth_base->pose.position.depth_offset;
            limit_value = limit;
            asm volatile("" : "+r"(limit_value));
            numerator = limit_value - numerator;
            pitch_sine_value = (s16)(pitch_sine_shifted >> 16);
            numerator *= pitch_sine_value;
            scaled_numerator = numerator / 0x100;
            asm volatile("" : "+r"(scaled_numerator));
            numerator_product = scaled_numerator;
            numerator_product *= ((struct PerspectiveView *)state)->focal_length;
            denominator_limit = limit;
            denominator = pitch_cosine_value;
            asm volatile("" : "+r"(denominator));
            denominator *= denominator_limit;
            scaled_denominator = denominator / 0x100;
            result = BiosDiv(numerator_product, scaled_denominator) +
                ((struct PerspectiveView *)state)->screen_center_y;
            if (result <= 159) {
                return (u8)result;
            }
        }
    }
    return PERSPECTIVE_FAR_CLIP_ROW_NONE;
}
