#include "m2c_prelude.h"
#include "battle.h"
extern u8 gBattleState[];
extern u8 D_000027BE[];

s32 IsBattleCombinedAttackLeader(u8 side, u8 unit_index) asm("func_080E8C48");

s32 IsBattleCombinedAttackLeader(u8 side, u8 unit_index) {
    register u8 leader_unit_index asm("r2");
    u8 *side_commands;

    leader_unit_index = unit_index;
    side_commands = gBattleState + (s32)D_000027BE;
    switch (*(u8 *)(side + (s32)side_commands)) {
    case 47:
        if (leader_unit_index == 4) {
            return 1;
        }
        break;
    case 48:
    case 46:
        if (leader_unit_index == 1) {
            return 1;
        }
        break;
    }
    return 0;
}
