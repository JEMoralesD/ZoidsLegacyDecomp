#include "m2c_prelude.h"
#include "../game/player_state.h"

extern void ClearWindowTextList(s32) asm("func_08098834");
extern void AppendWindowTextItem(s32, u16 *) asm("func_080988C8");
extern u8 TestBattleRuleFlag(s32) asm("func_080E6664");
extern void CopyBytes(void *, const void *, s32) asm("func_080ED038");
extern u16 gBattleRuleTextPrefix asm("D_02030564");
extern u16 gBattleRuleTextBytes asm("D_02030566");

void BuildBattleRuleToggleTextList(void) asm("func_080E125C");

void BuildBattleRuleToggleTextList(void) {
    u16 *rule_text_cursor;

    ClearWindowTextList(3);

    if ((TestBattleRuleFlag(BATTLE_RULE_EQUIPMENT_SLOTS_4_TO_7_ONLY) << 24) != 0) {
        u16 *text_color_prefix = &gBattleRuleTextPrefix;
        register s32 enabled_color_prefix asm("r0") = 0x201;
        *text_color_prefix = enabled_color_prefix;
    } else gBattleRuleTextPrefix = 1;
    rule_text_cursor = &gBattleRuleTextBytes;
    CopyBytes(rule_text_cursor, (const void *)0x081091C0, 45);
    rule_text_cursor--;
    AppendWindowTextItem(3, rule_text_cursor);

    if ((TestBattleRuleFlag(BATTLE_RULE_MELEE_ONLY) << 24) != 0) {
        register s32 enabled_color_prefix asm("r0") = 0x201;
        *rule_text_cursor = enabled_color_prefix;
    } else *rule_text_cursor = 1;
    rule_text_cursor = &gBattleRuleTextBytes;
    CopyBytes(rule_text_cursor, (const void *)0x0810913C, 23);
    rule_text_cursor--;
    AppendWindowTextItem(3, rule_text_cursor);

    if ((TestBattleRuleFlag(BATTLE_RULE_NO_MELEE) << 24) != 0) {
        register s32 enabled_color_prefix asm("r0") = 0x201;
        *rule_text_cursor = enabled_color_prefix;
    } else *rule_text_cursor = 1;
    rule_text_cursor = &gBattleRuleTextBytes;
    CopyBytes(rule_text_cursor, (const void *)0x08109154, 19);
    rule_text_cursor--;
    AppendWindowTextItem(3, rule_text_cursor);

    if ((TestBattleRuleFlag(BATTLE_RULE_NO_RECOVERY) << 24) != 0) {
        register s32 enabled_color_prefix asm("r0") = 0x201;
        *rule_text_cursor = enabled_color_prefix;
    } else *rule_text_cursor = 1;
    rule_text_cursor = &gBattleRuleTextBytes;
    CopyBytes(rule_text_cursor, (const void *)0x08109168, 23);
    rule_text_cursor--;
    AppendWindowTextItem(3, rule_text_cursor);

    if ((TestBattleRuleFlag(BATTLE_RULE_NO_DECK_COMMANDS) << 24) != 0) {
        register s32 enabled_color_prefix asm("r0") = 0x201;
        *rule_text_cursor = enabled_color_prefix;
    } else *rule_text_cursor = 1;
    rule_text_cursor = &gBattleRuleTextBytes;
    CopyBytes(rule_text_cursor, (const void *)0x08109180, 27);
    rule_text_cursor--;
    AppendWindowTextItem(3, rule_text_cursor);

    if ((TestBattleRuleFlag(BATTLE_RULE_NO_ORGANOID_OR_ZOS) << 24) != 0) {
        register s32 enabled_color_prefix asm("r0") = 0x201;
        *rule_text_cursor = enabled_color_prefix;
    } else *rule_text_cursor = 1;
    rule_text_cursor = &gBattleRuleTextBytes;
    CopyBytes(rule_text_cursor, (const void *)0x0810919C, 23);
    rule_text_cursor--;
    AppendWindowTextItem(3, rule_text_cursor);
}
