#include "m2c_prelude.h"
#include "window.h"
#define NULL ((void *)0)

M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
s32 CreateSprite(M2C_UNK, M2C_UNK, s32, s16, s32, s32, s32, s32, s32) asm("func_08094484"); /* extern */
M2C_UNK DestroySprite(s32) asm("func_08094554");                         /* extern */
M2C_UNK RequestWindowRefresh() asm("func_080972C8");                            /* extern */
M2C_UNK ReleaseWindowTile(u16) asm("func_08097980");                         /* extern */
M2C_UNK AllocateWindowShiftJisGlyphTiles(s32, u8, M2C_UNK *) asm("func_08097B2C");          /* extern */
M2C_UNK AllocateWindowSingleByteGlyphTile(u8, u8, M2C_UNK *) asm("func_08097CEC");           /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */
u8 *DrawWindowText(void *window, u8 *text) asm("func_08097DA8");

u8 *DrawWindowText(void *window, u8 *text) {
    M2C_UNK sp14;
    s32 sp18;
    s32 temp_r2;
    s32 temp_r5_3;
    s32 temp_r1_4;
    register s32 var_r2 asm("r2"); /*LOCK*/
    s32 var_r2_2;
    s32 vsl2;
    s32 var_r6;
    register s32 var_r6_2 asm("r6");
    s32 var_r7;
    s8 *temp_r5;
    s8 *temp_r5_2;
    u16 temp_r0_7;
    u16 temp_r2_2;
    u32 temp_r0_2;
    u32 temp_r0_3;
    u32 temp_r0_4;
    u32 temp_r0_6;
    u32 temp_r1;
    u32 temp_r1_2;
    u32 temp_r1_3;
    u32 temp_r1_5;
    u32 var_r1;
    register u32 var_sl asm("sl");
    u8 *text_cursor;
    s32 temp_r0;
    u8 temp_r0_5;
    s32 temp_r3;
    u8 var_r0;
    s8 *temp_r5_4;
    s32 pa;
    u8 *player_name;
    register s32 zz asm("r6");
    s32 qb;
    s32 rhsv;
    s8 *bp2;
    s32 w8;
    s32 tA;
    s32 tB;
    s32 *pA890;
    s32 *pA890b;
    s32 rv;
    u32 zr;
    player_name = (u8 *)0x02021774;
    tA = 0x080ED620;
    tB = 0x080ED648;
    text_cursor = text;
    var_r0 = *text;
    if (var_r0 == 0) {
    } else {
loop_2:
        if ((u32) var_r0 <= 0x1FU) {
            temp_r0 = M2C_FIELD(text_cursor, u8 *, 0);
            switch (temp_r0) {                      /* irregular */
            case WINDOW_TEXT_SET_COLOR:
                WINDOW_FIELD(window, u8, text_color) = (u8) M2C_FIELD(text_cursor, u8 *, 1);
                text_cursor += 2;
                break;
            case WINDOW_TEXT_SET_POSITION:
                WINDOW_FIELD(window, s16, text_column) = (s16) M2C_FIELD(text_cursor, u8 *, 1);
                WINDOW_FIELD(window, u16, text_row) = (u16) M2C_FIELD(text_cursor, u8 *, 2);
                var_r2 = 3;
                asm("" : "+r"(var_r2));
                text_cursor += var_r2;
                goto loop_bottom;
            case WINDOW_TEXT_PLAYER_NAME:
                DrawWindowText(window, player_name);
                { register s32 k1 asm("r3"); k1 = 1; asm("" : "+r"(k1)); text_cursor += k1; }
                { register s32 k2 asm("r0"); k2 = 2; asm("" : "+r"(k2)); var_sl = k2; }
                goto block_32;
            case WINDOW_TEXT_NEWLINE:
                WINDOW_FIELD(window, s16, text_column) = 0;
                WINDOW_FIELD(window, u16, text_row) = (u16) (WINDOW_FIELD(window, u16, text_row) + 2);
                WINDOW_FIELD(window, u16, page_progress_rows) = (u16) (WINDOW_FIELD(window, u16, page_progress_rows) + 2);
                goto block_40;
            default:
                goto block_32;
            }
        } else {
            { u8 t80; t80 = (u8) (var_r0 + 0x80);
            if ((u32) t80 <= 0x1FU) {
                if (((s32) WINDOW_FIELD(window, s16, text_column) < (s32) (WINDOW_FIELD(window, u16, width) - 2)) && ((s32) (s16) WINDOW_FIELD(window, u16, text_row) < (s32) (WINDOW_FIELD(window, u16, height) - 3))) {
                    { s32 hi; s32 lo; hi = M2C_FIELD(text_cursor, u8 *, 0) << 8; asm("" : "+r"(hi)); lo = M2C_FIELD(text_cursor, u8 *, 1); asm("" : "+r"(lo)); AllocateWindowShiftJisGlyphTiles(lo | hi, WINDOW_FIELD(window, u8, text_color), &sp14); }
                    { register s32 pa asm("r2"); s32 pb; pa = WINDOW_FIELD(window, s16, text_column); pb = (((s16) WINDOW_FIELD(window, u16, text_row) + 1) * WINDOW_FIELD(window, u16, width)); pb += 1;  temp_r5 = (s8 *) window + (((pa + pb) * 2) + 0x1E); }
                    { s32 hv; register s32 mh asm("r2"); s32 ml; hv = *(u16 *) temp_r5; mh = 0x3FF; ml = mh; asm("" :: "r"(mh)); temp_r1 = hv & ml; }
                    temp_r0_2 = *(u32 *)0x02021664;
                    if ((temp_r1 < temp_r0_2) || (temp_r1 >= (u32) (temp_r0_2 + 0x40))) {
                        ReleaseWindowTile((u16) (temp_r1 - *(s32 *)0x02021670));
                    }
                    { s32 hv; register s32 mh asm("r3"); s32 ml; hv = *(u16 *) ((WINDOW_FIELD(window, u16, width) * 2) + temp_r5); mh = 0x3FF; ml = mh; asm("" :: "r"(mh)); temp_r1_2 = hv & ml; }
                    temp_r0_3 = *(s32 *)0x02021664;
                    if ((temp_r1_2 < temp_r0_3) || (temp_r1_2 >= (u32) (temp_r0_3 + 0x40))) {
                        ReleaseWindowTile((u16) (temp_r1_2 - *(s32 *)0x02021670));
                    }
                    *(u16 *) temp_r5 = M2C_FIELD(&sp14, u16 *, 0);
                    { register u16 *dst asm("r1"); s32 v; { s32 b5; b5 = (s32) temp_r5; dst = (u16 *)((WINDOW_FIELD(window, u16, width) * 2) + b5); } v = M2C_FIELD(&sp14, u16 *, 2); *dst = (u16) v; }
                    WINDOW_FIELD(window, s16, text_column) = (s16) ((u16) WINDOW_FIELD(window, s16, text_column) + 1);
                }
                { s32 c2; c2 = 2; asm("" : "+r"(c2)); text_cursor += c2; var_sl = c2; }
            } else {
                { s32 lhs; lhs = (s32) WINDOW_FIELD(window, s16, text_column); asm volatile("" :: "r"(t80));
                if ((lhs < (s32) (WINDOW_FIELD(window, u16, width) - 2)) && ((s32) (s16) WINDOW_FIELD(window, u16, text_row) < (s32) (WINDOW_FIELD(window, u16, height) - 2))) {
                    AllocateWindowSingleByteGlyphTile(M2C_FIELD(text_cursor, u8 *, 0), WINDOW_FIELD(window, u8, text_color), &sp14);
                    { register s32 pa asm("r2"); s32 pb; pa = WINDOW_FIELD(window, s16, text_column); pb = (((s16) WINDOW_FIELD(window, u16, text_row) + 1) * WINDOW_FIELD(window, u16, width)); pb += 1;  temp_r5_2 = (s8 *) window + (((pa + pb) * 2) + 0x1E); }
                    { s32 hv; register s32 mh asm("r2"); s32 ml; hv = *(u16 *) temp_r5_2; mh = 0x3FF; ml = mh; asm("" :: "r"(mh)); temp_r1_3 = hv & ml; }
                    temp_r0_4 = *(s32 *)0x02021664;
                    if ((temp_r1_3 < temp_r0_4) || (temp_r1_3 >= (u32) (temp_r0_4 + 0x40))) {
                        ReleaseWindowTile((u16) (temp_r1_3 - *(s32 *)0x02021670));
                    }
                    *(u16 *) temp_r5_2 = M2C_FIELD(&sp14, u16 *, 0);
                    WINDOW_FIELD(window, s16, text_column) = (s16) ((u16) WINDOW_FIELD(window, s16, text_column) + 1);
                }
                { register s32 c1 asm("r3"); c1 = 1; text_cursor += c1; var_sl = c1; }
            } } }
block_32:
            temp_r1_4 = WINDOW_FIELD(window, s32, flags);
            temp_r2 = WINDOW_FLAG_TYPEWRITER_TEXT & temp_r1_4;
            if (temp_r2 == 0) {
                temp_r3 = *text_cursor;
                if (temp_r3 != 0xA) {
                    if (!(temp_r1_4 & WINDOW_FLAG_WRAP_TEXT)) {
                    } else if (WINDOW_FIELD(window, s16, text_column) != (WINDOW_FIELD(window, u16, width) - 2)) {
                    } else {
                        goto block_38;
                    }
                } else {
block_38:
                    WINDOW_FIELD(window, s16, text_column) = temp_r2;
                    WINDOW_FIELD(window, u16, text_row) = (u16) (WINDOW_FIELD(window, u16, text_row) + var_sl);
                    if (temp_r3 != 0xA) {
                    } else {
block_40:
                        text_cursor += 1;
                    }
                }
            } else {
                WINDOW_FIELD(window, s32, flags) = (s32) (temp_r1_4 | WINDOW_FLAG_TILEMAP_DIRTY);
                RequestWindowRefresh();
                if (*(u8 *)0x03000075 == 1) {
                    YieldTaskForUpdates(2);
                } else {
                    YieldTaskForUpdates(1);
                }
                temp_r0_5 = *text_cursor;
                if ((u32) temp_r0_5 <= 1U) {
                } else if (temp_r0_5 != 0xA) {
                    if (!(WINDOW_FIELD(window, s32, flags) & WINDOW_FLAG_WRAP_TEXT)) {
                    } else if (WINDOW_FIELD(window, s16, text_column) != (WINDOW_FIELD(window, u16, width) - 2)) {
                    } else {
                        goto block_52;
                    }
                } else {
block_52:
                    { s32 t10; t10 = WINDOW_FIELD(window, u16, page_progress_rows) + var_sl;
                    zz = 0;
                    WINDOW_FIELD(window, u16, page_progress_rows) = (u16) t10; }
                    temp_r5_3 = (s16) WINDOW_FIELD(window, u16, text_row);
                    temp_r2_2 = WINDOW_FIELD(window, u16, height);
                    vsl2 = var_sl + 2;
                    if (temp_r5_3 != (temp_r2_2 - vsl2)) {
                    } else {
                        if (WINDOW_FIELD(window, u16, page_progress_rows) == (temp_r2_2 - 2)) {
                            rv = CreateSprite(tA, tB, 1, (s16) (((pa = WINDOW_FIELD(window, s16, text_column), pa += 1, pa) + WINDOW_FIELD(window, u16, x)) * 8), (s32) ((((qb = temp_r5_3, qb += 1, qb) + WINDOW_FIELD(window, u16, y)) << 0x13) + 0x80000) >> 0x10, (s32) (u16) (*(u16 *)0x0202166A + 2), (s32) *(u16 *)0x0202166C, 0x20, zz);
                            pA890 = (s32 *)0x0200A890;
                            pA890[1] = rv;
                            if (!(3 & *(u16 *)0x0300000E)) {
                                do {
                                    YieldTaskForUpdates(1);
                                } while (!(3 & *(u16 *)0x0300000E));
                            }
                            pA890b = (s32 *)0x0200A890;
                                DestroySprite(pA890b[1]);
                            WINDOW_FIELD(window, u16, page_progress_rows) = 0U;
                            PlaySong(0x41);
                        }
                        var_r1 = 0;
                        zr = 0;
                        if (!(zr < var_sl)) {
                        } else {
loop_61:
                            var_r7 = 1;
                            while (var_r7 < (s32) (WINDOW_FIELD(window, u16, height) - 2)) {
                                {
                                    var_r6 = 1;
                                    while (var_r6 < (s32) (WINDOW_FIELD(window, u16, width) - 1)) {
                                        {
                                            temp_r5_4 = (s8 *) window + 0x1E;
                                            if ((var_r7 == 1) && ((temp_r1_5 = ({ s32 oc; oc = (var_r6 + WINDOW_FIELD(window, u16, width)) * 2; *(u16 *)(temp_r5_4 + oc); }) & 0x3FF, temp_r0_6 = *(s32 *)0x02021664, (temp_r1_5 < temp_r0_6)) || (temp_r1_5 >= (u32) (temp_r0_6 + 0x40)))) {
                                                sp18 = (var_r7 + 1);
                                                ReleaseWindowTile((u16) (temp_r1_5 - *(s32 *)0x02021670));
                                            }
                                            temp_r0_7 = WINDOW_FIELD(window, u16, width);
                                            { s32 od; od = (var_r6 + (var_r7 * temp_r0_7)) * 2; *(u16 *)(temp_r5_4 + od) = ({ s32 os; os = (var_r6 + (temp_r0_7 * (var_r7 + 1))) * 2; *(u16 *)(temp_r5_4 + os); }); }
                                            var_r6 += 1;
                                            asm volatile(
                                                ".macro blt target\n\t"
                                                ".purgem blt\n\t"
                                                ".short 0xdbd3\n\t"
                                                ".endm");
                                        }
                                    }
                                    var_r7 += 1;
                                }
                            }
                            var_r6_2 = 1;
                            if (var_r6_2 < (s32) ((w8 = WINDOW_FIELD(window, u16, width)) - 1)) {
                                bp2 = (s8 *) window + 0x1E;
                                rhsv = (*(s32 *)0x02021664 + 1) | *(u16 *)0x02021668;
                                do {
                                    { s32 off; off = (var_r6_2 + (var_r7 * w8)) * 2; *(u16 *)(bp2 + off) = rhsv; }
                                    var_r6_2 += 1;
                                } while (var_r6_2 < (s32) ((w8 = WINDOW_FIELD(window, u16, width)) - 1));
                            }
                            WINDOW_FIELD(window, u16, text_row) = (u16) (WINDOW_FIELD(window, u16, text_row) - 1);
                            asm("" ::: "memory");
                            WINDOW_FIELD(window, s32, flags) = (s32) (WINDOW_FIELD(window, s32, flags) | WINDOW_FLAG_TILEMAP_DIRTY);
                            RequestWindowRefresh();
                            if (*(u8 *)0x03000075 == 1) {
                                YieldTaskForUpdates(2);
                            } else {
                                YieldTaskForUpdates(1);
                            }
                            var_r1 += 1;
                            if (var_r1 < var_sl) {
                                goto loop_61;
                            }
                        }
                    }
                    WINDOW_FIELD(window, s16, text_column) = 0;
                    WINDOW_FIELD(window, u16, text_row) = (u16) (WINDOW_FIELD(window, u16, text_row) + var_sl);
                    if (*text_cursor == 0xA) {
                        text_cursor += 1;
                    }
                }
            }
        }
loop_bottom:
        var_r0 = *text_cursor;
        if (var_r0 != 0) {
            goto loop_2;
        }
    }
    return text_cursor;
}
