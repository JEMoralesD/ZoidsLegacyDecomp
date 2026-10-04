#include "player_selection.h"
M2C_UNK PrintWindowTextAt(M2C_UNK, s32, s32, s32, u32) asm("func_080981F0");
M2C_UNK PrintWindowText(M2C_UNK, s32, s32) asm("func_08098248");
M2C_UNK PrintWindowNumberAt(u8, s32, s32, s32, s32, s32, u32) asm("func_0809844C");
M2C_UNK PrintWindowNumber(s16, s32, s32, s32, s32) asm("func_080984C4");
M2C_UNK ClearWindow(s32) asm("func_80986B4");
s32 GetZoidFormIndex(u8) asm("func_080E523C");
M2C_UNK BuildZoidEquipmentStats(u8 *, M2C_UNK, s32, u8, void *) asm("func_080E59BC");
void *AcquireEquipmentStatBuffer() asm("func_80E669C");
M2C_UNK ReleaseEquipmentStatBuffer() asm("func_80E66B8");

void DrawZoidOptionalWeaponPower(u8 *zoid_address, M2C_UNK pilot_address) asm("func_080B8F44");

void DrawZoidOptionalWeaponPower(u8 *zoid_address, M2C_UNK pilot_address) {
    u8 optional_slot;
    int equipment_slot;
    int equipment_offset;
    void *equipment_stats;

    equipment_stats = AcquireEquipmentStatBuffer();
    ClearWindow(6);
    optional_slot = 0;
    do {
        equipment_slot = optional_slot + 4;
        equipment_offset = equipment_slot * 4;
        if (M2C_FIELD((zoid_address + equipment_offset), u16 *, PLAYER_ZOID_OFFSET(equipment[0].item_id)) != 0) {
            BuildZoidEquipmentStats(zoid_address, pilot_address, 0, equipment_slot, equipment_stats);
            if (!(1 & EQUIPMENT_RECORD_FIELD(equipment_stats, u16, flags))) {
                u8 *power_bonuses;
                u32 form_bonus_index;
                PrintWindowTextAt(OPTIONAL_WEAPON_POWER_LABEL_ROM, 0, 6, 0, optional_slot);
                PrintWindowNumber(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, power_or_value), 4, 0, 0xA, 6);
                form_bonus_index = optional_slot + ((u32)(GetZoidFormIndex(*zoid_address) << 0x18) >> 0x16);
                power_bonuses = zoid_address + PLAYER_ZOID_OFFSET(form_weapon_power_bonuses);
                PrintWindowNumberAt(power_bonuses[form_bonus_index], 1, 0, 0xA, 6, 9, optional_slot);
                PrintWindowText(OPTIONAL_WEAPON_POWER_BONUS_LIMIT_TEXT_ROM, 0, 6);
            }
        }
        optional_slot = optional_slot + 1;
    } while (optional_slot <= 3);
    ReleaseEquipmentStatBuffer();
}

void DrawZoidOptionalWeaponHitRate(u8 *zoid_address, M2C_UNK pilot_address) asm("func_080B8FF8");

void DrawZoidOptionalWeaponHitRate(u8 *zoid_address, M2C_UNK pilot_address) {
    u8 optional_slot;
    int equipment_slot;
    int equipment_offset;
    void *equipment_stats;

    equipment_stats = AcquireEquipmentStatBuffer();
    ClearWindow(6);
    optional_slot = 0;
    do {
        equipment_slot = optional_slot + 4;
        equipment_offset = equipment_slot * 4;
        if (M2C_FIELD((zoid_address + equipment_offset), u16 *, PLAYER_ZOID_OFFSET(equipment[0].item_id)) != 0) {
            BuildZoidEquipmentStats(zoid_address, pilot_address, 0, equipment_slot, equipment_stats);
            if (!(1 & EQUIPMENT_RECORD_FIELD(equipment_stats, u16, flags))) {
                PrintWindowTextAt(OPTIONAL_WEAPON_HIT_RATE_LABEL_ROM, 0, 6, 0, optional_slot);
                PrintWindowNumber(EQUIPMENT_RECORD_FIELD(equipment_stats, s16, accuracy_or_secondary_value), 3, 0, 0xA, 6);
                PrintWindowText(OPTIONAL_WEAPON_HIT_RATE_SUFFIX_ROM, 0, 6);
            }
        }
        optional_slot = optional_slot + 1;
    } while (optional_slot <= 3);
    ReleaseEquipmentStatBuffer();
}
