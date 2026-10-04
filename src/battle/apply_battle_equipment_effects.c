#include "m2c_prelude.h"
#include "battle.h"

s32 AddBattleEffect(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080BE65C"); /* extern */
void QueueBattleEffectDisplay(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080BE9D8"); /* extern */
void RemoveBattleUnitFromTurnOrder(u8, u8) asm("func_080C02B4");                            /* extern */
void LoadAuxiliaryPilotEffects() asm("func_080E77FC");                                  /* extern */
s16 DivideSigned32(s32, s32) asm("func_080ECD98");                         /* extern */
void jtbl_080BEF5C();                                  /* static */
extern u8 gBattleState[];

#define DIRECT_SP_ARG() ((s8)({ \
    register s32 direct_sp asm("r3") = sp18; \
    asm volatile("" : "+r"(direct_sp)); \
    direct_sp; \
}))

#define CALL_SP_R0() ((s8)({ \
    register s32 call_sp asm("r0") = sp18; \
    asm volatile("" : "+r"(call_sp)); \
    call_sp; \
}))

#define DUAL_SP_VALUE() ({ \
    register s32 dual_sp asm("r2") = sp18; \
    register s32 dual_value asm("r4") = (s8)dual_sp; \
    asm volatile("" : "+r"(dual_sp), "+r"(dual_value)); \
    dual_value; \
})

#define CASE17_SP_VALUE() ({ \
    register s32 case_sp asm("r0") = sp18; \
    register s32 case_value asm("r4") = (s8)case_sp; \
    asm volatile("" : "+r"(case_sp), "+r"(case_value)); \
    case_value; \
})

#define DUAL_SP_R3_VALUE() ({ \
    register s32 dual_sp asm("r3") = sp18; \
    register s32 dual_value asm("r4") = (s8)dual_sp; \
    asm volatile("" : "+r"(dual_sp), "+r"(dual_value)); \
    dual_value; \
})

#define FORMAT_ARG(code) ({ \
    register s32 format_const asm("r1") = (code); \
    register s32 format_value asm("r0") = temp_r0; \
    asm volatile("" : "+r"(format_const), "+r"(format_value)); \
    format_value | format_const; \
})

#define FORMAT_JOIN_ARG(code) ({ \
    register s32 format_const asm("r1") = (code); \
    asm volatile("" : "+r"(format_const)); \
    temp_r0 | format_const; \
})

#define COMMAND_VALUE_R1() ({ \
    register s32 field_value asm("r0"); \
    asm volatile("mov r1, #10\n\tldrsh r0, [r6, r1]" \
                 : "=r"(field_value) : "r"(command) : "r1", "cc"); \
    field_value; \
})

#define COMMAND_DURATION_R2() ({ \
    register s32 field_value asm("r0"); \
    asm volatile("mov r2, #14\n\tldrsh r0, [r6, r2]" \
                 : "=r"(field_value) : "r"(command) : "r2", "cc"); \
    field_value; \
})

#define COMMAND_EP_COST_R1() ({ \
    register s32 field_value asm("r0"); \
    asm volatile("mov r1, #16\n\tldrsh r0, [r6, r1]" \
                 : "=r"(field_value) : "r"(command) : "r1", "cc"); \
    field_value; \
})



typedef struct EquipmentEffectCommand {
    u8 pad00[2];
    u16 flags;
    u32 attributes;
    u8 pad08[2];
    s16 value;
    s16 secondary_value;
    s16 duration_or_threshold;
    s16 ep_cost;
} EquipmentEffectCommand;

typedef struct BattleEffectTarget {
    u8 pad00[4];
    u16 flags;
    u16 hp;
    u16 ep;
    s16 initiative;
    s16 evasion_score;
    s16 equipment_weight;
    s16 level;
    u8 pad12[0x18];
    u16 unk2A;
    u16 unk2C;
    u16 unk2E;
    u16 unk30;
    u16 unk32;
    u8 pad34[6];
    s16 max_hp;
    u8 pad3C[2];
    s16 max_ep;
    u8 pad40[0x61];
    u8 auxiliary_pilot_id;
    u8 padA2[0xE];
    u8 auxiliary_pilot_kind;
    u8 padB1[0x27];
    u8 unkD8;
} BattleEffectTarget;

