#include "m2c_prelude.h"
#include "player_state.h"
void BiosCpuSet() asm("func_80ECD2C");
void AddPlayerZoidWithPilot(s32, s32, s32) asm("func_080E5C8C");
void AssignZoidToPlayerTeam(s32, s32) asm("func_080E5FA8");
void AddPlayerMoney(s32) asm("func_080E5E64");
void InitializeNewGamePlayerState(void) asm("func_0809A00C");

void InitializeNewGamePlayerState(void) {
    s32 zero_fill;
    zero_fill = 0;
    BiosCpuSet(&zero_fill, NEW_GAME_PLAYER_STATE_RAM, NEW_GAME_PLAYER_CLEAR_CONTROL);
    AddPlayerZoidWithPilot(NEW_GAME_STARTER_ZOID_MODEL, 0, PLAYER_PROTAGONIST_PILOT_ID);
    AssignZoidToPlayerTeam(NEW_GAME_STARTER_ZOID_SLOT, NEW_GAME_STARTER_TEAM_SLOT);
    AddPlayerMoney(NEW_GAME_STARTING_MONEY);
}
