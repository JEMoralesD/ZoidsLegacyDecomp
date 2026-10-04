#include "m2c_prelude.h"
#include "battle_display.h"

void *CreateSpriteFromTable(void *, s32, u8, s32, s32, s32, s32, s32, s32) asm("func_08094374");
void DestroySprite(void *) asm("func_08094554");
void FormatNumberText(s32, s32, s32, void *) asm("func_08098284");
void AppendString(void *, void *) asm("func_08099F5C");
void CopyBytes(void *, void *, s32) asm("func_080ED038");

void UpdateBattleTargetPreviewNumberSprites(void *group) asm("func_080CD410");

void UpdateBattleTargetPreviewNumberSprites(void *group)
{
    register void **glyph_slots asm("sl");
    s32 *values;
    s32 *current_value_address;
    s32 current_value;
    register s32 target_value asm("r5");
    s32 next_value;
    s32 target_value_index;
    s32 value_offset;
    u32 unsigned_remaining;
    u8 *text_cursor;
    u8 character;
    u8 glyph_index;
    register int column_index asm("r4");
    u8 row_index;
    s32 next_row_index;
    void *old_glyph;
    u8 *number_text_buffer;
    void *percent_text;
    void *glyph_table;

    number_text_buffer = (u8 *)BATTLE_PREVIEW_NUMBER_BUFFER_RAM;
    percent_text = (void *)BATTLE_PREVIEW_PERCENT_TEXT_ROM;
    glyph_table = (void *)BATTLE_PREVIEW_GLYPH_TABLE_ROM;
    row_index = 0;
    glyph_slots = (void **)((u8 *)group + BATTLE_PREVIEW_NUMBER_OFFSET(glyph_sprites));
destroy_old_glyphs:
    {
        u32 slot_offset = row_index * 4;
        void **slots_base = (void **)((u8 *)group + BATTLE_PREVIEW_NUMBER_OFFSET(glyph_sprites));
        asm volatile("" : "+r"(slots_base));
        old_glyph = *(void **)((u32)slots_base + slot_offset);
    }
    if (old_glyph != 0) {
        DestroySprite(old_glyph);
    }
    row_index = row_index + 1;
    if (row_index <= 31) {
        goto destroy_old_glyphs;
    }
    if (*(s32 *)((u8 *)group + BATTLE_PREVIEW_NUMBER_OFFSET(target_values[0])) == 0) {
        return;
    }
    row_index = 0;
    values = (s32 *)((u8 *)group + BATTLE_PREVIEW_NUMBER_OFFSET(current_values[0]));
update_number_row:
    target_value_index = row_index + 2;
    target_value = values[target_value_index];
    if (target_value != -1) {
        current_value_address = &values[row_index];
        current_value = *current_value_address;
        unsigned_remaining = target_value - current_value;
        if (unsigned_remaining > 0x7CF) {
            next_value = current_value + 1000;
            goto store_current_value;
        }
        if (unsigned_remaining > 199) {
            next_value = current_value + 100;
            goto store_current_value;
        }
        if (unsigned_remaining > 19) {
            next_value = current_value + 10;
            goto store_current_value;
        }
        if (target_value != current_value) {
            next_value = current_value + 1;
store_current_value:
            *current_value_address = next_value;
        }
        if (row_index == 0) {
            FormatNumberText(values[0], 4, 10, number_text_buffer);
        } else {
            u8 *text_buffer;
            FormatNumberText(*(s32 *)((u8 *)values + (row_index << 2)), 3, 10, (text_buffer = (u8 *)BATTLE_PREVIEW_NUMBER_BUFFER_RAM));
            AppendString(text_buffer, percent_text);
        }
    } else {
        CopyBytes((void *)BATTLE_PREVIEW_NUMBER_BUFFER_RAM, (void *)BATTLE_PREVIEW_UNKNOWN_TEXT_ROM, 5);
    }
    text_cursor = (u8 *)BATTLE_PREVIEW_NUMBER_BUFFER_RAM;
    column_index = 0;
    character = *text_cursor;
    next_row_index = row_index + 1;
    if (character != 0) {
decode_character:
        if ((u8)(character - 0x30) <= 9) {
            glyph_index = character + 0xF0;
            goto create_glyph;
        }
        glyph_index = character - 0x41;
        if ((u8)glyph_index > 0x19) {
            register s32 character_code asm("r0");
            character_code = character;
            if (character_code == 0x2B) { glyph_index = 0x1A; goto create_glyph; }
            if (character_code == 0x2D) { glyph_index = 0x1B; goto create_glyph; }
            if (character_code == 0x2F) { glyph_index = 0x1C; goto create_glyph; }
            if (character_code == 0x3C) { glyph_index = 0x1D; goto create_glyph; }
            if (character_code == 0x3E) { glyph_index = 0x1E; goto create_glyph; }
            if (character_code == 0xA0) { glyph_index = 0x1F; goto create_glyph; }
            if (character_code == 0x25) { glyph_index = 0x2A; goto create_glyph; }
            if (character_code == 0x3F) { glyph_index = 0x2B; goto create_glyph; }
        } else {
create_glyph:
            glyph_slots[row_index * 4 + column_index] = CreateSpriteFromTable(glyph_table, 0, glyph_index,
                (s16)(*(s32 *)((u8 *)group + 4) + column_index * 6),
                (s32)(s16)(*(s32 *)((u8 *)group + 8) + row_index * 8),
                BATTLE_PREVIEW_GLYPH_TILE_OFFSET, 0xB, 0x48, 0);
        }
        text_cursor += 1;
        {
            register s32 next_column_index asm("r0");
            next_column_index = column_index + 1;
            column_index = (u8)next_column_index;
        }
        character = *text_cursor;
        if (character != 0) {
            goto decode_character;
        }
    }
    row_index = (u8)next_row_index;
    if (row_index <= 1) {
        goto update_number_row;
    }
}
