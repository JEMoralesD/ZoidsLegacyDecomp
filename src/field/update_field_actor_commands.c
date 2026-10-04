#include "field_actor.h"
void *CreateSpriteFromTable(M2C_UNK, u8, u8, s32, s32, s32, s32, s32, s32) asm("func_08094374");
asm(".set func_08094374_4, func_08094374");
void *CreateSpriteFromTable_4(M2C_UNK, s32, s32, s32) asm("func_08094374_4");
M2C_UNK DestroySprite(void *) asm("func_08094554");
M2C_UNK SetSpriteAnimation(void *, u16) asm("func_08094564");
u16 GetFieldActorAnimationIndex(void *) asm("func_080A9A54");
M2C_UNK InitializeFieldActorSprites(void *) asm("func_080A9AFC");
M2C_UNK PopFieldActorCommand(s32) asm("func_080AA550");
u16 BiosArcTan2(s16, s16) asm("func_080ECD24");
u16 BiosSqrt(s32) asm("func_080ECD3C");
extern s32 gFieldActorDirectionVectors[][2] asm("D_087A1B98");

void UpdateFieldActorCommands(u32 actor_slot, struct FieldActor *actor_in) asm("func_080AA5C0");

void UpdateFieldActorCommands(u32 actor_slot, struct FieldActor *actor_in) {
    register struct FieldActor *actor asm("r6") = actor_in;
    s32 saved_actor_slot;
    s32 *timer_address;
    register s32 target_y_distance_pixels asm("r0");
    s32 signed_x_distance;
    s32 signed_y_distance;
    s32 x_distance_pixels;
    s32 temp_r0_21;
    volatile s32 outgoing0;
    volatile s32 outgoing1;
    volatile s32 outgoing2;
    volatile s32 outgoing3;
    volatile s32 outgoing4;
    register u32 command_base asm("r9");
    register s32 *counter_base asm("r0");
    register s32 timer_offset asm("sl");
    s32 target_x_distance_pixels;
    register s32 queue_offset asm("r8");
    s32 var_r0;
    s32 var_r0_10;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r0_5;
    s32 var_r0_6;
    s32 var_r0_7;
    u16 temp_r0;
    u16 temp_r0_10;
    register s32 target_move_opcode asm("r0");
    u16 distance_root;
    u16 speed_command_opcode;
    register u32 special_animation_index asm("r0");
    u16 temp_r0_2;
    u16 temp_r0_8;
    u16 temp_r0_9;
    u16 distance_pixels;
    u32 velocity_command_index;
    u32 temp_r0_4;
    u32 temp_r0_5;
    u32 temp_r0_6;
    u32 temp_r0_7;
    s32 command_index;
    u32 temp_r2_3;
    u32 remaining_ticks;
    register u32 temp_r3_2 asm("r3");
    register u32 var_r0_8 asm("r0");
    u8 *special_animation_set;
    register u8 *scan_base asm("r3");
    register u8 *direction_base asm("r2");
    register u8 *direction_address asm("r0");
    register u16 *command_arg_ptr asm("r5");
    register u32 sprite_delta asm("r0");
    register u8 *status_base asm("r1");
    register s32 status_offset asm("r4");
    register u8 *status_address asm("r0");
    register u8 *case_command_base asm("r4");
    register u8 *case_command_address asm("r0");
    u8 actor_model_id;
    s32 target_direction;
    u8 keep_secondary_sprite;
    u8 var_r0_4;
    u8 var_r1_2;
    void *temp_r0_20;
    void *temp_r2_2;
    void *temp_r2_4;
    void *temp_r4;
    void *temp_r4_2;
    void *temp_r4_3;
    register void *special_animation asm("r4");
    void *temp_r5;
    void *temp_r5_2;
    void *temp_r5_3;
    void *temp_r5_4;
    void *temp_r5_5;
    void *temp_r5_6;

    asm volatile("" : "=m"(outgoing0), "=m"(outgoing1), "=m"(outgoing2),
                 "=m"(outgoing3), "=m"(outgoing4));
    actor_slot <<= 24;
    actor_slot >>= 24;
    saved_actor_slot = (s32)actor_slot;
    if (FIELD_ACTOR_FIELD(actor, s32 *, flags) & FIELD_ACTOR_INPUT_LOCKED) {
        register s32 zero asm("r4") = 0;

        FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = (u8)zero;
        temp_r5 = FIELD_ACTOR_FIELD(actor, void **, sprite);
        SetSpriteAnimation(temp_r5, GetFieldActorAnimationIndex(actor));
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_y_fixed8) = zero;
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_x_fixed8) = zero;
        return;
    }
    counter_base = (s32 *)FIELD_ACTOR_COMMAND_TIMERS_RAM;
    timer_offset = saved_actor_slot * 4;
    queue_offset = saved_actor_slot << 8;
    goto loop_48;
