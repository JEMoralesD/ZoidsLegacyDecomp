#include "m2c_prelude.h"
#include "../game/player_state.h"
#include "deck_commands/deck_commands.h"

M2C_UNK AddBattleEffect(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080BE65C"); /* extern */
M2C_UNK QueueBattleEffectDisplay(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080BE9D8"); /* extern */
u8 FindBattleEffect(u8, u8, s32) asm("func_080BF464");                      /* extern */
M2C_UNK RemoveBattleUnitFromTurnOrder(u8, u8) asm("func_080C02B4");                      /* extern */
s32 ScaleByPercent(s32, s32) asm("func_080E522C");                        /* extern */
s32 FindActiveBattleAuxiliaryPilotEffectValue(u8, u8, s32) asm("func_080E7AE0");                     /* extern */
M2C_UNK RecalculateBattleUnitStats(u8, u8) asm("func_080E8B08");                      /* extern */
s32 IsBattleUnitActive(u8, u8) asm("func_080E9D88");                          /* extern */

asm(
    ".macro EA408_NEG_STAGE3\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov r8, r1\n"
    ".endm\n"
    ".endm\n"
    ".macro EA408_NEG_STAGE2\n"
    ".macro neg dst, src\n"
    ".purgem neg\n"
    "neg r1, r1\n"
    "EA408_NEG_STAGE3\n"
    ".endm\n"
    ".endm\n"
    ".macro EA408_PATCH_NEG_R1\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov r1, #1\n"
    "EA408_NEG_STAGE2\n"
    ".endm\n"
    ".endm\n");

asm(
    ".macro EA408_SEL_STAGE9\n"
    ".macro lsl dst, lhs, rhs\n"
    ".purgem lsl\n"
    "lsl r4, r0, #24\n"
    ".endm\n"
    ".endm\n"
    ".macro EA408_SEL_STAGE8\n"
    ".macro eor dst, args:vararg\n"
    ".purgem eor\n"
    "eor r0, r1\n"
    "EA408_SEL_STAGE9\n"
    ".endm\n"
    ".endm\n"
    ".macro EA408_SEL_STAGE7\n"
    ".macro add dst, args:vararg\n"
    ".purgem add\n"
    "add r0, r6, #0\n"
    "EA408_SEL_STAGE8\n"
    ".endm\n"
    ".endm\n"
    ".macro EA408_SEL_STAGE6\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov r1, #1\n"
    "EA408_SEL_STAGE7\n"
    ".endm\n"
    ".endm\n"
    ".macro EA408_SEL_STAGE5\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov r9, r3\n"
    "EA408_SEL_STAGE6\n"
    ".endm\n"
    ".endm\n"
    ".macro EA408_SEL_STAGE4\n"
    ".macro lsl dst, lhs, rhs\n"
    ".purgem lsl\n"
    "lsl r3, r6, #2\n"
    "EA408_SEL_STAGE5\n"
    ".endm\n"
    ".endm\n"
    ".macro EA408_SEL_STAGE3\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov r5, #0\n"
    "EA408_SEL_STAGE4\n"
    ".endm\n"
    ".endm\n"
    ".macro EA408_SEL_STAGE2\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov r8, r2\n"
    "EA408_SEL_STAGE3\n"
    ".endm\n"
    ".endm\n"
    ".macro EA408_PATCH_SELECTION_INIT\n"
    ".macro mov dst, src\n"
    ".purgem mov\n"
    "mov r2, #0\n"
    "EA408_SEL_STAGE2\n"
    ".endm\n"
    ".endm\n");

void ApplyBattleAuxiliaryPilotEffects(s32 side_word, s32 unit_slot_word) asm("func_080EA408");

void ApplyBattleAuxiliaryPilotEffects(s32 side_word, s32 unit_slot_word) {
    /* No active opponents leave the native first-choice stack slot untouched. */
    u8 opponent_unit_choices[BATTLE_ACTIVE_UNIT_COUNT];
    void *auxiliary_pilot_record;
    s16 ally_hp_recovery_percent;
    s32 ally_recovered_hp;
    u8 shield_effect_slot;
    u8 opponent_side;
    u8 side;
    u8 unit_slot;
    u8 opponent_unit_slot;
    u8 ally_unit_slot;
    u8 opponent_choice_count;
    u32 opposite_shifted;
    void *side_action_byte_offset;
    void *war_cry_random_byte_address;
    void *active_unit_record;
    void *ally_unit_record;

    side_word <<= 24;
    side = (u32)side_word >> 24;
    unit_slot_word <<= 24;
    unit_slot = (u32)unit_slot_word >> 24;
    {
        u32 side_record_offset;
        u32 unit_record_offset;
        register u8 *base asm("r2");

        side_record_offset = side * (s32)sizeof(struct BattleSide);
        unit_record_offset = unit_slot * (s32)sizeof(struct BattleUnit);
        asm volatile("ldr %0, .LEA408_BASE_POOL" : "=r"(base));
        unit_record_offset += (u32)base;
        active_unit_record = side_record_offset + unit_record_offset;
    }
    auxiliary_pilot_record = active_unit_record + BATTLE_UNIT_PILOT_OFFSET(auxiliary_pilot);
    asm volatile("" : : "m"(auxiliary_pilot_record));
    if (FindBattleEffect(side, unit_slot, BATTLE_EFFECT_AUXILIARY_PILOT_ACTIVE) == BATTLE_EFFECT_NOT_FOUND) {
        goto block_2;
    }
    goto block_34;
block_2:
    asm volatile("EA408_PATCH_NEG_R1");
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_AUXILIARY_PILOT_ACTIVE, 0, 0, 0);
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_BRUTALITY_UP) << 0x10) == 0) {
        goto block_4;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_DOUBLE_MELEE_POWER, 0, 0, 0);
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, 0x23, 0, 0, 0);
block_4:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_MAX_HP_UP_1) << 0x10) == 0) {
        goto block_6;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_MAX_HP, 0x64, 0, 0);
