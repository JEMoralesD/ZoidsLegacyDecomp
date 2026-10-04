#include "m2c_prelude.h"
#include "battle.h"

void RemoveBattleActionCandidate(u8, u8) asm("func_080CA570");

void KeepBattleNonAttackCandidates(void) asm("func_080CA860");

void KeepBattleNonAttackCandidates(void) {
    u32 candidate_index;

    candidate_index = 0;
    if (candidate_index < ((struct BattleActionCandidates *)0x0203EF70)->equipment_count) {
        u8 *base;
        u8 *record_base;

        base = (u8 *)0x02034B4C;
        record_base = base + 0x27C8;
        do {
            register u8 *candidate_equipment_slots asm("r0");
            register s32 equipment_slot asm("r1");
            register s32 record_offset asm("r2");
            register s32 record_stride asm("r0");
            register s32 row_base_offset asm("r1");
            register u8 *row_slot asm("r0");
            register s32 action_index asm("r1");
            register s32 row_offset asm("r0");
            register u8 *equipment_action asm("r2");
            register s32 flags asm("r1");
            register s32 mask asm("r0");
            register u32 next_index asm("r0");

            candidate_equipment_slots = ((struct BattleActionCandidates *)0x0203EF70)->equipment_slots;
            asm volatile("" : "+r"(candidate_equipment_slots));
            candidate_equipment_slots = (u8 *)(candidate_index + (s32)candidate_equipment_slots);
            equipment_slot = *candidate_equipment_slots;
            record_stride = 0xA8C;
            record_offset = equipment_slot;
            record_offset *= record_stride;
            row_base_offset = 0xA1AF;
            row_slot = base + row_base_offset;
            action_index = *row_slot;
            row_offset = action_index << 3;
            row_offset -= action_index;
            row_offset <<= 5;
            row_offset += action_index;
            row_offset <<= 2;
            row_offset += (s32)record_base;
            record_offset += row_offset;
            equipment_action = (u8 *)record_offset;
            flags = BATTLE_ACTION_FIELD(equipment_action, u16, flags);
            mask = EQUIPMENT_ATTACK;
            mask &= flags;
            if (mask != 0) {
                RemoveBattleActionCandidate(candidate_index, BATTLE_REMOVE_EQUIPMENT_CANDIDATE);
                next_index = candidate_index - 1;
                candidate_index = (u8)next_index;
            }
            candidate_index = (u8)(candidate_index + 1);
        } while (candidate_index < ((struct BattleActionCandidates *)0x0203EF70)->equipment_count);
    }
}
