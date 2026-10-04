#include "m2c_prelude.h"
#include "window.h"
#include "scrolling_text.h"
#include "../battle/battle_display.h"
extern void PlaySong(s32) asm("func_8092E84");
extern void SetSpriteAnimation(s32 *, s32) asm("func_8094564");
extern void RefreshScrollingTextWindow(void) asm("func_080E2C24");
extern s32 *gScrollingTextArrowSprites[] asm("D_02032E68");
extern u8 gScrollingTextTopLine asm("D_02032E65");

void UpdateScrollingTextWindowInput(void) asm("func_080E2EA4");

void UpdateScrollingTextWindowInput(void) {
    u16 *pressed_keys = (u16 *)0x0300000E;
    if ((SCROLLING_TEXT_KEY_UP & *pressed_keys) && !(*gScrollingTextArrowSprites[SCROLLING_TEXT_UP_ARROW] & BATTLE_SPRITE_HIDDEN)) {
        PlaySong(SCROLLING_TEXT_INPUT_SOUND);
        gScrollingTextTopLine -= 1;
        RefreshScrollingTextWindow();
        SetSpriteAnimation(gScrollingTextArrowSprites[SCROLLING_TEXT_UP_ARROW], SCROLLING_TEXT_ARROW_FEEDBACK_ANIMATION);
        return;
    }
    if ((SCROLLING_TEXT_KEY_DOWN & *pressed_keys) && !(*gScrollingTextArrowSprites[SCROLLING_TEXT_DOWN_ARROW] & BATTLE_SPRITE_HIDDEN)) {
        PlaySong(SCROLLING_TEXT_INPUT_SOUND);
        gScrollingTextTopLine += 1;
        RefreshScrollingTextWindow();
        SetSpriteAnimation(gScrollingTextArrowSprites[SCROLLING_TEXT_DOWN_ARROW], SCROLLING_TEXT_ARROW_FEEDBACK_ANIMATION);
    }
}
