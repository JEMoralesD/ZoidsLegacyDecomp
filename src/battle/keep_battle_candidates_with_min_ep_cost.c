#include "m2c_prelude.h"
#include "battle.h"

void RemoveBattleActionCandidate(u32, u32) asm("func_080CA570");

void KeepBattleCandidatesWithMinEpCost(void) asm("func_080CAF50");

void KeepBattleCandidatesWithMinEpCost(void)
{
    u16 minimum_ep_cost;
    register u32 candidate_index asm("r4");
    register u8 *count_slot asm("r8");
    register u8 *count_load asm("r0");

    minimum_ep_cost = 0x7FFF;
    candidate_index = 0;
    {
        register u32 count asm("r3");

        count_load = &((struct BattleActionCandidates *)0x0203EF70)->equipment_count;
        count = *count_load;
        count_slot = count_load;

        if (candidate_index < count) {
            register u8 *row_slot asm("r2");
            register u32 action_index asm("r1");
            register u32 row_offset asm("r0");
            register u8 *record_base asm("r5");
            register u8 *index_table asm("ip");
            register u8 *index_table_load asm("r7");
            register u32 record_stride asm("r9");
            register u32 stride_load asm("r0");
            register s32 base_adjust asm("r1");

            row_slot = (u8 *)0x0203ECFB;
            action_index = *row_slot;
            row_offset = action_index << 3;
            row_offset -= action_index;
            row_offset <<= 5;
            row_offset += action_index;
            row_offset <<= 2;
            base_adjust = -0x79E7;
            row_slot += base_adjust;
            record_base = (u8 *)(row_offset + (s32)row_slot);
            index_table_load = ((struct BattleActionCandidates *)0x0203EF70)->equipment_slots;
            index_table = index_table_load;
            stride_load = 0xA8C;
            asm volatile("" : "+r"(stride_load));
            record_stride = stride_load;
            do {
                register u8 *table asm("r1") = index_table;
                register u32 equipment_slot asm("r0");
                register u8 *record asm("r2");
                register s32 signed_minimum asm("r0");
                register s32 value asm("r1");
                register u32 next asm("r0");
                register u8 *table_entry asm("r0");

                asm volatile("" : "+r"(table));
                table_entry = (u8 *)(candidate_index + (s32)table);
                equipment_slot = *table_entry;
                {
                    u32 record_offset;
                    register u32 offset_copy asm("r0");

                    asm volatile("" : "+r"(row_slot));
                    record_offset = equipment_slot * record_stride;
                    offset_copy = record_offset;
                    asm volatile("" : "+r"(offset_copy));
                    record = (u8 *)(offset_copy + (s32)record_base);
                }
                signed_minimum = (s16)minimum_ep_cost;
                value = *(s16 *)(record + 8);
                if (signed_minimum > value) {
                    minimum_ep_cost = *(u16 *)(record + 8);
                }
                next = candidate_index + 1;
                candidate_index = (u8)next;
            } while (candidate_index < count);
        }
    }

    candidate_index = 0;
    {
        register u32 second_temp asm("r0");

        second_temp = *count_slot;
        if (candidate_index < second_temp) {
        u8 *base;
        u8 *record_base;
        register s32 signed_minimum asm("r6");

        base = (u8 *)0x02034B4C;
        record_base = base + 0x27C8;
        asm volatile("" : "+r"(second_temp));
        second_temp = minimum_ep_cost << 16;
        signed_minimum = (s32)second_temp >> 16;
        do {
            register u8 *index_table asm("r0");
            register s32 equipment_slot asm("r1");
            register s32 record_offset asm("r2");
            register s32 record_stride asm("r0");
            register s32 row_base_offset asm("r1");
            register u8 *row_slot asm("r0");
            register s32 action_index asm("r1");
            register s32 row_offset asm("r0");
            register u8 *record asm("r2");
            register s32 value asm("r0");
            register u32 next_index asm("r0");

            index_table = ((struct BattleActionCandidates *)0x0203EF70)->equipment_slots;
            asm volatile("" : "+r"(index_table));
            index_table = (u8 *)(candidate_index + (s32)index_table);
            equipment_slot = *index_table;
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
            record = (u8 *)record_offset;
            value = *(s16 *)(record + 8);
            if (value != signed_minimum) {
                RemoveBattleActionCandidate(candidate_index, BATTLE_REMOVE_EQUIPMENT_CANDIDATE);
                next_index = candidate_index - 1;
                candidate_index = (u8)next_index;
            }
            {
                register u32 next asm("r0") = candidate_index + 1;
                candidate_index = (u8)next;
            }
        } while (candidate_index < ((struct BattleActionCandidates *)0x0203EF70)->equipment_count);
        }
    }
}