block_counter:
    if (FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) != FIELD_ACTOR_SPECIAL_SPRITE_ANIMATION) {
        var_r0 = remaining_ticks - 1;
        goto block_5;
    }
    goto block_9;
block_5:
    *timer_address = var_r0;
block_6:
    if (FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) != FIELD_ACTOR_SPECIAL_SPRITE_ANIMATION) {
        if (M2C_FIELD(timer_offset, u32 *, FIELD_ACTOR_COMMAND_TIMERS_RAM) != 0) {
            goto block_11;
        }
        goto block_37;
    }
block_9:
    if (FIELD_ACTOR_SPRITE_FIELD(FIELD_ACTOR_FIELD(actor, void **, sprite), s32 *, flags) & FIELD_SPRITE_ANIMATION_FINISHED) {
        goto block_37;
    }
block_11:
    velocity_command_index = M2C_FIELD(queue_offset, u16 *, FIELD_ACTOR_COMMAND_QUEUES_RAM) - FIELD_ACTOR_CMD_WAIT;
    switch (velocity_command_index) {
    case FIELD_ACTOR_CMD_WAIT - FIELD_ACTOR_CMD_WAIT:
    case FIELD_ACTOR_CMD_PLAY_SPECIAL_ANIMATION - FIELD_ACTOR_CMD_WAIT:
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_y_fixed8) = 0;
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_x_fixed8) = 0;
        break;
    case FIELD_ACTOR_CMD_MOVE_HALF_SPEED - FIELD_ACTOR_CMD_WAIT:
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_x_fixed8) = gFieldActorDirectionVectors[FIELD_ACTOR_FIELD(actor, u8 *, requested_direction)][0] / 2;
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_y_fixed8) = gFieldActorDirectionVectors[FIELD_ACTOR_FIELD(actor, u8 *, requested_direction)][1] / 2;
        break;
    case FIELD_ACTOR_CMD_MOVE_NORMAL_SPEED - FIELD_ACTOR_CMD_WAIT:
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_x_fixed8) = gFieldActorDirectionVectors[FIELD_ACTOR_FIELD(actor, u8 *, requested_direction)][0];
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_y_fixed8) = gFieldActorDirectionVectors[FIELD_ACTOR_FIELD(actor, u8 *, requested_direction)][1];
        break;
    case FIELD_ACTOR_CMD_MOVE_DOUBLE_SPEED - FIELD_ACTOR_CMD_WAIT:
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_x_fixed8) = gFieldActorDirectionVectors[FIELD_ACTOR_FIELD(actor, u8 *, requested_direction)][0] * 2;
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_y_fixed8) = gFieldActorDirectionVectors[FIELD_ACTOR_FIELD(actor, u8 *, requested_direction)][1] * 2;
        break;
    case FIELD_ACTOR_CMD_MOVE_TO_HALF_SPEED - FIELD_ACTOR_CMD_WAIT:
    case FIELD_ACTOR_CMD_MOVE_TO_NORMAL_SPEED - FIELD_ACTOR_CMD_WAIT:
    case FIELD_ACTOR_CMD_MOVE_TO_DOUBLE_SPEED - FIELD_ACTOR_CMD_WAIT:
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_y_fixed8) = 0;
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_x_fixed8) = 0;
        temp_r2_3 = FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.x_accumulator) + FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_distance_pixels);
        FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.x_accumulator) = temp_r2_3;
        FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.y_accumulator) = (u32) (FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.y_accumulator) + FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_distance_pixels));
        temp_r3_2 = FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.period_ticks);
        if (temp_r2_3 >= temp_r3_2) {
            register s32 direction asm("r2") = FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_decreases);
            register s32 positive asm("r5") = 0x100;
            register s32 negative asm("r4") = 0xFFFFFF00;
            register u32 threshold asm("r1") = temp_r3_2;

            do {
                if (direction == 0) {
                    var_r0_2 = FIELD_ACTOR_FIELD(actor, s32 *, velocity_x_fixed8) + positive;
                } else {
                    var_r0_2 = FIELD_ACTOR_FIELD(actor, s32 *, velocity_x_fixed8) + negative;
                }
                FIELD_ACTOR_FIELD(actor, s32 *, velocity_x_fixed8) = var_r0_2;
                temp_r0_6 = FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.x_accumulator) - threshold;
                FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.x_accumulator) = temp_r0_6;
            } while (temp_r0_6 >= threshold);
        }
        if ((u32) FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.y_accumulator) >= temp_r3_2) {
            register s32 direction asm("r2") = FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_decreases);
            register s32 positive asm("r5") = 0x100;
            register s32 negative asm("r4") = 0xFFFFFF00;
            register u32 threshold asm("r1") = temp_r3_2;

            do {
                if (direction == 0) {
                    var_r0_3 = FIELD_ACTOR_FIELD(actor, s32 *, velocity_y_fixed8) + positive;
                } else {
                    var_r0_3 = FIELD_ACTOR_FIELD(actor, s32 *, velocity_y_fixed8) + negative;
                }
                FIELD_ACTOR_FIELD(actor, s32 *, velocity_y_fixed8) = var_r0_3;
                temp_r0_7 = FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.y_accumulator) - threshold;
                FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.y_accumulator) = temp_r0_7;
            } while (temp_r0_7 >= threshold);
        }
        break;
    case FIELD_ACTOR_CMD_HOP - FIELD_ACTOR_CMD_WAIT:
        temp_r2_4 = FIELD_ACTOR_FIELD(actor, void **, sprite);
        sprite_delta = (u32)(M2C_FIELD(timer_offset, u32 *, FIELD_ACTOR_COMMAND_TIMERS_RAM) - 1) >> 1;
        sprite_delta -= 3;
        asm volatile("" : "+r"(sprite_delta));
        FIELD_ACTOR_SPRITE_FIELD(temp_r2_4, u16 *, offset_y) = (u16)(FIELD_ACTOR_SPRITE_FIELD(temp_r2_4, u16 *, offset_y) - sprite_delta);
        break;
    }
    goto block_46;
