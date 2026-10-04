#include "m2c_prelude.h"
#include "screen_effects.h"

s16 Sin256(s32) asm("func_8092A90");
s16 Cos256(s32) asm("func_8092ADC");

extern u8 gScreenTransitionProgress asm("D_03005F72");


extern struct WindowPolygon D_03006020;

void BuildRotatedWindowSquare(s32 center_x, s32 center_y, s32 half_size) asm("func_080966B4");

void BuildRotatedWindowSquare(s32 center_x, s32 center_y, s32 half_size)
{
    register u32 scale_raw asm("r4") = half_size;
    register u32 saved_y asm("r9");
    u8 *angle;
    s32 scale;
    register s32 first asm("r8");
    register s32 second asm("r2");
    register struct WindowPolygon *out asm("r4");
    register s32 sum asm("r5");
    register s32 adjusted asm("r0");
    s32 radius;
    register s32 first_delta asm("r3");
    register s32 delta_y asm("r12");
    register s32 second_delta asm("r2");
    s32 x;
    register s32 y asm("r1");

    x = (u16)center_x;
    saved_y = (u16)center_y;
    scale_raw = (u16)scale_raw;
    angle = &gScreenTransitionProgress;
    {
        register s32 call_result asm("r0") = Sin256(*angle << 2);
        register s32 product asm("r1");
        asm volatile("" : "+r"(call_result));
        scale = (s16)scale_raw;
        product = call_result;
        product *= scale;
        first = product;
    }
    {
        register s32 call_result asm("r0") = Cos256(*angle << 2);
        asm volatile("" : "+r"(call_result));
        second = call_result;
    }
    second *= scale;

    out = &D_03006020;
    out->count = 4;

    {
        register s32 first_view asm("r3") = first;
        asm volatile("" : "+r"(first_view));
        sum = second + first_view;
    }
    adjusted = sum;
    if (sum < 0) {
        adjusted += 255;
    }
    radius = adjusted >> 8;
    asm volatile("" : : "r"(sum));
    asm volatile("" : : "r"(radius));
    x = (s16)x;
    asm volatile("" : : "r"(x));
    out->x0 = x + radius;

    {
        register s32 first_view asm("r0") = first;
        asm volatile("" : "+r"(first_view));
        first_delta = first_view - second;
    }
    adjusted = first_delta;
    if (first_delta < 0) {
        adjusted += 255;
    }
    delta_y = adjusted >> 8;
    {
        register u32 y_view asm("r1") = saved_y;
        register s32 y_shift asm("r0");
        asm volatile("" : "+r"(y_view));
        y_shift = y_view << 16;
        y = y_shift >> 16;
    }
    {
        register s32 y_sum asm("r0") = y;
        asm volatile("" : "+r"(y_sum));
        y_sum += delta_y;
        out->y0 = y_sum;
    }

    second_delta = second - first;
    adjusted = second_delta;
    if (second_delta < 0) {
        adjusted += 255;
    }
    first = adjusted >> 8;
    {
        register s32 x_sum asm("r0") = x;
        asm volatile("" : "+r"(x_sum));
        x_sum += first;
        out->x1 = x_sum;
    }
    out->y1 = y + radius;
    out->x2 = x - radius;
    {
        register s32 delta_view asm("r3") = delta_y;
        register s32 y2 asm("r0");
        asm volatile("" : "+r"(delta_view));
        asm volatile("sub %0, %1, %2" : "=r"(y2) : "r"(y), "r"(delta_view));
        out->y2 = y2;
    }
    out->x3 = x - first;
    {
        register s32 y_final asm("r0");
        asm volatile("sub %0, %1, %2" : "=r"(y_final) : "r"(y), "r"(radius));
        out->y3 = y_final;
    }
    out->terminator = 0;
}
