#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
extern u8 gBattleState[];
extern u8 D_000027BE[];
extern u8 gBattleSceneSide asm("D_02033F36");
extern u8 gBattleSpeedLinesPending asm("D_020317D9");
extern u8 gZoidBaseStatTable[];
struct ZoidBaseEquipmentItemView {
    u8 reserved00[0x1A];
    u16 item_id;
};

extern void DestroySpriteGroup(s32) asm("func_08095114");
extern void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
extern void PlayBattlePhalanxVolleyAnimation(void) asm("func_080CD110");
extern void StartBattleEquipmentAnimation(u8, u8, s32, u8, s32) asm("func_080D0CA0");
extern s32 GetBattleAnimationResult(void) asm("func_080D1C18");
extern void StartBattleScanlineWindowClosing(void) asm("func_080D1D44");
extern u8 GetBattleScanlineWindowPhase(void) asm("func_080D1E38");
extern void RequestBattleBackgroundShakeStop(void) asm("func_080D2200");
extern s32 IsBattleBackgroundShakeFinished(void) asm("func_080D222C");
extern void StartBattleSpeedLineBackground(void) asm("func_080D18E4");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventPlayBattleMountedEquipmentAnimation(u8 script_slot, void **script_cursor) asm("func_080A3704");

s32 EventPlayBattleMountedEquipmentAnimation(u8 script_slot, void **script_cursor) {
    u8 *command;
    u8 *persistent_deck_commands;
    u8 *scene_side_address;
    u8 scene_side;
    u8 zoid_model_id;
    u8 equipment_slot;
    u16 item_id;
    u8 *zoid_base_records;
    u32 equipment_record_offset;

    if (GetBattleScanlineWindowPhase() == 1) {
        StartBattleScanlineWindowClosing();
        while ((u32)GetBattleScanlineWindowPhase() <= 1U) {
            YieldTaskForUpdates(1);
        }
    }
    if ((IsBattleBackgroundShakeFinished() << 0x18) == 0) {
        RequestBattleBackgroundShakeStop();
        while ((IsBattleBackgroundShakeFinished() << 0x18) == 0) {
            YieldTaskForUpdates(1);
        }
    }
    persistent_deck_commands = gBattleState;
    scene_side_address = &gBattleSceneSide;
    persistent_deck_commands += (int)D_000027BE;
    if (persistent_deck_commands[*scene_side_address] != 0) {
        DestroySpriteGroup(*(s32 *)0x020316F8);
        PlayBattlePhalanxVolleyAnimation();
    }
    if (gBattleSpeedLinesPending != 0) {
        StartBattleSpeedLineBackground();
        gBattleSpeedLinesPending = 0;
    }
    scene_side = *scene_side_address;
    zoid_model_id = *(u8 *)0x020317D6;
    zoid_base_records = gZoidBaseStatTable;
    command = (u8 *)*script_cursor;
    equipment_slot = command[1];
    equipment_record_offset = equipment_slot << 2;
    equipment_record_offset += zoid_model_id * 0x38;
    item_id = ((struct ZoidBaseEquipmentItemView *)(zoid_base_records + equipment_record_offset))->item_id;
    StartBattleEquipmentAnimation(scene_side, zoid_model_id, item_id, equipment_slot, 0);
    while ((GetBattleAnimationResult() << 0x18) == 0) {
        YieldTaskForUpdates(1);
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