block_37:
    status_base = (u8 *)FIELD_ACTOR_COMMAND_QUEUES_RAM;
    status_offset = queue_offset;
    status_address = (u8 *)((u32)status_offset - (0U - (u32)status_base));
    temp_r0_2 = *(u16 *)status_address;
    if (temp_r0_2 == FIELD_ACTOR_CMD_PLAY_SPECIAL_ANIMATION) {
        if (FIELD_ACTOR_FIELD(actor, u8 *, model_id) == 0x36) {
            status_address = status_base + 2;
            status_address += queue_offset;
            if (*(u16 *)status_address == 1) {
                FIELD_ACTOR_FIELD(actor, s32 *, world_x_fixed8) = (s32) (FIELD_ACTOR_FIELD(actor, s32 *, world_x_fixed8) + 0xFFFFF800);
                DestroySprite(FIELD_ACTOR_FIELD(actor, void **, sprite));
                InitializeFieldActorSprites(actor);
                temp_r4_2 = FIELD_ACTOR_FIELD(actor, void **, sprite);
                SetSpriteAnimation(temp_r4_2, GetFieldActorAnimationIndex(actor));
            }
        }
        {
            register u8 *status_counter_address asm("r0") = (u8 *)FIELD_ACTOR_COMMAND_TIMERS_RAM;
            register u32 status_counter_value asm("r1");

            status_counter_address += timer_offset;
            status_counter_value = 0;
            *(u32 *)status_counter_address = status_counter_value;
        }
    } else if (temp_r0_2 != FIELD_ACTOR_CMD_WAIT) {
        register s32 zero asm("r4") = 0;

        FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = (u8)zero;
        temp_r5_3 = FIELD_ACTOR_FIELD(actor, void **, sprite);
        SetSpriteAnimation(temp_r5_3, GetFieldActorAnimationIndex(actor));
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_y_fixed8) = zero;
        FIELD_ACTOR_FIELD(actor, s32 *, velocity_x_fixed8) = zero;
    }
    PopFieldActorCommand(saved_actor_slot);
    FIELD_ACTOR_FIELD(actor, u8 *, previous_movement_mode) = (u8) FIELD_ACTOR_FIELD(actor, u8 *, movement_mode);
