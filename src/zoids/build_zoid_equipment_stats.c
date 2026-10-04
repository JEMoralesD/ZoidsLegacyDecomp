#include "m2c_prelude.h"
#include "../game/player_state.h"
#include "../battle/battle.h"
s32 LoadZoidEquipmentStats(s32, u8, void *) asm("func_80E58DC");                     /* extern */
M2C_UNK ApplyZoidWeaponWeightPenalty(s32, void *) asm("func_80E59A0");                     /* extern */
M2C_UNK ApplyPilotWeaponModifiers(s32, s32, M2C_UNK, u16, void *) asm("func_080E6994");  /* extern */

s32 BuildZoidEquipmentStats(s32 zoid_address, s32 pilot_address, M2C_UNK auxiliary_pilot_address, u8 equipment_slot, void *output_stats) asm("func_080E59BC");

s32 BuildZoidEquipmentStats(s32 zoid_address, s32 pilot_address, M2C_UNK auxiliary_pilot_address, u8 equipment_slot, void *output_stats) {
    u8 slot_index;
    s32 equipment_slot_offset;

    slot_index = equipment_slot;
    if ((LoadZoidEquipmentStats(zoid_address, slot_index, output_stats) << 0x18) != 0) {
        if (!(EQUIPMENT_COMMAND & EQUIPMENT_RECORD_FIELD(output_stats, u16, flags))) {
            ApplyZoidWeaponWeightPenalty(zoid_address, output_stats);
            equipment_slot_offset = slot_index * 4;
            ApplyPilotWeaponModifiers(zoid_address, pilot_address, auxiliary_pilot_address, M2C_FIELD((zoid_address + equipment_slot_offset), u16 *, PLAYER_ZOID_OFFSET(equipment[0].item_id)), output_stats);
        }
        return 1;
    }
    return 0;
}
