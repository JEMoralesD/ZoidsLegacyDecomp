#include "m2c_prelude.h"
#include "battle_display.h"

extern u8 gBattleUnitSpriteMotionStates[2][6] asm("D_02032EEC");
extern s32 *gBattleUnitSprites[2][6] asm("D_02032E8C");
extern s32 gBattleUnitWorldPositions[][3] asm("D_087A2790");
extern s32 gBattleUnitWorldPositionsZColumn[][3] asm("D_087A2798");

void UpdateBattleUnitSpriteMotion(void) asm("func_080BB0B0");

void UpdateBattleUnitSpriteMotion(void) {
    register u8 *motion_states asm("sl");
    register s32 **unit_sprites asm("r8");
    register s32 *world_positions asm("r9");
    register s32 side asm("ip");

    side = 0;
    motion_states = gBattleUnitSpriteMotionStates;
    world_positions = gBattleUnitWorldPositions[0];
    {
        register s32 **descriptor_seed asm("r4") = gBattleUnitSprites[0];

        asm volatile("" : "+r"(descriptor_seed));
        unit_sprites = descriptor_seed;
    }
    do {
        register s32 unit asm("r5");
        s32 row6;
        register s32 row24 asm("r6");
        s32 next_side;

        unit = 0;
        {
            register s32 row_work asm("r0");
            register s32 next_seed asm("r1");

            next_seed = side;
            row_work = next_seed << 1;
            next_seed += 1;
            next_side = next_seed;
            row_work += side;
            row6 = row_work << 1;
            row24 = row_work << 3;
        }
        do {
            register s32 index asm("r2") = unit + row6;
            register u8 *state_ptr asm("r3");
            u8 state;

            {
                register u8 *state_view asm("r4") = motion_states;

                state_ptr = (u8 *)((u32)index + (u32)state_view);
            }
            state = *state_ptr;

            if (state != 0)
                goto nonzero_state;
            goto state_done;
nonzero_state:
            if (state == BATTLE_SPRITE_MOTION_RETURN_TO_FORMATION)
                goto state_one;
            if (state == BATTLE_SPRITE_MOTION_MOVE_OUTWARD)
                goto state_two;
            goto state_done;
state_one:
            {
                register u32 col4 asm("r2");

                {
                    register s32 target1_work asm("r0");
                    register s32 *descriptor1 asm("r1");

                    target1_work = (u32)(world_positions + index * 3);
                    col4 = unit << 2;
                    descriptor1 = (s32 *)(col4 + row24);
                    descriptor1 = (s32 *)((u32)descriptor1 + (u32)unit_sprites);
                    descriptor1 = *(s32 **)descriptor1;
                    target1_work = *(s32 *)(u32)target1_work;
                    {
                        register s32 field asm("r3") = BATTLE_SPRITE_FIELD(descriptor1, s32, user_data.position.x);
                        register s32 delta asm("r4");

                        if (target1_work > field) {
                            delta = 0x1000;
                            goto update_first_field;
                        } else if (target1_work < field) {
                            delta = -0x1000;
                            goto update_first_field;
                        }
                        goto first_field_done;
update_first_field:
                        {
                            register s32 adjusted asm("r0") = field + delta;

                            BATTLE_SPRITE_FIELD(descriptor1, s32, user_data.position.x) = adjusted;
                        }
first_field_done:
                    }
                }

                asm volatile("" : "+r"(row6));
                {
                    register u32 target2_address asm("r1");

                    {
                        register u32 target2_index asm("r0") = row6 + unit;

                        target2_address = target2_index << 1;
                        target2_address += target2_index;
                        target2_address <<= 2;
                    }
                    {
                        register s32 *target2_base asm("r0") = gBattleUnitWorldPositionsZColumn[0];

                        target2_address += (u32)target2_base;
                    }
                    {
                        s32 **slot2;
                        register s32 *descriptor2 asm("r3");
                        register s32 target2_work asm("r1");
                        register s32 field2 asm("r0");

                        slot2 = (s32 **)(col4 + row24);
                        slot2 = (s32 **)((u32)slot2 + (u32)unit_sprites);
                        descriptor2 = *slot2;
                        target2_work = *(s32 *)target2_address;
                        field2 = BATTLE_SPRITE_FIELD(descriptor2, s32, user_data.position.z);
                        if (target2_work > field2) {
                            register s32 delta2 asm("r1") = 0xA00;

                            field2 += delta2;
                            goto write_second_field;
                        } else if (target2_work < field2) {
                            register s32 delta2 asm("r4") = -0xA00;

                            field2 += delta2;
                            goto write_second_field;
                        }
                        goto second_field_done;
write_second_field:
                        BATTLE_SPRITE_FIELD(descriptor2, s32, user_data.position.z) = field2;
second_field_done:
                    }
                }

                asm volatile("" : "+r"(col4));

                {
                    register s32 completion_index asm("r4") = row6 + unit;
                    register u32 completion_work asm("r0");
                    register u32 completion_offset asm("r3");
                    register s32 *completion_target asm("r1");
                    register s32 **completion_slot asm("r0");
                    register s32 *completion_descriptor asm("r2");

                    completion_work = completion_index << 1;
                    completion_work += completion_index;
                    completion_offset = completion_work << 2;
                    completion_target = (s32 *)(completion_offset + (u32)world_positions);
                    completion_slot = (s32 **)(col4 + row24);
                    completion_slot = (s32 **)((u32)completion_slot + (u32)unit_sprites);
                    completion_descriptor = *completion_slot;

                    if (completion_target[0] == BATTLE_SPRITE_FIELD(completion_descriptor, s32, user_data.position.x)) {
                        register s32 *completion_alt_base asm("r1") = gBattleUnitWorldPositionsZColumn[0];
                        register s32 *completion_alt asm("r0") = (s32 *)(completion_offset + (u32)completion_alt_base);
                        register s32 completion_alt_value asm("r1");

                        asm volatile("" : "+r"(completion_offset));
                        completion_alt_value = *completion_alt;
                        if (completion_alt_value == BATTLE_SPRITE_FIELD(completion_descriptor, s32, user_data.position.z)) {
                            register u8 *clear_base asm("r2") = motion_states;
                            register u32 clear_ptr asm("r1") = completion_index;

                            clear_ptr += (u32)clear_base;
                            {
                                register s32 zero asm("r0") = 0;

                                *(u8 *)clear_ptr = (u8)zero;
                            }
                        }
                    }
                }
            }
                goto state_done;
state_two:
                if (side == 0) {
                    register s32 **slot0 asm("r0");
                    register s32 *descriptor0 asm("r0");
                    register s32 field0 asm("r1");
                    register s32 delta0 asm("r2");
                    register s32 *target0_ptr asm("r0");
                    register s32 target0 asm("r0");
                    register s32 limit0 asm("r4");

                    slot0 = (s32 **)(unit * 4);
                    slot0 = (s32 **)((u32)slot0 + (u32)unit_sprites);
                    descriptor0 = *slot0;
                    field0 = BATTLE_SPRITE_FIELD(descriptor0, s32, user_data.position.x);
                    delta0 = 0x1000;
                    field0 += delta0;
                    BATTLE_SPRITE_FIELD(descriptor0, s32, user_data.position.x) = field0;
                    target0_ptr = world_positions + unit * 3;
                    target0 = *target0_ptr;
                    limit0 = 0x10000;
                    target0 += limit0;
                    if (field0 == target0) {
                        register u8 *state_view asm("r1") = motion_states;
                        register u32 clear_ptr asm("r0") = unit;

                        clear_ptr += (u32)state_view;
                        {
                            register s32 clear_value asm("r2") = side;

                            *(u8 *)clear_ptr = (u8)clear_value;
                        }
                    }
                } else {
                    register s32 **slot1 asm("r0");
                    register s32 *descriptor1 asm("r0");
                    register s32 field1 asm("r1");
                    register s32 delta1 asm("r4");
                    register s32 *target1_ptr asm("r0");
                    register s32 target1_work asm("r0");
                    register s32 limit1 asm("r2");

                    slot1 = (s32 **)(unit * 4 + row24);
                    slot1 = (s32 **)((u32)slot1 + (u32)unit_sprites);
                    descriptor1 = *slot1;
                    field1 = BATTLE_SPRITE_FIELD(descriptor1, s32, user_data.position.x);
                    delta1 = -0x1000;
                    field1 += delta1;
                    BATTLE_SPRITE_FIELD(descriptor1, s32, user_data.position.x) = field1;
                    target1_ptr = world_positions + index * 3;
                    target1_work = *target1_ptr;
                    limit1 = -0x10000;
                    target1_work += limit1;
                    if (field1 == target1_work) {
                        register s32 zero asm("r0") = 0;

                        *state_ptr = (u8)zero;
                    }
                }
state_done:
            {
                register u32 next_col asm("r0") = unit + 1;

                next_col <<= 24;
                unit = next_col >> 24;
            }
        } while ((u32)unit <= 5);
        side = (u8)next_side;
    } while ((u32)side <= 1);
}
