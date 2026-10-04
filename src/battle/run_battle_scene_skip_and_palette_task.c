#include "m2c_prelude.h"
#include "battle_display.h"
void QueueCopy(s32, s32, s32) asm("func_08095208");
void SetDisplayWindowBounds(s32, u16, s32, u16) asm("func_0809544C");
void YieldTaskForUpdates(s32) asm("func_080ED17C");
extern s16 D_0300004E[];

void RunBattleSceneSkipAndPaletteTask(void) asm("func_080CFD98");

void RunBattleSceneSkipAndPaletteTask(void)
{
    register s32 palette_descending asm("r9");
    u8 closing_timer;
    u8 skip_window_state;
    u8 opening_timer;
    u8 event_active;
    s32 next_palette_frame;
    u8 palette_frame;
    u8 palette_delay;
    register u8 *window_timer asm("r4") = (u8 *)0x02033F55;
    register u8 *window_state asm("r8");

    palette_delay = 0;
    palette_frame = 0;
    palette_descending = 0;
    window_timer = (u8 *)0x02033F55;
loop_1:
    event_active = *(u8 *)0x02030664;
    window_state = (u8 *)0x02033F54;
    if ((event_active != 1) && (*(u8 *)0x02033FCC == 0)) {
        skip_window_state = *window_state;
        if (skip_window_state == BATTLE_SCENE_SKIP_WINDOW_IDLE) {
            register u16 held_input asm("r1") = *(u16 *)0x0300000C;
            register u8 state_value asm("r3") = BATTLE_SCENE_SKIP_WINDOW_CLOSING;
            if (2 & held_input) {
                register u8 *window_state_store asm("r1") = window_state;
                *window_state_store = state_value;
                *window_timer = skip_window_state;
            }
            goto block_6;
        }
        goto block_7;
    }
block_6:
    {
        register u8 *window_state_check asm("r1") = window_state;
        if (*window_state_check != BATTLE_SCENE_SKIP_WINDOW_IDLE) {
block_7:
        {
            register u8 *window_state_branch asm("r6") = window_state;
            if (*window_state_branch == BATTLE_SCENE_SKIP_WINDOW_OPENING) {
                *window_timer += 1;
                opening_timer = *window_timer;
                SetDisplayWindowBounds(0xF0, (u16)(0x50 - opening_timer), 0xF0,
                               (u16)(((opening_timer + 0x50) << 8) | 0xA0));
                if (*window_timer == 0x20)
                    *window_state_branch = BATTLE_SCENE_SKIP_WINDOW_IDLE;
            } else {
                *window_timer += 1;
                closing_timer = *window_timer;
                SetDisplayWindowBounds(0xF0, closing_timer + 0x30, 0xF0,
                               (u16)(((0x70 - closing_timer) << 8) | 0xA0));
                if (*window_timer == 0x20) {
                    register u8 zero asm("r0") = 0;
                    register u8 *window_state_store asm("r1") = window_state;
                    *window_state_store = zero;
                    *(u8 *)0x02033FCC = 1;
                    *(s16 *)0x03000052 = 0x10;
                    D_0300004E[0] = 0xFF;
                }
            }
        }
    }
    }
    if (palette_descending == 0) {
        if (palette_delay == 0) {
            QueueCopy((palette_frame << 5) + 0x083C773C, 0x05000080, 0x20);
            if (palette_frame == 5) {
                register u8 one asm("r1") = 1;
                palette_descending = one;
            } else {
                next_palette_frame = palette_frame + 1;
                goto block_22;
            }
            goto block_23;
        }
    } else if (palette_delay == 0) {
        QueueCopy((palette_frame << 5) + 0x083C773C, 0x05000080, 0x20);
        if (palette_frame == 0) {
            palette_descending = 0;
        } else {
            next_palette_frame = palette_frame - 1;
block_22:
            palette_frame = next_palette_frame;
        }
block_23:
        palette_delay = 4;
    }
    palette_delay -= 1;
    YieldTaskForUpdates(1);
    goto loop_1;
}
