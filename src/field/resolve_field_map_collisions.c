#include "field_actor.h"

extern u16 gCurrentMapId asm("D_0202ECF4");

u16 GetPackedFieldMapCellCollision(u8, s32, s32) asm("func_080AB224");
u16 GetFieldMapCellCollisionMask(u8, u16, s32, s32) asm("func_080AB268");
s32 UsesUnshiftedFieldActorCollisionOrigin(struct FieldActor *) asm("func_080AC098");

void ResolveFieldMapCollisions(struct FieldActor *actor) asm("func_080AB2EC");

void ResolveFieldMapCollisions(struct FieldActor *actor) {
    volatile s32 probe_x_fixed8;
    volatile s32 probe_y_fixed8;
    volatile s32 resolved_x_velocity_fixed8;
    volatile s32 resolved_y_velocity_fixed8;
    volatile s32 negative_side_collision;
    volatile s32 center_collision;
    volatile s32 positive_side_collision;
    register s32 temp_r0 asm("r0");
    register s32 temp_r0_11 asm("r0");
    register s32 temp_r0_16 asm("r0");
    register s32 temp_r0_6 asm("r0");
    s32 initial_x_velocity_fixed8;
    u16 map_id;
    register s32 temp_r1 asm("r9");
    register s32 temp_r2 asm("r10");
    register s32 temp_r2_2 asm("r10");
    register s32 temp_r4 asm("r4");
    register s32 temp_r5 asm("r5");
    s32 temp_r7_2;
    s32 var_r0;
    register s32 var_r0_10 asm("r0");
    register s32 var_r0_11 asm("r0");
    s32 var_r0_12;
    register s32 var_r0_13 asm("r0");
    s32 var_r0_14;
    register s32 var_r0_15 asm("r0");
    register s32 var_r0_2 asm("r0");
    s32 var_r0_3;
    s32 var_r0_4;
    register s32 var_r0_5 asm("r0");
    register s32 var_r0_6 asm("r0");
    s32 var_r0_7;
    register s32 var_r0_8 asm("r0");
    s32 var_r0_9;
    register s32 var_r1 asm("r1");
    register s32 var_r1_2 asm("r1");
    register s32 var_r1_3 asm("r1");
    register s32 var_r1_4 asm("r1");
    s32 var_r1_5;
    s32 var_r1_6;
    register s32 var_r1_7 asm("r1");
    register s32 var_r1_8 asm("r1");
    s32 var_r2;
    s32 var_r2_10;
    s32 var_r2_11;
    s32 var_r2_2;
    s32 var_r2_3;
    s32 var_r2_4;
    s32 var_r2_5;
    s32 var_r2_6;
    register s32 var_r2_7 asm("r2");
    register s32 var_r2_8 asm("r2");
    register s32 var_r2_9 asm("r2");
    register s32 var_r3 asm("r3");
    s32 var_r3_2;
    s32 var_r3_3;
    s32 var_r3_4;
    s32 var_r3_5;
    register s32 var_r3_6 asm("r3");
    s32 var_r3_7;
    s32 var_r3_8;
    s32 var_r3_9;
    s32 var_r4;
    s32 var_r4_2;
    register s32 var_sl asm("r10");
    s32 var_sl_2;
    u32 temp_r1_3;
    register u32 unpacked_x_remainder asm("r8");
    register u32 unpacked_x_remainder_check asm("r1");
    u32 temp_r7;
    u32 temp_r7_3;
    u8 temp_r0_10;
    u8 temp_r0_12;
    u8 temp_r0_13;
    u8 temp_r0_14;
    u8 temp_r0_15;
    u8 temp_r0_17;
    u8 temp_r0_18;
    u8 temp_r0_19;
    u8 temp_r0_20;
    u8 temp_r0_2;
    u8 temp_r0_3;
    u8 temp_r0_4;
    u8 temp_r0_5;
    u8 temp_r0_7;
    u8 temp_r0_8;
    u8 temp_r0_9;

    if (((u32) (u16) (actor->behavior - FIELD_ACTOR_SCRIPTED_CAMERA_FOLLOW) > 1U) && !(actor->flags & FIELD_ACTOR_COLLISION_DISABLED)) {
        resolved_x_velocity_fixed8 = actor->velocity_x_fixed8;
        resolved_y_velocity_fixed8 = actor->velocity_y_fixed8;
        map_id = gCurrentMapId;
        initial_x_velocity_fixed8 = resolved_x_velocity_fixed8;
        if (map_id == FIELD_MAP_PACKED_CELLS) {
            if (initial_x_velocity_fixed8 == 0) {
                goto block_74;
            }
            temp_r0_11 = actor->world_x_fixed8;
            asm volatile("" : "+r"(temp_r0_11));
            if (initial_x_velocity_fixed8 > 0) {
                register s32 adjusted asm("r2");
                adjusted = 0x800;
                asm volatile("" : "+r"(adjusted));
                adjusted = temp_r0_11 + adjusted;
                probe_x_fixed8 = adjusted;
                asm volatile("" ::: "memory");
            } else {
                register s32 adjusted asm("r4");
                adjusted = 0xFFFFF800;
                asm volatile("" : "+r"(adjusted));
                adjusted = temp_r0_11 + adjusted;
                probe_x_fixed8 = adjusted;
            }
            probe_y_fixed8 = actor->world_y_fixed8;
            {
                register s32 base asm("r1");
                base = probe_x_fixed8;
                asm volatile("" : "+r"(base));
                temp_r5 = base + initial_x_velocity_fixed8;
            }
            var_r1_3 = temp_r5;
            asm volatile("" : "+r"(var_r1_3));
            if (temp_r5 < 0) {
                register s32 rounding asm("r2");
                rounding = 0xFFF;
                asm volatile("" : "+r"(rounding));
                var_r1_3 = temp_r5 + rounding;
            }
            temp_r1 = var_r1_3 >> 0xC;
            var_r2_8 = probe_y_fixed8;
            if (var_r2_8 < 0) {
                register s32 rounding asm("r3");
                rounding = 0xFFF;
                asm volatile("" : "+r"(rounding));
                var_r2_8 += rounding;
            }
            temp_r2 = var_r2_8 >> 0xC;
            negative_side_collision = (s32) GetPackedFieldMapCellCollision(actor->model_id, temp_r1, temp_r2 - 1);
            center_collision = (s32) GetPackedFieldMapCellCollision(actor->model_id, temp_r1, temp_r2);
            positive_side_collision = (s32) GetPackedFieldMapCellCollision(actor->model_id, temp_r1, temp_r2 + 1);
            {
                register s32 mask asm("r0");
                temp_r4 = 0xFFF;
                asm volatile("" : "+r"(temp_r4));
                mask = temp_r4;
                asm volatile("" : "+r"(mask));
                temp_r7_3 = probe_y_fixed8 & mask;
            }
            if (((temp_r7_3 <= 0x7FFU) && ((center_collision != 0) || (({ register s32 query asm("r1") = negative_side_collision; asm volatile("" : "+r"(query)); query; }) != 0))) || (({ register s32 query asm("r2") = center_collision; asm volatile("" : "+r"(query)); query; }) != 0) || ((temp_r7_3 > 0x800U) && (({ register s32 query asm("r3") = positive_side_collision; asm volatile("" : "+r"(query)); query; }) != 0))) {
                if ((s32) actor->velocity_x_fixed8 > 0) {
                    var_r0_7 = temp_r5;
                    if (temp_r5 < 0) {
                        register s32 rounding asm("r4");
                        rounding = 0xFFF;
                        asm volatile("" : "+r"(rounding));
                        var_r0_7 = temp_r5 + rounding;
                    }
                    var_r0_8 = (var_r0_7 >> 0xC) << 0xC;
                    var_r1_4 = probe_x_fixed8;
                } else {
                    var_r0_9 = temp_r5;
                    if (temp_r5 < 0) {
                        register s32 rounding asm("r2");
                        rounding = 0xFFF;
                        asm volatile("" : "+r"(rounding));
                        var_r0_9 = temp_r5 + rounding;
                    }
                    var_r0_8 = (var_r0_9 >> 0xC) << 0xC;
                    {
                        register s32 base asm("r3");
                        register s32 offset asm("r4");
                        base = probe_x_fixed8;
                        offset = 0xFFFFF000;
                        asm volatile("" : "+r"(base), "+r"(offset));
                        var_r1_4 = base + offset;
                    }
                }
                asm volatile("" : "+r"(var_r1_4));
                resolved_x_velocity_fixed8 = var_r0_8 - var_r1_4;
            }
            if ((s32) actor->velocity_y_fixed8 <= 0) {
                if (temp_r7_3 <= 0x800U) {
                    if ((negative_side_collision == 0) && (({ register s32 query asm("r1") = center_collision; asm volatile("" : "+r"(query)); query; }) != 0)) {
                        u8 model_id;
                        s32 delta;
                        model_id = actor->model_id;
                        delta = actor->velocity_x_fixed8;
                        var_r1_5 = temp_r1 + 1;
                        if (delta > 0) {
                            var_r1_5 -= 2;
                        }
                        if ((GetPackedFieldMapCellCollision(model_id, var_r1_5, temp_r2 - 1) << 0x10) == 0) {
                            temp_r0_12 = actor->movement_mode;
                            if (temp_r0_12 == 2) {
                                register s32 correction asm("r2");
                                correction = -0xB5;
                                asm volatile("" : "+r"(correction));
                                resolved_y_velocity_fixed8 = correction;
                            } else if (temp_r0_12 == 3) {
                                register s32 correction asm("r3");
                                correction = -0x16A;
                                asm volatile("" : "+r"(correction));
                                resolved_y_velocity_fixed8 = correction;
                                asm volatile("");
                            }
                        } else {
                            goto block_39;
                        }
                    } else {
block_39:
                        if (temp_r7_3 > 0x800U) {
                            goto block_40;
                        }
                    }
                } else {
block_40:
                    if ((({ register s32 query asm("r4") = center_collision; asm volatile("" : "+r"(query)); query; }) == 0) && (positive_side_collision != 0)) {
                        temp_r0_13 = actor->movement_mode;
                        if (temp_r0_13 == 2) {
                            var_r0_10 = 0x7FF;
                            resolved_y_velocity_fixed8 = -0xB5;
                            if (temp_r7_3 > (u32)var_r0_10) {
                                var_r0_10 = probe_y_fixed8 - 0xB5;
                                goto block_49;
                            }
                        } else if (temp_r0_13 == 3) {
                            register s32 offset asm("r3");
                            var_r0_10 = 0x7FF;
                            offset = -0x16A;
                            asm volatile("" : "+r"(offset));
                            resolved_y_velocity_fixed8 = offset;
                            if (temp_r7_3 > (u32)var_r0_10) {
                                register s32 base asm("r4");
                                base = probe_y_fixed8;
                                asm volatile("" : "+r"(base));
                                var_r0_10 = base + offset;
block_49:
                                {
                                    register s32 masked asm("r0");
                                    register s32 threshold asm("r1");
                                    masked = var_r0_10;
                                    threshold = 0xFFF;
                                    asm volatile("" : "+r"(masked), "+r"(threshold));
                                    masked &= threshold;
                                    threshold = 0x800;
                                    asm volatile("" : "+r"(threshold));
                                    if (masked <= threshold) {
                                        threshold -= temp_r7_3;
                                        resolved_y_velocity_fixed8 = threshold;
                                    }
                                }
                            }
                        }
                    }
                }
                if (actor->velocity_y_fixed8 >= 0) {
                    goto block_52;
                }
                goto block_75;
            }
block_52:
            if (temp_r7_3 > 0x7FFU) {
                if ((positive_side_collision == 0) && (({ register s32 query asm("r1") = center_collision; asm volatile("" : "+r"(query)); query; }) != 0)) {
                    u8 model_id;
                    s32 delta;
                    model_id = actor->model_id;
                    delta = actor->velocity_x_fixed8;
                    var_r1_6 = temp_r1 + 1;
                    if (delta > 0) {
                        var_r1_6 -= 2;
                    }
                    if ((GetPackedFieldMapCellCollision(model_id, var_r1_6, temp_r2 + 1) << 0x10) == 0) {
                        temp_r0_14 = actor->movement_mode;
                        if (temp_r0_14 == 2) {
                            register s32 correction asm("r2");
                            correction = 0xB5;
                            asm volatile("" : "+r"(correction));
                            resolved_y_velocity_fixed8 = correction;
                        } else if (temp_r0_14 == 3) {
                            var_r3_9 = 0x16A;
                            goto block_73;
                        }
                    } else {
                        goto block_62;
                    }
                } else {
block_62:
                    if (temp_r7_3 <= 0x7FFU) {
                        goto block_63;
                    }
                }
            } else {
block_63:
                if ((({ register s32 query asm("r4") = center_collision; asm volatile("" : "+r"(query)); query; }) == 0) && (negative_side_collision != 0)) {
                    temp_r0_15 = actor->movement_mode;
                    if (temp_r0_15 == 2) {
                        var_r3_9 = 0x800;
                        {
                            register s32 correction asm("r1");
                            correction = 0xB5;
                            asm volatile("" : "+r"(correction));
                            resolved_y_velocity_fixed8 = correction;
                        }
                        if (temp_r7_3 <= (u32)var_r3_9) {
                            var_r0_11 = probe_y_fixed8 + 0xB5;
                            goto block_71;
                        }
                    } else if (temp_r0_15 == 3) {
                        var_r3_9 = 0x800;
                        temp_r4 = 0x16A;
                        resolved_y_velocity_fixed8 = temp_r4;
                        if (temp_r7_3 <= (u32)var_r3_9) {
                            register s32 base asm("r1");
                            base = probe_y_fixed8;
                            asm volatile("" : "+r"(base));
                            var_r0_11 = base + temp_r4;
block_71:
                            {
                                register s32 masked asm("r0");
                                register s32 threshold asm("r1");
                                masked = var_r0_11;
                                threshold = 0xFFF;
                                asm volatile("" : "+r"(masked), "+r"(threshold));
                                masked &= threshold;
                                threshold = 0x7FF;
                                asm volatile("" : "+r"(threshold));
                                if (masked > threshold) {
                                var_r3_9 -= temp_r7_3;
block_73:
                                resolved_y_velocity_fixed8 = var_r3_9;
                                }
                            }
                        }
                    }
                }
            }
block_74:
            if (actor->velocity_y_fixed8 != 0) {
block_75:
                {
                    register s32 base asm("r0");
                    register s32 delta asm("r3");
                    base = actor->world_x_fixed8;
                    asm volatile("" : "+r"(base));
                    delta = resolved_x_velocity_fixed8;
                    asm volatile("" : "+r"(delta));
                    base += delta;
                    probe_x_fixed8 = base;
                }
                temp_r0_16 = actor->world_y_fixed8;
                asm volatile("" : "+r"(temp_r0_16));
                if (actor->velocity_y_fixed8 > 0) {
                    register s32 adjusted asm("r4");
                    adjusted = 0x800;
                    asm volatile("" : "+r"(adjusted));
                    adjusted = temp_r0_16 + adjusted;
                    probe_y_fixed8 = adjusted;
                    asm volatile("" ::: "memory");
                } else {
                    register s32 adjusted asm("r1");
                    adjusted = 0xFFFFF800;
                    asm volatile("" : "+r"(adjusted));
                    adjusted = temp_r0_16 + adjusted;
                    probe_y_fixed8 = adjusted;
                }
                {
                    register s32 base asm("r3");
                    base = probe_y_fixed8;
                    asm volatile("" : "+r"(base));
                    temp_r7_2 = base + actor->velocity_y_fixed8;
                }
                var_r1_7 = probe_x_fixed8;
                asm volatile("" : "+r"(var_r1_7));
                if (var_r1_7 < 0) {
                    register s32 rounding asm("r4");
                    rounding = 0xFFF;
                    asm volatile("" : "+r"(rounding));
                    var_r1_7 += rounding;
                }
                temp_r1 = var_r1_7 >> 0xC;
                var_r2_9 = temp_r7_2;
                if (temp_r7_2 < 0) {
                    register s32 rounding asm("r0");
                    rounding = 0xFFF;
                    asm volatile("" : "+r"(rounding));
                    var_r2_9 = temp_r7_2 + rounding;
                }
                {
                    u8 model_id;
                    temp_r2_2 = var_r2_9 >> 0xC;
                    model_id = actor->model_id;
                    temp_r5 = temp_r1 - 1;
                    negative_side_collision = (s32) GetPackedFieldMapCellCollision(model_id, temp_r5, temp_r2_2);
                }
                center_collision = (s32) GetPackedFieldMapCellCollision(actor->model_id, temp_r1, temp_r2_2);
                {
                    u8 model_id;
                    model_id = actor->model_id;
                    temp_r4 = temp_r1 + 1;
                    positive_side_collision = (s32) GetPackedFieldMapCellCollision(model_id, temp_r4, temp_r2_2);
                }
                {
                    register s32 mask asm("r0");
                    register s32 mask_source asm("r1");
                    register u32 source asm("r2");
                    register u32 remainder asm("r8");
                    register u32 masked asm("r3");
                    mask_source = 0xFFF;
                    asm volatile("" : "+r"(mask_source));
                    mask = mask_source;
                    asm volatile("" : "+r"(mask));
                    source = probe_x_fixed8;
                    asm volatile("" : "+r"(source));
                    remainder = source;
                    asm volatile("" : "+r"(remainder));
                    masked = remainder;
                    asm volatile("" : "+r"(masked));
                    masked &= mask;
                    remainder = masked;
                    asm volatile("" : "+r"(remainder));
                    temp_r1_3 = remainder;
                }
                if (((temp_r1_3 <= 0x7FFU) && (negative_side_collision != 0)) || (({ register s32 query asm("r1") = center_collision; asm volatile("" : "+r"(query)); query; }) != 0) || ((temp_r1_3 > 0x800U) && (({ register s32 query asm("r2") = positive_side_collision; asm volatile("" : "+r"(query)); query; }) != 0))) {
                    if ((s32) actor->velocity_y_fixed8 > 0) {
                        var_r0_12 = temp_r7_2;
                        if (temp_r7_2 < 0) {
                            register s32 rounding asm("r3");
                            rounding = 0xFFF;
                            asm volatile("" : "+r"(rounding));
                            var_r0_12 = temp_r7_2 + rounding;
                        }
                        var_r0_13 = (var_r0_12 >> 0xC) << 0xC;
                        var_r1_8 = probe_y_fixed8;
                    } else {
                        var_r0_14 = temp_r7_2;
                        if (temp_r7_2 < 0) {
                            register s32 rounding asm("r2");
                            rounding = 0xFFF;
                            asm volatile("" : "+r"(rounding));
                            var_r0_14 = temp_r7_2 + rounding;
                        }
                        var_r0_13 = (var_r0_14 >> 0xC) << 0xC;
                        {
                            register s32 base asm("r3");
                            base = probe_y_fixed8;
                            asm volatile("" : "+r"(base));
                            var_r1_8 = base + 0xFFFFF000;
                        }
                    }
                    asm volatile("" : "+r"(var_r1_8));
                    resolved_y_velocity_fixed8 = var_r0_13 - var_r1_8;
                }
                if ((s32) actor->velocity_x_fixed8 <= 0) {
                    if (temp_r1_3 <= 0x800U) {
                        if ((({ register s32 query asm("r3") = negative_side_collision; asm volatile("" : "+r"(query)); query; }) == 0) && (center_collision != 0)) {
                            u8 model_id;
                            s32 delta;
                            model_id = actor->model_id;
                            delta = actor->velocity_y_fixed8;
                            var_r2_10 = temp_r2_2 + 1;
                            if (delta > 0) {
                                var_r2_10 -= 2;
                            }
                            if ((GetPackedFieldMapCellCollision(model_id, temp_r5, var_r2_10) << 0x10) == 0) {
                                temp_r0_17 = actor->movement_mode;
                                if (temp_r0_17 == 2) {
                                    register s32 correction asm("r1");
                                    correction = -0xB5;
                                    asm volatile("" : "+r"(correction));
                                    resolved_x_velocity_fixed8 = correction;
                                } else if (temp_r0_17 == 3) {
                                    register s32 correction asm("r2");
                                    correction = -0x16A;
                                    asm volatile("" : "+r"(correction));
                                    resolved_x_velocity_fixed8 = correction;
                                    asm volatile("");
                                }
                            } else {
                                goto block_108;
                            }
                        } else {
block_108:
                            if (temp_r1_3 > 0x800U) {
                                goto block_109;
                            }
                        }
                    } else {
block_109:
                        if ((center_collision == 0) && (({ register s32 query asm("r1") = positive_side_collision; asm volatile("" : "+r"(query)); query; }) != 0)) {
                            temp_r0_18 = actor->movement_mode;
                            if (temp_r0_18 == 2) {
                                var_r0_15 = 0x7FF;
                                {
                                    register s32 correction asm("r2");
                                    correction = -0xB5;
                                    asm volatile("" : "+r"(correction));
                                    resolved_x_velocity_fixed8 = correction;
                                }
                                if (temp_r1_3 > (u32)var_r0_15) {
                                    var_r0_15 = probe_x_fixed8 - 0xB5;
                                    goto block_118;
                                }
                            } else if (temp_r0_18 == 3) {
                                var_r0_15 = 0x7FF;
                                var_r1_8 = -0x16A;
                                resolved_x_velocity_fixed8 = var_r1_8;
                                if (temp_r1_3 > (u32)var_r0_15) {
                                    register s32 base asm("r2");
                                    base = probe_x_fixed8;
                                    asm volatile("" : "+r"(base));
                                    var_r0_15 = base + var_r1_8;
block_118:
                                    {
                                        register s32 masked asm("r0");
                                        register s32 threshold asm("r1");
                                        masked = var_r0_15;
                                        threshold = 0xFFF;
                                        asm volatile("" : "+r"(masked), "+r"(threshold));
                                        masked &= threshold;
                                        threshold = 0x800;
                                        asm volatile("" : "+r"(threshold));
                                        if (masked <= threshold) {
                                            masked = temp_r1_3;
                                            asm volatile("" : "+r"(masked));
                                            masked = threshold - masked;
                                            resolved_x_velocity_fixed8 = masked;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (actor->velocity_x_fixed8 < 0) {

                    } else {
                        goto block_122;
                    }
                } else {
block_122:
                    if (temp_r1_3 > 0x7FFU) {
                        if ((({ register s32 query asm("r1") = positive_side_collision; asm volatile("" : "+r"(query)); query; }) == 0) && (({ register s32 query asm("r2") = center_collision; asm volatile("" : "+r"(query)); query; }) != 0)) {
                            u8 model_id;
                            s32 delta;
                            model_id = actor->model_id;
                            delta = actor->velocity_y_fixed8;
                            var_r2_11 = temp_r2_2 + 1;
                            if (delta > 0) {
                                var_r2_11 -= 2;
                            }
                            if ((GetPackedFieldMapCellCollision(model_id, temp_r4, var_r2_11) << 0x10) == 0) {
                                temp_r0_19 = actor->movement_mode;
                                if (temp_r0_19 == 2) {
                                    var_r3_6 = 0xB5;
                                    goto block_326;
                                } else if (temp_r0_19 == 3) {
                                    register s32 correction asm("r4");
                                    correction = 0x16A;
                                    asm volatile("" : "+r"(correction));
                                    resolved_x_velocity_fixed8 = correction;
                                    asm volatile("" ::: "memory");
                                }
                            } else {
                                goto block_134;
                            }
                        } else {
block_134:
                            if (temp_r1_3 > 0x7FFU) {

                            } else {
                                goto block_136;
                            }
                        }
                    } else {
block_136:
                        if (center_collision != 0) {

                        } else if (({ register s32 query asm("r1") = negative_side_collision; asm volatile("" : "+r"(query)); query; }) == 0) {

                        } else {
                            temp_r0_20 = actor->movement_mode;
                            if (temp_r0_20 == 2) {
                                var_r2_11 = 0x800;
                                {
                                    register s32 correction asm("r3");
                                    correction = 0xB5;
                                    asm volatile("" : "+r"(correction));
                                    resolved_x_velocity_fixed8 = correction;
                                }
                                if (temp_r1_3 > (u32)var_r2_11) {

                                } else if ((s32) ((probe_x_fixed8 + 0xB5) & 0xFFF) <= 0x7FF) {

                                } else {
                                    register s32 correction asm("r4");
                                    correction = temp_r1_3;
                                    asm volatile("" : "+r"(correction));
                                    correction = var_r2_11 - correction;
                                    resolved_x_velocity_fixed8 = correction;
                                    asm volatile("" ::: "memory");
                                }
                            } else if (temp_r0_20 == 3) {
                                var_r2_11 = 0x800;
                                var_r0_15 = 0x16A;
                                resolved_x_velocity_fixed8 = var_r0_15;
                                if (temp_r1_3 > (u32)var_r2_11) {

                                } else if ((s32) ({
                                    register s32 aux asm("r1");
                                    aux = probe_x_fixed8;
                                    asm volatile("" : "+r"(aux));
                                    var_r0_15 = aux + var_r0_15;
                                    asm volatile("" : "+r"(var_r0_15));
                                    aux = 0xFFF;
                                    asm volatile("" : "+r"(aux));
                                    var_r0_15 &= aux;
                                    asm volatile("" : "+r"(var_r0_15));
                                    var_r0_15;
                                }) <= 0x7FF) {

                                } else {
                                    register s32 correction asm("r3");
                                    correction = temp_r1_3;
                                    asm volatile("" : "+r"(correction));
                                    correction = var_r2_11 - correction;
                                    var_r3_6 = correction;
                                    goto block_326;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            register s32 mode_delta asm("r4");
            mode_delta = resolved_x_velocity_fixed8;
            asm volatile("" : "+r"(mode_delta));
            if (mode_delta == 0) {
                goto block_240;
            }
            temp_r0 = actor->world_x_fixed8;
            asm volatile("" : "+r"(temp_r0));
            if (mode_delta > 0) {
                register s32 adjusted asm("r1");
                adjusted = 0x800;
                asm volatile("" : "+r"(adjusted));
                adjusted = temp_r0 + adjusted;
                probe_x_fixed8 = adjusted;
                asm volatile("" ::: "memory");
            } else {
                register s32 adjusted asm("r2");
                adjusted = 0xFFFFF800;
                asm volatile("" : "+r"(adjusted));
                adjusted = temp_r0 + adjusted;
                probe_x_fixed8 = adjusted;
            }
            probe_y_fixed8 = ({ register s32 value asm("r4") = actor->world_y_fixed8; value; });
            {
                register s32 base asm("r0");
                base = probe_x_fixed8;
                asm volatile("" : "+r"(base));
                temp_r5 = base + initial_x_velocity_fixed8;
            }
            var_r1 = temp_r5;
            asm volatile("" : "+r"(var_r1));
            if (temp_r5 < 0) {
                register s32 rounding asm("r2");
                rounding = 0x7FF;
                asm volatile("" : "+r"(rounding));
                var_r1 = temp_r5 + rounding;
            }
            temp_r1 = var_r1 >> 0xB;
            var_r4 = probe_y_fixed8;
            if (var_r4 < 0) {
                register s32 rounding asm("r3");
                rounding = 0x7FF;
                asm volatile("" : "+r"(rounding));
                var_r4 += rounding;
            }
            {
                s32 tile_y;
                s32 adjustment;
                tile_y = var_r4 >> 0xB;
                adjustment = UsesUnshiftedFieldActorCollisionOrigin(actor);
                adjustment <<= 0x18;
                var_sl = tile_y;
                if (adjustment == 0) {
                    register s32 one asm("r4");
                    one = 1;
                    asm volatile("" : "+r"(one));
                    var_sl += one;
                }
            }
            negative_side_collision = (s32) GetFieldMapCellCollisionMask(actor->model_id, actor->behavior, temp_r1, var_sl - 1);
            center_collision = (s32) GetFieldMapCellCollisionMask(actor->model_id, actor->behavior, temp_r1, var_sl);
            {
                u8 model_id;
                u16 behavior;
                model_id = actor->model_id;
                behavior = actor->behavior;
                temp_r4 = var_sl + 1;
                positive_side_collision = (s32) GetFieldMapCellCollisionMask(model_id, behavior, temp_r1, temp_r4);
            }
            {
                register s32 mask asm("r0");
                register s32 mask_source asm("r1");
                mask_source = 0x7FF;
                asm volatile("" : "+r"(mask_source));
                mask = mask_source;
                asm volatile("" : "+r"(mask));
                temp_r7 = probe_y_fixed8 & mask;
            }
            if ((({ register s32 query asm("r2") = negative_side_collision; asm volatile("" : "+r"(query)); query; }) != 0) || (({ register s32 query asm("r3") = center_collision; asm volatile("" : "+r"(query)); query; }) != 0) || ((temp_r7 != 0) && (positive_side_collision != 0))) {
                if ((s32) actor->velocity_x_fixed8 > 0) {
                    var_r0 = temp_r5;
                    if (temp_r5 < 0) {
                        var_r0 = temp_r5 + 0x7FF;
                    }
                    var_r0_2 = ((var_r0 >> 0xB) << 0xB) - ({ register s32 base asm("r2") = probe_x_fixed8; asm volatile("" : "+r"(base)); base; });
                    asm volatile("" : "+r"(var_r0_2));
                } else {
                    s32 base;
                    var_r0_3 = temp_r5;
                    if (temp_r5 < 0) {
                        register s32 rounding asm("r3");
                        rounding = 0x7FF;
                        asm volatile("" : "+r"(rounding));
                        var_r0_3 = temp_r5 + rounding;
                    }
                    var_r0_2 = (var_r0_3 >> 0xB) << 0xB;
                    asm volatile("" : "+r"(var_r0_2));
                    {
                        register s32 fixed_base asm("r1");
                        register s32 source asm("r2");
                        register s32 offset asm("r3");
                        source = probe_x_fixed8;
                        offset = 0xFFFFF800;
                        asm volatile("" : "+r"(source), "+r"(offset));
                        fixed_base = source + offset;
                        asm volatile("" : "+r"(fixed_base));
                        base = fixed_base;
                    }
                    var_r0_2 -= base;
                }
                resolved_x_velocity_fixed8 = var_r0_2;
            }
            if ((s32) actor->velocity_y_fixed8 <= 0) {
                if (temp_r7 != 0) {
                    if (negative_side_collision == 0) {
                        if ((({ register s32 query asm("r1") = center_collision; query; }) == 0) && (({ register s32 query asm("r2") = positive_side_collision; query; }) != 0)) {
                            temp_r0_2 = actor->movement_mode;
                            if (temp_r0_2 == 2) {
                                register s32 threshold asm("r0");
                                register s32 correction asm("r3");
                                threshold = 0x3FF;
                                correction = -0xB5;
                                resolved_y_velocity_fixed8 = correction;
                                if ((temp_r7 <= (u32)threshold) && ((s32) ((probe_y_fixed8 - 0xB5) & 0x7FF) > 0x400)) {
                                    resolved_y_velocity_fixed8 = 0 - temp_r7;
                                    asm volatile("" ::: "memory");
                                }
                            } else if (temp_r0_2 == 3) {
                                s32 threshold;
                                register s32 offset asm("r1");
                                threshold = 0x3FF;
                                offset = -0x16A;
                                resolved_y_velocity_fixed8 = offset;
                                if ((temp_r7 <= (u32)threshold) && ((s32) ({
                                    register s32 value asm("r0");
                                    register s32 base asm("r2");
                                    base = probe_y_fixed8;
                                    value = base + offset;
                                    offset = 0x7FF;
                                    value &= offset;
                                    value;
                                }) > 0x400)) {
                                    var_r3 = 0 - temp_r7;
                                    goto block_205;
                                }
                            }
                        } else {
                            goto block_192;
                        }
                    }
                } else {
block_192:
                    if ((negative_side_collision == 0) && (({ register s32 query asm("r1") = center_collision; query; }) != 0)) {
                        u8 model_id;
                        u16 behavior;
                        model_id = actor->model_id;
                        behavior = actor->behavior;
                        temp_r5 = var_sl - 2;
                        if ((GetFieldMapCellCollisionMask(model_id, behavior, temp_r1, temp_r5) << 0x10) == 0) {
                            u8 second_model_id;
                            u16 second_behavior;
                            s32 delta2;
                            second_model_id = actor->model_id;
                            second_behavior = actor->behavior;
                            delta2 = actor->velocity_x_fixed8;
                            var_r2 = temp_r1 + 1;
                            if (delta2 > 0) {
                                var_r2 -= 2;
                            }
                            if ((GetFieldMapCellCollisionMask(second_model_id, second_behavior, var_r2, temp_r5) << 0x10) == 0) {
                                u8 third_model_id;
                                u16 third_behavior;
                                s32 delta3;
                                third_model_id = actor->model_id;
                                third_behavior = actor->behavior;
                                delta3 = actor->velocity_x_fixed8;
                                var_r2_2 = temp_r1 + 2;
                                if (delta3 > 0) {
                                    var_r2_2 -= 4;
                                }
                                if ((GetFieldMapCellCollisionMask(third_model_id, third_behavior, var_r2_2, temp_r5) << 0x10) == 0) {
                                    temp_r0_3 = actor->movement_mode;
                                    if (temp_r0_3 == 2) {
                                        register s32 correction asm("r2");
                                        correction = -0xB5;
                                        resolved_y_velocity_fixed8 = correction;
                                    } else if (temp_r0_3 == 3) {
                                        var_r3 = -0x16A;
block_205:
                                        resolved_y_velocity_fixed8 = var_r3;
                                    }
                                }
                            }
                        }
                    }
                }
                if ((s32) actor->velocity_y_fixed8 < 0) {
                    goto block_242;
                }
                goto block_208;
            }
block_208:
            if ((negative_side_collision != 0) && (({ register s32 query asm("r1") = center_collision; asm volatile("" : "+r"(query)); query; }) == 0) && (({ register s32 query asm("r2") = positive_side_collision; asm volatile("" : "+r"(query)); query; }) == 0)) {
                u8 model_id;
                u16 behavior;
                s32 delta;
                register s32 call_y asm("r3");
                model_id = actor->model_id;
                behavior = actor->behavior;
                delta = actor->velocity_x_fixed8;
                var_r2_3 = temp_r1 + 1;
                if (delta > 0) {
                    var_r2_3 -= 2;
                }
                call_y = temp_r4;
                if ((GetFieldMapCellCollisionMask(model_id, behavior, var_r2_3, call_y) << 0x10) == 0) {
                    u8 second_model_id;
                    u16 second_behavior;
                    s32 delta2;
                    second_model_id = actor->model_id;
                    second_behavior = actor->behavior;
                    delta2 = actor->velocity_x_fixed8;
                    var_r2_4 = temp_r1 + 2;
                    if (delta2 > 0) {
                        var_r2_4 -= 4;
                    }
                    if ((GetFieldMapCellCollisionMask(second_model_id, second_behavior, var_r2_4, temp_r4) << 0x10) == 0) {
                        temp_r0_4 = actor->movement_mode;
                        if (temp_r0_4 == 2) {
                            if ((temp_r7 <= 0x400U) || ((s32) ((probe_y_fixed8 + 0xB5) & 0x7FF) > 0x3FF)) {
                                register s32 correction asm("r3");
                                correction = 0xB5;
                                resolved_y_velocity_fixed8 = correction;
                                asm volatile("" : "+r"(correction) : : "memory");
                            } else {
                                goto block_224;
                            }
                        } else if (temp_r0_4 == 3) {
                            if ((temp_r7 > 0x400U) && ((s32) ({
                                register s32 value asm("r0");
                                register s32 base asm("r4");
                                register s32 offset asm("r1");
                                base = probe_y_fixed8;
                                offset = 0x16A;
                                asm volatile("" : "+r"(base), "+r"(offset));
                                value = base + offset;
                                asm volatile("" : "+r"(value));
                                offset = 0x7FF;
                                asm volatile("" : "+r"(offset));
                                value &= offset;
                                asm volatile("" : "+r"(value));
                                value;
                            }) <= 0x3FF)) {
block_224:
                                resolved_y_velocity_fixed8 = 0x800 - temp_r7;
                                asm volatile("" ::: "memory");
                            } else {
                                register s32 correction asm("r2");
                                correction = 0x16A;
                                resolved_y_velocity_fixed8 = correction;
                                asm volatile("" : "+r"(correction) : : "memory");
                            }
                        }
                    } else {
                        goto block_226;
                    }
                } else {
                    goto block_226;
                }
            } else {
block_226:
                if ((temp_r7 != 0) && (({ register s32 query asm("r3") = center_collision; asm volatile("" : "+r"(query)); query; }) != 0) && (({ register s32 query asm("r4") = positive_side_collision; asm volatile("" : "+r"(query)); query; }) == 0)) {
                    u8 model_id;
                    u16 behavior;
                    model_id = actor->model_id;
                    behavior = actor->behavior;
                    temp_r4 = var_sl + 2;
                    if ((GetFieldMapCellCollisionMask(model_id, behavior, temp_r1, temp_r4) << 0x10) == 0) {
                        u8 second_model_id;
                        u16 second_behavior;
                        s32 delta2;
                        second_model_id = actor->model_id;
                        second_behavior = actor->behavior;
                        delta2 = actor->velocity_x_fixed8;
                        var_r2_5 = temp_r1 + 1;
                        if (delta2 > 0) {
                            var_r2_5 -= 2;
                        }
                        if ((GetFieldMapCellCollisionMask(second_model_id, second_behavior, var_r2_5, temp_r4) << 0x10) == 0) {
                            u8 third_model_id;
                            u16 third_behavior;
                            s32 delta3;
                            third_model_id = actor->model_id;
                            third_behavior = actor->behavior;
                            delta3 = actor->velocity_x_fixed8;
                            var_r2_6 = temp_r1 + 2;
                            if (delta3 > 0) {
                                var_r2_6 -= 4;
                            }
                            if ((GetFieldMapCellCollisionMask(third_model_id, third_behavior, var_r2_6, temp_r4) << 0x10) == 0) {
                                temp_r0_5 = actor->movement_mode;
                                if (temp_r0_5 == 2) {
                                    resolved_y_velocity_fixed8 = 0xB5;
                                    asm volatile("" ::: "memory");
                                } else if (temp_r0_5 == 3) {
                                    register s32 correction asm("r1");
                                    correction = 0x16A;
                                    asm volatile("" : "+r"(correction));
                                    resolved_y_velocity_fixed8 = correction;
                                }
                            }
                        }
                    }
                }
            }
block_240:
            if (actor->velocity_y_fixed8 == 0) {

            } else {
block_242:
                {
                    register s32 base asm("r0");
                    register s32 delta asm("r3");
                    base = actor->world_x_fixed8;
                    asm volatile("" : "+r"(base));
                    delta = resolved_x_velocity_fixed8;
                    asm volatile("" : "+r"(delta));
                    base += delta;
                    probe_x_fixed8 = base;
                }
                temp_r0_6 = actor->world_y_fixed8;
                asm volatile("" : "+r"(temp_r0_6));
                if (actor->velocity_y_fixed8 > 0) {
                    register s32 adjusted asm("r4");
                    adjusted = 0x800;
                    asm volatile("" : "+r"(adjusted));
                    adjusted = temp_r0_6 + adjusted;
                    probe_y_fixed8 = adjusted;
                    asm volatile("" ::: "memory");
                } else {
                    register s32 adjusted asm("r1");
                    adjusted = 0xFFFFF800;
                    asm volatile("" : "+r"(adjusted));
                    adjusted = temp_r0_6 + adjusted;
                    probe_y_fixed8 = adjusted;
                }
                {
                    register s32 base asm("r3");
                    base = probe_y_fixed8;
                    asm volatile("" : "+r"(base));
                    temp_r7_2 = base + actor->velocity_y_fixed8;
                }
                var_r1_2 = probe_x_fixed8;
                asm volatile("" : "+r"(var_r1_2));
                if (var_r1_2 < 0) {
                    register s32 rounding asm("r4");
                    rounding = 0x7FF;
                    asm volatile("" : "+r"(rounding));
                    var_r1_2 += rounding;
                }
                temp_r1 = var_r1_2 >> 0xB;
                var_r4_2 = temp_r7_2;
                if (temp_r7_2 < 0) {
                    register s32 rounding asm("r0");
                    rounding = 0x7FF;
                    asm volatile("" : "+r"(rounding));
                    var_r4_2 = temp_r7_2 + rounding;
                }
                {
                    s32 tile_y;
                    s32 adjustment;
                    tile_y = var_r4_2 >> 0xB;
                    adjustment = UsesUnshiftedFieldActorCollisionOrigin(actor);
                    adjustment <<= 0x18;
                    var_sl_2 = tile_y;
                    if (adjustment == 0) {
                        register s32 one asm("r1");
                        one = 1;
                        asm volatile("" : "+r"(one));
                        var_sl_2 += one;
                    }
                }
                negative_side_collision = (s32) GetFieldMapCellCollisionMask(actor->model_id, actor->behavior, temp_r1 - 1, var_sl_2);
                center_collision = (s32) GetFieldMapCellCollisionMask(actor->model_id, actor->behavior, temp_r1, var_sl_2);
                {
                    u8 model_id;
                    u16 behavior;
                    model_id = actor->model_id;
                    behavior = actor->behavior;
                    temp_r4 = temp_r1 + 1;
                    positive_side_collision = (s32) GetFieldMapCellCollisionMask(model_id, behavior, temp_r4, var_sl_2);
                }
                {
                    register s32 mask asm("r0");
                    register s32 mask_source asm("r2");
                    mask_source = 0x7FF;
                    asm volatile("" : "+r"(mask_source));
                    mask = mask_source;
                    asm volatile("" : "+r"(mask));
                    {
                        register s32 source asm("r3");
                        source = probe_x_fixed8;
                        asm volatile("" : "+r"(source));
                        unpacked_x_remainder = source;
                        asm volatile("" : "+r"(unpacked_x_remainder));
                        unpacked_x_remainder_check = unpacked_x_remainder;
                        asm volatile("" : "+r"(unpacked_x_remainder_check));
                        unpacked_x_remainder_check &= mask;
                        unpacked_x_remainder = unpacked_x_remainder_check;
                        asm volatile("" : "+r"(unpacked_x_remainder));
                    }
                }
                if ((({ register s32 query asm("r2") = negative_side_collision; asm volatile("" : "+r"(query)); query; }) != 0) || (({ register s32 query asm("r3") = center_collision; asm volatile("" : "+r"(query)); query; }) != 0) || ((unpacked_x_remainder_check != 0) && (positive_side_collision != 0))) {
                    if ((s32) actor->velocity_y_fixed8 > 0) {
                        var_r0_4 = temp_r7_2;
                        if (temp_r7_2 < 0) {
                            register s32 rounding asm("r1");
                            rounding = 0x7FF;
                            asm volatile("" : "+r"(rounding));
                            var_r0_4 = temp_r7_2 + rounding;
                        }
                        var_r0_5 = ((var_r0_4 >> 0xB) << 0xB) - ({ register s32 base asm("r2") = probe_y_fixed8; asm volatile("" : "+r"(base)); base; });
                        asm volatile("" : "+r"(var_r0_5));
                    } else {
                        s32 base;
                        var_r0_6 = temp_r7_2;
                        asm volatile("" : "+r"(var_r0_6));
                        if (temp_r7_2 < 0) {
                            register s32 rounding asm("r3");
                            rounding = 0x7FF;
                            asm volatile("" : "+r"(rounding));
                            var_r0_6 = temp_r7_2 + rounding;
                        }
                        var_r0_5 = (var_r0_6 >> 0xB) << 0xB;
                        asm volatile("" : "+r"(var_r0_5));
                        {
                            register s32 fixed_base asm("r1");
                            register s32 source asm("r2");
                            register s32 offset asm("r3");
                            source = probe_y_fixed8;
                            offset = 0xFFFFF800;
                            asm volatile("" : "+r"(source), "+r"(offset));
                            fixed_base = source + offset;
                            asm volatile("" : "+r"(fixed_base));
                            base = fixed_base;
                        }
                        var_r0_5 -= base;
                    }
                    resolved_y_velocity_fixed8 = var_r0_5;
                }
                if ((s32) actor->velocity_x_fixed8 > 0) {
                    goto block_293;
                }
                {
                    register u32 remainder_check asm("r0");
                    remainder_check = unpacked_x_remainder;
                    asm volatile("" : "+r"(remainder_check));
                if (remainder_check != 0) {
                    if (({ register s32 query asm("r1") = negative_side_collision; asm volatile("" : "+r"(query)); query; }) == 0) {
                        if ((({ register s32 query asm("r2") = center_collision; asm volatile("" : "+r"(query)); query; }) == 0) && (({ register s32 query asm("r3") = positive_side_collision; asm volatile("" : "+r"(query)); query; }) != 0)) {
                            temp_r0_7 = actor->movement_mode;
                            if (temp_r0_7 == 2) {
                                register s32 threshold asm("r0");
                                register s32 correction asm("r1");
                                threshold = 0x3FF;
                                asm volatile("" : "+r"(threshold));
                                correction = -0xB5;
                                asm volatile("" : "+r"(correction));
                                resolved_x_velocity_fixed8 = correction;
                                if ((unpacked_x_remainder <= (u32)threshold) && ((s32) ((probe_x_fixed8 - 0xB5) & 0x7FF) > 0x400)) {
                                    var_r2_7 = unpacked_x_remainder;
                                    asm volatile("" : "+r"(var_r2_7));
                                    var_r2_7 = -var_r2_7;
                                    goto block_290;
                                }
                            } else if (temp_r0_7 == 3) {
                                s32 threshold;
                                register s32 offset asm("r3");
                                threshold = 0x3FF;
                                offset = -0x16A;
                                resolved_x_velocity_fixed8 = offset;
                                if ((unpacked_x_remainder <= (u32)threshold) && ((s32) ({
                                    register s32 value asm("r0");
                                    register s32 base asm("r1");
                                    base = probe_x_fixed8;
                                    value = base + offset;
                                    base = 0x7FF;
                                    value &= base;
                                    value;
                                }) > 0x400)) {
                                    var_r2_7 = unpacked_x_remainder;
                                    asm volatile("" : "+r"(var_r2_7));
                                    var_r2_7 = -var_r2_7;
                                    goto block_290;
                                }
                            }
                        } else {
                            goto block_277;
                        }
                    }
                } else {
block_277:
                    if ((({ register s32 query asm("r3") = negative_side_collision; asm volatile("" : "+r"(query)); query; }) == 0) && (center_collision != 0)) {
                        u8 model_id;
                        u16 behavior;
                        model_id = actor->model_id;
                        behavior = actor->behavior;
                        temp_r5 = temp_r1 - 2;
                        if ((GetFieldMapCellCollisionMask(model_id, behavior, temp_r5, var_sl_2) << 0x10) == 0) {
                            u8 second_model_id;
                            u16 second_behavior;
                            s32 delta2;
                            second_model_id = actor->model_id;
                            second_behavior = actor->behavior;
                            delta2 = actor->velocity_y_fixed8;
                            var_r3_2 = var_sl_2 + 1;
                            if (delta2 > 0) {
                                var_r3_2 -= 2;
                            }
                            if ((GetFieldMapCellCollisionMask(second_model_id, second_behavior, temp_r5, var_r3_2) << 0x10) == 0) {
                                u8 third_model_id;
                                u16 third_behavior;
                                s32 delta3;
                                third_model_id = actor->model_id;
                                third_behavior = actor->behavior;
                                delta3 = actor->velocity_y_fixed8;
                                var_r3_3 = var_sl_2 + 2;
                                if (delta3 > 0) {
                                    var_r3_3 -= 4;
                                }
                                if ((GetFieldMapCellCollisionMask(third_model_id, third_behavior, temp_r5, var_r3_3) << 0x10) == 0) {
                                    temp_r0_8 = actor->movement_mode;
                                    if (temp_r0_8 == 2) {
                                        register s32 correction asm("r1");
                                        correction = -0xB5;
                                        resolved_x_velocity_fixed8 = correction;
                                        asm volatile("" : "+r"(correction) : : "memory");
                                    } else if (temp_r0_8 == 3) {
                                        var_r2_7 = -0x16A;
block_290:
                                        asm volatile("" : "+r"(var_r2_7));
                                        resolved_x_velocity_fixed8 = var_r2_7;
                                    }
                                }
                            }
                        }
                    }
                }
                }
                if ((s32) actor->velocity_x_fixed8 < 0) {

                } else {
block_293:
                    if ((({ register s32 query asm("r3") = negative_side_collision; asm volatile("" : "+r"(query)); query; }) != 0) && (center_collision == 0) && (({ register s32 query asm("r1") = positive_side_collision; asm volatile("" : "+r"(query)); query; }) == 0)) {
                        u8 model_id;
                        u16 behavior;
                        s32 delta;
                        register s32 tile_x asm("r2");
                        model_id = actor->model_id;
                        behavior = actor->behavior;
                        tile_x = temp_r4;
                        delta = actor->velocity_y_fixed8;
                        var_r3_4 = var_sl_2 + 1;
                        if (delta > 0) {
                            var_r3_4 -= 2;
                        }
                        if ((GetFieldMapCellCollisionMask(model_id, behavior, tile_x, var_r3_4) << 0x10) == 0) {
                            u8 second_model_id;
                            u16 second_behavior;
                            s32 delta2;
                            second_model_id = actor->model_id;
                            second_behavior = actor->behavior;
                            delta2 = actor->velocity_y_fixed8;
                            var_r3_5 = var_sl_2 + 2;
                            if (delta2 > 0) {
                                var_r3_5 -= 4;
                            }
                            if ((GetFieldMapCellCollisionMask(second_model_id, second_behavior, temp_r4, var_r3_5) << 0x10) == 0) {
                                temp_r0_9 = actor->movement_mode;
                                if (temp_r0_9 == 2) {
                                    if ((unpacked_x_remainder <= 0x400U) || ((s32) ((probe_x_fixed8 + 0xB5) & 0x7FF) > 0x3FF)) {
                                        var_r3_6 = 0xB5;
                                        asm volatile("" : "+r"(var_r3_6));
                                        goto block_326;
                                    }
                                    goto block_310;
                                }
                                if (temp_r0_9 == 3) {
                                    if ((unpacked_x_remainder > 0x400U) && ((s32) ({
                                        register s32 value asm("r0");
                                        register s32 base asm("r4");
                                        register s32 offset asm("r1");
                                        base = probe_x_fixed8;
                                        offset = 0x16A;
                                        asm volatile("" : "+r"(base), "+r"(offset));
                                        value = base + offset;
                                        asm volatile("" : "+r"(value));
                                        offset = 0x7FF;
                                        asm volatile("" : "+r"(offset));
                                        value &= offset;
                                        asm volatile("" : "+r"(value));
                                        value;
                                    }) <= 0x3FF)) {
block_310:
                                        {
                                            register s32 constant asm("r0");
                                            register s32 correction asm("r2");
                                            constant = 0x800;
                                            asm volatile("" : "+r"(constant));
                                            correction = unpacked_x_remainder;
                                            asm volatile("" : "+r"(correction));
                                            correction = constant - correction;
                                            resolved_x_velocity_fixed8 = correction;
                                        }
                                    } else {
                                        goto block_325;
                                    }
                                }
                            } else {
                                goto block_312;
                            }
                        } else {
                            goto block_312;
                        }
                    } else {
block_312:
                        if ((({ register u32 check asm("r4") = unpacked_x_remainder; asm volatile("" : "+r"(check)); check; }) != 0) && (center_collision != 0) && (({ register s32 query asm("r1") = positive_side_collision; asm volatile("" : "+r"(query)); query; }) == 0)) {
                            u8 model_id;
                            u16 behavior;
                            model_id = actor->model_id;
                            behavior = actor->behavior;
                            temp_r4 = temp_r1 + 2;
                            if ((GetFieldMapCellCollisionMask(model_id, behavior, temp_r4, var_sl_2) << 0x10) == 0) {
                                u8 second_model_id;
                                u16 second_behavior;
                                s32 delta2;
                                second_model_id = actor->model_id;
                                second_behavior = actor->behavior;
                                delta2 = actor->velocity_y_fixed8;
                                var_r3_7 = var_sl_2 + 1;
                                if (delta2 > 0) {
                                    var_r3_7 -= 2;
                                }
                                if ((GetFieldMapCellCollisionMask(second_model_id, second_behavior, temp_r4, var_r3_7) << 0x10) == 0) {
                                    u8 third_model_id;
                                    u16 third_behavior;
                                    s32 delta3;
                                    third_model_id = actor->model_id;
                                    third_behavior = actor->behavior;
                                    delta3 = actor->velocity_y_fixed8;
                                    var_r3_8 = var_sl_2 + 2;
                                    if (delta3 > 0) {
                                        var_r3_8 -= 4;
                                    }
                                    if ((GetFieldMapCellCollisionMask(third_model_id, third_behavior, temp_r4, var_r3_8) << 0x10) == 0) {
                                        temp_r0_10 = actor->movement_mode;
                                        if (temp_r0_10 != 2) {
                                            goto block_328;
                                        }
                                        {
                                            register s32 correction asm("r2");
                                            correction = 0xB5;
                                            resolved_x_velocity_fixed8 = correction;
                                            asm volatile("" : "+r"(correction) : : "memory");
                                        }
                                        goto block_327;
block_328:
                                        if (temp_r0_10 != 3) {
                                            goto block_327;
                                        }
                                        asm volatile("" ::: "memory");
block_325:
                                        var_r3_6 = 0x16A;
                                        goto block_326;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        goto block_327;
block_326:
        resolved_x_velocity_fixed8 = var_r3_6;
block_327:
        actor->velocity_x_fixed8 = ({ register s32 value asm("r4") = resolved_x_velocity_fixed8; asm volatile("" : "+r"(value)); value; });
        actor->velocity_y_fixed8 = resolved_y_velocity_fixed8;
    }
}