block_6:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_MAX_HP_UP_2) << 0x10) == 0) {
        goto block_8;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_MAX_HP, 0xC8, 0, 0);
block_8:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_MAX_HP_UP_3) << 0x10) == 0) {
        goto block_10;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_MAX_HP, 0x12C, 0, 0);
block_10:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_SELF_REPAIR_1) << 0x10) == 0) {
        goto block_12;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_HP_RECOVERY, 0x32, 0, 0);
block_12:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_SELF_REPAIR_2) << 0x10) == 0) {
        goto block_14;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_HP_RECOVERY, 0x64, 0, 0);
block_14:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_MAX_EP_UP_1) << 0x10) == 0) {
        goto block_16;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_MAX_EP, 0xA, 0, 0);
block_16:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_MAX_EP_UP_2) << 0x10) == 0) {
        goto block_18;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_MAX_EP, 0x14, 0, 0);
block_18:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_MAX_EP_UP_3) << 0x10) == 0) {
        goto block_20;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_MAX_EP, 0x1E, 0, 0);
block_20:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_GEP_UP_1) << 0x10) == 0) {
        goto block_22;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_EP_REGEN, 1, 0, 0);
block_22:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_GEP_UP_2) << 0x10) == 0) {
        goto block_24;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_EP_REGEN, 2, 0, 0);
block_24:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_ULTRA_REACTION) << 0x10) == 0) {
        goto block_26;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_INITIATIVE, 0x1F4, 0, 0);
block_26:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_ULTRA_ACCELERATION) << 0x10) == 0) {
        goto block_28;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_SPEED, 0x1F4, 0, 0);
block_28:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_ZOS_1) << 0x10) == 0) {
        goto block_31;
    }
    if (FindBattleEffect(side, unit_slot, BATTLE_EFFECT_EXTRA_TURNS) != BATTLE_EFFECT_NOT_FOUND) {
        goto block_31;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_EXTRA_TURNS, 1, 0, 0);
block_31:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_ZOS_2) << 0x10) == 0) {
        goto block_34;
    }
    if (FindBattleEffect(side, unit_slot, BATTLE_EFFECT_EXTRA_TURNS) != BATTLE_EFFECT_NOT_FOUND) {
        goto block_34;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_EXTRA_TURNS, 2, 0, 0);
