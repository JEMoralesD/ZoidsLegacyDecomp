#include "m2c_prelude.h"
#include "battle.h"

void AppendBattleActionSelection(s32 equipment_slot, s32 target_choice) asm("func_080CA1B4");

void AppendBattleActionSelection(s32 equipment_slot, s32 target_choice) {
    u8 index;
    s32 offset;
    u8 *base;
    u8 *entry;
    u8 *state_ptr;
    u16 *total;

    equipment_slot <<= 24;
    equipment_slot = (u32)equipment_slot >> 24;
    base = (u8 *)0x02034B4C;

    index = base[0x27A4];
    entry = base + (index << 2);
    asm volatile("" : "+r"(entry));
    entry += 0xA07C;
    *entry = 0;

    state_ptr = base + 0xA1AF;
    entry = base + 0xA1B0;
    entry[*state_ptr] = equipment_slot;
    entry = base + 0xA1B3;
    entry[*state_ptr] = target_choice;

    total = (u16 *)0x0203EFB2;
    index = *state_ptr;
    offset = index << 3;
    offset -= index;
    offset <<= 5;
    offset += index;
    offset <<= 2;
    equipment_slot *= 0xA8C;
    offset += equipment_slot;
    offset += (s32)base;
    offset += 0x27D0;
    *total += *(u16 *)offset;
    *state_ptr += 1;
}
