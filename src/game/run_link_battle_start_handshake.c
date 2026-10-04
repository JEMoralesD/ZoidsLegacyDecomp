#include "m2c_prelude.h"
#include "link_battle.h"
#include "../graphics/screen_effects.h"
#include "../game/game_state.h"

void StartTask(s32, s32) asm("func_08092D8C");
void StopTask(s32) asm("func_08092E0C");
void ConfigureDisplayWindows(s32, s32, s32, s32, s32, s32, s32, s32) asm("func_0809538C");
void StartScreenTransition(s32, s32) asm("func_08096308");
s32 IsScreenTransitionComplete(void) asm("func_0809669C");
void RunMenuScript(s32) asm("func_08098BB4");
void StopLinkConnection(void) asm("func_0809AEA0");
void BeginLinkSend(s32, s32, s32) asm("func_0809AEC0");
u8 PollLinkSend(void) asm("func_0809AEF4");
void BeginLinkReceive(s32, s32, s32) asm("func_0809B00C");
s32 PollLinkReceive(void) asm("func_0809B040");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

extern u8 gLinkBattleStartHandshakePhase asm("D_02032E54");
extern u8 gLinkBattleStartSendComplete asm("D_02032E55");
extern u8 gLinkBattleStartReceiveComplete asm("D_02032E56");
extern u8 gLinkBattleStartLocalDecision asm("D_02032E57");
extern u8 gLinkBattleStartRemoteDecision asm("D_02032E58");
extern s32 gGameMode;

void RunLinkBattleStartHandshake(void) asm("func_080E1438");

void RunLinkBattleStartHandshake(void) {
    s32 receive_result;
    u8 *handshake_phase = &gLinkBattleStartHandshakePhase;

    *handshake_phase = LINK_BATTLE_START_WAIT_FOR_DECISION;
    gLinkBattleStartSendComplete = 0;
    gLinkBattleStartReceiveComplete = 0;
    BeginLinkReceive((s32)&gLinkBattleStartRemoteDecision, 1, 0x081091F0);
    while (1) {
    if (*handshake_phase == LINK_BATTLE_START_SEND_DECISION) {
        BeginLinkSend((s32)&gLinkBattleStartLocalDecision, 1, 0x081091F0);
        *handshake_phase = LINK_BATTLE_START_WAIT_FOR_SEND;
    }
    if (*handshake_phase == LINK_BATTLE_START_WAIT_FOR_SEND) {
        gLinkBattleStartSendComplete = PollLinkSend();
    }
    receive_result = PollLinkReceive();
    gLinkBattleStartReceiveComplete = receive_result;
    if ((gLinkBattleStartSendComplete != 0 && gLinkBattleStartLocalDecision != 0) ||
        ((receive_result << 24) != 0 && gLinkBattleStartRemoteDecision != 0)) {
    StopTask(1);
    StopLinkConnection();
    ConfigureDisplayWindows(1, 0x30C0, 0x2878, 0, 0, 0, 0x2F, 0x3F);
    RunMenuScript(0x08028B81);
    gGameMode = -1;
    StartScreenTransition(SCREEN_TRANSITION_DITHER_CONCEAL, 0);
    goto check_transition;
wait_for_transition:
    YieldTaskForUpdates(1);
check_transition:
    if ((IsScreenTransitionComplete() << 24) == 0) {
        goto wait_for_transition;
    }
    StopTask(3);
    StopTask(4);
    StopTask(5);
    StopTask(6);
    StopTask(7);
    StopTask(8);
    gGameMode = GAME_MODE_OPTIONS;
    StartTask(1, 0x0809A0B1);
    return;
    }
    YieldTaskForUpdates(1);
    }
}
