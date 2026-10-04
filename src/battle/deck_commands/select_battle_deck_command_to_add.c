#include "m2c_prelude.h"
#include "deck_commands.h"

void RunMenuScript(s32) asm("func_08098BB4");
void AppendDeckCommandCatalogRows(s32) asm("func_080C098C");
void ShowDeckCommandDescription(u8) asm("func_080C0A9C");
void PlaySong(s32) asm("func_08092E84");

extern volatile u8 gMenuSelectionIndex asm("D_0200A880");
extern volatile u8 gMenuSelectionResult asm("D_0200A882");
extern volatile u16 gMenuSelectionKeys asm("D_0200A884");

s32 SelectBattleDeckCommandToAdd(void) asm("func_080C2518");

s32 SelectBattleDeckCommandToAdd(void)
{
    register u32 catalog_order_or_result asm("r4");
    register u32 *unlocked_commands asm("r6");
    register u8 *selected_commands asm("r5");

    RunMenuScript(DECK_COMMAND_ADD_OPEN_SCRIPT);
    AppendDeckCommandCatalogRows(DECK_COMMAND_BATTLE_LIST);
    catalog_order_or_result = DECK_COMMAND_CATALOG_ORDER_ROM;
    unlocked_commands = (u32 *)0x020282E0;
    selected_commands = (u8 *)0x02037300;

select_command:
    {
        register u32 menu_result asm("r2");

        RunMenuScript(DECK_COMMAND_ADD_SELECT_SCRIPT);
        menu_result = gMenuSelectionResult;
        if (menu_result == DECK_COMMAND_MENU_CONFIRMED) {
            register u32 command_id asm("r3");
            register u32 catalog_index asm("r0");

            catalog_index = gMenuSelectionIndex;
            catalog_index += 1;
            command_id = *(u8 *)(catalog_index + catalog_order_or_result);
            if (command_id == 0) {
                goto command_selected;
            }
            {
                register u32 unlock_word asm("r0") = command_id >> 5;

                unlock_word <<= 2;
                asm volatile("" : "+r"(unlock_word));
                unlock_word += (u32)unlocked_commands;
                {
                    register u32 unlock_bit_index asm("r1") = 0x1F;

                    unlock_bit_index &= command_id;
                    menu_result <<= unlock_bit_index;
                }
                unlock_word = *(u32 *)unlock_word;
                unlock_word &= menu_result;
                if (unlock_word != 0) {
                register u32 selected_slot asm("r1") = 0;

                if (selected_commands[0] != command_id) {
                    register u8 *selection_list asm("r2") = (u8 *)0x02037300;

                    do {
                        register u32 next_slot_shifted asm("r0") = selected_slot + 1;

                        next_slot_shifted <<= 24;
                        selected_slot = next_slot_shifted >> 24;
                        if (selected_slot > PLAYER_SELECTED_DECK_COMMAND_COUNT - 1) {
                            break;
                        }
                    } while (*(u8 *)(selected_slot + (u32)selection_list) != command_id);
                }
                if (selected_slot <= PLAYER_SELECTED_DECK_COMMAND_COUNT - 1) {
                    goto select_command;
                }
command_selected:
                catalog_order_or_result = command_id;
                goto selection_done;
                }
            }
            goto select_command;
        } else {
            if ((gMenuSelectionKeys & DECK_COMMAND_DESCRIPTION_KEY) != 0) {
                register u32 catalog_index asm("r0");

                catalog_index = gMenuSelectionIndex;
                catalog_index += 1;
                ShowDeckCommandDescription(*(u8 *)(catalog_index + catalog_order_or_result));
                PlaySong(DECK_COMMAND_DESCRIPTION_SOUND);
                goto select_command;
            }
            catalog_order_or_result = 0;
        }
    }

selection_done:
    RunMenuScript(DECK_COMMAND_ADD_CLOSE_SCRIPT);
    return catalog_order_or_result;
}
