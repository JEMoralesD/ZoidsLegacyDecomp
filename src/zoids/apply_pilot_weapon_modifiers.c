#include "m2c_prelude.h"
#include "../battle/battle.h"

s32 ScaleByPercent(s32, s32) asm("func_080E522C");                        /* extern */
s32 FindAbilityValue(void *, s32, s32) asm("func_080E74F0");                /* extern */
s16 DivideSigned32(s32, u8) asm("func_080ECD98");                         /* extern */

void ApplyPilotWeaponModifiers(void *, void *, void *, M2C_UNK, void *) asm("func_080E6994");

void ApplyPilotWeaponModifiers(void *unit_arg, void *pilot_arg, void *auxiliary_pilot_arg, M2C_UNK equipment_id, void *weapon_stats_arg) {
    register void *unit asm("r8") = unit_arg;
    register void *pilot asm("r6") = pilot_arg;
    register void *auxiliary_pilot asm("r9") = auxiliary_pilot_arg;
    register void *weapon_stats asm("r5") = weapon_stats_arg;
    register s16 accuracy_bonus_percent asm("r4");
    s32 delta;
    u8 effective_pilot_level;

    asm volatile(""
        : "+r"(unit), "+r"(pilot), "+r"(auxiliary_pilot), "+r"(weapon_stats));

    if (pilot == 0) {
        return;
    }
    if (1 & M2C_FIELD(weapon_stats, u16 *, 2)) {
        return;
    }
    effective_pilot_level = M2C_FIELD(pilot, u8 *, 0x30);
    if ((FindAbilityValue(pilot, 3, M2C_FIELD(unit, u8 *, 0)) << 0x10) != 0) {
        effective_pilot_level += 5;
    }
    {
        register void *bonus_base asm("r1") = unit;
        asm volatile("" : "+r"(bonus_base));
        if ((FindAbilityValue(pilot, 4, M2C_FIELD(bonus_base, u8 *, 0)) << 0x10) != 0) {
            effective_pilot_level += 0xA;
        }
    }
    {
        register void *bonus_base asm("r3") = unit;
        asm volatile("" : "+r"(bonus_base));
        if ((FindAbilityValue(pilot, 5, M2C_FIELD(bonus_base, u8 *, 0)) << 0x10) != 0) {
            effective_pilot_level += 0x14;
        }
    }
    if (auxiliary_pilot != 0) {
        effective_pilot_level += M2C_FIELD(auxiliary_pilot, u8 *, 0x28);
    }
    if ((u32) effective_pilot_level > 0x63U) {
        effective_pilot_level = 0x63;
    }
    accuracy_bonus_percent = M2C_FIELD(pilot, u16 *, 0x3A);
#define APPLY_STAT(ID, OP) do {                                             \
        delta = FindAbilityValue(pilot, (ID), M2C_FIELD(weapon_stats, s32 *, 4));       \
        asm volatile(                                                       \
            "lsl %0, %0, #16\n"                                           \
            "asr %0, %0, #16\n"                                           \
            OP " %0, %0, %1\n"                                            \
            "lsl %0, %0, #16\n"                                           \
            "lsr %0, %0, #16"                                             \
            : "+r"(accuracy_bonus_percent)                                                   \
            : "r"(delta));                                                 \
    } while (0)
    APPLY_STAT(PILOT_ABILITY_RANGED_ACCURACY_PENALTY, "sub");
    APPLY_STAT(PILOT_ABILITY_MELEE_ACCURACY_PENALTY, "sub");
    APPLY_STAT(PILOT_ABILITY_MELEE_ACCURACY_BONUS, "add");
    APPLY_STAT(0x19, "sub");
    APPLY_STAT(0x1A, "sub");
    APPLY_STAT(PILOT_ABILITY_MISSILE_ACCURACY_PENALTY, "sub");
    APPLY_STAT(PILOT_ABILITY_LASER_ACCURACY_PENALTY, "sub");
    APPLY_STAT(PILOT_ABILITY_PARTICLE_ACCURACY_PENALTY, "sub");
    APPLY_STAT(PILOT_ABILITY_BALLISTIC_ACCURACY_PENALTY, "sub");
    APPLY_STAT(0x22, "add");
    APPLY_STAT(0x23, "add");
    APPLY_STAT(PILOT_ABILITY_MISSILE_ACCURACY_BONUS, "add");
    APPLY_STAT(PILOT_ABILITY_LASER_ACCURACY_BONUS, "add");
    APPLY_STAT(PILOT_ABILITY_PARTICLE_ACCURACY_BONUS, "add");
