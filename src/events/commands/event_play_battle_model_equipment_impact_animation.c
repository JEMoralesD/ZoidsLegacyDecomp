#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"

extern u8 gEventBattleImpactFlags asm("D_020317D7");
extern u8 gBattleSceneSide asm("D_02033F36");
extern u8 gEventBattleZoidModelId asm("D_020317D6");
extern u8 gZoidBaseStatTable[];

void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
s32 GetBattleAnimationResult(void) asm("func_080D1C18");
void StartBattleScanlineWindowClosing(void) asm("func_080D1D44");
u8 GetBattleScanlineWindowPhase(void) asm("func_080D1E38");
void RequestBattleBackgroundShakeStop(void) asm("func_080D2200");
s32 IsBattleBackgroundShakeFinished(void) asm("func_080D222C");
void StartBattleImpactAnimation(u8, u8, u16, s32) asm("func_080D0D50");
void StartBattleShieldImpactAnimation(u8, u8, u16, s32) asm("func_080D0F08");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventPlayBattleModelEquipmentImpactAnimation(u8 script_slot, u8 **script_cursor) asm("func_080A38B0");

s32 EventPlayBattleModelEquipmentImpactAnimation(u8 script_slot, u8 **script_cursor)
{
    if (GetBattleScanlineWindowPhase() == 1) {
        StartBattleScanlineWindowClosing();
        while ((u32)GetBattleScanlineWindowPhase() <= 1U) {
            YieldTaskForUpdates(1);
        }
    }
    if ((IsBattleBackgroundShakeFinished() << 24) == 0) {
        RequestBattleBackgroundShakeStop();
        while ((IsBattleBackgroundShakeFinished() << 24) == 0) {
            YieldTaskForUpdates(1);
        }
    }
    if ((gEventBattleImpactFlags & 1) == 0) {
        u8 *command;

        StartBattleImpactAnimation(gBattleSceneSide, gEventBattleZoidModelId, ({
            register u8 *zoid_base_records asm("r5") = gZoidBaseStatTable;
            register u32 equipment_entry_address asm("r3");
            register u32 zoid_model_id asm("r4");
            register u32 zoid_record_offset asm("r2");

            command = *script_cursor;
            equipment_entry_address = command[2] << 2;
            zoid_model_id = command[1];
            zoid_record_offset = zoid_model_id << 3;
            zoid_record_offset -= zoid_model_id;
            zoid_record_offset <<= 3;
            equipment_entry_address += zoid_record_offset;
            equipment_entry_address += (u32)zoid_base_records;
            *(u16 *)(equipment_entry_address + 26);
        }), 0);
    } else {
        u8 *command;

        StartBattleShieldImpactAnimation(gBattleSceneSide, gEventBattleZoidModelId, ({
            register u8 *zoid_base_records asm("r5") = gZoidBaseStatTable;
            register u32 equipment_entry_address asm("r3");
            register u32 zoid_model_id asm("r4");
            register u32 zoid_record_offset asm("r2");

            command = *script_cursor;
            equipment_entry_address = command[2] << 2;
            zoid_model_id = command[1];
            zoid_record_offset = zoid_model_id << 3;
            zoid_record_offset -= zoid_model_id;
            zoid_record_offset <<= 3;
            equipment_entry_address += zoid_record_offset;
            equipment_entry_address += (u32)zoid_base_records;
            *(u16 *)(equipment_entry_address + 26);
        }), 0);
    }
    while ((GetBattleAnimationResult() << 24) == 0) {
        YieldTaskForUpdates(1);
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
