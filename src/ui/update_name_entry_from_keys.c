#include "m2c_prelude.h"
#include "name_entry.h"

M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
M2C_UNK SetSpriteAnimation(s32 *, s32) asm("func_08094564");                  /* extern */
M2C_UNK RequestWindowRefresh() asm("func_080972C8");                            /* extern */
M2C_UNK ClearWindow(s32) asm("func_080986B4");                         /* extern */
s32 IsNameEntrySubtypeGridActive() asm("func_0809C434");                                /* extern */
M2C_UNK PrintNameEntryText() asm("func_0809C45C");                            /* extern */
M2C_UNK SetNameEntryCharacterCategory(u8, u8) asm("func_0809C540");                      /* extern */
u8 ModuloSigned32(s32, u8) asm("func_080ECE30");                          /* extern */

extern s32 gNameEntryCursorSprite asm("D_020216F8");
extern s32 gNameEntryUpArrowSprite asm("D_020216FC");
extern s32 gNameEntryDownArrowSprite asm("D_02021700");
extern u8 gNameEntryCharacterCategory asm("D_020216F5");
extern u8 gNameEntryGridColumn asm("D_02021704");
extern u8 gNameEntryGridRow asm("D_02021706");
extern u8 gNameEntryFirstVisibleRow asm("D_02021708");
extern u16 *gNameEntryTextBuffer asm("D_0202170C");
extern volatile u8 gNameEntryCharacterLimit asm("D_02021710");
extern s32 gNameEntryPositionSprites[] asm("D_02021714");
extern u8 gNameEntryTextPosition asm("D_0202176C");
extern u16 gNameEntryCharacterTables[][NAME_ENTRY_CATEGORY_HALFWORDS] asm("D_087A11D8");

s32 UpdateNameEntryFromKeys(void) asm("func_0809C7B0");

