#include "m2c_prelude.h"
#include "name_entry.h"

extern s32 CreateSprite(void *, void *, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
extern void SetSpriteAnimation(void *, s32) asm("func_08094564");
extern void ResetMenuKeyRepeat(void) asm("func_08096F3C");
extern void RequestWindowRefresh(void) asm("func_080972C8");
extern void PrintNameEntryText(void) asm("func_0809C45C");
extern void SetNameEntryCharacterCategory(s32, s32) asm("func_0809C540");
extern void BiosLz77ToVram(void *, void *) asm("func_080ECD34");
extern u8 ModuloUnsigned32(u8, u8) asm("func_080ECF78");

extern s32 gNameEntryCursorSprite asm("D_020216F8");
extern s32 gNameEntryUpArrowSprite asm("D_020216FC");
extern s32 gNameEntryDownArrowSprite asm("D_02021700");
extern u8 gNameEntryGridColumn asm("D_02021704");
extern u8 gNameEntryGridRow asm("D_02021706");
extern u8 gNameEntryFirstVisibleRow asm("D_02021708");
extern u16 *gNameEntryTextBuffer asm("D_0202170C");
extern volatile u8 gNameEntryCharacterLimit asm("D_02021710");
extern u8 gNameEntryTextColumn asm("D_02021711");
extern s32 gNameEntryPositionSprites[] asm("D_02021714");
extern u8 gNameEntryTextPosition asm("D_0202176C");

void InitializeNameEntryUi(u16 *text_buffer, u8 character_limit, u8 text_column, u8 first_character_tile_column) asm("func_0809C5C8");

void InitializeNameEntryUi(u16 *text_buffer, u8 character_limit, u8 text_column, u8 first_character_tile_column)
{
    u8 character_position;
    s32 sprite_callback_zero;
    s32 initial_text_position;

    BiosLz77ToVram((void *)NAME_ENTRY_UI_GRAPHICS_ROM, (void *)0x06010000);
    {
        u8 *grid_column_address;
        u8 *grid_row_address;

        grid_column_address = &gNameEntryGridColumn;
        grid_row_address = &gNameEntryGridRow;
        *grid_row_address = 0;
        *grid_column_address = 0;
    }

    {
        register void *cursor_frames asm("r0");
        register void *cursor_animations asm("r1");
        register s32 initial_text_position_seed asm("r2");

        cursor_frames = (void *)NAME_ENTRY_CURSOR_FRAMES_ROM;
        cursor_animations = (void *)NAME_ENTRY_CURSOR_ANIMATIONS_ROM;
        sprite_callback_zero = 0;
        initial_text_position_seed = 0;
        asm volatile("" : "+r"(initial_text_position_seed));
        initial_text_position = initial_text_position_seed;
        gNameEntryCursorSprite = CreateSprite(cursor_frames, cursor_animations,
            0, 8, 0x48, 0x3F4, 0xF, 0x20, sprite_callback_zero);
    }
    gNameEntryUpArrowSprite = CreateSprite((void *)NAME_ENTRY_UP_ARROW_FRAMES_ROM,
        (void *)NAME_ENTRY_UP_ARROW_ANIMATIONS_ROM, 0, 0xE8, 0x48, 0x3E6, 0xF, 0x20030, sprite_callback_zero);
    gNameEntryDownArrowSprite = CreateSprite((void *)NAME_ENTRY_DOWN_ARROW_FRAMES_ROM,
        (void *)NAME_ENTRY_DOWN_ARROW_ANIMATIONS_ROM, 0, 0xE8, 0x90, 0x3E8, 0xF, 0x20030, sprite_callback_zero);

    gNameEntryTextBuffer = text_buffer;
    gNameEntryCharacterLimit = character_limit;
    {
        register volatile u8 *text_column_address asm("r0");
        register u32 text_column_value asm("r2");

        text_column_address = &gNameEntryTextColumn;
        text_column_value = text_column;
        *text_column_address = text_column_value;
    }
    {
        register u8 *text_position_address asm("r1");

        text_position_address = &gNameEntryTextPosition;
        *text_position_address = initial_text_position;
        if (*text_buffer != 0) {
            register u8 *text_position_store asm("r3");
            register s32 text_length asm("r2");
            s32 next_text_length;

            text_position_store = text_position_address;
            text_length = initial_text_position;
            do {
                next_text_length = text_length + 1;
                text_length = next_text_length;
                text_buffer++;
            } while (*text_buffer != 0);
            *text_position_store = next_text_length;
        }
    }

    {
        u8 *text_position_address;
        register u32 character_limit_carrier asm("r5");

        text_position_address = &gNameEntryTextPosition;
        character_limit_carrier = (u32)&gNameEntryCharacterLimit;
        *text_position_address = ModuloUnsigned32(*text_position_address, *(volatile u8 *)character_limit_carrier);
        character_position = 0;
        character_limit_carrier = *(volatile u8 *)character_limit_carrier;
        asm volatile("" : "+r"(character_limit_carrier));
        if (character_position < character_limit_carrier) {
            do {
                gNameEntryPositionSprites[character_position] = CreateSprite((void *)NAME_ENTRY_POSITION_FRAMES_ROM,
                    (void *)NAME_ENTRY_POSITION_ANIMATIONS_ROM, 2, (first_character_tile_column + character_position) * 8, 0x38,
                    0x36, 0xF, 8, 0);
                character_position = (u8)(character_position + 1);
            } while (character_position < gNameEntryCharacterLimit);
        }
    }

    {
        s32 *position_sprites;
        u8 *text_position_address;

        position_sprites = gNameEntryPositionSprites;
        text_position_address = &gNameEntryTextPosition;
        SetSpriteAnimation((void *)position_sprites[*text_position_address], 1);
        *(u32 *)position_sprites[*text_position_address] |= NAME_ENTRY_SPRITE_SELECTED;
    }

    CreateSprite((void *)NAME_ENTRY_POSITION_FRAMES_ROM, (void *)NAME_ENTRY_POSITION_ANIMATIONS_ROM,
        3, 0xF0, 0, 0x36, 0xF, 8, 0);
    gNameEntryFirstVisibleRow = 0;
    SetNameEntryCharacterCategory(NAME_ENTRY_LATIN_CHARACTERS, NAME_ENTRY_NO_SUBTYPE);
    PrintNameEntryText();
    RequestWindowRefresh();
    ResetMenuKeyRepeat();
}
