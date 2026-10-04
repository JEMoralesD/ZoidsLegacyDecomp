#include "m2c_prelude.h"
#include "battle_display.h"
extern s16 gBattleSelectionCursorTargetPositions[] asm("D_02033F90");
extern s16 gBattleSelectionCursorStartPositions[] asm("D_02033F74");
extern void *gBattleSelectionCursorSprites[] asm("D_02033F58");
extern s8 gBattleSelectionCursorProgress[] asm("D_02033FAC");

void MoveBattleSelectionCursorTo(u8 cursor_slot, s16 target_x, s16 target_y) asm("func_080CC400");

void MoveBattleSelectionCursorTo(u8 cursor_slot, s16 target_x, s16 target_y) {
    register s32 position_offset asm("r4");
    register s32 target_position_address asm("r3");
    register void * volatile *cursor_sprite_slot asm("r2");
    register s16 *target_x_slot asm("r5");
    s8 start_progress;
    target_position_address = (s32)gBattleSelectionCursorTargetPositions;
    position_offset = cursor_slot * 4;
    target_x_slot = (s16 *)(position_offset + target_position_address);
    start_progress = 0;
    *target_x_slot = target_x;
    target_position_address += 2;
    *(s16 *)(position_offset + target_position_address) = target_y;
    *(u16 *)(position_offset + (s32)gBattleSelectionCursorStartPositions) =
        *(u16 *)((s8 *)*(cursor_sprite_slot = (void * volatile *)(position_offset + (s32)gBattleSelectionCursorSprites)) + BATTLE_SPRITE_OFFSET(x));
    *(u16 *)(position_offset + ((s32)gBattleSelectionCursorStartPositions + 2)) =
        *(u16 *)((s8 *)*cursor_sprite_slot + BATTLE_SPRITE_OFFSET(y));
    *(s8 *)(cursor_slot + (s32)gBattleSelectionCursorProgress) = start_progress;
}
