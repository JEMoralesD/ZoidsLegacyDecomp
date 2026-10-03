#include "m2c_prelude.h"
#include "../battle/battle.h"
extern struct EquipmentRecord gEquipmentCatalog[] asm("D_087B2524");
s32 func_80E523C(u8);

s32 LoadZoidEquipmentStats(u8 *, u8, void *) asm("func_080E58DC");

s32 LoadZoidEquipmentStats(u8 *zoid, u8 equipment_slot, void *output_stats) {
    u16 *item_id = (u16 *)(zoid + equipment_slot * 4 + 0x52);
    if (*item_id == 0) {
        return 0;
    }
    *(struct EquipmentRecord *)output_stats = gEquipmentCatalog[*item_id];
    if (!(*(u16 *)((u8 *)output_stats + 2) & 1)) {
        if (equipment_slot > 3) {
            s32 t = ((u32)(func_80E523C(*zoid) << 0x18) >> 0x16) - 4;
            s32 off = equipment_slot + t;
            u8 *base = zoid + 0x1E;
            *(u16 *)((u8 *)output_stats + 0xA) = (u16)(base[off] * 0x14 + *(u16 *)((u8 *)output_stats + 0xA));
        }
        if ((s16)*(u16 *)((u8 *)output_stats + 0xA) > 0x270F) {
            *(u16 *)((u8 *)output_stats + 0xA) = 0x270F;
        }
        if ((s16)*(u16 *)((u8 *)output_stats + 0xC) > (s16)*(u16 *)((u8 *)output_stats + 0xE)) {
            *(u16 *)((u8 *)output_stats + 0xC) = *(u16 *)((u8 *)output_stats + 0xE);
        }
    }
    return 1;
}
