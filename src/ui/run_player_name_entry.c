#include "m2c_prelude.h"
#include "name_entry.h"
#include "../graphics/screen_effects.h"

struct PilotDisplayNameRecord {
    u32 reserved00;
    void *name;
};

extern void PlaySong(s32) asm("func_08092E84");
extern s32 WriteSaveBlock1(void) asm("func_080940AC");
extern void ClearSpritePools(void) asm("func_08094330");
extern void CreateSprite() asm("func_08094484");
extern void DisableDisplayWindows(void) asm("func_0809534C");
extern void ConfigureDisplayWindows(s32, s32, s32, s32, s32, s32, s32, s32) asm("func_0809538C");
extern void StartScreenTransition(s32, s32) asm("func_08096308");
extern s32 IsScreenTransitionComplete(void) asm("func_0809669C");
extern void InitializeWindowGraphics() asm("func_08096FBC");
extern void RequestWindowRefresh(void) asm("func_080972C8");
extern void PrintWindowTextAt() asm("func_080981F0");
extern void RunMenuScript(s32) asm("func_08098BB4");
extern void LoadPilotPortraitGraphics(s32, s32, s32, s32, s32) asm("func_0809A94C");
extern void LoadMenuGradientBackground(s32, s32, s32, s32, s32) asm("func_0809AB44");
extern void PrintNameEntryText(void) asm("func_0809C45C");
extern void InitializeNameEntryUi(s32, s32, s32, s32) asm("func_0809C5C8");
extern s32 UpdateNameEntryFromKeys(void) asm("func_0809C7B0");
extern void CopyString(void *, void *) asm("func_080ED128");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");
extern u16 gPlayerNameBuffer[] asm("D_02021774");

void RunPlayerNameEntry(u8 save_after_confirmation) asm("func_0809CC94");

void RunPlayerNameEntry(u8 save_after_confirmation) {
    s32 *cursor_flags_on_retry;
    s32 *cursor_flags;
    s32 *up_arrow_flags;
    s32 *down_arrow_flags;
    s32 save_result;
    s32 hidden_sprite_flag;
    s32 text_row_zero;
    s32 confirmation_window_x_bounds;
    s32 confirmation_window_y_bounds;

    *(s16 *)0x0300004C = 0x1140;
    ClearSpritePools();
    InitializeWindowGraphics(0, 1, 0, 0x3C0, 0x3C0, 0, 0xE, 0, 0x3E6, 0xF);
    LoadMenuGradientBackground(2, 3, 0, 0, 1);
    RunMenuScript(0x08000CF5);
    InitializeNameEntryUi(NAME_ENTRY_PLAYER_NAME_RAM, NAME_ENTRY_PLAYER_CHARACTER_LIMIT, 3, 0xC);
    CreateSprite(NAME_ENTRY_POSITION_FRAMES_ROM, NAME_ENTRY_POSITION_ANIMATIONS_ROM,
        0, 0x60, 0x38, 0x36, 0xF, 8, 0);
    CreateSprite(NAME_ENTRY_POSITION_FRAMES_ROM, NAME_ENTRY_POSITION_ANIMATIONS_ROM,
        4, 0xA0, 0x38, 0x36, 0xF, 8, 0);
    LoadPilotPortraitGraphics(1, 0, 0, 0x3C2, 0xE);
    CreateSprite(0x08359850, 0x0835985C,
        0, 8, 8, 0x3C2, 0xE, 8, 0);
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_REVEAL, 0x10);
    while ((IsScreenTransitionComplete() << 24) == 0) {
        YieldTaskForUpdates(1);
    }

    hidden_sprite_flag = NAME_ENTRY_SPRITE_HIDDEN;
retry:
    confirmation_window_x_bounds = 0x30C0;
    confirmation_window_y_bounds = 0x2850;
    if ((UpdateNameEntryFromKeys() << 24) == 0) {
        goto retry_wait;
    }
    cursor_flags = *(s32 **)NAME_ENTRY_CURSOR_RAM;
    *cursor_flags |= hidden_sprite_flag;
    up_arrow_flags = *(s32 **)NAME_ENTRY_UP_ARROW_RAM;
    *up_arrow_flags |= hidden_sprite_flag;
    down_arrow_flags = *(s32 **)NAME_ENTRY_DOWN_ARROW_RAM;
    *down_arrow_flags |= hidden_sprite_flag;

    text_row_zero = 0;
    ConfigureDisplayWindows(1, confirmation_window_x_bounds, confirmation_window_y_bounds, 0, 0, 0, 0x2F, 0x3F);
    RunMenuScript(0x08000D0F);
    if (gPlayerNameBuffer[0] == 0) {
        CopyString(gPlayerNameBuffer,
            ((struct PilotDisplayNameRecord *)0x087EDFB4)->name);
        PrintNameEntryText();
    }
    PrintWindowTextAt(gPlayerNameBuffer, 2, 4, 0, text_row_zero);
    RunMenuScript(0x08000D3B);
    RequestWindowRefresh();
    DisableDisplayWindows();
    if (*(u8 *)0x0200A882 != 1) {
        goto retry_clear;
    }
    if (*(u8 *)0x0200A880 != 0) {
        goto retry_clear;
    }
    if (save_after_confirmation != 0) {
        save_result = WriteSaveBlock1();
        *(u8 *)0x0202169A = save_result;
        if ((save_result << 24) == 0) {
            YieldTaskForUpdates(1);
            PlaySong(0x58);
            RunMenuScript(0x08000D92);
        }
    }
    goto final_wait;

retry_clear:
    cursor_flags_on_retry = *(s32 **)NAME_ENTRY_CURSOR_RAM;
    *cursor_flags_on_retry &= 0xFFFDFFFF;
retry_wait:
    YieldTaskForUpdates(1);
    goto retry;

final_wait:
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0x10);
    while ((IsScreenTransitionComplete() << 24) == 0) {
        YieldTaskForUpdates(1);
    }
}
