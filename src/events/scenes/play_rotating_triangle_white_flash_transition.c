#include "m2c_prelude.h"
#include "../../graphics/screen_effects.h"

extern void PlaySong(u16) asm("func_08092E84");
extern s16 Sin256(s16) asm("func_08092A90");
extern s16 Cos256(s16) asm("func_08092ADC");
extern void DisableDisplayWindows(void) asm("func_0809534C");
extern void ConfigurePolygonScanlineWindows(void *, s16, s16, u8) asm("func_080955A0");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");
extern volatile s16 gDisplayWindowInsideLayers asm("D_03005EF8");
extern volatile s16 gDisplayWindowOutsideLayers asm("D_03005EFA");
extern volatile s16 gBlendBrightnessShadow asm("D_03000052");

void PlayRotatingTriangleWhiteFlashTransition(void) asm("func_080A6C40");

void PlayRotatingTriangleWhiteFlashTransition(void)
{
    /* Native code writes the terminator beyond this array; preserve its stack layout. */
    s16 triangle_window_stream[21];
    u8 elapsed_updates;
    register s32 blade_half_angle asm("r9");

    PlaySong(0x56);
    ConfigurePolygonScanlineWindows(triangle_window_stream, 0x3F3F, 0x1F1F, SCANLINE_WINDOW_HBLANK_CALLBACK);
    *(s16 *)0x0300004E = 0xBF;
    *(s16 *)0x05000000 = 0x7FFF;
    elapsed_updates = 0;
    *(u8 *)0x03000075 = 2;

loop:
    {
        elapsed_updates++;
        if (elapsed_updates <= 31) {
            blade_half_angle = elapsed_updates >> 1;
            *(s16 *)0x03000052 = blade_half_angle;
        } else {
            register u32 update_count_view asm("r0") = elapsed_updates;

            if (update_count_view > 61) {
                register s32 maximum_half_angle asm("r1") = 16;
                register volatile s16 *brightness_shadow asm("r1");
                s32 fade_progress;

                blade_half_angle = maximum_half_angle;
                gDisplayWindowInsideLayers = 0;
                gDisplayWindowOutsideLayers = 0x3F3F;
                brightness_shadow = &gBlendBrightnessShadow;
                fade_progress = elapsed_updates;
                fade_progress -= 60;
                if (fade_progress < 0) {
                    fade_progress += 3;
                }
                *brightness_shadow = fade_progress >> 2;
            }
        }

        {
            u8 blade_index = 0;

            do {
                s32 index = blade_index * 7;
                register s32 blade_rotation asm("r5");

                triangle_window_stream[index] = 3;
                triangle_window_stream[index + SCENE_TRIANGLE_CENTER_X] = 120;
                triangle_window_stream[index + SCENE_TRIANGLE_CENTER_Y] = 32;
                blade_rotation = DivideSigned32(blade_index << 8, 3);
                asm("" : "+r"(blade_rotation));
                blade_rotation += (s32)elapsed_updates * 4;
                {
                    register s32 phase_add asm("r0") = blade_half_angle;
                    register s32 first_edge_angle asm("r4");
                    register s32 second_edge_half_angle asm("r1");

                    first_edge_angle = blade_rotation + phase_add;
                    first_edge_angle = (s16)first_edge_angle;
                    triangle_window_stream[index + SCENE_TRIANGLE_FIRST_X] = Cos256(first_edge_angle) + 120;
                    triangle_window_stream[index + SCENE_TRIANGLE_FIRST_Y] = Sin256(first_edge_angle) + 32;
                    second_edge_half_angle = blade_half_angle;
                    blade_rotation -= second_edge_half_angle;
                    blade_rotation = (s16)blade_rotation;
                    triangle_window_stream[index + SCENE_TRIANGLE_SECOND_X] = Cos256(blade_rotation) + 120;
                    triangle_window_stream[index + SCENE_TRIANGLE_SECOND_Y] = Sin256(blade_rotation) + 32;
                }
                blade_index++;
            } while (blade_index <= 2);
            triangle_window_stream[blade_index * 7] = 0;
        }
    }
    YieldTaskForUpdates(1);
    {
        register u32 update_count_view asm("r0") = elapsed_updates;

        if (update_count_view != 126) {
            goto loop;
        }
    }
    *(u8 *)0x03000075 = 1;
    DisableDisplayWindows();
}
