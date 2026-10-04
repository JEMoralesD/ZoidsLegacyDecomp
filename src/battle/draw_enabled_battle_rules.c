#include "m2c_prelude.h"
#include "../game/player_state.h"
M2C_UNK PrintWindowTextAt(M2C_UNK, s32, s32, s32, s32) asm("func_080981F0"); /* extern */
M2C_UNK PrintWindowText(M2C_UNK, s32, s32) asm("func_08098248");           /* extern */
M2C_UNK ClearWindow(s32) asm("func_080986B4");                         /* extern */
s32 TestBattleRuleFlag(s32) asm("func_080E6664");                             /* extern */

void DrawEnabledBattleRules(void) asm("func_080E0F44");

void DrawEnabledBattleRules(void) {
    s32 text_row;

    ClearWindow(1);
    text_row = 0;
    if ((TestBattleRuleFlag(BATTLE_RULE_UP_TO_ONE_ZOID) << 0x18) != 0) {
        PrintWindowTextAt(0x08108FBC, 0, 1, 0, 0);
        text_row = 2;
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_UP_TO_TWO_ZOIDS) << 0x18) != 0) {
        PrintWindowTextAt(0x08108FD4, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_UP_TO_THREE_ZOIDS) << 0x18) != 0) {
        PrintWindowTextAt(0x08108FE4, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_UP_TO_FOUR_ZOIDS) << 0x18) != 0) {
        PrintWindowTextAt(0x08108FF4, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_SIZE_S_ONLY) << 0x18) != 0) {
        PrintWindowTextAt(0x08109004, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_UP_TO_SIZE_M) << 0x18) != 0) {
        PrintWindowTextAt(0x0810901C, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_SIZE_M_ONLY) << 0x18) != 0) {
        PrintWindowTextAt(0x08109038, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_UP_TO_SIZE_L) << 0x18) != 0) {
        PrintWindowTextAt(0x08109050, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_AT_LEAST_ONE_SIZE_L_OR_LARGER) << 0x18) != 0) {
        PrintWindowTextAt(0x0810906C, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_AT_LEAST_ONE_SIZE_XL_OR_LARGER) << 0x18) != 0) {
        PrintWindowTextAt(0x08109088, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_NO_FLYING_ZOIDS) << 0x18) != 0) {
        PrintWindowTextAt(0x081090A4, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_FLYING_ZOIDS_ONLY) << 0x18) != 0) {
        PrintWindowTextAt(0x081090C0, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_LIGER_MODELS_ONLY) << 0x18) != 0) {
        PrintWindowTextAt(0x081090DC, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_TIGER_MODELS_ONLY) << 0x18) != 0) {
        PrintWindowTextAt(0x081090F4, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_WOLF_MODELS_ONLY) << 0x18) != 0) {
        PrintWindowTextAt(0x0810910C, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_EQUIPMENT_SLOTS_4_TO_7_ONLY) << 0x18) != 0) {
        PrintWindowTextAt(0x08109128, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_MELEE_ONLY) << 0x18) != 0) {
        PrintWindowTextAt(0x0810913C, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_NO_MELEE) << 0x18) != 0) {
        PrintWindowTextAt(0x08109154, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_NO_RECOVERY) << 0x18) != 0) {
        PrintWindowTextAt(0x08109168, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_NO_DECK_COMMANDS) << 0x18) != 0) {
        PrintWindowTextAt(0x08109180, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if ((TestBattleRuleFlag(BATTLE_RULE_NO_ORGANOID_OR_ZOS) << 0x18) != 0) {
        PrintWindowTextAt(0x0810919C, 0, 1, 0, text_row);
        text_row = (s32) (u8) (text_row + 2);
    }
    if (text_row == 0) {
        PrintWindowText(0x081091B4, 0, 1);
    }
}
