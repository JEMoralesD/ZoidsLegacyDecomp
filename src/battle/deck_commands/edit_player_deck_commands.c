#include "m2c_prelude.h"
#include "deck_commands.h"

extern void PlaySong(s32) asm("func_08092E84");
extern void RequestWindowRefresh(void) asm("func_080972C8");
extern void ClearWindowTextList(s32) asm("func_08098834");
extern void RunMenuScript(s32) asm("func_08098BB4");
extern void AppendSelectedDeckCommandRows(s32) asm("func_080C0930");
extern void AppendDeckCommandCatalogRows(s32) asm("func_080C098C");
extern void ShowDeckCommandDescription(u8) asm("func_080C0A9C");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");

extern volatile u8 gMenuSelectionIndex asm("D_0200A880");
extern volatile u8 gMenuSelectionResult asm("D_0200A882");
extern volatile u16 gMenuSelectionKeys asm("D_0200A884");
extern u8 gPlayerSelectedDeckCommands[] asm("D_020281F6");

void EditPlayerDeckCommands(void) asm("func_080C0AFC");

void EditPlayerDeckCommands(void) {
    register u8 *player_deck asm("r9");

    RunMenuScript(DECK_COMMAND_EDITOR_OPEN_SCRIPT);
    AppendSelectedDeckCommandRows(DECK_COMMAND_PLAYER_LIST);
    AppendDeckCommandCatalogRows(DECK_COMMAND_PLAYER_LIST);
    player_deck = gPlayerSelectedDeckCommands;

select_deck_slot:
    RunMenuScript(DECK_COMMAND_EDITOR_SELECT_SLOT_SCRIPT);
    if (gMenuSelectionResult == DECK_COMMAND_MENU_CONFIRMED) {
        u8 *menu_selection_index;
        register u8 *menu_selection_seed asm("r0") = (u8 *)&gMenuSelectionIndex;
        register u32 selected_deck_slot asm("r1") = *menu_selection_seed;
        register u8 *selected_commands asm("r5");
        register u8 *selected_command_seed asm("r1");
        register u8 *selected_command_entry asm("r8");
        register u32 *unlocked_commands asm("r6");

        menu_selection_index = menu_selection_seed;
        asm volatile("" : : "r"(menu_selection_index));
        selected_commands = gPlayerSelectedDeckCommands;
        selected_command_seed = (u8 *)(selected_deck_slot + (u32)selected_commands);
        selected_command_entry = selected_command_seed;
        unlocked_commands = (u32 *)((u8 *)selected_commands + PLAYER_STATE_OFFSET(unlocked_deck_commands) - PLAYER_STATE_OFFSET(selected_deck_commands));

select_deck_command:
        RunMenuScript(DECK_COMMAND_EDITOR_SELECT_COMMAND_SCRIPT);
        {
            register u8 *catalog_order asm("r0");
            register u32 catalog_index asm("r1");
            register u32 command_id asm("r4");
            register u32 menu_result asm("r2");

            catalog_order = (u8 *)DECK_COMMAND_CATALOG_ORDER_ROM;
            asm volatile("" : "+r"(catalog_order));
            catalog_index = *menu_selection_index;
            asm volatile("add %0, %1, %0"
                         : "+r"(catalog_order) : "r"(catalog_index));
            command_id = *catalog_order;
            menu_result = gMenuSelectionResult;

            if (menu_result == DECK_COMMAND_MENU_CONFIRMED) {
                if (catalog_index != 0) {
                    register u32 unlock_word asm("r0") = command_id >> 5;

                    unlock_word <<= 2;
                    unlock_word += (u32)unlocked_commands;
                    {
                        register u32 unlock_bit_index asm("r1") = 0x1F;
                        unlock_bit_index &= command_id;
                        menu_result <<= unlock_bit_index;
                    }
                    unlock_word = *(u32 *)unlock_word;
                    unlock_word &= menu_result;
                    if (unlock_word == 0) {
                        goto select_deck_command;
                    }
                    {
                        register u32 previous_command_slot asm("r1") = 0;

                        if (selected_commands[0] != command_id) {
                            register u8 *previous_selection_list asm("r2") = gPlayerSelectedDeckCommands;

                            do {
                                register u32 next_command_slot_shifted asm("r0") = previous_command_slot + 1;

                                next_command_slot_shifted <<= 24;
                                previous_command_slot = next_command_slot_shifted >> 24;
                                if (previous_command_slot > PLAYER_SELECTED_DECK_COMMAND_COUNT - 1) {
                                    break;
                                }
                            } while (*(u8 *)(previous_command_slot + (u32)previous_selection_list) != command_id);
                        }
                        if (previous_command_slot <= PLAYER_SELECTED_DECK_COMMAND_COUNT - 1) {
                            *(u8 *)(previous_command_slot + (u32)selected_commands) = 0;
                        }
                    }
                }

                *selected_command_entry = command_id;
                ClearWindowTextList(DECK_COMMAND_CATALOG_WINDOW);
                AppendDeckCommandCatalogRows(DECK_COMMAND_PLAYER_LIST);
                RequestWindowRefresh();
                YieldTaskForUpdates(1);
                ClearWindowTextList(DECK_COMMAND_SELECTED_WINDOW);
                AppendSelectedDeckCommandRows(DECK_COMMAND_PLAYER_LIST);
                goto select_deck_slot;
            }

            if ((gMenuSelectionKeys & DECK_COMMAND_DESCRIPTION_KEY) != 0) {
                PlaySong(DECK_COMMAND_DESCRIPTION_SOUND);
                if (*menu_selection_index != 0) {
                    register u32 unlock_word asm("r2") = command_id >> 5;

                    unlock_word <<= 2;
                    unlock_word += (u32)unlocked_commands;
                    {
                        register u32 unlock_bit_index asm("r0") = 0x1F;
                        register u32 unlock_bit asm("r1");
                        register u32 unlock_flags asm("r0");

                        unlock_bit_index &= command_id;
                        unlock_bit = 1;
                        unlock_bit <<= unlock_bit_index;
                        unlock_flags = *(u32 *)unlock_word;
                        unlock_flags &= unlock_bit;
                        if (unlock_flags != 0) {
                        ShowDeckCommandDescription(command_id);
                        } else {
                        ShowDeckCommandDescription(0);
                        }
                    }
                }
                goto select_deck_command;
            }
        }
        goto select_deck_slot;
    }

    if ((gMenuSelectionKeys & DECK_COMMAND_DESCRIPTION_KEY) != 0) {
        PlaySong(DECK_COMMAND_DESCRIPTION_SOUND);
        ShowDeckCommandDescription(player_deck[gMenuSelectionIndex]);
        goto select_deck_slot;
    }
    RunMenuScript(DECK_COMMAND_EDITOR_CLOSE_SCRIPT);
}
