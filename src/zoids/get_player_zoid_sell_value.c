#include "m2c_prelude.h"
#include "../game/player_state.h"
extern struct PlayerZoidRecordView gPlayerZoidRecords[] asm("D_020218E8");
extern struct ZoidShopRecordView gZoidShopRecords[] asm("D_087B1E04");
extern M2C_UNK ScaleByPercent(s32, s32) asm("func_80E522C");

s32 GetPlayerZoidSellValue(u8 stored_zoid_slot) asm("func_080E67F0");

s32 GetPlayerZoidSellValue(u8 stored_zoid_slot) {
    struct PlayerZoidRecordView *zoid = &gPlayerZoidRecords[stored_zoid_slot];
    s32 zoid_level = zoid->level;
    return ScaleByPercent(gZoidShopRecords[zoid->model_id].sell_value, zoid_level / 4 + 0x64);
}
