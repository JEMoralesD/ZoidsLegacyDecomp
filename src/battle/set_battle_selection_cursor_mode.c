#include "m2c_prelude.h"
#include "battle_display.h"


extern struct BattleDisplaySprite *gBattleSelectionCursorSprites[] asm("D_02033F58");
extern struct BattleSelectionCursorPosition gBattleSelectionCursorStartPositions[] asm("D_02033F74");
extern struct BattleSelectionCursorPosition gBattleSelectionCursorTargetPositions[] asm("D_02033F90");
extern u8 gBattleSelectionCursorProgress[] asm("D_02033FAC");
extern u8 gBattleSelectionCursorMode asm("D_02033FB3");
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");

void SetBattleSelectionCursorMode(u8 requested_mode) asm("func_080CC2E4");

void SetBattleSelectionCursorMode(u8 requested_mode)
{
    u8 cursor_slot;
    u8 next_mode;

    switch (gBattleSelectionCursorMode) {
    case BATTLE_SELECTION_CURSOR_TRAIL:
        if (requested_mode == BATTLE_SELECTION_CURSOR_TARGET_AREA) {
            register u8 *movement_progress asm("r9");
            register struct BattleDisplaySprite **cursor_sprites asm("r6");
            register s32 *camera_scroll_fixed8 asm("r4");

            cursor_slot = 1;
            movement_progress = gBattleSelectionCursorProgress;
            cursor_sprites = gBattleSelectionCursorSprites;
            camera_scroll_fixed8 = gFieldCameraScrollOffsets;
            do {
                struct BattleDisplaySprite **cursor_sprite_slot;
                struct BattleDisplaySprite *cursor_sprite;
                s32 scroll_fixed8;

                cursor_sprite_slot = (struct BattleDisplaySprite **)((cursor_slot << 2) + (s32)cursor_sprites);
                cursor_sprite = *cursor_sprite_slot;
                cursor_sprite->flags &= ~BATTLE_SPRITE_BACKGROUND_RELATIVE;
                scroll_fixed8 = camera_scroll_fixed8[0];
                if (scroll_fixed8 < 0)
                    scroll_fixed8 += 0xFF;
                cursor_sprite->x = cursor_sprite->x - (scroll_fixed8 >> 8);
                cursor_sprite = *cursor_sprite_slot;
                scroll_fixed8 = camera_scroll_fixed8[1];
                if (scroll_fixed8 < 0)
                    scroll_fixed8 += 0xFF;
                cursor_sprite->y = cursor_sprite->y - (scroll_fixed8 >> 8);
                *(u8 *)(cursor_slot + (s32)movement_progress) = BATTLE_SELECTION_CURSOR_MOVE_UPDATES;
                cursor_slot++;
            } while (cursor_slot <= BATTLE_SELECTION_CURSOR_LAST);
            next_mode = BATTLE_SELECTION_CURSOR_TARGET_AREA;
            goto store_mode;
        }
        break;
    case BATTLE_SELECTION_CURSOR_TARGET_AREA:
        if (requested_mode == BATTLE_SELECTION_CURSOR_TRAIL) {
            register u8 *movement_progress asm("r9");
            register struct BattleDisplaySprite **cursor_sprites asm("r6");
            register u16 *target_x_positions asm("ip");
            register u16 *target_y_positions asm("r10");
            register u16 *start_x_positions;
            register u16 *start_y_positions asm("r8");

            cursor_slot = 1;
            {
                register u8 *movement_progress_initial asm("r0") = gBattleSelectionCursorProgress;
                asm volatile("" : "+r"(movement_progress_initial));
                movement_progress = movement_progress_initial;
            }
            cursor_sprites = gBattleSelectionCursorSprites;
            {
                register u16 *target_x_positions_initial asm("r1") = &gBattleSelectionCursorTargetPositions[0].x;
                asm volatile("" : "+r"(target_x_positions_initial));
                target_x_positions = target_x_positions_initial;
            }
            {
                register s32 y_field_offset asm("r0") = 2;
                asm volatile("" : "+r"(y_field_offset));
                target_y_positions = (u16 *)(y_field_offset + (s32)target_x_positions);
            }
            start_x_positions = &gBattleSelectionCursorStartPositions[0].x;
            {
                register u16 *start_y_positions_initial asm("r1") = &gBattleSelectionCursorStartPositions[0].y;
                asm volatile("" : "+r"(start_y_positions_initial));
                start_y_positions = start_y_positions_initial;
            }
            asm volatile("" : "+r"(start_x_positions));
            do {
                struct BattleDisplaySprite **cursor_sprite_slot;
                struct BattleDisplaySprite *cursor_sprite;

                cursor_sprite_slot = (struct BattleDisplaySprite **)((cursor_slot << 2) + (s32)cursor_sprites);
                cursor_sprite = *cursor_sprite_slot;
                cursor_sprite->flags |= BATTLE_SPRITE_BACKGROUND_RELATIVE;
                cursor_sprite->x = cursor_sprites[0]->x;
                (*cursor_sprite_slot)->y = cursor_sprites[0]->y;
                target_x_positions[cursor_slot * 2] = cursor_sprites[0]->x;
                target_y_positions[cursor_slot * 2] = cursor_sprites[0]->y;
                start_x_positions[cursor_slot * 2] = cursor_sprites[0]->x;
                start_y_positions[cursor_slot * 2] = cursor_sprites[0]->y;
                *(u8 *)(cursor_slot + (s32)movement_progress) = BATTLE_SELECTION_CURSOR_MOVE_UPDATES;
                cursor_slot++;
            } while (cursor_slot <= BATTLE_SELECTION_CURSOR_LAST);
            next_mode = BATTLE_SELECTION_CURSOR_TRAIL;
store_mode:
            gBattleSelectionCursorMode = next_mode;
        }
        break;
    }
}
