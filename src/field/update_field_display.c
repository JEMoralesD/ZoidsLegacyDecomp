#include "m2c_prelude.h"
#include "field_display.h"

extern u16 gFieldMapState asm("D_0202ECF4");
extern u8 gFieldMapModeFlags asm("D_020324B0");
extern s32 gFieldBg1ScrollDeltaXFixed8 asm("D_02032590");
extern s32 gFieldBg1ScrollDeltaYFixed8 asm("D_02032594");
extern s32 gFieldBg2ScrollDeltaYFixed8 asm("D_02032598");
extern u8 gFieldPaletteAnimationFrame asm("D_0203259C");
extern u8 gFieldPaletteAnimationTimer asm("D_0203259D");
extern u16 gBlendControlShadow asm("D_0300004E");
extern u16 gBlendAlphaShadow asm("D_03000050");
extern struct FieldBackgroundScrollOffsets gFieldCameraScrollOffsets asm("D_03000054");

extern void QueueCopy(u32, u32, u32) asm("func_08095208");
extern s32 IsScreenTransitionComplete(void) asm("func_0809669C");
extern void UpdateFieldMapDecorationSprites(void) asm("func_0809D734");
extern void StreamFieldBackgroundTilemaps(void) asm("func_0809DD80");
extern void UpdateFieldActors(void) asm("func_080A9F8C");

void UpdateFieldDisplay(void) asm("func_0809DFFC");

void UpdateFieldDisplay(void)
{
    u16 map_id;

    UpdateFieldActors();
    if (gFieldMapState == 0 && (gFieldMapModeFlags & 4) != 0) {
        struct FieldBackgroundScrollOffsets *background_scroll = &gFieldCameraScrollOffsets;
        background_scroll->bg3_x_fixed8 = background_scroll->bg0_x_fixed8;
        background_scroll->bg3_y_fixed8 = background_scroll->bg0_y_fixed8;
    }
    StreamFieldBackgroundTilemaps();
    map_id = gFieldMapState;

    if (map_id == 0 || map_id == 0x40) {
        register s32 scroll_or_flags_carrier asm("r2");
        register u8 *display_flags_address asm("r4");
        register u8 *display_flags_source asm("r1");
        register s32 display_flag_bits asm("r0");
        struct FieldBackgroundScrollOffsets *background_scroll;
        s32 next_bg1_y_fixed8;

        display_flags_source = &gFieldMapModeFlags;
        scroll_or_flags_carrier = *display_flags_source;
        display_flag_bits = 1;
        display_flag_bits &= scroll_or_flags_carrier;
        display_flags_address = display_flags_source;
        if (display_flag_bits != 0) {
            register s32 next_bg1_delta_x_fixed8 asm("r3");

            next_bg1_delta_x_fixed8 = gFieldBg1ScrollDeltaXFixed8 - 0x40;
            gFieldBg1ScrollDeltaXFixed8 = next_bg1_delta_x_fixed8;
            scroll_or_flags_carrier = gFieldBg1ScrollDeltaYFixed8 + 0x30;
            gFieldBg1ScrollDeltaYFixed8 = scroll_or_flags_carrier;
            background_scroll = &gFieldCameraScrollOffsets;
            background_scroll->bg1_x_fixed8 = background_scroll->bg0_x_fixed8 + next_bg1_delta_x_fixed8;
            next_bg1_y_fixed8 = background_scroll->bg0_y_fixed8 + scroll_or_flags_carrier;
            goto store_scroll_y;
        }
        display_flag_bits = 2;
        display_flag_bits &= scroll_or_flags_carrier;
        if (display_flag_bits != 0) {
            background_scroll = &gFieldCameraScrollOffsets;
            scroll_or_flags_carrier = background_scroll->bg1_y_fixed8;
            if (scroll_or_flags_carrier <= 0x4FFF) {
                background_scroll->bg1_x_fixed8 -= 0x40;
                next_bg1_y_fixed8 = scroll_or_flags_carrier;
                next_bg1_y_fixed8 += 0x30;
                goto store_scroll_y;
            }
        }
        goto after_scroll;
store_scroll_y:
        background_scroll->bg1_y_fixed8 = next_bg1_y_fixed8;
after_scroll:
        if (gFieldMapState == 0) {
            scroll_or_flags_carrier = gFieldBg2ScrollDeltaYFixed8 + 0x10;
            gFieldBg2ScrollDeltaYFixed8 = scroll_or_flags_carrier;
            background_scroll = &gFieldCameraScrollOffsets;
            background_scroll->bg2_x_fixed8 = background_scroll->bg0_x_fixed8;
            background_scroll->bg2_y_fixed8 = background_scroll->bg0_y_fixed8 + scroll_or_flags_carrier;
        }
        if (*display_flags_address != 0 && (u8)IsScreenTransitionComplete() == 0) {
            gBlendControlShadow = 0x1542;
            if ((*display_flags_address & 4) != 0)
                gBlendControlShadow |= 0x0808;
            gBlendAlphaShadow = 0x0C0A;
        }
        UpdateFieldMapDecorationSprites();
    } else if (map_id == 0x7D) {
        u8 *palette_animation_timer = &gFieldPaletteAnimationTimer;
        if (*palette_animation_timer == 0) {
            u8 *palette_animation_frame = &gFieldPaletteAnimationFrame;

            QueueCopy(0x085CE1FC + (*palette_animation_frame << 5), 0x05000020, 0x20);
            {
                u32 palette_frame_offset = *palette_animation_frame << 5;
                register u32 palette_table_base asm("r1");

                palette_table_base = 0x085CE39C;
                /* Preserve the source literal in r1 without emitting code. */
                asm("" : "+r"(palette_table_base));
                QueueCopy(palette_frame_offset + palette_table_base, 0x05000040, 0x20);
            }
        }
        (*palette_animation_timer)++;
        if (*palette_animation_timer == FIELD_PALETTE_ANIMATION_INTERVAL) {
            *palette_animation_timer = 0;
            {
                u8 *palette_animation_frame = &gFieldPaletteAnimationFrame;

                (*palette_animation_frame)++;
                if (*palette_animation_frame == 0xD)
                    *palette_animation_frame = 0;
            }
        }
    } else if (map_id == 0x7E) {
        u8 *palette_animation_timer = &gFieldPaletteAnimationTimer;
        if (*palette_animation_timer == 0) {
            u32 palette_frame_offset = gFieldPaletteAnimationFrame << 5;
            register u32 palette_table_base asm("r1");

            palette_table_base = 0x085CE1FC;
            /* Preserve the source literal in r1 without emitting code. */
            asm("" : "+r"(palette_table_base));
            QueueCopy(palette_frame_offset + palette_table_base, 0x05000060, 0x20);
        }
        (*palette_animation_timer)++;
        if (*palette_animation_timer == FIELD_PALETTE_ANIMATION_INTERVAL) {
            *palette_animation_timer = 0;
            {
                u8 *palette_animation_frame = &gFieldPaletteAnimationFrame;

                (*palette_animation_frame)++;
                if (*palette_animation_frame == 6)
                    *palette_animation_frame = 0;
            }
        }
    }
}