block_46:
    counter_base = (s32 *)FIELD_ACTOR_COMMAND_TIMERS_RAM;
    if (*(u32 *)((u32)timer_offset - (0U - (u32)counter_base)) != 0) {
        return;
    }
loop_48:
    timer_address = (s32 *)((u32)timer_offset - (0U - (u32)counter_base));
    remaining_ticks = (u32)*timer_address;
    if (remaining_ticks != 0) {
        goto block_counter;
    }
    if (M2C_FIELD(queue_offset, u16 *, FIELD_ACTOR_COMMAND_QUEUES_RAM) == 0) {
        goto block_empty;
    }
    if (FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) == FIELD_ACTOR_SPECIAL_SPRITE_ANIMATION) {
        temp_r2_2 = FIELD_ACTOR_FIELD(actor, void **, sprite);
        if (FIELD_SPRITE_ANIMATION_FIELD(temp_r2_2, s32 *, frame_table) == 0x083554B4) {
            *(s16 *)0x0300004E = (s16) remaining_ticks;
        }
        DestroySprite(temp_r2_2);
        if (FIELD_ACTOR_FIELD(actor, void **, secondary_sprite) != 0) {
            DestroySprite(FIELD_ACTOR_FIELD(actor, void **, secondary_sprite));
        }
        InitializeFieldActorSprites(actor);
    }
    {
        register u8 *command_seed asm("r0") = (u8 *)FIELD_ACTOR_COMMAND_QUEUES_RAM;
        register s32 command_offset asm("r2") = queue_offset;
        register s32 dispatch asm("r1");

        dispatch = *(u16 *)((u32)command_offset -
                            (0U - (u32)command_seed)) - FIELD_ACTOR_CMD_WAIT;
        asm volatile("" : "+r"(dispatch));
        command_base = (u32)command_seed;
        command_index = dispatch;
    }
    switch (command_index) {
    case FIELD_ACTOR_CMD_WAIT - FIELD_ACTOR_CMD_WAIT:
        FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = 0U;
        case_command_base = (u8 *)FIELD_ACTOR_COMMAND_QUEUES_RAM;
        asm volatile("" : "+r"(case_command_base));
        case_command_address = case_command_base + 2;
        case_command_address += queue_offset;
        temp_r0 = *(u16 *)case_command_address;
        FIELD_ACTOR_FIELD(actor, u8 *, current_direction) = (u8) temp_r0;
        FIELD_ACTOR_FIELD(actor, u8 *, requested_direction) = (u8) temp_r0;
        temp_r5_2 = FIELD_ACTOR_FIELD(actor, void **, sprite);
        SetSpriteAnimation(temp_r5_2, GetFieldActorAnimationIndex(actor));
        timer_address = timer_offset + FIELD_ACTOR_COMMAND_TIMERS_RAM;
        case_command_base += 4;
        case_command_base += queue_offset;
        var_r0 = (s32)*(u16 *)case_command_base;
        goto block_5;
    case FIELD_ACTOR_CMD_MOVE_HALF_SPEED - FIELD_ACTOR_CMD_WAIT:
        FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = 2U;
        case_command_base = (u8 *)FIELD_ACTOR_COMMAND_QUEUES_RAM;
        asm volatile("" : "+r"(case_command_base));
        case_command_address = case_command_base + 2;
        case_command_address += queue_offset;
        temp_r0_8 = *(u16 *)case_command_address;
        FIELD_ACTOR_FIELD(actor, u8 *, current_direction) = (u8) temp_r0_8;
        FIELD_ACTOR_FIELD(actor, u8 *, requested_direction) = (u8) temp_r0_8;
        temp_r5_4 = FIELD_ACTOR_FIELD(actor, void **, sprite);
        SetSpriteAnimation(temp_r5_4, GetFieldActorAnimationIndex(actor));
        timer_address = timer_offset + FIELD_ACTOR_COMMAND_TIMERS_RAM;
        case_command_base += 4;
        case_command_base += queue_offset;
        var_r0 = *(u16 *)case_command_base * 0x10;
        goto block_5;
    case FIELD_ACTOR_CMD_MOVE_NORMAL_SPEED - FIELD_ACTOR_CMD_WAIT:
        FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = 2U;
        case_command_base = (u8 *)FIELD_ACTOR_COMMAND_QUEUES_RAM;
        asm volatile("" : "+r"(case_command_base));
        case_command_address = case_command_base + 2;
        case_command_address += queue_offset;
        temp_r0_9 = *(u16 *)case_command_address;
        FIELD_ACTOR_FIELD(actor, u8 *, current_direction) = (u8) temp_r0_9;
        FIELD_ACTOR_FIELD(actor, u8 *, requested_direction) = (u8) temp_r0_9;
        temp_r5_5 = FIELD_ACTOR_FIELD(actor, void **, sprite);
        SetSpriteAnimation(temp_r5_5, GetFieldActorAnimationIndex(actor));
        timer_address = timer_offset + FIELD_ACTOR_COMMAND_TIMERS_RAM;
        case_command_base += 4;
        case_command_base += queue_offset;
        var_r0 = *(u16 *)case_command_base * 8;
        goto block_5;
    case FIELD_ACTOR_CMD_MOVE_DOUBLE_SPEED - FIELD_ACTOR_CMD_WAIT:
        FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = 3U;
        case_command_base = (u8 *)FIELD_ACTOR_COMMAND_QUEUES_RAM;
        asm volatile("" : "+r"(case_command_base));
        case_command_address = case_command_base + 2;
        case_command_address += queue_offset;
        temp_r0_10 = *(u16 *)case_command_address;
        FIELD_ACTOR_FIELD(actor, u8 *, current_direction) = (u8) temp_r0_10;
        FIELD_ACTOR_FIELD(actor, u8 *, requested_direction) = (u8) temp_r0_10;
        temp_r5_6 = FIELD_ACTOR_FIELD(actor, void **, sprite);
        SetSpriteAnimation(temp_r5_6, GetFieldActorAnimationIndex(actor));
        timer_address = timer_offset + FIELD_ACTOR_COMMAND_TIMERS_RAM;
        case_command_base += 4;
        case_command_base += queue_offset;
        var_r0 = *(u16 *)case_command_base * 4;
        goto block_5;
    case FIELD_ACTOR_CMD_MOVE_TO_HALF_SPEED - FIELD_ACTOR_CMD_WAIT:
    case FIELD_ACTOR_CMD_MOVE_TO_NORMAL_SPEED - FIELD_ACTOR_CMD_WAIT:
    case FIELD_ACTOR_CMD_MOVE_TO_DOUBLE_SPEED - FIELD_ACTOR_CMD_WAIT:
        target_move_opcode = *(u16 *)((u32)queue_offset - (0U - command_base));
        switch (target_move_opcode) {
        case FIELD_ACTOR_CMD_MOVE_TO_HALF_SPEED:
            FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = 1U;
            break;
        case FIELD_ACTOR_CMD_MOVE_TO_NORMAL_SPEED:
            FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = 2U;
            break;
        case FIELD_ACTOR_CMD_MOVE_TO_DOUBLE_SPEED:
            FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = 3U;
            break;
        }
        direction_base = (u8 *)FIELD_ACTOR_COMMAND_QUEUES_RAM;
        asm volatile("" : "+r"(direction_base));
        direction_address = direction_base + 2;
        direction_address += queue_offset;
        target_x_distance_pixels = *(u16 *)direction_address * 8;
        var_r0_5 = FIELD_ACTOR_FIELD(actor, s32 *, world_x_fixed8);
        if (var_r0_5 < 0) {
            var_r0_5 += 0xFF;
        }
        target_x_distance_pixels -= var_r0_5 >> 8;
        FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_distance_pixels) = target_x_distance_pixels;
        {
            register u32 y_raw asm("r0");
            register s32 y_component asm("r2");

            direction_address = direction_base + 4;
            direction_address += queue_offset;
            y_raw = *(u16 *)direction_address;
            asm volatile("" : "+r"(y_raw));
            y_component = y_raw * 8;
            var_r0_6 = FIELD_ACTOR_FIELD(actor, s32 *, world_y_fixed8);
            if (var_r0_6 < 0) {
                var_r0_6 += 0xFF;
            }
            target_y_distance_pixels = y_component - (var_r0_6 >> 8);
        }
        FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_distance_pixels) = target_y_distance_pixels;
        {
            register u32 raw asm("r0");
            register u32 normalized asm("r1");
            register s32 bias asm("r4");
            register u32 sum asm("r0");
            register s32 quotient asm("r1");
            register s32 remainder asm("r0");
            register s32 sign_zero asm("r1");

            raw = BiosArcTan2((s16)(0 - target_y_distance_pixels), (s16)target_x_distance_pixels);
            raw <<= 16;
            normalized = raw >> 16;
            bias = 0x80;
            bias <<= 5;
            sum = normalized + bias;
            asm volatile("" : "+r"(sum));
            quotient = (s32)sum >> 0xD;
            remainder = quotient;
            asm volatile("" : "+r"(remainder));
            remainder >>= 3;
            remainder <<= 3;
            remainder = quotient - remainder;
            target_direction = remainder;
            sign_zero = 0;
            asm volatile("" : "+r"(sign_zero));
            FIELD_ACTOR_FIELD(actor, u8 *, current_direction) = target_direction;
            FIELD_ACTOR_FIELD(actor, u8 *, requested_direction) = target_direction;
            signed_x_distance = FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_distance_pixels);
            if (signed_x_distance >= 0) {
                FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_decreases) = sign_zero;
            } else {
                FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_distance_pixels) = (s32) (0 - signed_x_distance);
                FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_decreases) = 1;
            }
        }
        signed_y_distance = FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_distance_pixels);
        if (signed_y_distance >= 0) {
            var_r0_7 = 0;
        } else {
            FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_distance_pixels) = (s32) (0 - signed_y_distance);
            var_r0_7 = 1;
        }
        FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_decreases) = var_r0_7;
        FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.y_accumulator) = 0U;
        FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.x_accumulator) = 0U;
        x_distance_pixels = FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.x_distance_pixels);
        {
            register s32 first_product asm("r4");
            register s32 first_sum asm("r0");
            register s32 second_value asm("r1");
            register s32 second_product asm("r2");
            register s32 second_sum asm("r1");
            register u32 sqrt_shift asm("r2");

            first_product = x_distance_pixels;
            first_product *= x_distance_pixels;
            asm volatile("" : "+r"(first_product));
            first_sum = first_product;
            asm volatile("" : "+r"(first_sum));
            second_value = FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.target_movement.y_distance_pixels);
            second_product = second_value;
            second_product *= second_value;
            second_sum = second_product;
            distance_root = BiosSqrt(first_sum + second_sum);
            sqrt_shift = (u32)distance_root << 16;
            distance_pixels = sqrt_shift >> 16;
            FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.period_ticks) = (u32)distance_pixels;
            speed_command_opcode = M2C_FIELD(queue_offset, u16 *, FIELD_ACTOR_COMMAND_QUEUES_RAM);
            if (speed_command_opcode == FIELD_ACTOR_CMD_MOVE_TO_HALF_SPEED) {
                var_r0_8 = distance_pixels * 2;
                goto block_95;
            }
            if (speed_command_opcode == FIELD_ACTOR_CMD_MOVE_TO_DOUBLE_SPEED) {
                var_r0_8 = sqrt_shift >> 17;
block_95:
                FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.period_ticks) = var_r0_8;
            }
        }
        temp_r4_3 = FIELD_ACTOR_FIELD(actor, void **, sprite);
        SetSpriteAnimation(temp_r4_3, GetFieldActorAnimationIndex(actor));
        {
            register u8 *case_counter_address asm("r0") = (u8 *)FIELD_ACTOR_COMMAND_TIMERS_RAM;
            register u32 case_counter_value asm("r1");

            case_counter_address += timer_offset;
            case_counter_value = FIELD_ACTOR_FIELD(actor, u32 *, behavior_state.target_movement.period_ticks);
            *(u32 *)case_counter_address = case_counter_value;
            asm volatile("");
        }
        goto block_6;
    case FIELD_ACTOR_CMD_HOP - FIELD_ACTOR_CMD_WAIT:
        {
            register u8 *case_counter_address asm("r0") = (u8 *)FIELD_ACTOR_COMMAND_TIMERS_RAM;
            register u32 case_counter_value asm("r1");

            case_counter_address += timer_offset;
            case_counter_value = 0xE;
            *(u32 *)case_counter_address = case_counter_value;
            asm volatile("");
        }
        goto block_6;
    case FIELD_ACTOR_CMD_PLAY_SPECIAL_ANIMATION - FIELD_ACTOR_CMD_WAIT:
        var_r1_2 = 0;
        special_animation_set = (u8 *)FIELD_ACTOR_SPECIAL_ANIMATION_SETS_ROM;
        actor_model_id = FIELD_ACTOR_FIELD(actor, u8 *, model_id);
        scan_base = special_animation_set;
        goto loop_scan_test;