block_34:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_COMBAT_POWER_UP) << 0x10) == 0) {
        goto block_36;
    }
    AddBattleEffect(side, unit_slot, -1, 0x20, 0, 1, BATTLE_EFFECT_DOUBLE_MELEE_POWER, 0, 0, 0);
block_36:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_ANTI_AIR_BATTLE) << 0x10) == 0) {
        goto block_38;
    }
    AddBattleEffect(side, unit_slot, -1, 0x20, 0, 1, BATTLE_EFFECT_MELEE_ANTI_AIR_BONUS, 0, 0, 0);
block_38:
    {
        register s32 armor_damage_effect_value asm("r0") = FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_ARMOR_DAMAGE);

        asm volatile(
            "b 1f\n\t"
            ".align 2, 0\n\t"
            ".LEA408_BASE_POOL:\n\t"
            ".word 0x02034B4C\n\t"
            "1:"
            : "+r"(armor_damage_effect_value));
        if ((armor_damage_effect_value << 0x10) == 0) {
            goto block_42;
        }
    }
    AddBattleEffect(side, unit_slot, -1, 0x20, 0, 1, BATTLE_EFFECT_MELEE_DEFENSE_DAMAGE_CHANCE, 0, 0, 0);
block_42:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_ARMOR_PENETRATION) << 0x10) == 0) {
        goto block_44;
    }
    AddBattleEffect(side, unit_slot, -1, 0x20, 0, 1, BATTLE_EFFECT_IGNORE_MELEE_DEFENSE, 0, 0, 0);
block_44:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_ENERGY_GRAPPLE) << 0x10) == 0) {
        goto block_46;
    }
    AddBattleEffect(side, unit_slot, -1, 0x20, 0, 1, 0x21, 0, 0, 0);
block_46:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_ENERGY_SHIELD) << 0x10) == 0) {
        goto block_50;
    }
    shield_effect_slot = FindBattleEffect(side, unit_slot, BATTLE_EFFECT_ENERGY_SHIELD);
    if (shield_effect_slot != BATTLE_EFFECT_NOT_FOUND) {
        goto block_49;
    }
    AddBattleEffect(side, unit_slot, -1, 0, 0, 1, BATTLE_EFFECT_ENERGY_SHIELD, 0x64, 0, 0);
    goto block_50;
block_49:
    {
        u8 *base = (u8 *)0x02034B4C;
        u32 address;

        asm volatile("" : "+r"(base));
        address = shield_effect_slot * (u32)sizeof(struct BattleEffect);
        address += unit_slot * (s32)sizeof(struct BattleUnit);
        address += side * (s32)sizeof(struct BattleSide);
        address += (u32)base;
        address += BATTLE_UNIT_OFFSET(effects[0].value);
        *(u16 *)address = (u16)(*(u16 *)address + 0x64);
    }
block_50:
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_WAR_CRY) << 0x10) == 0) {
        goto block_56;
    }
    asm volatile("EA408_PATCH_SELECTION_INIT");
    opponent_choice_count = 0;
    opponent_unit_slot = 0;
    side_action_byte_offset = (void *)(side * 4);
    asm volatile("" : : "r"(side_action_byte_offset));
    opposite_shifted = side ^ 1;
    opposite_shifted <<= 24;
collect_active_opponents:
    if ((IsBattleUnitActive(opposite_shifted >> 24, opponent_unit_slot) << 0x18) == 0) {
        goto next_opponent_slot;
    }
    opponent_unit_choices[opponent_choice_count] = opponent_unit_slot;
    opponent_choice_count += 1;
