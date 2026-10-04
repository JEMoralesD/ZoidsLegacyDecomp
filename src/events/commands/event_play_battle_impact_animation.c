#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"

extern u8 gEventBattleImpactFlags asm("D_020317D7");
extern u8 gBattleSceneSide asm("D_02033F36");
extern u8 gEventBattleZoidModelId asm("D_020317D6");

void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
s32 GetBattleAnimationResult(void) asm("func_080D1C18");
void StartBattleScanlineWindowClosing(void) asm("func_080D1D44");
u8 GetBattleScanlineWindowPhase(void) asm("func_080D1E38");
void RequestBattleBackgroundShakeStop(void) asm("func_080D2200");
s32 IsBattleBackgroundShakeFinished(void) asm("func_080D222C");
void StartBattleImpactAnimation(u8, u8, u16, s32) asm("func_080D0D50");
void StartBattleShieldImpactAnimation(u8, u8, u16, s32) asm("func_080D0F08");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventPlayBattleImpactAnimation(u8 script_slot, u8 **script_cursor) asm("func_080A37E8");

s32 EventPlayBattleImpactAnimation(u8 script_slot, u8 **script_cursor)
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
        s32 item_id_high_bits;

        StartBattleImpactAnimation(gBattleSceneSide, gEventBattleZoidModelId,
            (command = *script_cursor, item_id_high_bits = command[2] << 8, command[1] | item_id_high_bits), 0);
    } else {
        u8 *command;
        s32 item_id_high_bits;

        StartBattleShieldImpactAnimation(gBattleSceneSide, gEventBattleZoidModelId,
            (command = *script_cursor, item_id_high_bits = command[2] << 8, command[1] | item_id_high_bits), 0);
    }
    while ((GetBattleAnimationResult() << 24) == 0) {
        YieldTaskForUpdates(1);
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