void ApplyBattleEquipmentEffects(EquipmentEffectCommand *, s32, s32, s32, s32) asm("func_080BEE04");

void ApplyBattleEquipmentEffects(EquipmentEffectCommand *command, s32 target_side, s32 target_unit_slot, s32 equipment_slot, s32 passive_equipment) {
    volatile s32 sp18;
    s32 temp_r0;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r7;
    register s32 var_r1 asm("r1");
    register s32 second_arg4 asm("r0");
    register s32 second_zero asm("r5");
    u16 second_flags;
    s32 temp_r4_2;
    s32 var_r4;
    s32 temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r0_5;
    s32 temp_r0_6;
    s32 temp_r7_2;
    u32 temp_r0_2;
    u8 temp_r1;
    u8 temp_r2;
    u8 temp_r4;
    BattleEffectTarget *target;

    temp_r1 = (u8)target_side;
    temp_r2 = (u8)target_unit_slot;
    sp18 = (u8)equipment_slot;
    temp_r4 = (u8) passive_equipment;
    temp_r0 = ((s32) ((0 - temp_r4) | temp_r4) >> 0x1F) & BATTLE_EFFECT_PASSIVE_EQUIPMENT;
    {
        register s32 row_value asm("r1") = temp_r1 * 0x1380;
        register s32 col_value asm("r0") = temp_r2 * 0x270;
        register u8 *record_base asm("r2");
        asm volatile("" : "+r"(row_value), "+r"(col_value));
        record_base = gBattleState;
        asm volatile("" : "+r"(record_base));
        col_value += (s32)record_base;
        target = (BattleEffectTarget *)(row_value + col_value);
    }
    temp_r7 = 1 & command->flags;
    if (temp_r7 == 0) {
        if (command->attributes & WEAPON_DEFENSE_DAMAGE) {
            register s32 neg_one asm("r4") = -1;
            asm volatile("" : "+r"(neg_one));
            AddBattleEffect(temp_r1, temp_r2, neg_one, 0U,
                         temp_r7, temp_r7, BATTLE_EFFECT_DEFENSE,
                         (s32)({
                             s16 result;
                             asm volatile("" ::: "memory");
                             result = DivideSigned32(0 - command->value, 0xA);
                             result;
                         }),
                         temp_r7, temp_r7);
        }
        if (command->attributes & WEAPON_PILOT_INACTIVE) {
            AddBattleEffect(temp_r1, temp_r2, -1, 0x18U, temp_r7, temp_r7, BATTLE_EFFECT_PILOT_INACTIVE, temp_r7, temp_r7, temp_r7);
        }
        temp_r1_2 = command->attributes;
        if (((temp_r1_2 & WEAPON_FREEZE) || ((temp_r1_2 & 0x4000) && !(0x40 & target->flags))) && ((AddBattleEffect(temp_r1, temp_r2, -1, 0x18U, temp_r7, temp_r7, BATTLE_EFFECT_FREEZE, temp_r7, temp_r7, temp_r7) << 0x18) != 0)) {
            RemoveBattleUnitFromTurnOrder(temp_r1, temp_r2);
        }
        if (!(command->attributes & WEAPON_CONFUSION)) {
            return;
        }
        AddBattleEffect(temp_r1, temp_r2, -1, 0x18U, 0, 0, BATTLE_EFFECT_CONFUSION, 0, 0, 0);
        return;
    }
    temp_r0_2 = (u8) command->attributes - 1;
    switch (temp_r0_2) {                            /* irregular */
    case EQUIPMENT_EFFECT_DEFENSE - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_DEFENSE), (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_DEFENSE_AND_ARMOR_RATE - 1:
        var_r4 = DUAL_SP_VALUE();
        AddBattleEffect(temp_r1, temp_r2, var_r4, command->flags, command->attributes, (second_zero = 0), FORMAT_ARG(BATTLE_EFFECT_DEFENSE), COMMAND_VALUE_R1(), COMMAND_DURATION_R2(), COMMAND_EP_COST_R1());
        second_flags = command->flags;
        second_arg4 = command->attributes;
        asm volatile("str %0, [sp]\n\tstr %1, [sp, #4]"
                     : : "r"(second_arg4), "r"(second_zero) : "memory");
        var_r1 = BATTLE_EFFECT_ARMOR_RATE;
        goto block_50;
    case EQUIPMENT_EFFECT_SPEED - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_SPEED), (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_MOBILITY - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_MOBILITY), (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_SPEED_AND_MOBILITY - 1:
        var_r4 = DUAL_SP_VALUE();
        AddBattleEffect(temp_r1, temp_r2, var_r4, command->flags, command->attributes, (second_zero = 0), FORMAT_ARG(BATTLE_EFFECT_SPEED), COMMAND_VALUE_R1(), COMMAND_DURATION_R2(), COMMAND_EP_COST_R1());
        second_flags = command->flags;
        second_arg4 = command->attributes;
        asm volatile("str %0, [sp]\n\tstr %1, [sp, #4]"
                     : : "r"(second_arg4), "r"(second_zero) : "memory");
        var_r1 = BATTLE_EFFECT_MOBILITY;
        goto block_50;
    case EQUIPMENT_EFFECT_SENSOR_ACCURACY - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_SENSOR_ACCURACY), (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_HIT_RATE - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_HIT_RATE), (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_SENSOR_ACCURACY_AND_HIT_RATE - 1:
        var_r4 = DUAL_SP_VALUE();
        AddBattleEffect(temp_r1, temp_r2, var_r4, command->flags, command->attributes, (second_zero = 0), FORMAT_ARG(BATTLE_EFFECT_SENSOR_ACCURACY), COMMAND_VALUE_R1(), COMMAND_DURATION_R2(), COMMAND_EP_COST_R1());
        second_flags = command->flags;
        second_arg4 = command->attributes;
        asm volatile("str %0, [sp]\n\tstr %1, [sp, #4]"
                     : : "r"(second_arg4), "r"(second_zero) : "memory");
        var_r1 = BATTLE_EFFECT_HIT_RATE;
        goto block_50;
    case EQUIPMENT_EFFECT_HIT_RATE_PENALTY - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_HIT_RATE),
                     (s32)(s16)(0 - (u16)command->value),
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_EP_REGEN - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_EP_REGEN), (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_EVASION_SCORE - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_EVASION_SCORE), (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_EVASION_RATE - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_EVASION_RATE), (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_MAX_HP - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_MAX_HP), (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_MAX_EP - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_MAX_EP), (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_MAX_HP_AND_EP - 1:
        var_r4 = DUAL_SP_VALUE();
        AddBattleEffect(temp_r1, temp_r2, var_r4, command->flags, command->attributes, (second_zero = 0), FORMAT_ARG(BATTLE_EFFECT_MAX_HP), COMMAND_VALUE_R1(), COMMAND_DURATION_R2(), COMMAND_EP_COST_R1());
        second_flags = command->flags;
        second_arg4 = command->attributes;
        asm volatile("str %0, [sp]\n\tstr %1, [sp, #4]"
                     : : "r"(second_arg4), "r"(second_zero) : "memory");
        var_r1 = BATTLE_EFFECT_MAX_EP;
        goto block_50;
    case EQUIPMENT_EFFECT_HP_RECOVERY - 1: {
        register s32 case15_flag asm("r4");
        asm volatile("ldrh r1, [r6, #2]\n\t"
                     "mov r0, #2\n\t"
                     "and r0, r0, r1\n\t"
                     "lsl r0, r0, #16\n\t"
                     "lsr r4, r0, #16"
                     : "=r"(case15_flag) : "r"(command) : "r0", "r1", "cc");
        if (case15_flag == 0) {
            register u32 add_value asm("r0") = (u16)command->value;
            register u32 current_value asm("r3") = target->hp;
            asm volatile("" : "+r"(add_value), "+r"(current_value));
            temp_r0_3 = add_value + current_value;
            target->hp = temp_r0_3;
            if ((s32) (s16) temp_r0_3 > (s32) (s16) target->max_hp) {
                target->hp = (u16) target->max_hp;
            }
            QueueBattleEffectDisplay(temp_r1, temp_r2, CALL_SP_R0(), command->flags,
                          command->attributes, case15_flag, FORMAT_JOIN_ARG(BATTLE_EFFECT_HP_RECOVERY),
                          (s32)command->value, (s32)command->duration_or_threshold,
                          (s32)command->ep_cost);
        } else {
            AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                         command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_HP_RECOVERY), (s32)command->value,
                         (s32)command->duration_or_threshold, (s32)command->ep_cost);
        }
        break;
    }
    case EQUIPMENT_EFFECT_EP_RECOVERY - 1: {
        register s32 case16_flag asm("r4");
        asm volatile("ldrh r1, [r6, #2]\n\t"
                     "mov r0, #2\n\t"
                     "and r0, r0, r1\n\t"
                     "lsl r0, r0, #16\n\t"
                     "lsr r4, r0, #16"
                     : "=r"(case16_flag) : "r"(command) : "r0", "r1", "cc");
        if (case16_flag != 0) {
            goto case16_false;
        }
        {
            register u32 add_value asm("r0") = (u16)command->value;
            register u32 current_value asm("r2") = target->ep;
            asm volatile("" : "+r"(add_value), "+r"(current_value));
            temp_r0_4 = add_value + current_value;
        }
        target->ep = temp_r0_4;
        if ((s32) (s16) temp_r0_4 > (s32) (s16) target->max_ep) {
            target->ep = (u16) target->max_ep;
        }
        QueueBattleEffectDisplay(temp_r1, temp_r2, CALL_SP_R0(), command->flags,
                      command->attributes, case16_flag, FORMAT_JOIN_ARG(BATTLE_EFFECT_EP_RECOVERY),
                      (s32)command->value, (s32)command->duration_or_threshold,
                      (s32)command->ep_cost);
        break;
    }
