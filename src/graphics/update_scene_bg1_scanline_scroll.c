#include "m2c_prelude.h"
#include "screen_effects.h"

extern void SetHBlankCallback(s32, s32) asm("func_08094290");
extern void ClearHBlankCallback(s32) asm("func_080942E0");
extern u8 gSceneBg1ScrollState asm("D_02031C98");
extern u8 gSceneBg1DisplayedBank asm("D_02031C99");
extern u8 gSceneBg1ScrollBuffers[] asm("D_02031C9A");
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern u8 gDisplayUpdateFlags asm("D_03000074");

void UpdateSceneBg1ScanlineScroll(s32 advance_buffer) asm("func_080A8ED4");

void UpdateSceneBg1ScanlineScroll(s32 advance_buffer)
{
    u8 scroll_state;

    if ((advance_buffer << 24) != 0) {
        scroll_state = gSceneBg1ScrollState;
        if (scroll_state == SCENE_SCANLINE_STARTING) {
            s16 *interrupt = (s16 *)0x04000208;

            *interrupt = 0;
            SetHBlankCallback(2, SCENE_SCANLINE_CALLBACK_RAM);
            *interrupt = scroll_state;
            gSceneBg1ScrollState = SCENE_SCANLINE_ACTIVE;
        } else if (scroll_state == SCENE_SCANLINE_STOPPING) {
            s16 *interrupt = (s16 *)0x04000208;

            *interrupt = 0;
            ClearHBlankCallback(2);
            gDisplayUpdateFlags &= 0xDF;
            *interrupt = 1;
            gSceneBg1ScrollState = SCENE_SCANLINE_DISABLED;
            return;
        }
        gSceneBg1DisplayedBank ^= 1;
    }

    if (gSceneBg1ScrollState == SCENE_SCANLINE_ACTIVE) {
        register u16 *display asm("r4") = (u16 *)0x04000014;
        register u8 *scroll_buffer_base asm("r3") = gSceneBg1ScrollBuffers;
        register u8 *displayed_bank_address asm("r2") = &gSceneBg1DisplayedBank;
        u8 *y_scroll_buffer_base;

        asm volatile("" : "+r"(display), "+r"(scroll_buffer_base), "+r"(displayed_bank_address));
        display[0] = *(u16 *)(scroll_buffer_base + *displayed_bank_address * 0x280);
        display++;
        asm volatile("" : "+r"(display));
        {
            register u32 selected asm("r1") = *displayed_bank_address;
            register u32 offset asm("r0");
            register u8 *y_scroll_buffer_view asm("r5");

            offset = selected << 2;
            offset += selected;
            offset <<= 7;
            y_scroll_buffer_view = scroll_buffer_base + SCENE_BG1_SCROLL_OFFSET(y);
            y_scroll_buffer_base = y_scroll_buffer_view;
            display[0] = *(u16 *)(offset + (u32)y_scroll_buffer_view);
        }
        {
            register s32 *state asm("r4") = gFieldCameraScrollOffsets;

            state[2] = *(s16 *)(scroll_buffer_base + *displayed_bank_address * 0x280) << 8;
            state[3] = *(s16 *)(y_scroll_buffer_base + *displayed_bank_address * 0x280) << 8;
        }
    }
}
