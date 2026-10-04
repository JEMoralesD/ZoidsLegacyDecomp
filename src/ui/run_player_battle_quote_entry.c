#include "m2c_prelude.h"
#include "name_entry.h"
#include "../graphics/screen_effects.h"

extern void PlaySong(s32) asm("func_08092E84");
extern s32 WriteSaveBlock1(void) asm("func_080940AC");
extern void ClearSpritePools(void) asm("func_08094330");
extern void CreateSprite(s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
extern void StartScreenTransition(s32, s32) asm("func_08096308");
extern s32 IsScreenTransitionComplete(void) asm("func_0809669C");
extern void InitializeWindowGraphics(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08096FBC");
extern void RunMenuScript(s32) asm("func_08098BB4");
extern void LoadMenuGradientBackground(s32, s32, s32, s32, s32) asm("func_0809AB44");
extern void PrintNameEntryText(void) asm("func_0809C45C");
extern void InitializeNameEntryUi(s32, s32, s32, s32) asm("func_0809C5C8");
extern s32 UpdateNameEntryFromKeys(void) asm("func_0809C7B0");
extern void CopyString(void *, s32) asm("func_080ED128");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");
extern u16 gPlayerNameBuffer[] asm("D_02021774");

void RunPlayerBattleQuoteEntry(void) asm("func_0809CED8");

void RunPlayerBattleQuoteEntry(void) {
    s32 *cursor_flags_on_retry;
    s32 *up_arrow_flags;
    s32 *down_arrow_flags;
    s32 save_result;
    s32 hidden_sprite_flag;

    *(s16 *)0x0300004C = 0x1140;
    ClearSpritePools();
    InitializeWindowGraphics(0, 1, 0, 0x3C0, 0x3C0, 0, 0xE, 0, 0x3E6, 0xF);
    LoadMenuGradientBackground(2, 3, 0, 0, 1);
    RunMenuScript(0x08000D40);
    InitializeNameEntryUi(NAME_ENTRY_BATTLE_QUOTE_RAM, NAME_ENTRY_QUOTE_CHARACTER_LIMIT, 0, 1);
    CreateSprite(0x08106180, 0x08106198, 0, 8, 0x38, 0x122, 0xF, 8, 0);
    CreateSprite(0x08106180, 0x08106198, 1, 0xB8, 0x38, 0x122, 0xF, 8, 0);
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_REVEAL, 0x10);
    while ((IsScreenTransitionComplete() << 24) == 0) {
        YieldTaskForUpdates(1);
    }
    hidden_sprite_flag = NAME_ENTRY_SPRITE_HIDDEN;
retry:
    if ((UpdateNameEntryFromKeys() << 24) == 0) {
        goto retry_no_clear;
    }
    {
        register s32 *cursor_flags asm("r1");
        cursor_flags = *(s32 **)NAME_ENTRY_CURSOR_RAM;
        *cursor_flags |= hidden_sprite_flag;
    }
    up_arrow_flags = *(s32 **)NAME_ENTRY_UP_ARROW_RAM;
    *up_arrow_flags |= hidden_sprite_flag;
    down_arrow_flags = *(s32 **)NAME_ENTRY_DOWN_ARROW_RAM;
    *down_arrow_flags |= hidden_sprite_flag;
    if (gPlayerNameBuffer[NAME_ENTRY_PLAYER_BUFFER_HALFWORDS] == 0) {
        CopyString(&gPlayerNameBuffer[NAME_ENTRY_PLAYER_BUFFER_HALFWORDS], NAME_ENTRY_DEFAULT_BATTLE_QUOTE_ROM);
        PrintNameEntryText();
    }
    RunMenuScript(0x08000D4F);
    if (*(u8 *)0x0200A882 != 1) {
        goto retry_clear;
    }
    if (*(u8 *)0x0200A880 != 0) {
        goto retry_clear;
    }
    save_result = WriteSaveBlock1();
    *(u8 *)0x0202169A = save_result;
    if ((save_result << 24) == 0) {
        PlaySong(0x58);
        RunMenuScript(0x08000E00);
    }
    goto final_wait;

retry_clear:
    cursor_flags_on_retry = *(s32 **)NAME_ENTRY_CURSOR_RAM;
    *cursor_flags_on_retry &= 0xFFFDFFFF;
retry_no_clear:
    YieldTaskForUpdates(1);
    goto retry;

final_wait:
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0x10);
    while ((IsScreenTransitionComplete() << 24) == 0) {
        YieldTaskForUpdates(1);
    }
}
