#include "m2c_prelude.h"
#include "field_display.h"

extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u16 DivideUnsigned32(u32, u32) asm("func_080ECF00");
extern struct FieldEncounterActorView *gPlayerFieldActor asm("D_02032990");
extern u16 gFieldMapState asm("D_0202ECF4");
extern u16 gFieldMapDimensions asm("D_020324A4");
extern s32 gFieldMovementCounter asm("D_020324A8");
extern s32 gPreviousFieldMovementCounter asm("D_020324AC");
extern u8 gFieldEncounterTableIndex asm("D_020324B7");
extern u8 gBattleSetup[];
extern u32 gRandomFunction asm("D_03000010");
extern u16 gWorldMapEncounterTiles[] asm("D_08478FA0");
extern u8 gFieldEncounterFormations[] asm("D_087B9454");
extern u16 gWorldMapEncounterThresholds[] asm("D_087C3864");
extern u8 gFieldEncounterDefinitions[] asm("D_087C3884");

s32 TrySelectFieldRandomEncounter(void) asm("func_0809E72C");

s32 TrySelectFieldRandomEncounter(void)
{
    u8 formation_candidates[20];
    register struct FieldEncounterActorView *player_actor asm("r4") = gPlayerFieldActor;
    register s32 *movement_counter_address asm("r3");
    u32 encounter_group;
    register u32 encounter_threshold asm("r8");

    if ((u8)(player_actor->model_id - 0x69) > 2) {
        goto fail;
    }
    if (player_actor->velocity_x_fixed8 == 0 && player_actor->velocity_y_fixed8 == 0) {
        goto fail;
    }
    movement_counter_address = &gFieldMovementCounter;
    asm volatile("" : "+r"(movement_counter_address));
    if ((*movement_counter_address & ~0xF) == (gPreviousFieldMovementCounter & ~0xF)) {
        goto fail;
    }
    if (player_actor->map_cell_value & 0x40) {
        goto fail;
    }

    if (gFieldMapState == 0) {
        s32 world_x_fixed8 = player_actor->world_x_fixed8;
        s32 world_y_fixed8;
        u32 cell_x;
        register u32 byte_mask asm("r5");
        u32 cell_y;
        u32 tile_offset;
        u16 map_width_in_tiles;
        u8 *encounter_tiles;
        u16 encounter_tile;

        if (world_x_fixed8 < 0) {
            world_x_fixed8 += 0xFFF;
        }
        cell_x = world_x_fixed8 >> 12;
        byte_mask = 0xFF;
        cell_x &= byte_mask;
        world_y_fixed8 = player_actor->world_y_fixed8;
        if (world_y_fixed8 < 0) {
            world_y_fixed8 += 0xFFF;
        }
        cell_y = (world_y_fixed8 >> 12) & byte_mask;
        map_width_in_tiles = gFieldMapDimensions;
        tile_offset = (((cell_y * map_width_in_tiles) >> 1) + cell_x) << 1;
        encounter_tiles = (u8 *)gWorldMapEncounterTiles;
        encounter_tile = *(u16 *)(encounter_tiles + tile_offset);
        encounter_threshold = gWorldMapEncounterThresholds[encounter_tile >> 8];
        encounter_group = byte_mask;
        encounter_group &= encounter_tile;
    } else {
        register u8 *encounter_table_bytes asm("r2") = gFieldEncounterDefinitions;
        register u32 offset asm("r1");
        register u8 *threshold_address asm("r0");

        asm volatile("" : "+r"(encounter_table_bytes));
        offset = gFieldEncounterTableIndex * 6;
        threshold_address = encounter_table_bytes + 2;
        threshold_address = (u8 *)(offset + (u32)threshold_address);
        encounter_threshold = *(u16 *)threshold_address;
        encounter_table_bytes += 4;
        offset += (u32)encounter_table_bytes;
        encounter_group = *(u16 *)offset;
    }

    if (encounter_group == 0) {
        goto fail;
    }

    {
        register u32 movement_chance_divisor asm("r6");
        register u32 *random_function_address asm("r5");
        register u32 chance_roll asm("r4");
        register u32 *random_function_late_address asm("ip");
        register u32 shift asm("r0");
        u16 adjusted_threshold;

        shift = *movement_counter_address;
        asm volatile("" : "+r"(shift));
        shift <<= 12;
        movement_chance_divisor = (u32)shift >> 16;
        if (movement_chance_divisor > 7) {
            movement_chance_divisor = 1;
        } else {
            movement_chance_divisor = (u16)(0x100 >> movement_chance_divisor);
        }

        random_function_address = &gRandomFunction;
        chance_roll = ((u32)(CallFunctionR0(*random_function_address) * 1000)) >> 15;
        adjusted_threshold = DivideUnsigned32(encounter_threshold, movement_chance_divisor);
        random_function_late_address = random_function_address;
        asm volatile("" : "+r"(random_function_late_address));
        if ((s32)chance_roll >= (s32)adjusted_threshold) {
            goto fail;
        }

        gBattleSetup[3] = encounter_group;
        {
            register u32 candidate_count asm("r6") = 0;
            register u32 formation_index asm("r4") = 0;
            register u32 group_byte_offset asm("r2");
            register u8 *formation_table_bytes asm("r5");
            register u32 formation_bytes asm("r8");
            register u32 group_offset_carrier asm("r0");

            group_offset_carrier = encounter_group << 5;
            asm volatile("" : "+r"(group_offset_carrier));
            formation_table_bytes = gFieldEncounterFormations;
            formation_bytes = 100;
            group_offset_carrier -= encounter_group;
            group_offset_carrier <<= 2;
            group_offset_carrier += encounter_group;
            group_byte_offset = group_offset_carrier << 4;
outer_loop:
            {
                register u32 member_index asm("r1") = 0;
                register u32 next_formation_index asm("r7") = formation_index + 1;
                u32 row_offset = formation_bytes * formation_index;

scan_loop:
                {
                    u32 offset = member_index << 4;
                    struct FieldEncounterMemberView *entry;

                    offset += row_offset;
                    offset += group_byte_offset;
                    offset += (u32)formation_table_bytes;
                    entry = (struct FieldEncounterMemberView *)offset;
                    if (entry->zoid_id != 0) {
                        register u8 *stack_base asm("r1") = formation_candidates;
                        register u8 *destination asm("r0");
                        register u32 next_count asm("r0");

                        asm volatile("" : "+r"(stack_base));
                        destination = stack_base + candidate_count;
                        asm volatile("" : "+r"(destination));
                        *destination = formation_index;
                        next_count = candidate_count + 1;
                        asm volatile("" : "+r"(next_count));
                        candidate_count = (u8)next_count;
                        goto next_row;
                    }
                }
                {
                    register u32 next_member_index asm("r0") = member_index + 1;

                    asm volatile("" : "+r"(next_member_index));
                    member_index = (u8)next_member_index;
                }
                if (member_index <= 5) {
                    goto scan_loop;
                }
next_row:
                {
                    register u32 formation_index_bits asm("r0");

                    formation_index_bits = next_formation_index << 24;
                    asm volatile("" : "+r"(formation_index_bits));
                    formation_index = formation_index_bits >> 24;
                }
            }
            if (formation_index <= 0x13) {
                goto outer_loop;
            }
            {
                register u32 *random_function_late_view asm("r1") = random_function_late_address;

                asm volatile("" : "+r"(random_function_late_view));
                gBattleSetup[4] = formation_candidates[
                    (u32)(CallFunctionR0(*random_function_late_view) * candidate_count) >> 15];
            }
        }
    }
    return 1;

fail:
    return 0;
}
