#include "m2c_prelude.h"
#include "../battle/battle.h"

u16 ScaleByPercent(s16, s16) asm("func_080E522C");
s32 FindAbilityValue(s32, s32, s32) asm("func_080E74F0");

struct DerivedStatEquipmentRecord {
    u16 data00;
    u16 flags;
    u32 attributes;
    u8 range;
    u8 area_type;
    u16 value;
    u8 rest[12];
};

struct Zoid {
    u8 pad0[0xA];
    u16 initiative;
    u16 evasion_score;
    u8 padE[0x34];
    s16 speed;
    s16 mobility;
    u8 pad46[4];
    u16 sensor_accuracy;
    u8 pad4C[4];
    struct EquipmentSlot equipment[8];
};

extern struct DerivedStatEquipmentRecord gEquipmentCatalog[] asm("D_087B2524");

void CalculateZoidDerivedStats(struct Zoid *zoid, s32 abilities) {
    u8 equipment_slot;

    zoid->evasion_score = ScaleByPercent(zoid->speed, zoid->mobility);
    equipment_slot = 0;
    do {
        register s32 slot_value asm("r0");
        register u16 *slot asm("r1");

        slot_value = equipment_slot << 2;
        asm volatile("" : "+r"(slot_value));
        slot_value = (s32)zoid + slot_value;
        asm volatile("" : "+r"(slot_value));
        slot = (u16 *)slot_value;
        asm volatile("" : "+r"(slot));
        slot = (u16 *)((u8 *)slot + 0x52);
        asm volatile("" : "+r"(slot));
        slot_value = *slot;
        asm volatile("" : "+r"(slot_value));
        if (slot_value != 0) {
            register struct DerivedStatEquipmentRecord *item asm("r2");

            {
                register s32 item_index asm("r1");

                item_index = slot_value;
                asm volatile("" : "+r"(item_index));
                slot_value = item_index << 1;
                slot_value += item_index;
                slot_value <<= 3;
                asm volatile("" : "+r"(slot_value));
            }
            {
                register struct DerivedStatEquipmentRecord *base asm("r1");

                base = gEquipmentCatalog;
                asm volatile("" : "+r"(base));
                item = (struct DerivedStatEquipmentRecord *)(slot_value + (s32)base);
                asm volatile("" : "+r"(item));
            }
            if ((7 & item->flags) == 3 && item->area_type == 6) {
                u32 attributes;

                attributes = item->attributes;
                if (!(BATTLE_CONDITION_WEAPON_TYPE_MASK & attributes)) {
                    u32 low;

                    low = EQUIPMENT_EFFECT_KIND_MASK;
                    low &= attributes;
                    if (low != 0x16) {
                        asm volatile("" : "+r"(low));
                        if (low == EQUIPMENT_EFFECT_EVASION_SCORE) {
                            register u16 value asm("r0");
                            register u16 current asm("r1");

                            value = item->value;
                            asm volatile("" : "+r"(value));
                            current = zoid->evasion_score;
                            asm volatile("" : "+r"(current));
                            value += current;
                            zoid->evasion_score = value;
                        }
                    }
                }
            }
        }
        equipment_slot += 1;
    } while ((u32)equipment_slot <= 7U);

    zoid->initiative = (u16)zoid->speed + zoid->sensor_accuracy;
    if ((FindAbilityValue(abilities, PILOT_ABILITY_ULTRA_REACTION_1, 0) << 16) != 0) {
        register u32 add asm("r2");
        register u32 sum asm("r0");
        register u32 current asm("r1");

        add = 0xFA;
        add <<= 1;
        asm volatile("" : "+r"(add));
        sum = add;
        asm volatile("" : "+r"(sum));
        current = zoid->initiative;
        asm volatile("" : "+r"(current));
        sum += current;
        zoid->initiative = sum;
    }
    if ((FindAbilityValue(abilities, PILOT_ABILITY_ULTRA_REACTION_2, 0) << 16) != 0) {
        register u32 add asm("r2");
        register u32 sum asm("r0");
        register u32 current asm("r1");

        add = 0xFA;
        add <<= 2;
        asm volatile("" : "+r"(add));
        sum = add;
        asm volatile("" : "+r"(sum));
        current = zoid->initiative;
        asm volatile("" : "+r"(current));
        sum += current;
        zoid->initiative = sum;
    }
}
