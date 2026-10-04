#include "m2c_prelude.h"
#include "deck_commands.h"
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");
M2C_UNK InitializeScrollingTextWindow(s32, s32) asm("func_080E2DCC");
M2C_UNK UpdateScrollingTextWindowInput() asm("func_080E2EA4");
M2C_UNK DestroyScrollingTextWindowArrows() asm("func_080E2F30");
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");
extern s32 gDeckCommandDescriptionTable[] asm("D_087EF200");

void ShowDeckCommandDescription(u8 command_id) asm("func_080C0A9C");

void ShowDeckCommandDescription(u8 command_id) {
    RunMenuScript(DECK_COMMAND_DESCRIPTION_OPEN_SCRIPT);
    InitializeScrollingTextWindow(DECK_COMMAND_DESCRIPTION_WINDOW, gDeckCommandDescriptionTable[command_id]);
    if (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E)) {
        do {
            UpdateScrollingTextWindowInput();
            YieldTaskForUpdates(1);
        } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
    }
    RunMenuScript(DECK_COMMAND_DESCRIPTION_CLOSE_SCRIPT);
    DestroyScrollingTextWindowArrows();
}
