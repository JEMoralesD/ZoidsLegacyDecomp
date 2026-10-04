#include "m2c_prelude.h"
#include "deck_commands.h"
extern void AppendWindowTextItem(s32, s32) asm("func_080988C8");
extern s32 gDeckCommandNameTable[] asm("D_087EF130");
extern u8 gPlayerSelectedDeckCommands[] asm("D_020281F6");
extern u8 gBattleSelectedDeckCommands[] asm("D_02037300");

void AppendSelectedDeckCommandRows(u8 battle_deck) asm("func_080C0930");

void AppendSelectedDeckCommandRows(u8 battle_deck) {
    s32 command_slot = 0;
    s32 *command_names = gDeckCommandNameTable;
    u8 *player_deck = gPlayerSelectedDeckCommands;
    u8 *battle_deck_commands = gBattleSelectedDeckCommands;
    do {
        if (battle_deck == 0) {
            AppendWindowTextItem(DECK_COMMAND_SELECTED_WINDOW, command_names[*(u8 *)(command_slot + (s32)player_deck)]);
        } else {
            AppendWindowTextItem(DECK_COMMAND_SELECTED_WINDOW, command_names[*(u8 *)(command_slot + (s32)battle_deck_commands)]);
        }
        command_slot = (u8)(command_slot + 1);
    } while ((u32)command_slot <= PLAYER_SELECTED_DECK_COMMAND_COUNT - 1);
}