#undef APPLY_STAT
    delta = FindAbilityValue(pilot, PILOT_ABILITY_BALLISTIC_ACCURACY_BONUS, M2C_FIELD(weapon_stats, s32 *, 4));
    asm volatile(
        "lsl %0, %0, #16\n"
        "asr %0, %0, #16\n"
        "add %0, %0, %1"
        : "+r"(accuracy_bonus_percent)
        : "r"(delta));
    {
        register s32 result asm("r0");
        asm volatile(
            "movs r1, #12\n"
            "ldrsh r0, [r5, r1]\n"
            "lsl %1, %1, #16\n"
            "asr %1, %1, #16\n"
            "add r1, %1, #0\n"
            "bl func_080E522C"
            : "=r"(result), "+r"(accuracy_bonus_percent)
            :
            : "r1", "r2", "r3", "lr", "cc", "memory");
        M2C_FIELD(weapon_stats, s16 *, 0xC) = (u16) M2C_FIELD(weapon_stats, s16 *, 0xC) + result;
    }
    {
        u8 *required_pilot_level = &M2C_FIELD(unit, u8 *, 0x39);
        if ((u32) effective_pilot_level < (u32) *required_pilot_level) {
            M2C_FIELD(weapon_stats, s16 *, 0xC) = DivideSigned32(M2C_FIELD(weapon_stats, s16 *, 0xC) * effective_pilot_level, *required_pilot_level);
        }
    }
    if (auxiliary_pilot != 0) {
        register s32 result asm("r0");
        asm volatile(
            "movs r1, #10\n"
            "ldrsh r0, [r5, r1]\n"
            "mov r2, r9\n"
            "movs r3, #44\n"
            "ldrsh r1, [r2, r3]\n"
            "bl func_080E522C"
            : "=r"(result)
            :
            : "r1", "r2", "r3", "lr", "cc", "memory");
        M2C_FIELD(weapon_stats, s16 *, 0xA) = (u16) M2C_FIELD(weapon_stats, s16 *, 0xA) + result;
    }
    if (M2C_FIELD(weapon_stats, s16 *, 0x10) != 0) {
        if ((FindAbilityValue(pilot, PILOT_ABILITY_RANGED_EP_SAVING_1, M2C_FIELD(weapon_stats, s32 *, 4)) << 0x10) != 0) {
            M2C_FIELD(weapon_stats, s16 *, 0x10) = ScaleByPercent(M2C_FIELD(weapon_stats, s16 *, 0x10), 0x5A);
        }
        if ((FindAbilityValue(pilot, PILOT_ABILITY_RANGED_EP_SAVING_2, M2C_FIELD(weapon_stats, s32 *, 4)) << 0x10) != 0) {
            M2C_FIELD(weapon_stats, s16 *, 0x10) = ScaleByPercent(M2C_FIELD(weapon_stats, s16 *, 0x10), 0x46);
        }
        if ((FindAbilityValue(pilot, PILOT_ABILITY_RANGED_EP_SAVING_3, M2C_FIELD(weapon_stats, s32 *, 4)) << 0x10) != 0) {
            M2C_FIELD(weapon_stats, s16 *, 0x10) = ScaleByPercent(M2C_FIELD(weapon_stats, s16 *, 0x10), 0x32);
        }
        if ((FindAbilityValue(pilot, PILOT_ABILITY_MELEE_EP_SAVING_1, M2C_FIELD(weapon_stats, s32 *, 4)) << 0x10) != 0) {
            M2C_FIELD(weapon_stats, s16 *, 0x10) = ScaleByPercent(M2C_FIELD(weapon_stats, s16 *, 0x10), 0x5A);
        }
        if ((FindAbilityValue(pilot, PILOT_ABILITY_MELEE_EP_SAVING_2, M2C_FIELD(weapon_stats, s32 *, 4)) << 0x10) != 0) {
            M2C_FIELD(weapon_stats, s16 *, 0x10) = ScaleByPercent(M2C_FIELD(weapon_stats, s16 *, 0x10), 0x46);
        }
        if ((FindAbilityValue(pilot, PILOT_ABILITY_MELEE_EP_SAVING_3, M2C_FIELD(weapon_stats, s32 *, 4)) << 0x10) != 0) {
            M2C_FIELD(weapon_stats, s16 *, 0x10) = ScaleByPercent(M2C_FIELD(weapon_stats, s16 *, 0x10), 0x32);
        }
        if (M2C_FIELD(weapon_stats, s16 *, 0x10) == 0) {
            M2C_FIELD(weapon_stats, s16 *, 0x10) = 1;
        }
    }
    if ((s32) M2C_FIELD(weapon_stats, s16 *, 0xA) > 0x270F) {
        M2C_FIELD(weapon_stats, s16 *, 0xA) = 0x270F;
    }
    {
        u16 accuracy_limit = M2C_FIELD(weapon_stats, u16 *, 0xE);
        if ((s32) M2C_FIELD(weapon_stats, s16 *, 0xC) > (s32) (s16) accuracy_limit) {
            M2C_FIELD(weapon_stats, s16 *, 0xC) = accuracy_limit;
        }
    }
}
