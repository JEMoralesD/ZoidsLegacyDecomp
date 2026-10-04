#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
extern u8 gBattleState[];
extern u8 D_000027BE[];
extern u8 gBattleSceneSide asm("D_02033F36");

extern void DestroySpriteGroup(s32) asm("func_08095114");
extern void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
extern void PlayBattlePhalanxVolleyAnimation(void) asm("func_080CD110");
extern void StartBattleEquipmentAnimation(u8, u8, s32, u8, s32) asm("func_080D0CA0");
extern s32 GetBattleAnimationResult(void) asm("func_080D1C18");
extern void StartBattleScanlineWindowClosing(void) asm("func_080D1D44");
extern u8 GetBattleScanlineWindowPhase(void) asm("func_080D1E38");
extern void RequestBattleBackgroundShakeStop(void) asm("func_080D2200");
extern s32 IsBattleBackgroundShakeFinished(void) asm("func_080D222C");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventPlayBattleEquipmentAnimation(u8 script_slot, void **script_cursor) asm("func_080A3640");

s32 EventPlayBattleEquipmentAnimation(u8 script_slot, void **script_cursor) {
    u8 *command;
    u8 *persistent_deck_commands;
    u8 *scene_side_address;
    int item_id_high_bits;

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
    StartBattleEquipmentAnimation(*scene_side_address, *(u8 *)0x020317D6, (command = (u8 *)*script_cursor, item_id_high_bits = command[3] << 8, command[2] | item_id_high_bits), command[1], 0);
    while ((GetBattleAnimationResult() << 0x18) == 0) {
        YieldTaskForUpdates(1);
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
