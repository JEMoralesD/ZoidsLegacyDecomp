#include "m2c_prelude.h"
#include "screen_effects.h"

/* Deterministically repaired m2c baseline. */
M2C_UNK ConfigureScaledScanlineWindows(M2C_UNK, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080954D8"); /* extern */
M2C_UNK ConfigurePolygonScanlineWindows(M2C_UNK, M2C_UNK, s32, s32) asm("func_080955A0");  /* extern */
M2C_UNK ConfigureRotatingSplitScanlineWindows(s32, s32, s32) asm("func_080955F4");               /* extern */
M2C_UNK ConfigureChevronScanlineWindow(u32, s32, s32) asm("func_0809564C");               /* extern */
M2C_UNK ConfigureCheckerboardScanlineWindows(s32, s32, s32) asm("func_080956B4");               /* extern */
M2C_UNK BuildScreenTransitionFrame() asm("func_08096774");                            /* extern */

asm(".set sub_08096308_state70, 0x03005F70\n"
    ".set sub_08096308_state72, 0x03005F72\n"
    ".set sub_08096308_state7A, 0x03005F7A\n"
    ".set sub_08096308_state7B, 0x03005F7B\n"
    ".set sub_08096308_state71_a, 0x03005F71\n"
    ".set sub_08096308_state71_b, 0x03005F71\n"
    ".set sub_08096308_state71_c, 0x03005F71");
extern u8 gScreenTransitionFlagsAtStart asm("sub_08096308_state70");
extern s8 gScreenTransitionProgressAtStart asm("sub_08096308_state72");
extern s8 gScreenTransitionBankAtStart asm("sub_08096308_state7A");
extern s8 gScreenTransitionHBlankActionAtStart asm("sub_08096308_state7B");
extern u8 gRotatingSplitTransitionDuration asm("sub_08096308_state71_a");
extern u8 gScaledWindowTransitionDuration asm("sub_08096308_state71_b");
extern u8 gWindowShapeTransitionDuration asm("sub_08096308_state71_c");

void StartScreenTransition(s32 transition_flags, s32 duration_updates) asm("func_08096308");

void StartScreenTransition(s32 transition_flags, s32 duration_updates) {
    s32 temp_r1;
    s32 temp_r1_3;
    s32 temp_r1_6;
    s32 temp_r2_2;
    s32 temp_r2_4;
    s32 var_r1;
    s32 var_r3;
    s32 var_r4;
    s8 temp_r0_2;
    s8 temp_r0_3;
    s8 temp_r1_5;
    s8 temp_r2_6;
    s8 temp_r3;
    s8 temp_r3_2;
    u16 color_index;
    s32 temp_r2;
    u32 var_r0_2;
    u8 *source_palette;
    u8 *palette_color_states;
    register u32 zero asm("ip");
    register u32 duration_or_transition_flags asm("r1");
    u32 channel_mask;
    u32 decrease_channel;
    u8 transition_kind;
    u8 temp_r1_2;
    u8 temp_r4;
    u8 var_r0;
    u32 duration;
    void *temp_r0_4;
    void *temp_r1_4;
    void *temp_r1_7;
    void *temp_r2_3;
    void *temp_r2_5;
    u8 *duration_override;

    transition_kind = transition_flags;
    /* Shape transitions use fixed durations; fades use the caller's duration. */
    duration_or_transition_flags = (u8)duration_updates;
    duration = duration_or_transition_flags;
    gScreenTransitionFlagsAtStart = (duration_or_transition_flags = transition_kind);
    asm volatile("" : "+r"(transition_kind));
    gScreenTransitionProgressAtStart = 0;
    temp_r2 = SCREEN_TRANSITION_KIND_MASK & transition_kind;
    transition_kind = temp_r2;
    if (temp_r2 > 4U) {
        goto high_dispatch;
    }
    temp_r1_2 = SCREEN_TRANSITION_SOFTWARE_PALETTE & duration_or_transition_flags;
    if (temp_r1_2 == 0) {
        switch (temp_r2) {                          /* irregular */
        case SCREEN_TRANSITION_FADE_FROM_BLACK:
            *(s16 *)0x03000052 = 0x10;
            goto block_14;
        case SCREEN_TRANSITION_FADE_TO_BLACK:
            *(s16 *)0x03000052 = (s16) temp_r1_2;
block_14:
            *(s16 *)0x0300004E = 0xFF;
            break;
        case SCREEN_TRANSITION_FADE_FROM_WHITE:
            *(s16 *)0x03000052 = 0x10;
            goto block_17;
        case SCREEN_TRANSITION_FADE_TO_WHITE:
            *(s16 *)0x03000052 = (s16) temp_r1_2;
block_17:
            *(s16 *)0x0300004E = 0xBF;
            break;
        }
    } else {
        source_palette = (u8 *)0x05000000;
        color_index = 0;
        palette_color_states = (u8 *)0x02000F40;
        zero = color_index;
        channel_mask = 0x1F;
        decrease_channel = 1;
        do {
            {
                register s32 dispatch_mode asm("r0") = transition_kind;
                asm volatile("" : "+r"(dispatch_mode));
                if (dispatch_mode == 2) {
                    goto palette_case2;
                }
                if (dispatch_mode > 2) {
                    goto palette_high_dispatch;
                }
                if (dispatch_mode == 1) {
                    goto palette_case1;
                }
palette_default_low:
                var_r3 = color_index * 2;
                goto palette_common;
palette_high_dispatch:
                if (transition_kind == 3) {
                    goto palette_case3;
                }
                if (transition_kind == 4) {
                    goto palette_case4;
                }
palette_default_high:
                asm volatile("" : : "r"(dispatch_mode));
                var_r3 = color_index * 2;
                goto palette_common;
            }
palette_case1:
            var_r3 = color_index * 2;
            temp_r1_3 = color_index * 0xC;
            temp_r1_4 = temp_r1_3 + palette_color_states;
            TRANSITION_COLOR_FIELD(temp_r1_4, s8, blue.value) = zero;
            TRANSITION_COLOR_FIELD(temp_r1_4, s8, green.value) = zero;
            TRANSITION_COLOR_FIELD(temp_r1_4, s8, red.value) = zero;
            TRANSITION_COLOR_FIELD(temp_r1_4, s8, red.increment) = (s8) (channel_mask & *source_palette);
            TRANSITION_COLOR_FIELD(temp_r1_4, s8, green.increment) = (s8) ((*(u16 *)source_palette >> 5) & channel_mask);
            TRANSITION_COLOR_FIELD(temp_r1_4, s8, blue.increment) = (s8) ((*(u16 *)source_palette >> 0xA) & channel_mask);
            TRANSITION_COLOR_FIELD(temp_r1_4, s8, red.decrease) = zero;
            TRANSITION_COLOR_FIELD(temp_r1_4, s8, green.decrease) = zero;
            TRANSITION_COLOR_FIELD(temp_r1_4, s8, blue.decrease) = zero;
            goto palette_common;

palette_case2:
            var_r4 = color_index * 2;
            asm volatile("" : "+r"(var_r4));
            temp_r1_6 = var_r4 + color_index;
            temp_r1_6 *= 4;
            temp_r1_7 = temp_r1_6 + palette_color_states;
            temp_r3_2 = channel_mask & *source_palette;
            TRANSITION_COLOR_FIELD(temp_r1_7, s8, red.value) = temp_r3_2;
            temp_r2_6 = (*(u16 *)source_palette >> 5) & channel_mask;
            TRANSITION_COLOR_FIELD(temp_r1_7, s8, green.value) = temp_r2_6;
            temp_r0_3 = (*(u16 *)source_palette >> 0xA) & channel_mask;
            TRANSITION_COLOR_FIELD(temp_r1_7, s8, blue.value) = temp_r0_3;
            TRANSITION_COLOR_FIELD(temp_r1_7, s8, red.increment) = temp_r3_2;
            TRANSITION_COLOR_FIELD(temp_r1_7, s8, green.increment) = temp_r2_6;
            TRANSITION_COLOR_FIELD(temp_r1_7, s8, blue.increment) = temp_r0_3;
            TRANSITION_COLOR_FIELD(temp_r1_7, s8, red.decrease) = decrease_channel;
            TRANSITION_COLOR_FIELD(temp_r1_7, s8, green.decrease) = decrease_channel;
            TRANSITION_COLOR_FIELD(temp_r1_7, s8, blue.decrease) = decrease_channel;
            goto palette_copy_r4;

palette_case3:
            var_r3 = color_index * 2;
            temp_r2_4 = color_index * 0xC;
            temp_r2_5 = temp_r2_4 + palette_color_states;
            TRANSITION_COLOR_FIELD(temp_r2_5, s8, blue.value) = channel_mask;
            TRANSITION_COLOR_FIELD(temp_r2_5, s8, green.value) = channel_mask;
            TRANSITION_COLOR_FIELD(temp_r2_5, s8, red.value) = channel_mask;
            TRANSITION_COLOR_FIELD(temp_r2_5, s8, red.increment) = (s8) (channel_mask & ~*source_palette);
            TRANSITION_COLOR_FIELD(temp_r2_5, s8, green.increment) = (s8) (channel_mask & ~(*(u16 *)source_palette >> 5));
            TRANSITION_COLOR_FIELD(temp_r2_5, s8, blue.increment) = (s8) (channel_mask & ~(*(u16 *)source_palette >> 0xA));
            TRANSITION_COLOR_FIELD(temp_r2_5, s8, red.decrease) = decrease_channel;
            TRANSITION_COLOR_FIELD(temp_r2_5, s8, green.decrease) = decrease_channel;
            TRANSITION_COLOR_FIELD(temp_r2_5, s8, blue.decrease) = decrease_channel;
            goto palette_common;

palette_case4:
            var_r4 = color_index * 2;
            temp_r2_2 = color_index * 0xC;
            temp_r2_3 = temp_r2_2 + palette_color_states;
            temp_r3 = channel_mask & *source_palette;
            TRANSITION_COLOR_FIELD(temp_r2_3, s8, red.value) = temp_r3;
            temp_r1_5 = (*(u16 *)source_palette >> 5) & channel_mask;
            TRANSITION_COLOR_FIELD(temp_r2_3, s8, green.value) = temp_r1_5;
            temp_r0_2 = (*(u16 *)source_palette >> 0xA) & channel_mask;
            TRANSITION_COLOR_FIELD(temp_r2_3, s8, blue.value) = temp_r0_2;
            TRANSITION_COLOR_FIELD(temp_r2_3, s8, red.increment) = (s8) (channel_mask - temp_r3);
            TRANSITION_COLOR_FIELD(temp_r2_3, s8, green.increment) = (s8) (channel_mask - temp_r1_5);
            TRANSITION_COLOR_FIELD(temp_r2_3, s8, blue.increment) = (s8) (channel_mask - temp_r0_2);
            TRANSITION_COLOR_FIELD(temp_r2_3, s8, red.decrease) = zero;
            TRANSITION_COLOR_FIELD(temp_r2_3, s8, green.decrease) = zero;
            TRANSITION_COLOR_FIELD(temp_r2_3, s8, blue.decrease) = zero;
palette_copy_r4:
            var_r3 = var_r4;
palette_common:
            temp_r0_4 = ((var_r3 + color_index) * 4) + palette_color_states;
            TRANSITION_COLOR_FIELD(temp_r0_4, s8, blue.fraction) = zero;
            TRANSITION_COLOR_FIELD(temp_r0_4, s8, green.fraction) = zero;
            TRANSITION_COLOR_FIELD(temp_r0_4, s8, red.fraction) = zero;
            source_palette += 2;
            color_index += 1;
            asm volatile("" : "+r"(color_index));
        } while ((u32) color_index <= 0x1FFU);
        goto palette_done;
    }
palette_done:
    asm volatile("" : "=r"(duration) : "0"((u8)duration));
    *(u8 *)SCREEN_TRANSITION_ADDRESS(duration_updates) = duration;
    return;

high_dispatch:
    if (temp_r2 > 0x10U) {
        goto high_over16;
    }
    if ((u32) (u8) (temp_r2 - 5) <= 1U) {
        if (temp_r2 == 5) {
            ConfigureRotatingSplitScanlineWindows(0x3F3F, 0, SCANLINE_WINDOW_HBLANK_CALLBACK);
            *(s16 *)0x0300004E = 0;
        } else {
            ConfigureRotatingSplitScanlineWindows(0, 0x3F, SCANLINE_WINDOW_HBLANK_CALLBACK);
        }
        duration_override = &gRotatingSplitTransitionDuration;
        var_r0 = 0x21;
        goto high_store;
    }
    if ((u32) (u8) (temp_r2 - 7) <= 1U) {
        if (temp_r2 == 7) {
            register u32 base7 asm("r0") = 0x087ED854;
            register u32 off7 asm("r2") = 0xA0;
            asm volatile("" : "+r"(off7));
            ConfigureScaledScanlineWindows(base7, 0, 0, base7 + (off7 << 2), 0, 0, 0x3F3F, 0, SCANLINE_WINDOW_HBLANK_CALLBACK);
            *(s16 *)0x0300004E = 0;
        } else {
            register u32 base8 asm("r0") = 0x087ED854;
            register u32 off8 asm("r1") = 0xA0;
            asm volatile("" : "+r"(off8));
            ConfigureScaledScanlineWindows(base8, 0, 0, base8 + (off8 << 2), 0, 0, 0, 0x3F, SCANLINE_WINDOW_HBLANK_CALLBACK);
        }
        duration_override = &gScaledWindowTransitionDuration;
        var_r0 = 0x24;
        goto high_store;
    }
    temp_r4 = temp_r2 - 9;
    if ((u32) temp_r4 <= 3U) {
        var_r0_2 = 0;
        if (temp_r2 != 9) {
            temp_r1 = 0xB ^ temp_r2;
            var_r0_2 = (u32) ((0 - temp_r1) | temp_r1) >> 0x1F;
        }
        var_r1 = 0;
        if ((u32) temp_r4 > 1U) {
            var_r1 = 1;
        }
        ConfigureChevronScanlineWindow(var_r0_2, var_r1, 8);
        if ((u32) temp_r4 > 1U) {
            goto high_twenty;
        }
high_zero:
        *(s16 *)0x0300004E = 0;
        goto high_twenty;
    }
    if ((u32) (u8) (temp_r2 - 0xD) <= 1U) {
        if (temp_r2 == 0xD) {
            ConfigurePolygonScanlineWindows(0x03006020, 0x3F3F, 0, SCANLINE_WINDOW_HBLANK_CALLBACK);
            goto high_zero;
        } else {
            ConfigurePolygonScanlineWindows(0x03006020, 0x3F3F, 0, SCANLINE_WINDOW_HBLANK_CALLBACK);
        }
high_twenty:
        duration_override = &gWindowShapeTransitionDuration;
        var_r0 = 0x20;
high_store:
        *duration_override = var_r0;
    }
    if ((u32) (u8) (transition_kind - 0xF) <= 1U) {
        register u32 mode15 asm("r2") = transition_kind;
        asm volatile("" : "+r"(mode15));
        if (mode15 == 0xF) {
            ConfigureCheckerboardScanlineWindows(0x3F3F, 0, SCANLINE_WINDOW_HBLANK_CALLBACK);
            *(s16 *)0x0300004E = 0;
        } else {
            ConfigureCheckerboardScanlineWindows(0, 0x3F, SCANLINE_WINDOW_HBLANK_CALLBACK);
        }
        *(u8 *)SCREEN_TRANSITION_ADDRESS(duration_updates) = 0x14U;
    }
    *(s16 *)0x05000000 = 0;
    return;

high_over16:
    if (transition_kind == 0x11) {
        *(s16 *)0x0300004E = 0;
    }
    *(u8 *)SCREEN_TRANSITION_ADDRESS(duration_updates) = 0x1C;
    gScreenTransitionBankAtStart = 0;
    gScreenTransitionHBlankActionAtStart = SCREEN_TRANSITION_HBLANK_INSTALL;
    BuildScreenTransitionFrame();
}