s32 UpdateNameEntryFromKeys(void) {
    register s32 fill_or_space_value asm("r4");
    register u16 *erase_cursor asm("r2");
    u16 *previous_erase_cursor;
    u16 *previous_write_cursor;
    u16 previous_erase_character;
    u16 previous_write_character;
    u16 repeated_keys;
    register u16 pressed_keys asm("r2");
    u16 left_key_bits;
    u16 selected_character;
    register u16 *erase_buffer_base asm("r1");
    u16 *write_buffer_base;
    u32 grid_character_index;
    u16 *write_cursor;
    u8 *position_or_subtype_address;
    u8 row_before_up;
    u8 first_visible_row_before_up;
    u8 row_before_down;
    u8 total_grid_rows;
    u8 first_visible_row_before_down;
    u8 position_before_right;
    u8 position_before_erase;
    u8 previous_category;
    u8 previous_first_visible_row;
    u8 previous_subtype;
    u8 position_before_left;
    s32 position_or_subtype_value;
    u8 fill_character_position;
    struct NameEntryCursorPositionView *cursor_position;

    previous_first_visible_row = *(u8 *)NAME_ENTRY_FIRST_VISIBLE_ROW_RAM;
    previous_category = *(u8 *)NAME_ENTRY_CATEGORY_RAM;
    previous_subtype = *(u8 *)NAME_ENTRY_SUBTYPE_RAM;
    {
        s32 *position_sprites;
        u8 *text_position_address;

        position_sprites = gNameEntryPositionSprites;
        text_position_address = &gNameEntryTextPosition;
        SetSpriteAnimation((s32 *)position_sprites[*text_position_address], 2);
        *(u32 *)position_sprites[*text_position_address] &= 0xFFDFFFFF;
    }
    if (NAME_ENTRY_KEY_UP & *(u16 *)NAME_ENTRY_REPEATED_KEYS_RAM) {
        row_before_up = *(u8 *)NAME_ENTRY_GRID_ROW_RAM;
        if (row_before_up != 0) {
            *(u8 *)NAME_ENTRY_GRID_ROW_RAM = row_before_up - 1;
            PlaySong(0x40);
        } else {
            first_visible_row_before_up = *(u8 *)NAME_ENTRY_FIRST_VISIBLE_ROW_RAM;
            if (first_visible_row_before_up != 0) {
                *(u8 *)NAME_ENTRY_FIRST_VISIBLE_ROW_RAM = first_visible_row_before_up - 1;
                SetSpriteAnimation(*(s32 **)NAME_ENTRY_UP_ARROW_RAM, 1);
                PlaySong(0x40);
            }
        }
    }
    if (NAME_ENTRY_KEY_DOWN & *(u16 *)NAME_ENTRY_REPEATED_KEYS_RAM) {
        row_before_down = *(u8 *)NAME_ENTRY_GRID_ROW_RAM;
        if ((u32) row_before_down <= 3U) {
            *(u8 *)NAME_ENTRY_GRID_ROW_RAM = (u8) (row_before_down + 1);
            PlaySong(0x40);
        } else {
            first_visible_row_before_down = *(u8 *)NAME_ENTRY_FIRST_VISIBLE_ROW_RAM;
            if ((s32) first_visible_row_before_down < (s32) (*(u8 *)NAME_ENTRY_TOTAL_ROWS_RAM - 5)) {
                *(u8 *)NAME_ENTRY_FIRST_VISIBLE_ROW_RAM = (u8) (first_visible_row_before_down + 1);
                SetSpriteAnimation(*(s32 **)NAME_ENTRY_DOWN_ARROW_RAM, 1);
                PlaySong(0x40);
            }
        }
    }
    if (NAME_ENTRY_KEY_LEFT & *(u16 *)NAME_ENTRY_REPEATED_KEYS_RAM) {
        u8 *position;
        s32 next;

        position = (u8 *)NAME_ENTRY_GRID_COLUMN_RAM;
        next = *position;
        if (next != 0) {
            next -= 1;
        } else {
            next = 0xC;
        }
        *position = next;
        PlaySong(0x40);
    }
    if (NAME_ENTRY_KEY_RIGHT & *(u16 *)NAME_ENTRY_REPEATED_KEYS_RAM) {
        u8 *position;
        s32 next;

        position = (u8 *)NAME_ENTRY_GRID_COLUMN_RAM;
        next = *position;
        if ((u32) next <= 0xBU) {
            next += 1;
        } else {
            next = 0;
        }
        *position = next;
        PlaySong(0x40);
    }
    pressed_keys = *(u16 *)NAME_ENTRY_PRESSED_KEYS_RAM;
    if (!(NAME_ENTRY_KEY_A & pressed_keys)) {
        goto no_confirm;
    }
    {
        u8 *position;

        position = &gNameEntryGridColumn;
        grid_character_index = (NAME_ENTRY_GRID_COLUMNS * (gNameEntryGridRow + gNameEntryFirstVisibleRow)) + *position;
    }
    if ((IsNameEntrySubtypeGridActive() << 0x18) == 0) {
        if (grid_character_index < (u32)gNameEntryCharacterTables[gNameEntryCharacterCategory][0]) {
            selected_character = gNameEntryCharacterTables[gNameEntryCharacterCategory][grid_character_index + 1];
        } else {
            selected_character = NAME_ENTRY_SPACE_CHARACTER;
        }
    }
    if ((*(u8 *)NAME_ENTRY_CATEGORY_RAM != NAME_ENTRY_SUBTYPE_CATEGORY) || (*(u8 *)NAME_ENTRY_SUBTYPE_RAM != NAME_ENTRY_NO_SUBTYPE)) {
        if (selected_character != NAME_ENTRY_SPACE_CHARACTER) {
            register u8 *text_position_address asm("r1");

            write_cursor = *(u16 **)NAME_ENTRY_TEXT_BUFFER_RAM;
            fill_or_space_value = 0;
            fill_character_position = 0;
            text_position_address = &gNameEntryTextPosition;
            if (fill_or_space_value < (s32)(*text_position_address + 1)) {
                do {
                    if (*(u8 *)write_cursor == 0) {
                        fill_or_space_value = 1;
                    }
                    if (fill_or_space_value != 0) {
                        *write_cursor = NAME_ENTRY_SPACE_CHARACTER;
                    }
                    write_cursor += 1;
                    fill_character_position += 1;
                } while ((s32)fill_character_position < (s32)(*text_position_address + 1));
            }
            *(write_cursor - 1) = selected_character;
            if (fill_or_space_value == 0) {
                goto after_list_update;
            }
            goto block_49;
        } else {
            write_cursor = gNameEntryTextBuffer + gNameEntryTextPosition;
            M2C_FIELD(write_cursor, u16 *, 0) = selected_character;
            if (M2C_FIELD(write_cursor, u8 *, 2) == 0) {
                write_buffer_base = gNameEntryTextBuffer;
                if (write_cursor > write_buffer_base) {
                    previous_write_cursor = write_cursor - 1;
                    previous_write_character = *previous_write_cursor;
                    if (previous_write_character == selected_character) {
                        register u32 space_character_carrier asm("r0");

                        asm volatile("" : "=r"(space_character_carrier));
                        fill_or_space_value = space_character_carrier;
loop_47:
                        write_cursor = previous_write_cursor;
                        if (write_cursor > write_buffer_base) {
                            previous_write_cursor = write_cursor - 1;
                            if (*previous_write_cursor == fill_or_space_value) {
                                goto loop_47;
                            }
                        }
                    }
                }
block_49:
                *write_cursor = 0;
            }
        }
after_list_update:
        ClearWindow(0);
        PrintNameEntryText();
        RequestWindowRefresh();
        gNameEntryTextPosition = ModuloSigned32(gNameEntryTextPosition + 1, gNameEntryCharacterLimit);
    }
    PlaySong(0x3E);
    goto block_88;
no_confirm:
    if (NAME_ENTRY_KEY_B & pressed_keys) {
        if ((IsNameEntrySubtypeGridActive() << 0x18) != 0) {
            *(u8 *)NAME_ENTRY_SUBTYPE_RAM = NAME_ENTRY_NO_SUBTYPE;
            gNameEntryGridColumn = *(u8 *)NAME_ENTRY_SAVED_COLUMN_RAM;
            gNameEntryGridRow = *(u8 *)NAME_ENTRY_SAVED_ROW_RAM;
        } else {
            register u8 *text_position_address asm("r1");

            {
                register u8 *selected_source asm("r0");

                selected_source = &gNameEntryTextPosition;
                position_before_erase = *selected_source;
                text_position_address = selected_source;
            }
            {
                s32 previous;

                if (position_before_erase != 0) {
                    previous = position_before_erase - 1;
                } else {
                    previous = gNameEntryCharacterLimit - 1;
                }
                *text_position_address = previous;
            }
            {
                u16 **text_buffer_address;
                register u32 selected_offset asm("r0");

                text_buffer_address = &gNameEntryTextBuffer;
                selected_offset = *text_position_address;
                selected_offset <<= 1;
                erase_buffer_base = *text_buffer_address;
                erase_cursor = (u16 *)((u8 *)erase_buffer_base + selected_offset);
                if (M2C_FIELD(erase_cursor, u8 *, 0) != 0) {
                register s32 space_character_source asm("r0");

                space_character_source = NAME_ENTRY_SPACE_CHARACTER;
                asm volatile("" : "+r"(space_character_source));
                fill_or_space_value = space_character_source;
                *erase_cursor = fill_or_space_value;
                if (M2C_FIELD(erase_cursor, u8 *, 2) == 0) {
                    asm volatile("" ::: "memory");
                    erase_buffer_base = *text_buffer_address;
                    if (erase_cursor > erase_buffer_base) {
                        previous_erase_cursor = erase_cursor - 1;
                        previous_erase_character = *previous_erase_cursor;
                        if (previous_erase_character == fill_or_space_value) {
                            fill_or_space_value = previous_erase_character;
loop_65:
                            erase_cursor = previous_erase_cursor;
                            if (erase_cursor > erase_buffer_base) {
                                previous_erase_cursor = erase_cursor - 1;
                                if (*previous_erase_cursor == fill_or_space_value) {
                                    goto loop_65;
                                }
                            }
                        }
                    }
                    *erase_cursor = 0;
                }
                }
            }
            ClearWindow(0);
            PrintNameEntryText();
            RequestWindowRefresh();
        }
        PlaySong(0x3F);
        goto block_88;
    }
    repeated_keys = *(u16 *)NAME_ENTRY_REPEATED_KEYS_RAM;
    left_key_bits = NAME_ENTRY_KEY_L & repeated_keys;
    if (left_key_bits != 0) {
        position_or_subtype_address = (s8 *)NAME_ENTRY_TEXT_POSITION_RAM;
        position_before_left = *(u8 *)NAME_ENTRY_TEXT_POSITION_RAM;
        if (position_before_left == 0) {
            position_before_left = *(u8 *)NAME_ENTRY_CHARACTER_LIMIT_RAM;
        }
        position_or_subtype_value = position_before_left - 1;
        goto block_83;
    }
    {
        s32 right_test;

        right_test = NAME_ENTRY_KEY_R;
        right_test &= repeated_keys;
        if (right_test != 0) {
            position_before_right = *(u8 *)NAME_ENTRY_TEXT_POSITION_RAM;
            if ((s32) position_before_right < (s32) (*(u8 *)NAME_ENTRY_CHARACTER_LIMIT_RAM - 1)) {
                *(u8 *)NAME_ENTRY_TEXT_POSITION_RAM = (u8) (position_before_right + 1);
            } else {
                *(u8 *)NAME_ENTRY_TEXT_POSITION_RAM = (u8) left_key_bits;
            }
            goto block_84;
        }
    }
    if (NAME_ENTRY_KEY_SELECT & pressed_keys) {
        *(u8 *)NAME_ENTRY_CATEGORY_RAM = ModuloSigned32(*(u8 *)NAME_ENTRY_CATEGORY_RAM + 1, 3U);
        if (previous_category == NAME_ENTRY_SUBTYPE_CATEGORY) {
            position_or_subtype_address = (u8 *)NAME_ENTRY_SUBTYPE_RAM;
            position_or_subtype_value = NAME_ENTRY_NO_SUBTYPE;
block_83:
            *position_or_subtype_address = position_or_subtype_value;
        }
block_84:
        PlaySong(0x40);
        goto block_88;
    }
    if (NAME_ENTRY_KEY_START & pressed_keys) {
        PlaySong(0x3E);
        return 1;
    }
block_88:
    {
        u8 page;
        u8 *category;
        u8 *subtype;

        page = gNameEntryFirstVisibleRow;
        category = &gNameEntryCharacterCategory;
        subtype = (u8 *)NAME_ENTRY_SUBTYPE_RAM;
        if ((page != previous_first_visible_row) || (*category != previous_category) || (*subtype != previous_subtype)) {
            SetNameEntryCharacterCategory(*category, *subtype);
            RequestWindowRefresh();
        }
    }
    cursor_position = (struct NameEntryCursorPositionView *)gNameEntryCursorSprite;
    cursor_position->x = (gNameEntryGridColumn * 0x10) + 8;
    cursor_position->y = (gNameEntryGridRow * 0x10) + 0x48;
    {
        u32 *arrow_sprite_flags;
        register u32 flags asm("r0");

        if (gNameEntryFirstVisibleRow != 0) {
            arrow_sprite_flags = (u32 *)gNameEntryUpArrowSprite;
            flags = *arrow_sprite_flags & 0xFFFDFFFF;
        } else {
            arrow_sprite_flags = (u32 *)gNameEntryUpArrowSprite;
            flags = *arrow_sprite_flags | NAME_ENTRY_SPRITE_HIDDEN;
        }
        *arrow_sprite_flags = flags;
    }
    {
        register u8 *total_rows_address asm("r2");

        total_rows_address = (u8 *)NAME_ENTRY_TOTAL_ROWS_RAM;
        total_grid_rows = *total_rows_address;
    }
    {
        u32 *arrow_sprite_flags;
        register u32 flags asm("r0");

        if (((u32) total_grid_rows > 5U) && ((s32) gNameEntryFirstVisibleRow < (s32) (total_grid_rows - 5))) {
            arrow_sprite_flags = (u32 *)gNameEntryDownArrowSprite;
            flags = *arrow_sprite_flags & 0xFFFDFFFF;
        } else {
            arrow_sprite_flags = (u32 *)gNameEntryDownArrowSprite;
            flags = *arrow_sprite_flags | NAME_ENTRY_SPRITE_HIDDEN;
        }
        *arrow_sprite_flags = flags;
    }
    {
        s32 *position_sprites;
        u8 *text_position_address;

        position_sprites = gNameEntryPositionSprites;
        text_position_address = &gNameEntryTextPosition;
        SetSpriteAnimation((s32 *)position_sprites[*text_position_address], 1);
        *(u32 *)position_sprites[*text_position_address] |= NAME_ENTRY_SPRITE_SELECTED;
    }
    return 0;
}
