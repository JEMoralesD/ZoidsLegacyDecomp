#include "m2c_prelude.h"
#include "../battle/battle.h"

void ApplyEquipmentStatBonuses(void *zoid) {
    u8 *base = (u8 *)zoid;
    u8 equipment_slot;
    u32 equipment_effect;

    equipment_slot = 0;
    do {
        register s32 sv asm("r0");
        register u16 *slot asm("r1");

        sv = equipment_slot << 2;
        asm volatile("" : "+r"(sv));
        sv = (s32)base + sv;
        asm volatile("" : "+r"(sv));
        slot = (u16 *)sv;
        asm volatile("" : "+r"(slot));
        slot = (u16 *)((u8 *)slot + 0x52);
        sv = *slot;
        if (sv != 0) {
            register s32 t asm("r1");
            register s32 off asm("r0");
            register u8 *cat asm("r1");
            u8 *e;

            t = sv;
            asm volatile("" : "+r"(t));
            off = t << 1;
            off += t;
            off <<= 3;
            cat = (u8 *)0x087B2524;
            asm volatile("" : "+r"(cat));
            e = (u8 *)(off + (s32)cat);
            if ((*(u16 *)(e + 2) & 7) == 3
                && *(u8 *)(e + 9) == 6
                && (*(u32 *)(e + 4) & 0x1F000000) == 0) {
                equipment_effect = *(u32 *)(e + 4);
                if ((equipment_effect & EQUIPMENT_EFFECT_KIND_MASK) != 0x16) {
                    switch (equipment_effect & EQUIPMENT_EFFECT_KIND_MASK) {
                    case EQUIPMENT_EFFECT_DEFENSE_AND_ARMOR_RATE:
                        *(u16 *)(base + 0x48) += *(u16 *)(e + 12);
                    case EQUIPMENT_EFFECT_DEFENSE:
                        *(u16 *)(base + 0x46) += *(u16 *)(e + 10);
                        break;
                    case EQUIPMENT_EFFECT_SPEED_AND_MOBILITY:
                        *(u16 *)(base + 0x44) += *(u16 *)(e + 12);
                    case EQUIPMENT_EFFECT_SPEED:
                        *(u16 *)(base + 0x42) += *(u16 *)(e + 10);
                        break;
                    case EQUIPMENT_EFFECT_MOBILITY:
                        *(u16 *)(base + 0x44) += *(u16 *)(e + 10);
                        break;
                    case EQUIPMENT_EFFECT_SENSOR_ACCURACY:
                    case EQUIPMENT_EFFECT_SENSOR_ACCURACY_AND_HIT_RATE:
                        *(u16 *)(base + 0x4A) += *(u16 *)(e + 10);
                        break;
                    case EQUIPMENT_EFFECT_EP_REGEN:
                        *(u16 *)(base + 0x40) += *(u16 *)(e + 10);
                        break;
                    case EQUIPMENT_EFFECT_MAX_HP_AND_EP:
                        *(u16 *)(base + 0x3E) += *(u16 *)(e + 12);
                    case EQUIPMENT_EFFECT_MAX_HP:
                        *(u16 *)(base + 0x3A) += *(u16 *)(e + 10);
                        break;
                    case EQUIPMENT_EFFECT_MAX_EP:
                        *(u16 *)(base + 0x3E) += *(u16 *)(e + 10);
                        break;
                    }
                }
            }
        }
        equipment_slot = equipment_slot + 1;
    } while (equipment_slot <= 7);
}
