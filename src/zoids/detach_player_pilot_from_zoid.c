#include "m2c_prelude.h"
#include "../game/player_state.h"

void RecalculateZoidStats(void *, s32) asm("func_080E5880");

void DetachPlayerPilotFromZoid(s32 stored_pilot_slot) asm("func_080E700C");

void DetachPlayerPilotFromZoid(s32 stored_pilot_slot) {
    register s32 pilot_records_base asm("r2");
    u8 *pilot;
    u8 stored_zoid_slot;

    pilot = (u8 *)(((u32)(stored_pilot_slot << 0x18) >> 0x12) + ({
        asm volatile("ldr %0, [pc, #60]" : "=r"(pilot_records_base));
        pilot_records_base;
    }));
    stored_zoid_slot = pilot[PLAYER_PILOT_OFFSET(zoid_slot)];
    if (stored_zoid_slot != 0) {
        u8 zoid_slot_copy;
        u8 *zoid;
        register s32 zoid_base_from_pilot_base_offset asm("r4");
        s32 zoid_record_offset;
        register s32 zoid_records_base asm("r1");

        zoid_slot_copy = stored_zoid_slot;
        asm volatile("" : "+&r"(zoid_slot_copy) : "r"(stored_zoid_slot));
        zoid_record_offset = zoid_slot_copy * 0x70;
        asm volatile("ldr %0, [pc, #44]" : "=r"(zoid_base_from_pilot_base_offset));
        zoid_records_base = pilot_records_base + zoid_base_from_pilot_base_offset;
        zoid = (u8 *)(zoid_record_offset + zoid_records_base);
        if (*(u16 *)(zoid + PLAYER_ZOID_OFFSET(flags)) & PLAYER_RECORD_IN_TEAM) {
            register u16 clear_in_team_mask asm("r0");
            u16 pilot_flags;

            pilot_flags = *(u16 *)(pilot + PLAYER_PILOT_OFFSET(flags_and_pilot_id));
            asm volatile("ldr %0, [pc, #32]" : "=r"(clear_in_team_mask));
            clear_in_team_mask &= pilot_flags;
            *(u16 *)(pilot + PLAYER_PILOT_OFFSET(flags_and_pilot_id)) = clear_in_team_mask;
        }
        pilot[PLAYER_PILOT_OFFSET(zoid_slot)] = 0;
        zoid[PLAYER_ZOID_OFFSET(pilot_slot)] = 0;
        RecalculateZoidStats(zoid, 0);
    }
}

asm(".align 2, 0\n"
    ".word 0x02027378\n"
    ".word 0xFFFFA570\n"
    ".word 0x0000FFFB\n");
