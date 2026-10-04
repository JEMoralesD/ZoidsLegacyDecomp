#include "m2c_prelude.h"
#include "field_display.h"

void RequestWindowRefresh(void) asm("func_080972C8");
void PrintWindowTextAt(void *, s32, s32, s32, s32) asm("func_080981F0");
void FormatNumberText(s32, s32, s32, void *) asm("func_08098284");
u8 CountEncodedTextGlyphs(void *) asm("func_08098B58");
void RunMenuScript(const void *) asm("func_08098BB4");
void AppendString(void *, const void *) asm("func_08099F5C");
void CopyString(void *, const void *) asm("func_080ED128");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

extern u8 gFieldMapNameTextBuffer[] asm("D_02030564");
extern u8 gFieldMapNameNumberTextBuffer[] asm("D_020305E4");
extern struct FieldMapNameGroupView gFieldMapDefinitions[] asm("D_087C4434");
extern const void *gFieldMapGroupNames[] asm("D_087EF4E0");

void ShowFieldMapName(s32 map_id) asm("func_0809E8CC");

void ShowFieldMapName(s32 map_id)
{
    register s32 selected_map_id asm("r4") = map_id;
    u8 *map_name_text;
    u8 map_group_id;
    s32 floor_number;
    u32 elapsed_updates;

    map_group_id = gFieldMapDefinitions[selected_map_id].map_group_id;
    map_name_text = gFieldMapNameTextBuffer;
    CopyString(map_name_text, gFieldMapGroupNames[map_group_id]);
    if ((u32)(selected_map_id - 0x49) <= 3) {
        AppendString(map_name_text, (void *)0x08103AC8);
        floor_number = selected_map_id - 0x48;
        goto format_value;
    }
    if ((u32)(selected_map_id - 0x80) <= 4) {
        AppendString(map_name_text, (void *)0x08103AC8);
        floor_number = selected_map_id - 0x7F;
        goto format_value;
    }
    if ((u32)(selected_map_id - 0x8C) <= 4) {
        AppendString(map_name_text, (void *)0x08103AF0);
        floor_number = selected_map_id - 0x8B;
        goto format_value;
    }
    if ((u32)(selected_map_id - 0x96) <= 3) {
        AppendString(map_name_text, (void *)0x08103AC8);
        floor_number = selected_map_id - 0x95;
        goto format_value;
    }
    if ((u32)(selected_map_id - 0x9A) <= 4) {
        AppendString(map_name_text, (void *)0x08103AC8);
        floor_number = selected_map_id - 0x99;
        goto format_value;
    }
    if ((u32)(selected_map_id - 0xA0) <= 6) {
        AppendString(map_name_text, (void *)0x08103AC8);
        floor_number = selected_map_id - 0x9F;
        goto format_value;
    }
    if ((u32)(selected_map_id - 0xA9) <= 3) {
        AppendString(map_name_text, (void *)0x08103AC8);
        floor_number = selected_map_id - 0xA8;
        goto format_value;
    }
    if ((u32)(selected_map_id - 0xAE) <= 1) {
        AppendString(map_name_text, (void *)0x08103AC8);
        floor_number = selected_map_id - 0xAD;
format_value:
        {
            u8 *floor_number_text = gFieldMapNameNumberTextBuffer;

            FormatNumberText(floor_number, 1, 0, floor_number_text);
            AppendString(map_name_text, floor_number_text);
            AppendString(map_name_text, (void *)0x08103AE0);
        }
    } else {
        u32 final_test = selected_map_id;

        final_test -= 0xB0;
        if (final_test <= 2) {
            AppendString(map_name_text, (void *)0x08103AC8);
            floor_number = selected_map_id - 0xAF;
            {
                u8 *floor_number_text = gFieldMapNameNumberTextBuffer;

                FormatNumberText(floor_number, 1, 0, floor_number_text);
                AppendString(map_name_text, floor_number_text);
                AppendString(map_name_text, (void *)0x08103AE0);
            }
        }
    }

    RunMenuScript((void *)0x08017ACA);
    {
        u8 *map_name_to_center = gFieldMapNameTextBuffer;
        s32 text_column;

        text_column = 0x1C - CountEncodedTextGlyphs(map_name_to_center);
        text_column += (u32)text_column >> 31;
        text_column >>= 1;
        PrintWindowTextAt(map_name_to_center, 0, 9, text_column, 0);
    }
    RequestWindowRefresh();
    elapsed_updates = 0;
    do {
        YieldTaskForUpdates(1);
        elapsed_updates++;
    } while (elapsed_updates < FIELD_MAP_NAME_VISIBLE_UPDATES);
    RunMenuScript((void *)0x08017AD2);
}