loop_104:
        var_r1_2 += 1;
        temp_r0_21 = var_r1_2 * 0x24;
        special_animation_set = (u8 *)((u32)temp_r0_21 - (0U - (u32)scan_base));
        if (*special_animation_set == 0xFF) {
            goto block_46;
        }
loop_scan_test:
        if (*special_animation_set != actor_model_id) {
            goto loop_104;
        }
        {
            register u8 *command_address asm("r0") = (u8 *)command_base;
            register s32 command_offset asm("r4");

            command_address += 2;
            command_offset = queue_offset;
            command_arg_ptr = (u16 *)((u32)command_offset -
                                      (0U - (u32)command_address));
            special_animation_index = *command_arg_ptr;
        }
        if ((u32)special_animation_index > 7U) {
            goto loop_104;
        }
        special_animation = special_animation_set + ((special_animation_index * 4) + 4);
        if (FIELD_ACTOR_SPECIAL_ANIMATION_FIELD(special_animation, u8 *, sprite_resource_id) == 0xFF) {
            goto loop_104;
        }
        {
            register s32 zero_seed asm("r0") = 0;
            command_base = zero_seed;
        }
        FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = FIELD_ACTOR_SPECIAL_SPRITE_ANIMATION;
        DestroySprite(FIELD_ACTOR_FIELD(actor, void **, sprite));
        {
            register void *descriptor_ip asm("ip");
            register u32 sprite_resource_id asm("r1");
            register u32 animation_id asm("r2");
            register s32 call_x asm("r3");
            register s32 stack_r0 asm("r0");

            {
                register void *descriptor_seed asm("r1") = (void *)FIELD_ACTOR_SPECIAL_SPRITE_DEFINITIONS_ROM;
                descriptor_ip = descriptor_seed;
            }
            sprite_resource_id = FIELD_ACTOR_SPECIAL_ANIMATION_FIELD(special_animation, u8 *, sprite_resource_id);
            animation_id = FIELD_ACTOR_SPECIAL_ANIMATION_FIELD(special_animation, u8 *, animation_id);
            {
                register s32 call_x_raw asm("r0");

                call_x_raw = FIELD_ACTOR_FIELD(actor, s32 *, world_x_fixed8);
                if (call_x_raw < 0) {
                    call_x_raw += 0xFF;
                }
                call_x_raw <<= 8;
                call_x = call_x_raw >> 0x10;
            }
            var_r0_10 = FIELD_ACTOR_FIELD(actor, s32 *, world_y_fixed8);
            if (var_r0_10 < 0) {
                var_r0_10 += 0xFF;
            }
            stack_r0 = (s32)(var_r0_10 << 8) >> 0x10;
            asm volatile("str %0, [sp, #0]" : "+r"(stack_r0) : : "memory");
            stack_r0 = FIELD_ACTOR_FIELD(actor, u8 *, actor_id) * 0x10;
            asm volatile("str %0, [sp, #4]" : "+r"(stack_r0) : : "memory");
            stack_r0 = FIELD_ACTOR_FIELD(actor, u8 *, actor_id);
            asm volatile("str %0, [sp, #8]" : "+r"(stack_r0) : : "memory");
            stack_r0 = 0x4410D0;
            asm volatile("str %0, [sp, #12]" : "+r"(stack_r0) : : "memory");
            stack_r0 = command_base;
            asm volatile("str %0, [sp, #16]" : "+r"(stack_r0) : : "memory");
            temp_r0_20 = CreateSpriteFromTable_4((M2C_UNK)descriptor_ip, sprite_resource_id,
                                         animation_id, call_x);
        }
        FIELD_ACTOR_FIELD(actor, void **, sprite) = temp_r0_20;
        if (FIELD_ACTOR_SPECIAL_ANIMATION_FIELD(special_animation, u8 *, use_zero_y_offset) == 0) {
            FIELD_ACTOR_SPRITE_FIELD(temp_r0_20, u16 *, offset_y) = (u16) (FIELD_ACTOR_SPRITE_FIELD(temp_r0_20, u16 *, offset_y) - 2);
        }
        keep_secondary_sprite = FIELD_ACTOR_SPECIAL_ANIMATION_FIELD(special_animation, u8 *, keep_secondary_sprite);
        if (keep_secondary_sprite == 0) {
            DestroySprite(FIELD_ACTOR_FIELD(actor, void **, secondary_sprite));
            FIELD_ACTOR_FIELD(actor, void **, secondary_sprite) = (void *) keep_secondary_sprite;
        }
        if ((FIELD_ACTOR_FIELD(actor, u8 *, model_id) == 0x34) && (*command_arg_ptr == 0)) {
            FIELD_SPRITE_ANIMATION_FIELD(FIELD_ACTOR_FIELD(actor, void **, sprite), s32 *, update_callback) = FIELD_SPRITE_BLINK_CALLBACK_THUMB;
        }
        M2C_FIELD(timer_offset, u32 *, FIELD_ACTOR_COMMAND_TIMERS_RAM) = 1U;
        if (*special_animation_set == 0xFF) {
            goto block_46;
        }
        goto block_6;
    }
    goto block_6;
block_empty:
    {
        register u32 current_state asm("r0") = FIELD_ACTOR_FIELD(actor, u8 *, movement_mode);
        register u32 previous_state asm("r1") = FIELD_ACTOR_FIELD(actor, u8 *, previous_movement_mode);

        asm volatile("" : "+r"(previous_state));
        if (current_state != previous_state) {
            temp_r4 = FIELD_ACTOR_FIELD(actor, void **, sprite);
            SetSpriteAnimation(temp_r4, GetFieldActorAnimationIndex(actor));
            FIELD_ACTOR_FIELD(actor, u8 *, previous_movement_mode) = (u8) FIELD_ACTOR_FIELD(actor, u8 *, movement_mode);
        }
    }
    return;
}
