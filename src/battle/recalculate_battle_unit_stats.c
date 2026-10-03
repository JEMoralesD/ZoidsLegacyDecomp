#include "m2c_prelude.h"
#include "battle.h"
extern u8 gBattleState[];
extern u8 gBattleSetup[];

u8 FindBattleEffect(u8, u8, s32) asm("func_080BF464");
M2C_UNK ApplyBattleStatEffects(u8, u8) asm("func_080BFD20");
M2C_UNK ApplyBattleDerivedStatEffects(u8, u8) asm("func_080BFE30");
M2C_UNK LoadZoidBaseStats(void *) asm("func_080E5538");
M2C_UNK ApplyEquipmentWeightPenalty(void *) asm("func_080E5674");
M2C_UNK CalculateZoidDerivedStats(void *, void *, s32) asm("func_080E570C");
M2C_UNK func_080E57D0(void *);
M2C_UNK func_080E6830(void *, void *, s32);

void RecalculateBattleUnitStats(s32 side_arg, s32 unit_slot_arg) {
    s16 temp_r1;
    s32 auxiliary_pilot;
    register u32 temp_r0 asm("r0");
    u8 unit_slot;
    u8 side;
    void *unit;
    void *pilot;

    side_arg <<= 24;
    side = (u32)side_arg >> 24;
    unit_slot_arg <<= 24;
    unit_slot = (u32)unit_slot_arg >> 24;
    {
        u32 row_offset;
        u32 col_offset;
        u8 *base;

        row_offset = side * 0x1380;
        col_offset = unit_slot * 0x270;
        base = (u8 *)0x02034B4C;
        asm volatile("" : "+r"(base));
        col_offset += (u32)base;
        unit = row_offset + col_offset;
    }
    if (FindBattleEffect(side, unit_slot, BATTLE_EFFECT_PILOT_INACTIVE) == BATTLE_EFFECT_NOT_FOUND) {
        pilot = unit + 0x70;
    } else {
        pilot = 0;
    }
    if (M2C_FIELD(pilot, u8 *, 0x31) == 0) {
        goto no_aux;
    }
    if (FindBattleEffect(side, unit_slot, BATTLE_EFFECT_AUXILIARY_PILOT_ACTIVE) == BATTLE_EFFECT_NOT_FOUND) {
        goto no_aux;
    }
    {
        register s32 selected asm("r7");

        selected = (s32)unit + 0xB0;
        asm volatile("" : "+r"(selected));
        auxiliary_pilot = selected;
    }
    goto aux_done;
no_aux:
    {
        register s32 selected asm("r7");

        selected = 0;
        asm volatile("" : "+r"(selected));
        auxiliary_pilot = selected;
    }
aux_done:
    LoadZoidBaseStats(unit);
    ApplyBattleStatEffects(side, unit_slot);
    ApplyEquipmentWeightPenalty(unit);
    func_080E6830(unit, pilot, auxiliary_pilot);
    if ((gBattleSetup[1] == 0xB) && !(0xC0 & M2C_FIELD(unit, u8 *, 0x36))) {
        temp_r1 = M2C_FIELD(unit, s16 *, 0x44);
        M2C_FIELD(unit, s16 *, 0x44) = (s16)((s32)(temp_r1 + ((u32)temp_r1 >> 31)) >> 1);
    }
    temp_r0 = FindBattleEffect(side, unit_slot, BATTLE_EFFECT_SENSOR_ACCURACY_OVERRIDE);
    if (temp_r0 != BATTLE_EFFECT_NOT_FOUND) {
        register u32 offset asm("r1");
        register u32 work asm("r0");
        register u8 *base asm("r2");

        base = gBattleState;
        asm volatile("" : "+r"(base));
        offset = temp_r0 * 0xC;
        work = unit_slot * 0x270;
        offset += work;
        work = side * 0x1380;
        offset += work;
        offset += (u32)base;
        offset += 0xEA;
        temp_r0 = *(u16 *)offset;
        {
            register u16 *dest asm("r2");

            dest = (u16 *)((u8 *)unit + 0x4A);
            *dest = temp_r0;
        }
    }
    func_080E57D0(unit);
    CalculateZoidDerivedStats(unit, pilot, auxiliary_pilot);
    ApplyBattleDerivedStatEffects(side, unit_slot);
    func_080E57D0(unit);
    if ((s32)M2C_FIELD(unit, s16 *, 6) > (s32)(s16)M2C_FIELD(unit, u16 *, 0x3A)) {
        M2C_FIELD(unit, s16 *, 6) = (s16)M2C_FIELD(unit, u16 *, 0x3A);
    }
    if ((s32)M2C_FIELD(unit, s16 *, 8) > (s32)(s16)M2C_FIELD(unit, u16 *, 0x3E)) {
        M2C_FIELD(unit, s16 *, 8) = (s16)M2C_FIELD(unit, u16 *, 0x3E);
    }
}
