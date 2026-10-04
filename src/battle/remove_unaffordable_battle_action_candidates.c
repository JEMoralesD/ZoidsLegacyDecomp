#include "m2c_prelude.h"
#include "battle.h"

void RemoveBattleActionCandidate(u8, u8) asm("func_080CA570");

void RemoveUnaffordableBattleActionCandidates(void) asm("func_080CA758");

void RemoveUnaffordableBattleActionCandidates(void)
{
    u32 candidate_index;

    candidate_index = 0;
    if (candidate_index < *(u8 *)0x0203EFA8) {
        u8 *battle_state;
        u8 **active_unit_slot;

        battle_state = (u8 *)0x02034B4C;
        active_unit_slot = (u8 **)(battle_state + 0x27A8);
        do {
            register u8 *candidate_equipment_slots asm("r0");
            register s32 equipment_slot asm("r1");
            register s32 record_offset asm("r2");
            register s32 record_stride asm("r0");
            register s32 row_base_offset asm("r1");
            register u8 *row_slot asm("r0");
            register s32 action_index asm("r1");
            register s32 row_offset asm("r0");
            register s32 record_base_offset asm("r3");
            register u8 *record_base asm("r1");
            register u8 *equipment_action asm("r2");
            register s32 unit_ep asm("r3");
            register s32 planned_ep_cost asm("r0");
            register s32 action_ep_cost asm("r1");
            register u32 next_index asm("r0");

            candidate_equipment_slots = (u8 *)0x0203EF70;
            asm volatile("" : "+r"(candidate_equipment_slots));
            candidate_equipment_slots = (u8 *)(candidate_index + (s32)candidate_equipment_slots);
            equipment_slot = *candidate_equipment_slots;
            record_stride = 0xA8C;
            record_offset = equipment_slot;
            record_offset *= record_stride;
            row_base_offset = 0xA1AF;
            row_slot = battle_state + row_base_offset;
            action_index = *row_slot;
            row_offset = action_index << 3;
            row_offset -= action_index;
            row_offset <<= 5;
            row_offset += action_index;
            row_offset <<= 2;
            record_base_offset = 0x27C8;
            record_base = battle_state + record_base_offset;
            row_offset += (s32)record_base;
            record_offset += row_offset;
            equipment_action = (u8 *)record_offset;

            unit_ep = M2C_FIELD(*active_unit_slot, s16 *, 8);
            planned_ep_cost = M2C_FIELD((void *)0x0203EFB2, s16 *, 0);
            action_ep_cost = BATTLE_ACTION_FIELD(equipment_action, s16, ep_cost);
            planned_ep_cost += action_ep_cost;
            if (unit_ep < planned_ep_cost) {
                RemoveBattleActionCandidate(candidate_index, BATTLE_REMOVE_EQUIPMENT_CANDIDATE);
                next_index = candidate_index - 1;
                candidate_index = (u8)next_index;
            }
            candidate_index = (u8)(candidate_index + 1);
        } while (candidate_index < *(u8 *)0x0203EFA8);
    }
}