case16_false:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, FORMAT_ARG(BATTLE_EFFECT_EP_RECOVERY), (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_HP_AND_EP_RECOVERY - 1:
        temp_r7_2 = (u16)(2 & command->flags);
        if (temp_r7_2 == 0) {
            {
                register u32 add_value asm("r0") = (u16)command->value;
                register u32 current_value asm("r2") = target->hp;
                asm volatile("" : "+r"(add_value), "+r"(current_value));
                temp_r0_5 = add_value + current_value;
            }
            target->hp = temp_r0_5;
            if ((s32) (s16) temp_r0_5 > (s32) (s16) target->max_hp) {
                target->hp = (u16) target->max_hp;
            }
            temp_r0_6 = (u16)command->secondary_value + target->ep;
            target->ep = temp_r0_6;
            if ((s32) (s16) temp_r0_6 > (s32) (s16) target->max_ep) {
                target->ep = (u16) target->max_ep;
            }
            temp_r4_2 = CASE17_SP_VALUE();
            QueueBattleEffectDisplay(temp_r1, temp_r2, temp_r4_2, command->flags, command->attributes, (s32) temp_r7_2, FORMAT_ARG(BATTLE_EFFECT_HP_RECOVERY), (s32) command->value, (s32) command->duration_or_threshold, (s32) command->ep_cost);
            QueueBattleEffectDisplay(temp_r1, temp_r2, temp_r4_2, command->flags, command->attributes, (s32) temp_r7_2, FORMAT_ARG(BATTLE_EFFECT_EP_RECOVERY), (s32) (s16) command->secondary_value, (s32) command->duration_or_threshold, (s32) command->ep_cost);
            goto block_after_50;
        } else {
            var_r4 = DUAL_SP_R3_VALUE();
            AddBattleEffect(temp_r1, temp_r2, var_r4, command->flags, command->attributes, (second_zero = 0), FORMAT_ARG(BATTLE_EFFECT_HP_RECOVERY), (s32) command->value, (s32) command->duration_or_threshold, (s32) command->ep_cost);
            second_flags = command->flags;
            second_arg4 = command->attributes;
            asm volatile("str %0, [sp]\n\tstr %1, [sp, #4]"
                         : : "r"(second_arg4), "r"(second_zero) : "memory");
            var_r1 = BATTLE_EFFECT_EP_RECOVERY;
            goto block_50;
        }
        AddBattleEffect(temp_r1, temp_r2, var_r4, second_flags,
                     second_arg4, second_zero, (s32)({
                         block_50:
                         temp_r0 | var_r1;
                     }),
                     (s32)(s16)command->secondary_value, (s32)command->duration_or_threshold,
                     (s32)command->ep_cost);
        asm volatile("");
block_after_50:
        break;
    case EQUIPMENT_EFFECT_SENSOR_ACCURACY_OVERRIDE - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, BATTLE_EFFECT_SENSOR_ACCURACY_OVERRIDE, (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case EQUIPMENT_EFFECT_ENERGY_SHIELD - 1:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, BATTLE_EFFECT_ENERGY_SHIELD, (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case 20:
        AddBattleEffect(temp_r1, temp_r2, DIRECT_SP_ARG(), command->flags,
                     command->attributes, 0, 0x1C, (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
        break;
    case 21: {
        register u8 *state_ptr asm("r2") = (u8 *)target + 0xA1;
        asm volatile("" : "+r"(state_ptr));
        if (*state_ptr == 0) {
            register u32 state_value asm("r1") = 6;
            register u8 *setup_ptr asm("r0");
            register u8 *source_ptr asm("r2");
            asm volatile("" : "+r"(state_value));
            *state_ptr = (u8)state_value;
            setup_ptr = (u8 *)target + 0xB0;
            source_ptr = (u8 *)0x087B77BC;
            asm volatile("" : "+r"(setup_ptr), "+r"(source_ptr));
            *setup_ptr = (u8)state_value;
            {
                register u32 source_byte asm("r3") = source_ptr[0];
                register u8 *byte_dest asm("r1") = (u8 *)target + 0xD8;
                asm volatile("" : "+r"(source_byte), "+r"(byte_dest));
                *byte_dest = (u8)source_byte;
            }
            *(u16 *)(setup_ptr + 0x2A) = *(u16 *)(source_ptr + 2);
            *(u16 *)(setup_ptr + 0x2C) = *(u16 *)(source_ptr + 4);
            *(u16 *)(setup_ptr + 0x2E) = *(u16 *)(source_ptr + 6);
            *(u16 *)(setup_ptr + 0x30) = *(u16 *)(source_ptr + 8);
            *(u16 *)(setup_ptr + 0x32) = *(u16 *)(source_ptr + 10);
            LoadAuxiliaryPilotEffects();
        }
        break;
    }
    }
    temp_r1_3 = command->attributes;
    if (EQUIPMENT_EFFECT_ANTI_AIR & temp_r1_3) {
        register s32 final_sp18 asm("r3") = sp18;
        asm volatile("" : "+r"(final_sp18));
        AddBattleEffect(temp_r1, temp_r2, (s8)final_sp18, command->flags,
                     temp_r1_3, 0, temp_r0 | BATTLE_EFFECT_ANTI_AIR, (s32)command->value,
                     (s32)command->duration_or_threshold, (s32)command->ep_cost);
    }
}
