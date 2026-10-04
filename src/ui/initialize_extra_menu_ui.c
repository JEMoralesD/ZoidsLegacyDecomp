#include "m2c_prelude.h"
#include "name_entry.h"
#include "../graphics/screen_effects.h"
void InitializeWindowGraphics(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08096FBC");
void BiosLz77ToVram(s32, s32) asm("func_80ECD34");
void LoadMenuGradientBackground(s32, s32, s32, s32, s32) asm("func_0809AB44");
void ClearSpritePools(void) asm("func_8094330");
void RunMenuScript(s32) asm("func_8098BB4");
void AppendExtraMenuRows(void) asm("func_0809D094");
u8 *GetWindow(s32) asm("func_809716C");
void StartScreenTransition(s32, s32) asm("func_08096308");

void InitializeExtraMenuUi(void) asm("func_0809D200");

void InitializeExtraMenuUi(void) {
    *(s16 *)0x0300004C = 0x1140;
    InitializeWindowGraphics(0, 1, 0, 0x3C0, 0x3C0, 0, 0xE, 0, 0x3E6, 0xF);
    BiosLz77ToVram(NAME_ENTRY_UI_GRAPHICS_ROM, 0x06015840);
    LoadMenuGradientBackground(2, 3, 0, 0, 1);
    ClearSpritePools();
    RunMenuScript(0x08000B77);
    AppendExtraMenuRows();
    GetWindow(0)[0x16] = *(u8 *)EXTRA_MENU_SELECTED_ROW_RAM;
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_REVEAL, 0);
}
