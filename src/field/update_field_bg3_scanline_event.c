#include "m2c_prelude.h"
#include "field_display.h"
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");

void UpdateFieldBg3ScanlineEvent(u8 synchronize_display) asm("func_0809EB68");

void UpdateFieldBg3ScanlineEvent(u8 synchronize_display) {
    u8 *state_or_hide_request_address;
    register u8 next_hide_request asm("r0");
    u8 hide_request;

    state_or_hide_request_address = (u8 *)FIELD_BG3_SCANLINE_STATE_RAM;
    switch (*state_or_hide_request_address) {
    case FIELD_BG3_SCANLINE_READY:
        if (synchronize_display != 0) {
            *(u8 *)FIELD_BG3_SCANLINE_STATE_RAM = FIELD_BG3_SCANLINE_ACTIVE;
        case FIELD_BG3_SCANLINE_ACTIVE:
            hide_request = *(u8 *)FIELD_BG3_SCANLINE_HIDE_REQUEST_RAM;
            state_or_hide_request_address = (u8 *)FIELD_BG3_SCANLINE_HIDE_REQUEST_RAM;
            if (hide_request != 0) {
                register volatile u16 *bg3_register asm("r1") = (volatile u16 *)0x0400000E;
                *bg3_register = 0x1F0E;
                if (synchronize_display == 0) {
                    bg3_register += 7;
                    *bg3_register = gFieldCameraScrollOffsets[6] >> 8;
                    bg3_register += 1;
                    *bg3_register = gFieldCameraScrollOffsets[7] >> 8;
                    bg3_register += 25;
                    *bg3_register = *(volatile u16 *)FIELD_BLEND_CONTROL_SHADOW_RAM;
                    goto block_12;
                }
                goto block_13;
            }
        block_12:
            if (synchronize_display != 0) {
            block_13:
                next_hide_request = *(u8 *)FIELD_MESSAGE_WINDOW_STATE_RAM | *(u8 *)FIELD_BG3_FORCE_HIDE_RAM;
                goto block_17;
            }
        }
        return;
    case FIELD_BG3_SCANLINE_STOPPING:
        if (synchronize_display != 0) {
            *(volatile u16 *)0x0400000E = 0x300;
            *(volatile u8 *)FIELD_DISPLAY_UPDATE_FLAGS_RAM &= 0xEF;
            next_hide_request = 0;
        block_17:
            *state_or_hide_request_address = next_hide_request;
        }
        break;
    }
}
