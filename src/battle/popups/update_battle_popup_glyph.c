#include "m2c_prelude.h"
#include "battle_popup.h"
extern int Sin256(s32) asm("func_8092A90");
extern void DestroySprite(void *) asm("func_8094554");

void UpdateBattlePopupGlyph(struct BattleDisplaySprite *glyph_sprite) asm("func_080C8EF4");

void UpdateBattlePopupGlyph(struct BattleDisplaySprite *glyph_sprite) {
    s32 elapsed_updates = BATTLE_SPRITE_FIELD(glyph_sprite, s32, user_data.popup_glyph.elapsed_updates);
    if (elapsed_updates <= BATTLE_POPUP_GLYPH_RISE_UPDATES) {
        if (elapsed_updates >= 0) {
            if (elapsed_updates == 0) {
                BATTLE_SPRITE_FIELD(glyph_sprite, u32, flags) &= BATTLE_POPUP_GLYPH_VISIBLE_MASK;
            }
            BATTLE_SPRITE_FIELD(glyph_sprite, s16, offset_y) = -(s16)Sin256((BATTLE_SPRITE_FIELD(glyph_sprite, s32, user_data.popup_glyph.elapsed_updates) << 20) >> 16) / 32;
        }
    } else {
        if (elapsed_updates == BATTLE_SPRITE_FIELD(glyph_sprite, s32, user_data.popup_glyph.destroy_update)) {
            DestroySprite(glyph_sprite);
        }
    }
    BATTLE_SPRITE_FIELD(glyph_sprite, s32, user_data.popup_glyph.elapsed_updates) += 1;
}
