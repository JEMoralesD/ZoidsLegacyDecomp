#include "player_selection.h"

extern void SetWindowTextPosition(s32, s32, s32) asm("func_080981D0");
extern void PrintWindowTextAt(const void *, s32, s32, s32, s32) asm("func_080981F0");
extern void PrintWindowText(const void *, s32, s32) asm("func_08098248");
extern void PrintWindowNumberAt(s32, s32, s32, s32, s32, s32, s32) asm("func_0809844C");
extern u8 CountEncodedTextGlyphs(const void *) asm("func_08098B58");
extern void CopyBytes(void *, const void *, s32) asm("func_080ED038");
extern void CopyString(void *, const void *) asm("func_080ED128");

extern u8 gPlayerSelectionTextBuffer[] asm("D_02030564");
extern u8 gPlayerSelectionSpaceText[] asm("D_081061C4");
extern const void *gZoidNameTable[];
extern const void *gZoidCoreNameTable[] asm("D_087EEE60");

void DrawZoidDevelopmentRequirements(struct ZoidDevelopmentRequirementView *requirements, s32 window_id) asm("func_080AC87C");

void DrawZoidDevelopmentRequirements(struct ZoidDevelopmentRequirementView *requirements, s32 window_id)
{
    struct ZoidDevelopmentRequirementView *recipe = requirements;
    register u32 saved_window_id asm("r5");
    register u32 padding_column asm("r4");
    const void *core_or_blank_text;

    window_id <<= 24;
    saved_window_id = (u32)window_id >> 24;
    SetWindowTextPosition(saved_window_id, 0, 2);

    {
        register u32 base_model_or_series_id asm("r0");

        asm volatile("ldrb %0, [%1]"
                     : "=r"(base_model_or_series_id)
                     : "r"(recipe));
        if (base_model_or_series_id <= ZOID_DEVELOPMENT_LAST_BASE_MODEL) {
            register void *destination asm("r0") = gPlayerSelectionTextBuffer;
            register const void **name_table asm("r2") = gZoidNameTable;
            register u32 name_index asm("r1");

            asm volatile("ldrb %0, [%1]"
                         : "=r"(name_index)
                         : "r"(recipe), "r"(destination), "r"(name_table));
            CopyString(destination, name_table[name_index]);
        } else {
            register u32 series_id asm("r0");

            asm volatile("ldrb %0, [%1]"
                         : "=r"(series_id)
                         : "r"(recipe));

            switch (series_id) {
            case ZOID_DEVELOPMENT_LIGER_ZERO_SERIES:
                CopyBytes(gPlayerSelectionTextBuffer, (const void *)ZOID_DEVELOPMENT_LIGER_ZERO_SERIES_TEXT_ROM, 0x1B);
                break;
            case ZOID_DEVELOPMENT_FURY_SERIES:
                CopyBytes(gPlayerSelectionTextBuffer, (const void *)ZOID_DEVELOPMENT_FURY_SERIES_TEXT_ROM, 0x17);
                break;
            }
        }
    }

    {
        register u32 glyph_count asm("r0") = CountEncodedTextGlyphs(gPlayerSelectionTextBuffer);
        register u32 name_columns asm("r1");
        register u32 padding_columns asm("r0");

        glyph_count <<= 24;
        name_columns = glyph_count >> 24;
        padding_column = 0;
        padding_columns = ZOID_DEVELOPMENT_NAME_COLUMNS - name_columns;
        if (padding_column < padding_columns) {
            register u32 padding_column_count asm("r6") = padding_columns;

            do {
                PrintWindowText(gPlayerSelectionSpaceText, 0, saved_window_id);
                padding_column++;
            } while (padding_column < padding_column_count);
        }
    }
    PrintWindowText(gPlayerSelectionTextBuffer, 0, saved_window_id);
    SetWindowTextPosition(saved_window_id, 0, 6);

    if ((*(u32 *)recipe & 0xFFFF00) == 0) {
        core_or_blank_text = (const void *)ZOID_DEVELOPMENT_NO_CORE_TEXT_ROM;
        goto draw_text;
    }

    {
        register u32 first_core_or_width asm("r0");

        asm volatile("ldrb %0, [%1, #1]"
                     : "=r"(first_core_or_width)
                     : "r"(recipe));
        if (first_core_or_width != 0) {
        {
            register const void **name_table asm("r1") = gZoidCoreNameTable;
            register u32 name_columns asm("r1");
            register u32 padding_columns asm("r0");

            asm volatile("" : "+r"(first_core_or_width) : "r"(name_table));
            first_core_or_width = CountEncodedTextGlyphs(name_table[first_core_or_width]);
            first_core_or_width <<= 24;
            name_columns = first_core_or_width >> 24;
            padding_column = 0;
            padding_columns = ZOID_DEVELOPMENT_NAME_COLUMNS - name_columns;
            if (padding_column < padding_columns) {
                register u32 padding_column_count asm("r6") = padding_columns;

                do {
                    PrintWindowText(gPlayerSelectionSpaceText, 0, saved_window_id);
                    padding_column++;
                } while (padding_column < padding_column_count);
            }
        }
        {
            register const void **name_table asm("r4") = gZoidCoreNameTable;
            register u32 name_index asm("r0");

            asm volatile("ldrb %0, [%1, #1]"
                         : "=r"(name_index)
                         : "r"(recipe), "r"(name_table));
            PrintWindowText(name_table[name_index], 0, saved_window_id);
            SetWindowTextPosition(saved_window_id, 0, 8);

        {
            register u32 second_core_or_width asm("r0");

            asm volatile("ldrb %0, [%1, #2]"
                         : "=r"(second_core_or_width)
                         : "r"(recipe));
            if (second_core_or_width != 0) {
            {
                register u32 name_columns asm("r1");
                register u32 padding_columns asm("r0");

                second_core_or_width = CountEncodedTextGlyphs(name_table[second_core_or_width]);
                second_core_or_width <<= 24;
                name_columns = second_core_or_width >> 24;
                padding_column = 0;
                padding_columns = ZOID_DEVELOPMENT_NAME_COLUMNS - name_columns;
                if (padding_column < padding_columns) {
                    register u32 padding_column_count asm("r6") = padding_columns;

                    do {
                        PrintWindowText(gPlayerSelectionSpaceText, 0, saved_window_id);
                        padding_column++;
                    } while (padding_column < padding_column_count);
                }
            }
                {
                    register const void **name_table asm("r0") = gZoidCoreNameTable;
                    register u32 name_index asm("r1");

                    asm volatile("ldrb %0, [%1, #2]"
                                 : "=r"(name_index)
                                 : "r"(recipe), "r"(name_table));
                    core_or_blank_text = name_table[name_index];
                }
            } else {
                core_or_blank_text = (const void *)ZOID_DEVELOPMENT_BLANK_LINE_TEXT_ROM;
            }
        }
        }

draw_text:
        PrintWindowText(core_or_blank_text, 0, saved_window_id);
    } else {
        {
            register const void **name_table asm("r1") = gZoidCoreNameTable;
            register u32 glyph_count asm("r0");
            register u32 name_columns asm("r1");
            register u32 padding_columns asm("r0");

            asm volatile("ldrb %0, [%1, #2]"
                         : "=r"(glyph_count)
                         : "r"(recipe), "r"(name_table));
            glyph_count = CountEncodedTextGlyphs(name_table[glyph_count]);
            glyph_count <<= 24;
            name_columns = glyph_count >> 24;
            padding_column = 0;
            padding_columns = ZOID_DEVELOPMENT_NAME_COLUMNS - name_columns;
            if (padding_column < padding_columns) {
                register u32 padding_column_count asm("r6") = padding_columns;

                do {
                    PrintWindowText(gPlayerSelectionSpaceText, 0, saved_window_id);
                    padding_column++;
                } while (padding_column < padding_column_count);
            }
        }
        {
            register const void **name_table asm("r1") = gZoidCoreNameTable;
            register u32 name_index asm("r0");

            asm volatile("ldrb %0, [%1, #2]"
                         : "=r"(name_index)
                         : "r"(recipe), "r"(name_table));
            PrintWindowText(name_table[name_index], 0, saved_window_id);
        }
        PrintWindowTextAt((const void *)ZOID_DEVELOPMENT_BLANK_LINE_TEXT_ROM, 0, saved_window_id, 0, 8);
    }
    }

    PrintWindowNumberAt(recipe->development_price, 7, 0, 2, saved_window_id, 5, 0xA);
    {
        register u32 base_model_or_series_id asm("r0");

        asm volatile("ldrb %0, [%1]"
                     : "=r"(base_model_or_series_id)
                     : "r"(recipe));
        if ((u8)(base_model_or_series_id + 0x38) <= 1) {
        PrintWindowTextAt((const void *)ZOID_DEVELOPMENT_CAU_ONLY_TEXT_ROM, 1, saved_window_id, 0, 0xC);
        } else {
        PrintWindowTextAt((const void *)ZOID_DEVELOPMENT_BLANK_LINE_TEXT_ROM, 1, saved_window_id, 0, 0xC);
        }
    }
}