next_opponent_slot:
    opponent_unit_slot += 1;
    if ((u32) opponent_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
        goto collect_active_opponents;
    }
    opponent_side = side ^ 1;
    {
        register u8 *base asm("r0") = (u8 *)0x02034B4C;
        register u32 offset asm("r4");

        asm volatile("" : "+r"(base));
        base += (u32)side_action_byte_offset;
        offset = BATTLE_DECK_PREPARATION_OFFSET(actions[0].random_or_target);
        asm volatile("" : "+r"(offset));
        asm volatile(
            ".macro add dst, args:vararg\n\t"
            ".purgem add\n\t"
            "add r4, r4, r0\n\t"
            ".endm");
        war_cry_random_byte_address = base + offset;
    }
    {
        register void *saved_war_cry_random_address asm("r9");

        asm volatile(
            "mov %0, %1\n\t"
            "ldrb r0, [%1]\n\t"
            "mov r1, %2\n\t"
            "mul r1, r0\n\t"
            "add r0, r1, #0\n\t"
            "asr r0, r0, #8\n\t"
            "add r0, sp\n\t"
            "add r0, #24\n\t"
            "ldrb r1, [r0]\n\t"
            "mov r2, #1\n\t"
            "neg r2, r2\n\t"
            "mov r3, #0\n\t"
            "str r3, [sp, #0]\n\t"
            "mov %1, #1\n\t"
            "str %1, [sp, #4]\n\t"
            "mov r0, #25\n\t"
            "str r0, [sp, #8]\n\t"
            "str r3, [sp, #12]\n\t"
            "str %1, [sp, #16]\n\t"
            "str r3, [sp, #20]\n\t"
            "add r0, %3, #0\n\t"
            "bl func_080BE65C"
            : "=r"(saved_war_cry_random_address), "+r"(war_cry_random_byte_address)
            : "r"(opponent_choice_count), "r"(opponent_side)
            : "r0", "r1", "r2", "r3", "lr", "cc", "memory");
        {
            register void *war_cry_random_address asm("r2") = saved_war_cry_random_address;

            asm volatile("" : "+r"(war_cry_random_address));
            RemoveBattleUnitFromTurnOrder(opponent_side, opponent_unit_choices[(s32) (opponent_choice_count * M2C_FIELD(war_cry_random_address, u8 *, 0)) >> 8]);
        }
    }
