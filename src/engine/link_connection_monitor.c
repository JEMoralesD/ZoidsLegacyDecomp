#include "m2c_prelude.h"
#include "link_transfer.h"
#include "../battle/battle_display.h"
#include "../graphics/screen_effects.h"
#include "../game/game_state.h"

#define FRAME_UPDATE_COUNT (*(u8 *)0x03000075)

extern u16 gLinkControlFlags asm("D_030009EC");
extern u8 gLinkDisconnectTimeoutCounter asm("D_0300605A");
extern u8 gLinkSavedFrameUpdateCount asm("D_0300603D");
extern s32 gLinkConnectionStatusBits asm("D_030009E8");
s32 IsScreenTransitionComplete(void) asm("func_0809669C");
void ResetLinkTransferState(void) asm("func_0809AC98");
void StopTask(s32) asm("func_08092E0C");
void StartScreenTransition(s32, s32) asm("func_08096308");
void YieldTaskForUpdates(s32) asm("func_080ED17C");
void StartTask(s32, s32) asm("func_08092D8C");
void RunMenuScript(s32) asm("func_08098BB4");

void DestroySprite(struct BattleDisplaySprite *) asm("func_08094554");

extern s32 gGameMode;
extern u8 gTitleResumeFlag asm("D_02021698");

s32 GetLinkConnectionStatus(u8 advance_timeout) asm("func_0809ACC4");

s32 GetLinkConnectionStatus(u8 advance_timeout) {
    u8 saved_frame_update_count;
    u8 *timeout_counter;

    if ((*(s32 *)&gLinkConnectionStatusBits & LINK_CONNECTION_STATUS_MASK) == LINK_CONNECTION_STATUS_READY) {
        *(u8 *)&gLinkDisconnectTimeoutCounter = 0;
        saved_frame_update_count = *(u8 *)&gLinkSavedFrameUpdateCount;
        if (saved_frame_update_count != 0) {
            FRAME_UPDATE_COUNT = saved_frame_update_count;
            *(u8 *)&gLinkSavedFrameUpdateCount = 0;
        }
        return LINK_CONNECTION_READY;
    }
    timeout_counter = (u8 *)&gLinkDisconnectTimeoutCounter;
    if (advance_timeout != 0) {
        *timeout_counter = *timeout_counter + 1;
    }
    if (*timeout_counter < LINK_DISCONNECT_TIMEOUT_UPDATES) {
        return LINK_CONNECTION_WAITING;
    }
    return LINK_CONNECTION_TIMED_OUT;
}

void RunLinkConnectionMonitorTask(void) asm("func_0809AD2C");

void RunLinkConnectionMonitorTask(void) {
    s32 sprite_index_sign_bits;
    s32 next_sprite_index;
    u8 connection_status;
    u8 sprite_index;
    struct BattleDisplaySprite *sprite_pool;
    struct BattleDisplaySprite *sprite;

    while (1) {
        if ((IsScreenTransitionComplete() << 0x18) != 0) {
            connection_status = GetLinkConnectionStatus(1);
            if (connection_status == LINK_CONNECTION_WAITING) {
                if (*(u8 *)&gLinkSavedFrameUpdateCount == 0) {
                    *(u8 *)&gLinkSavedFrameUpdateCount = FRAME_UPDATE_COUNT;
                    FRAME_UPDATE_COUNT = 1;
                }
            } else if (connection_status == LINK_CONNECTION_TIMED_OUT) {
                StopTask(1);
                ResetLinkTransferState();
                *(u16 *)&gLinkControlFlags |= LINK_CONTROL_REQUEST_STOP;
                sprite_index = 0;
                sprite_pool = (struct BattleDisplaySprite *)0x03003FE4;
                do {
                    sprite = (struct BattleDisplaySprite *)(sprite_index * sizeof(struct BattleDisplaySprite) + (s32)sprite_pool);
                    if ((BATTLE_SPRITE_FIELD(sprite, s32, flags) & LINK_TIMEOUT_SPRITE_PRIORITY_MASK) == 0) {
                        DestroySprite(sprite);
                    }
                    next_sprite_index = sprite_index + 1;
                    sprite_index_sign_bits = next_sprite_index << 0x18;
                    sprite_index = next_sprite_index;
                } while (sprite_index_sign_bits >= 0);
                RunMenuScript(0x080283AC);
                gGameMode = -1;
                StartScreenTransition(SCREEN_TRANSITION_DITHER_CONCEAL, 0);
                while ((IsScreenTransitionComplete() << 0x18) == 0) {
                    YieldTaskForUpdates(1);
                }
                StopTask(2);
                StopTask(3);
                StopTask(4);
                StopTask(5);
                StopTask(6);
                StopTask(7);
                StopTask(8);
                gGameMode = GAME_MODE_TITLE;
                gTitleResumeFlag = 0;
                StartTask(1, 0x0809A0B1);
                return;
            }
        }
        YieldTaskForUpdates(1);
    }
}
