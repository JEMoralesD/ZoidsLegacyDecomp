#include "sound_engine.h"

void UpdatePsgChannelVolumeAndPan(void *channel) asm("func_080EC1D8");

void UpdatePsgChannelVolumeAndPan(void *channel) {
    register u8 *state asm("r1");
    register s32 value asm("r0");
    register s32 component asm("r2");
    register s32 other_component asm("r3");
    register s32 right_volume asm("r4");

    state = channel;
    value = PSG_CHANNEL_FIELD(state, u8 *, volume_right);
    component = value << 24;
    right_volume = (u32)component >> 24;
    other_component = PSG_CHANNEL_FIELD(state, u8 *, volume_left);
    value = other_component << 24;
    other_component = (u32)value >> 24;
    if ((u32)right_volume >= (u32)other_component) {
        value = (u32)component >> 25;
        if ((u32)value >= (u32)other_component) {
            PSG_CHANNEL_FIELD(state, u8 *, output_routing) = PSG_OUTPUT_RIGHT;
            goto clamp_sum;
        }
    } else {
        value = (u32)value >> 25;
        if ((u32)value >= (u32)right_volume) {
            PSG_CHANNEL_FIELD(state, u8 *, output_routing) = PSG_OUTPUT_LEFT;
            goto clamp_sum;
        }
    }
    PSG_CHANNEL_FIELD(state, u8 *, output_routing) = PSG_OUTPUT_BOTH;
    component = PSG_CHANNEL_FIELD(state, u8 *, volume_left);
    other_component = PSG_CHANNEL_FIELD(state, u8 *, volume_right);
    value = (u32)(component + other_component) >> 4;
    goto store_sum;

clamp_sum:
    component = PSG_CHANNEL_FIELD(state, u8 *, volume_left);
    other_component = PSG_CHANNEL_FIELD(state, u8 *, volume_right);
    value = (u32)(component + other_component) >> 4;
    PSG_CHANNEL_FIELD(state, u8 *, amplitude) = value;
    if ((u32)value <= PSG_MAX_AMPLITUDE) {
        goto finish_sum;
    }
    value = PSG_MAX_AMPLITUDE;
store_sum:
    PSG_CHANNEL_FIELD(state, u8 *, amplitude) = value;
finish_sum:
    component = PSG_CHANNEL_FIELD(state, u8 *, sustain);
    asm volatile("" : "+r"(component));
    other_component = PSG_CHANNEL_FIELD(state, u8 *, amplitude);
    value = component;
    value *= other_component;
    value += PSG_MAX_AMPLITUDE;
    asm volatile("" : "+r"(value));
    value >>= 4;
    PSG_CHANNEL_FIELD(state, u8 *, sustain_level) = value;
    value = PSG_CHANNEL_FIELD(state, u8 *, output_pan_mask);
    component = PSG_CHANNEL_FIELD(state, u8 *, output_routing);
    value &= component;
    PSG_CHANNEL_FIELD(state, u8 *, output_routing) = value;
}
