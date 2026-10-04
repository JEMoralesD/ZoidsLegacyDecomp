#include "m2c_prelude.h"
#include "battle_popup.h"
extern void *CreateSpriteFromTable(void *, int, int, int, int, int, int, int, int) asm("func_8094374");

void CreateBattlePopupTextSprites(u8 *text, void *popup, u8 first_sprite_slot) asm("func_080C8F48");

void CreateBattlePopupTextSprites(u8 *text, void *popup, u8 first_sprite_slot) {
    s32 glyph_start_update;
    u8 text_byte;
    u8 glyph_animation_id;
    void *glyph_sprite;
    register int character_index asm("r4");
    register int text_byte_or_next_index asm("r0");
    void **sprite_slots;
    s32 sprite_slot_byte_offset;

    character_index = 0;
    while ((text_byte = *text) != 0) {
        if ((u8)(text_byte - '0') <= 9) {
            glyph_animation_id = text_byte + BATTLE_POPUP_GLYPH_DIGIT_ANIMATION_BIAS;
            goto create_glyph_sprite;
        }
        glyph_animation_id = text_byte - 'A';
        if ((u8)glyph_animation_id > 0x19) {
            text_byte_or_next_index = text_byte;
            if (text_byte_or_next_index == '+') { glyph_animation_id = BATTLE_POPUP_GLYPH_PLUS; goto create_glyph_sprite; }
            if (text_byte_or_next_index == '-') { glyph_animation_id = BATTLE_POPUP_GLYPH_MINUS; goto create_glyph_sprite; }
            if (text_byte_or_next_index == '/') { glyph_animation_id = BATTLE_POPUP_GLYPH_SLASH; goto create_glyph_sprite; }
            if (text_byte_or_next_index == '<') { glyph_animation_id = BATTLE_POPUP_GLYPH_UP_ARROW; goto create_glyph_sprite; }
            if (text_byte_or_next_index == '>') { glyph_animation_id = BATTLE_POPUP_GLYPH_DOWN_ARROW; goto create_glyph_sprite; }
            if (text_byte_or_next_index == 0xA0) { glyph_animation_id = BATTLE_POPUP_GLYPH_SOLID_SQUARE; goto create_glyph_sprite; }
            if (text_byte_or_next_index == '%') { glyph_animation_id = BATTLE_POPUP_GLYPH_PERCENT; goto create_glyph_sprite; }
            if (text_byte_or_next_index == '?') { glyph_animation_id = BATTLE_POPUP_GLYPH_QUESTION_MARK; goto create_glyph_sprite; }
        } else {
        create_glyph_sprite:
            glyph_sprite = CreateSpriteFromTable((void *)BATTLE_POPUP_GLYPH_RESOURCE_TABLE_ROM, 0, glyph_animation_id,
                (s16)(BATTLE_POPUP_FIELD(popup, s32, x) + character_index * BATTLE_POPUP_GLYPH_SPACING_PIXELS),
                (s32)(s16)(BATTLE_POPUP_FIELD(popup, s32, y) - 8),
                BATTLE_POPUP_GLYPH_TILE_OFFSET, BATTLE_POPUP_GLYPH_PALETTE_BANK, BATTLE_POPUP_GLYPH_FLAGS, BATTLE_POPUP_GLYPH_CALLBACK_THUMB);
            sprite_slot_byte_offset = (first_sprite_slot + character_index) * 4;
            sprite_slots = (void **)((u8 *)popup + BATTLE_POPUP_OFFSET(sprites));
            *(void **)((u8 *)sprite_slots + sprite_slot_byte_offset) = glyph_sprite;
            glyph_start_update = (0 - character_index) * BATTLE_POPUP_GLYPH_STAGGER_UPDATES;
            BATTLE_SPRITE_FIELD(glyph_sprite, s32, user_data.popup_glyph.elapsed_updates) = glyph_start_update;
            BATTLE_SPRITE_FIELD(glyph_sprite, s32, user_data.popup_glyph.destroy_update) = glyph_start_update + BATTLE_POPUP_GLYPH_LIFETIME_UPDATES;
        }
        text += 1;
        text_byte_or_next_index = character_index + 1;
        character_index = (u8)text_byte_or_next_index;
    }
}
