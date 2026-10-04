#include "m2c_prelude.h"
#include "../battle/battle.h"

struct AbilitySet {
    u8 pad[8];
    u8 ability_kinds[10];
    s16 ability_values[10];
};

s32 FindAbilityValue(struct AbilitySet *ability_set, s32 ability_kind, s32 ability_context) {
    register struct AbilitySet *abilities asm("ip") = ability_set;
    register s32 context asm("r4") = ability_context;
    register u32 kind asm("r9") = (u8)ability_kind;
    register u32 ability_slot asm("r6") = 0;
    register u32 fallback asm("r8");

scan_entries:
    if (abilities->ability_kinds[ability_slot] != kind) {
        goto increment;
    }
    fallback = 0;
    switch (kind - 3) {
        case 0:
        case 1:
        case 2: {
            register u32 scan asm("r2") = 0;
            register u8 *table asm("r5") = (u8 *)0x087A5810;
            register u32 entry_offset asm("r1") = ability_slot << 1;
            register s16 *entry_base asm("r0") = abilities->ability_values;
            s16 *entry = (s16 *)((u8 *)entry_base + entry_offset);
            register s32 row asm("r1") = *entry;
            register s32 offset asm("r0") = row * 0x1A;

            if (*(u8 *)(offset + (s32)table) != context) {
                register s16 *loop_entry asm("r1") = entry;
                register s32 stride asm("r3") = 0x1A;

                do {
                    register u32 next asm("r0") = scan + 1;
                    next <<= 24;
                    scan = next >> 24;
                    if (scan > 0x19) {
                        break;
                    }
                } while (*(u8 *)(scan + (*loop_entry * stride) +
                                 (s32)table) != context);
            }
            if (scan == 0x1A) {
                goto check_fallback;
            }
            goto return_entry;
        }
        case PILOT_ABILITY_RANGED_EP_SAVING_1 - 3:
        case PILOT_ABILITY_RANGED_EP_SAVING_2 - 3:
        case PILOT_ABILITY_RANGED_EP_SAVING_3 - 3:
        case PILOT_ABILITY_RANGED_ACCURACY_PENALTY - 3:
        case PILOT_ABILITY_RANGED_EVASION_BONUS - 3:
            if ((context & WEAPON_MELEE) != 0) {
                goto check_fallback;
            }
            goto return_entry;
        case PILOT_ABILITY_MELEE_EP_SAVING_1 - 3:
        case PILOT_ABILITY_MELEE_EP_SAVING_2 - 3:
        case PILOT_ABILITY_MELEE_EP_SAVING_3 - 3:
        case 17:
        case PILOT_ABILITY_MELEE_ACCURACY_PENALTY - 3:
        case PILOT_ABILITY_MELEE_EVASION_BONUS - 3:
        case PILOT_ABILITY_MELEE_ACCURACY_BONUS - 3:
            if ((context & WEAPON_MELEE) == 0) {
                goto check_fallback;
            }
            goto return_entry;
        case 22:
        case 23:
        case 31:
        case 32:
            goto check_fallback;
        case PILOT_ABILITY_MISSILE_ACCURACY_PENALTY - 3:
        case PILOT_ABILITY_MISSILE_ACCURACY_BONUS - 3:
            if ((context & WEAPON_MISSILE) == 0) {
                goto check_fallback;
            }
            goto return_entry;
        case PILOT_ABILITY_LASER_ACCURACY_PENALTY - 3:
        case PILOT_ABILITY_LASER_ACCURACY_BONUS - 3:
            if ((context & WEAPON_LASER) == 0) {
                goto check_fallback;
            }
            goto return_entry;
        case PILOT_ABILITY_PARTICLE_ACCURACY_PENALTY - 3:
        case PILOT_ABILITY_PARTICLE_ACCURACY_BONUS - 3:
            if ((context & WEAPON_PARTICLE) == 0) {
                goto check_fallback;
            }
            goto return_entry;
        case PILOT_ABILITY_BALLISTIC_ACCURACY_PENALTY - 3:
        case PILOT_ABILITY_BALLISTIC_ACCURACY_BONUS - 3:
            if ((context & WEAPON_BALLISTIC) == 0) {
                goto check_fallback;
            }
            goto return_entry;
        default:
            fallback = 1;
            goto check_fallback;
    }

check_fallback:
    if (fallback == 0) {
        goto increment;
    }

return_entry:
    {
        register u32 entry_offset asm("r0") = ability_slot << 1;
        register s16 *entry_base asm("r1") = abilities->ability_values;
        register s16 *entry asm("r1") =
            (s16 *)((u8 *)entry_base + entry_offset);
        register s32 value asm("r0") = *entry;
        if (value == 0) {
            value = 1;
        }
        return value;
    }

increment:
    {
        register u32 next asm("r0") = ability_slot + 1;
        next <<= 24;
        ability_slot = next >> 24;
    }
    if (ability_slot <= 9) {
        goto scan_entries;
    }
    return 0;
}