block_56:
    RecalculateBattleUnitStats(side, unit_slot);
    asm volatile(
        "mov r4, %0\n\t"
        "mov r5, #58\n\t"
        "ldrsh r1, [r4, r5]\n\t"
        "mov r2, #6\n\t"
        "ldrsh r0, [r4, r2]\n\t"
        "cmp r1, r0\n\t"
        "ble 2f\n\t"
        "add r0, r1, #0\n\t"
        "ldr r4, [sp, #32]\n\t"
        "mov r5, #42\n\t"
        "ldrsh r1, [r4, r5]\n\t"
        "bl func_080E522C\n\t"
        "mov r2, %0\n\t"
        "ldrh r1, [r2, #6]\n\t"
        "lsl r0, r0, #16\n\t"
        "asr r4, r0, #16\n\t"
        "add r1, r4, r1\n\t"
        "mov r3, #0\n\t"
        "strh r1, [r2, #6]\n\t"
        "lsl r1, r1, #16\n\t"
        "asr r1, r1, #16\n\t"
        "ldrh r5, [r2, #58]\n\t"
        "mov r8, r5\n\t"
        "mov r5, #58\n\t"
        "ldrsh r0, [r2, r5]\n\t"
        "cmp r1, r0\n\t"
        "ble 1f\n\t"
        "mov r0, r8\n\t"
        "strh r0, [r2, #6]\n\t"
        "1:\n\t"
        "mov r2, #1\n\t"
        "neg r2, r2\n\t"
        "str r3, [sp, #0]\n\t"
        "mov r0, #1\n\t"
        "str r0, [sp, #4]\n\t"
        "str r0, [sp, #8]\n\t"
        "str r4, [sp, #12]\n\t"
        "str r3, [sp, #16]\n\t"
        "str r3, [sp, #20]\n\t"
        "add r0, r6, #0\n\t"
        "add r1, r7, #0\n\t"
        "mov r3, #0\n\t"
        "bl func_080BE9D8\n\t"
        "2:"
        :
        : "r"(active_unit_record)
        : "r0", "r1", "r2", "r3", "r4", "r5", "r8", "lr", "cc", "memory");
    asm volatile(
        "mov r2, %0\n\t"
        "mov r3, #62\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "mov r4, #8\n\t"
        "ldrsh r0, [r2, r4]\n\t"
        "cmp r1, r0\n\t"
        "ble 1f\n\t"
        "mov r2, #1\n\t"
        "neg r2, r2\n\t"
        "mov r3, #0\n\t"
        "str r3, [sp, #0]\n\t"
        "mov r0, #1\n\t"
        "str r0, [sp, #4]\n\t"
        "mov r0, #4\n\t"
        "str r0, [sp, #8]\n\t"
        "mov r5, %0\n\t"
        "ldrh r0, [r5, #62]\n\t"
        "ldrh r1, [r5, #8]\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r0, r0, #16\n\t"
        "asr r0, r0, #16\n\t"
        "str r0, [sp, #12]\n\t"
        "str r3, [sp, #16]\n\t"
        "str r3, [sp, #20]\n\t"
        "add r0, r6, #0\n\t"
        "add r1, r7, #0\n\t"
        "bl func_080BE9D8\n\t"
        "ldrh r0, [r5, #62]\n\t"
        "strh r0, [r5, #8]\n\t"
        "1:"
        :
        : "r"(active_unit_record)
        : "r0", "r1", "r2", "r3", "r4", "r5", "lr", "cc", "memory");
    if ((FindActiveBattleAuxiliaryPilotEffectValue(side, unit_slot, AUXILIARY_EFFECT_RECOVERY_FIELD) << 0x10) == 0) {
        goto effects_applied;
    }
    {
        u32 side_record_offset;

        ally_unit_slot = 0;
        side_record_offset = side * (s32)sizeof(struct BattleSide);
heal_other_allies:
    if (ally_unit_slot == unit_slot) {
        goto next_ally_slot;
    }
    if ((IsBattleUnitActive(side, ally_unit_slot) << 0x18) == 0) {
        goto next_ally_slot;
    }
    {
        u32 unit_record_offset;
        u8 *base;
        register s32 ally_max_hp asm("r0");
        register u32 ally_side_offset_carrier asm("r1");
        register s32 ally_stat_offset asm("r2");
        register void *auxiliary_record_carrier asm("r3");

        unit_record_offset = ally_unit_slot * (s32)sizeof(struct BattleUnit);
        base = (u8 *)0x02034B4C;
        asm volatile("" : "+r"(base));
        unit_record_offset += (u32)base;
        ally_side_offset_carrier = side_record_offset;
        asm volatile("" : "+r"(ally_side_offset_carrier));
        ally_unit_record = (void *)(ally_side_offset_carrier + unit_record_offset);
        asm volatile(
            ".macro mov dst, src\n\t"
            ".purgem mov\n\t"
            "mov r2, \\src\n\t"
            ".endm\n\t"
            ".macro ldrsh dst, addr:vararg\n\t"
            ".purgem ldrsh\n\t"
            "ldrsh r0, [r4, r2]\n\t"
            ".endm");
        ally_stat_offset = BATTLE_UNIT_OFFSET(max_hp);
        ally_max_hp = *(s16 *)((u8 *)ally_unit_record + ally_stat_offset);
        auxiliary_record_carrier = auxiliary_pilot_record;
        ally_stat_offset = PLAYER_AUXILIARY_PILOT_OFFSET(hp_recovery_percent);
        ally_hp_recovery_percent = *(s16 *)((u8 *)auxiliary_record_carrier + ally_stat_offset);
        ally_recovered_hp = M2C_FIELD(ally_unit_record, u16 *, BATTLE_UNIT_OFFSET(hp)) + ScaleByPercent(ally_max_hp, (s16) ((s32) (ally_hp_recovery_percent + ((u32) ally_hp_recovery_percent >> 0x1F)) >> 1));
    }
    M2C_FIELD(ally_unit_record, u16 *, BATTLE_UNIT_OFFSET(hp)) = ally_recovered_hp;
    if ((s32) (s16) ally_recovered_hp <= (s32) M2C_FIELD(ally_unit_record, s16 *, BATTLE_UNIT_OFFSET(max_hp))) {
        goto next_ally_slot;
    }
    M2C_FIELD(ally_unit_record, u16 *, BATTLE_UNIT_OFFSET(hp)) = (u16) M2C_FIELD(ally_unit_record, s16 *, BATTLE_UNIT_OFFSET(max_hp));
next_ally_slot:
    ally_unit_slot += 1;
    if ((u32) ally_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
        goto heal_other_allies;
    }
    }
effects_applied:
    return;
}
