#include "m2c_prelude.h"
#include "screen_effects.h"
extern void SetHBlankCallback(s32, s32) asm("func_08094290");
extern void ClearHBlankCallback(s32) asm("func_080942E0");
extern u8 gSceneBg0ScanlineScrollState asm("D_0203198C");
extern u8 gSceneBg0ScanlineScrollBufferIndex asm("D_0203198D");
extern u8 gSceneBg0ScanlineScrollBuffers[] asm("D_0203198E");
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern u8 gDisplayUpdateFlags asm("D_03000074");

void UpdateSceneBg0ScanlineScroll(s32 swap_at_frame_boundary) asm("func_080A7860");

void UpdateSceneBg0ScanlineScroll(s32 swap_at_frame_boundary) {
    u8 scroll_state;
    if ((swap_at_frame_boundary << 0x18) != 0) {
        scroll_state = gSceneBg0ScanlineScrollState;
        if (scroll_state == SCENE_BG0_SCROLL_STARTING) {
            s16 *interrupt_master_enable = (s16 *)0x04000208;
            *interrupt_master_enable = 0;
            SetHBlankCallback(SCENE_BG0_SCROLL_CALLBACK_SLOT, SCENE_BG0_SCROLL_CALLBACK_RAM);
            *interrupt_master_enable = scroll_state;
            gSceneBg0ScanlineScrollState = SCENE_BG0_SCROLL_ACTIVE;
        } else if (scroll_state == SCENE_BG0_SCROLL_STOPPING) {
            s16 *interrupt_master_enable = (s16 *)0x04000208;
            *interrupt_master_enable = 0;
            ClearHBlankCallback(SCENE_BG0_SCROLL_CALLBACK_SLOT);
            gDisplayUpdateFlags &= 0xF7;
            *interrupt_master_enable = 1;
            gSceneBg0ScanlineScrollState = SCENE_BG0_SCROLL_DISABLED;
            return;
        }
        gSceneBg0ScanlineScrollBufferIndex ^= 1;
    }
    if (gSceneBg0ScanlineScrollState == SCENE_BG0_SCROLL_ACTIVE) {
        u16 *bg0_vertical_scroll = (u16 *)0x04000012;
        u8 *scroll_buffers = gSceneBg0ScanlineScrollBuffers;
        *bg0_vertical_scroll = *(u16 *)(scroll_buffers + gSceneBg0ScanlineScrollBufferIndex * SCENE_BG0_SCROLL_BUFFER_BYTES);
        gFieldCameraScrollOffsets[1] = *(s16 *)(scroll_buffers + gSceneBg0ScanlineScrollBufferIndex * SCENE_BG0_SCROLL_BUFFER_BYTES) << 8;
    }
}
