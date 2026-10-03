#include "m2c_prelude.h"
#include "battle.h"
extern u8 gBattleState[];

void ApplyBattleStatEffects(u8 side, u8 unit_slot) {
    u32 outer_offset;
    u32 row_offset;
    u32 entry_offset;
    register u32 type asm("r1");
    register u32 mask asm("r0");
    u8 *entry;
    register u16 *field asm("r1");
    register void *object asm("r12");
    u8 index;
    register u32 base asm("r2");

    outer_offset = side << 2;
    outer_offset += side;
    outer_offset <<= 3;
    outer_offset -= side;
    outer_offset <<= 7;
    row_offset = unit_slot * 0x270;
    base = (u32)gBattleState;
    row_offset += base;
    outer_offset += row_offset;
    object = (void *)outer_offset;
    index = 0;
    do {
        outer_offset = side << 2;
        outer_offset += side;
        outer_offset <<= 3;
        outer_offset -= side;
        outer_offset <<= 7;
        row_offset = unit_slot * 0x270;
        row_offset += base;
        outer_offset += row_offset;
        entry_offset = index * 0xC;
        entry_offset += 0xE4;
        entry = (u8 *)(outer_offset + entry_offset);
        if (!(*(u32 *)entry & 0x1FFF0000)) {
            type = *(u16 *)(entry + 4);
            asm volatile("" : "+r"(type));
            mask = BATTLE_EFFECT_KIND_MASK;
            mask &= type;
            mask -= BATTLE_EFFECT_MAX_HP;
            switch (mask) {
            case BATTLE_EFFECT_MAX_HP - BATTLE_EFFECT_MAX_HP: {
                register u16 value asm("r0");
                register u16 other asm("r1");
                register u16 sum asm("r0");
                register void *read_object asm("r1");
                register void *write_object asm("r3");

                value = M2C_FIELD(entry, u16 *, 6);
                read_object = object;
                other = M2C_FIELD(read_object, u16 *, 0x3A);
                sum = value;
                asm volatile("" : "+r"(sum));
                sum += other;
                write_object = object;
                M2C_FIELD(write_object, u16 *, 0x3A) = sum;
                break;
            }
            case BATTLE_EFFECT_DCP - BATTLE_EFFECT_MAX_HP: {
                register u16 value asm("r0");
                register u16 other asm("r1");
                register u16 sum asm("r0");
                register void *read_object asm("r1");
                register void *write_object asm("r3");

                value = M2C_FIELD(entry, u16 *, 6);
                read_object = object;
                other = M2C_FIELD(read_object, u16 *, 0x3C);
                sum = value;
                asm volatile("" : "+r"(sum));
                sum += other;
                write_object = object;
                M2C_FIELD(write_object, u16 *, 0x3C) = sum;
                break;
            }
            case BATTLE_EFFECT_MAX_EP - BATTLE_EFFECT_MAX_HP: {
                register u16 value asm("r0");
                register u16 other asm("r1");
                register u16 sum asm("r0");
                register void *read_object asm("r1");
                register void *write_object asm("r3");

                value = M2C_FIELD(entry, u16 *, 6);
                read_object = object;
                other = M2C_FIELD(read_object, u16 *, 0x3E);
                sum = value;
                asm volatile("" : "+r"(sum));
                sum += other;
                write_object = object;
                M2C_FIELD(write_object, u16 *, 0x3E) = sum;
                break;
            }
            case BATTLE_EFFECT_EP_REGEN - BATTLE_EFFECT_MAX_HP:
                field = (u16 *)(object + 0x40);
                goto add_field;
            case BATTLE_EFFECT_SPEED - BATTLE_EFFECT_MAX_HP:
                field = (u16 *)(object + 0x42);
                goto add_field;
            case BATTLE_EFFECT_MOBILITY - BATTLE_EFFECT_MAX_HP:
                field = (u16 *)(object + 0x44);
                goto add_field;
            case BATTLE_EFFECT_DEFENSE - BATTLE_EFFECT_MAX_HP:
                field = (u16 *)(object + 0x46);
                goto add_field;
            case BATTLE_EFFECT_ARMOR_RATE - BATTLE_EFFECT_MAX_HP:
                field = (u16 *)(object + 0x48);
                goto add_field;
            case BATTLE_EFFECT_SENSOR_ACCURACY - BATTLE_EFFECT_MAX_HP:
                field = (u16 *)(object + 0x4A);
                goto add_field;
            case BATTLE_EFFECT_LOAD_CAPACITY - BATTLE_EFFECT_MAX_HP:
                field = (u16 *)(object + 0x4C);
add_field:
                {
                    register u16 value asm("r0");
                    register u16 other asm("r3");
                    register u16 sum asm("r0");

                    value = *(u16 *)(entry + 6);
                    other = *field;
                    sum = value;
                    asm volatile("" : "+r"(sum));
                    sum += other;
                    *field = sum;
                }
                break;
            }
        }
        index++;
    } while (index <= (BATTLE_EFFECT_SLOT_COUNT - 1));
}
