#include "m2c_prelude.h"
#include "title_menu.h"
#include "../game/game_state.h"
extern u16 gFirstTitleSecretCode[] asm("D_087A1194");
extern u16 gSecondTitleSecretCode[] asm("D_087A11B6");

extern void PlaySong(s32) asm("func_08092E84");
extern void WriteSaveBlock2(void) asm("func_080940C0");
extern void WriteSaveBlock3(void) asm("func_080940D4");
extern void UnlockZoidData(s32) asm("func_080E5DC4");
extern void AddZoidCoresToInventory(s32, s32) asm("func_080E5E0C");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");

void RunTitleSecretCodeMonitor(void) asm("func_0809BAB0");

void RunTitleSecretCodeMonitor(void) {
    u8 first_code_position;
    u8 second_code_position;
    u16 pressed_keys;

    if (*(u8 *)GAME_COMPLETION_FLAG_RAM != 0) {
        second_code_position = 0;
        first_code_position = 0;
        do {
            pressed_keys = *(u16 *)0x0300000E;
            if (pressed_keys != 0) {
                if (pressed_keys == gFirstTitleSecretCode[first_code_position]) {
                    first_code_position += 1;
                    if (first_code_position == 0x11) {
                        UnlockZoidData(6);
                        UnlockZoidData(0x85);
                        UnlockZoidData(0x86);
                        AddZoidCoresToInventory(3, 1);
                        AddZoidCoresToInventory(5, 1);
                        WriteSaveBlock3();
                        WriteSaveBlock2();
                        PlaySong(0x49);
                        first_code_position = 0;
                    }
                } else {
                    first_code_position = 0;
                }
                if (*(u16 *)0x0300000E == gSecondTitleSecretCode[second_code_position]) {
                    second_code_position += 1;
                    if (second_code_position == 0x11) {
                        UnlockZoidData(0x75);
                        UnlockZoidData(0x92);
                        AddZoidCoresToInventory(0xD, 1);
                        AddZoidCoresToInventory(0x11, 1);
                        WriteSaveBlock3();
                        WriteSaveBlock2();
                        PlaySong(0x4E);
                        second_code_position = 0;
                    }
                } else {
                    second_code_position = 0;
                }
            }
            YieldTaskForUpdates(1);
        } while (1);
    }
}
