#include "m2c_prelude.h"

extern s32 CallFunctionR0(s32) asm("func_080ECD5C");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");

extern s32 gRandomCallback asm("D_03000010");
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern u16 gFieldMapDimensions[] asm("D_020324A4");

void ShakeFieldCameraAndDecreaseBg1ScrollY(void) asm("func_080A6AE4");

void ShakeFieldCameraAndDecreaseBg1ScrollY(void)
{
    u32 elapsed_updates;
    s32 shake_x_fixed8;
    s32 shake_y_fixed8;
    s32 bg1_y_offset_fixed8;
    s32 shake_finished;
    s32 zero;
    u16 *map_dimensions;
    s32 *bg_scroll_offsets;
    s32 scroll_limit;

    elapsed_updates = 0;
    shake_y_fixed8 = 0;
    shake_x_fixed8 = 0;
    bg1_y_offset_fixed8 = 0;
    shake_finished = 0;
    zero = 0;
    map_dimensions = gFieldMapDimensions;

loop:
    if (elapsed_updates > 43) {
        bg1_y_offset_fixed8 -= 16;
    }
    if (elapsed_updates <= 0x12B) {
        s32 *random_callback_address;
        s32 random_value;
        s32 rounded_shake_y_or_delta;
        s32 rounded_shake_x;
        s32 shake_x_delta;
        s32 camera_x;
        s32 camera_y;

        random_callback_address = &gRandomCallback;
        random_value = (u32)(CallFunctionR0(*random_callback_address) * 7) >> 15;
        random_value -= 3;
        random_value <<= 8;
        rounded_shake_x = shake_x_fixed8;
        if (shake_x_fixed8 < 0) {
            rounded_shake_x += 0xFF;
        }
        rounded_shake_x >>= 8;
        rounded_shake_x <<= 8;
        shake_x_delta = random_value - rounded_shake_x;

        random_value = (u32)(CallFunctionR0(*random_callback_address) * 7) >> 15;
        random_value -= 3;
        random_value <<= 8;
        rounded_shake_y_or_delta = shake_y_fixed8;
        if (shake_y_fixed8 < 0) {
            rounded_shake_y_or_delta += 0xFF;
        }
        rounded_shake_y_or_delta >>= 8;
        rounded_shake_y_or_delta <<= 8;
        rounded_shake_y_or_delta = random_value - rounded_shake_y_or_delta;

        shake_x_fixed8 += shake_x_delta;
        shake_y_fixed8 += rounded_shake_y_or_delta;
        bg_scroll_offsets = gFieldCameraScrollOffsets;
        camera_x = bg_scroll_offsets[0] + shake_x_delta;
        bg_scroll_offsets[0] = camera_x;
        bg_scroll_offsets[1] += rounded_shake_y_or_delta;

        if (camera_x < 0) {
            scroll_limit = zero;
            goto store_x1;
        }
        scroll_limit = (map_dimensions[0] << 11) - 0xF000;
        if (camera_x > scroll_limit) {
store_x1:
            bg_scroll_offsets[0] = scroll_limit;
        }
        camera_y = bg_scroll_offsets[1];
        if (camera_y < 0) {
            bg_scroll_offsets[1] = zero;
        } else {
            scroll_limit = (map_dimensions[1] << 11) - 0xA000;
            if (camera_y > scroll_limit) {
                bg_scroll_offsets[1] = scroll_limit;
            }
        }
        elapsed_updates = (u16)(elapsed_updates + 1);
    } else {
        s32 restored_camera_x;
        s32 restored_camera_y;

        bg_scroll_offsets = gFieldCameraScrollOffsets;
        restored_camera_x = bg_scroll_offsets[0] - shake_x_fixed8;
        bg_scroll_offsets[0] = restored_camera_x;
        bg_scroll_offsets[1] -= shake_y_fixed8;

        if (restored_camera_x < 0) {
            scroll_limit = zero;
            goto store_x2;
        }
        scroll_limit = (map_dimensions[0] << 11) - 0xF000;
        if (restored_camera_x > scroll_limit) {
store_x2:
            bg_scroll_offsets[0] = scroll_limit;
        }
        restored_camera_y = bg_scroll_offsets[1];
        if (restored_camera_y < 0) {
            scroll_limit = zero;
            goto store_y2;
        }
        scroll_limit = (map_dimensions[1] << 11) - 0xA000;
        if (restored_camera_y > scroll_limit) {
store_y2:
            bg_scroll_offsets[1] = scroll_limit;
        }
        shake_finished = 1;
    }

    asm volatile("" : : "r"(bg_scroll_offsets), "r"(bg_scroll_offsets), "r"(bg_scroll_offsets), "r"(bg_scroll_offsets), "r"(bg_scroll_offsets), "r"(bg_scroll_offsets));
    asm volatile("" : : "r"(bg_scroll_offsets), "r"(bg_scroll_offsets), "r"(bg_scroll_offsets), "r"(bg_scroll_offsets), "r"(bg_scroll_offsets), "r"(bg_scroll_offsets));
    asm volatile("" : : "r"(bg_scroll_offsets), "r"(bg_scroll_offsets), "r"(bg_scroll_offsets), "r"(bg_scroll_offsets), "r"(bg_scroll_offsets), "r"(bg_scroll_offsets));
    bg_scroll_offsets[2] = bg_scroll_offsets[0];
    bg_scroll_offsets[3] = bg_scroll_offsets[1] + bg1_y_offset_fixed8;
    YieldTaskForUpdates(1);
    if (shake_finished == 0) {
        goto loop;
    }
}
