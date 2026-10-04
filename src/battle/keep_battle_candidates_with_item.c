#include "m2c_prelude.h"
#include "battle.h"
extern volatile u8 gBattleActionCandidateCount asm("D_0203EFA8");
extern u8 *gActiveBattleUnit asm("D_020372F4");
extern u8 gBattleActionCandidateEquipmentSlots[] asm("D_0203EF70");

void RemoveBattleActionCandidate(u8, u8) asm("func_080CA570");

void KeepBattleCandidatesWithItem(u16 item_id) asm("func_080CB2EC");

void KeepBattleCandidatesWithItem(u16 item_id) {
    u8 candidate_index;

    candidate_index = 0;
    if (candidate_index < gBattleActionCandidateCount) {
        register u8 **base_slot asm("r6");
        u8 *equipment_item_id_ptr;

        base_slot = &gActiveBattleUnit;
        do {
            equipment_item_id_ptr = *base_slot;
            equipment_item_id_ptr += gBattleActionCandidateEquipmentSlots[candidate_index] * 4;
            equipment_item_id_ptr += 0x52;
            if (*(u16 *)equipment_item_id_ptr != item_id) {
                RemoveBattleActionCandidate(candidate_index, BATTLE_REMOVE_EQUIPMENT_CANDIDATE);
                candidate_index = (u8)(candidate_index - 1);
            }
            candidate_index = (u8)(candidate_index + 1);
        } while (candidate_index < gBattleActionCandidateCount);
    }
}
