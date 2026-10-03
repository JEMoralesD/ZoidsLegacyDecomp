#include "m2c_prelude.h"
#include "game_state.h"

extern void RunIntro(void) asm("func_0809B180");
extern void RunTitleMenu(void) asm("func_0809BB74");
extern void RunLoadGame(void) asm("func_0809C1D4");
extern void RunOptionsMenu(void) asm("func_0809D288");
extern void RunField(void) asm("func_0809EC20");
extern void RunClearDataSave(void) asm("func_080A68F0");
extern void RunPauseMenu(void) asm("func_080B571C");
extern void RunTownMap(void) asm("func_080B5EA8");
extern void RunItemShop(void) asm("func_080B68F8");
extern void RunEquipmentShop(void) asm("func_080B7390");
extern void RunZoidsLab(void) asm("func_080BA588");
extern void RunBattle(void) asm("func_080BBA04");
extern void RunBattleScene(void) asm("func_080CD6D8");
extern void RunZoidFormChangeScene(void) asm("func_080CFEE4");
extern void RunLinkBattle(void) asm("func_080E155C");
extern void RunLinkTrade(void) asm("func_080E1F40");
extern void RunDatabase(void) asm("func_080E3EA0");
extern void RunChallengeMenu(void) asm("func_080E4A44");
extern void RunCredits(void) asm("func_080E5000");
extern void func_080E6690(void);
extern void func_080ED17C(s32);

void RunGameStateMachine(void) {
    u32 mode;

    func_080E6690();
    *(u8 *)0x02030665 = 0;
    *(u8 *)0x03000075 = 1;
    for (;;) {
        mode = *(u32 *)0x02021690;
        switch (mode) {
        case GAME_MODE_INTRO: RunIntro(); break;
        case GAME_MODE_TITLE: RunTitleMenu(); break;
        case GAME_MODE_OPTIONS: RunOptionsMenu(); break;
        case GAME_MODE_FIELD: RunField(); break;
        case GAME_MODE_PAUSE_MENU: RunPauseMenu(); break;
        case GAME_MODE_ITEM_SHOP: RunItemShop(); break;
        case GAME_MODE_WEAPON_SHOP:
        case GAME_MODE_ARMOR_SHOP: RunEquipmentShop(); break;
        case GAME_MODE_ZOIDS_LAB: RunZoidsLab(); break;
        case GAME_MODE_BATTLE: RunBattle(); break;
        case GAME_MODE_BATTLE_SCENE: RunBattleScene(); break;
        case GAME_MODE_ZOID_FORM_CHANGE: RunZoidFormChangeScene(); break;
        case GAME_MODE_TOWN_MAP: RunTownMap(); break;
        case GAME_MODE_LINK_BATTLE: RunLinkBattle(); break;
        case GAME_MODE_LINK_TRADE: RunLinkTrade(); break;
        case GAME_MODE_DATABASE: RunDatabase(); break;
        case GAME_MODE_CHALLENGE_MENU: RunChallengeMenu(); break;
        case GAME_MODE_LOAD_GAME: RunLoadGame(); break;
        case GAME_MODE_CLEAR_DATA_SAVE: RunClearDataSave(); break;
        case GAME_MODE_CREDITS: RunCredits(); break;
        case (u32)GAME_MODE_WAIT: func_080ED17C(1); break;
        }
    }
}
