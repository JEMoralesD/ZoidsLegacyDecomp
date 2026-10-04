#include "m2c_prelude.h"
#include "battle_display.h"

extern void LoadSpriteGraphicsFromTable(void *, s32, s32, s32) asm("func_0809AA64");
extern struct BattleDisplaySprite *CreateSpriteFromTable(void *, s32, s32, s16, s32, s32, s32, s32, s32) asm("func_08094374");

extern u8 gBattleState[];
extern u8 gBattleSelectedEquipmentSlots[] asm("D_0203ECFC");
extern u8 *gBattleSceneSelectedUnit asm("D_02033F38");
extern s16 gBattleEquipmentCursorPositions[] asm("D_087EC38C");
extern s16 gBattleEquipmentCursorYPositions[] asm("D_087EC38E");
extern u8 gBattleSelectionCursorGraphics[] asm("D_087AC9D8");
extern u8 gBattleSelectionCursorDefinitions[] asm("D_087AC9E0");
extern struct BattleDisplaySprite *gBattleSelectionCursorSprites[] asm("D_02033F58");
extern struct BattleSelectionCursorPosition gBattleSelectionCursorStartPositions[] asm("D_02033F74");
extern struct BattleSelectionCursorPosition gBattleSelectionCursorTargetPositions[] asm("D_02033F90");
extern u16 gBattleSelectionCursorTargetYPositions[] asm("D_02033F92");
extern s8 gBattleSelectionCursorProgress[] asm("D_02033FAC");
extern s8 gBattleSelectionCursorMode asm("D_02033FB3");

void CreateBattleSelectionCursorSprites(void) asm("func_080CC1E8");

void CreateBattleSelectionCursorSprites(void) {
    u8 cursor_slot;
    s32 trail_mode;
    struct BattleSelectionCursorPosition *start_positions;
    struct BattleSelectionCursorPosition *target_positions;
    register u8 *battle_state asm("r8");
    register s16 *equipment_cursor_positions asm("r9");
    register u16 *start_y_positions asm("r10");
    struct BattleDisplaySprite *cursor_sprite;
    s32 equipment_position_offset;

    LoadSpriteGraphicsFromTable(gBattleSelectionCursorGraphics, 0, BATTLE_SELECTION_CURSOR_TILE_OFFSET, BATTLE_SELECTION_CURSOR_PALETTE_BANK);
    cursor_slot = 0;
    equipment_cursor_positions = gBattleEquipmentCursorPositions;
    {
        register u8 *battle_state_initial asm("r1") = gBattleState;

        asm volatile("" : "+r"(battle_state_initial));
        battle_state = battle_state_initial;
    }
    target_positions = gBattleSelectionCursorTargetPositions;
    start_positions = gBattleSelectionCursorStartPositions;
    {
        register u16 *start_y_positions_initial asm("r2") = &start_positions[0].y;

        asm volatile("" : "+r"(start_y_positions_initial));
        start_y_positions = start_y_positions_initial;
    }
    do {
        register struct BattleDisplaySprite **cursor_sprite_slot asm("r3");
        register u8 action_index asm("r0");
        register u8 *equipment_slots asm("r1");
        register u8 *equipment_slot_address asm("r0");
        register u8 equipment_slot asm("r1");

        action_index = *(u8 *)(battle_state + BATTLE_SELECTION_OFFSET(action_count));
        equipment_slots = gBattleSelectedEquipmentSlots;
        equipment_slot_address = (u8 *)(s32)action_index;
        equipment_slot_address += (s32)equipment_slots;
        equipment_slot = *equipment_slot_address;
        equipment_position_offset = equipment_slot * 4 + (*gBattleSceneSelectedUnit << 5);
        cursor_sprite = CreateSpriteFromTable(gBattleSelectionCursorDefinitions, 0, 0,
                *(s16 *)(equipment_position_offset + (s32)equipment_cursor_positions),
                (s32)*(s16 *)((u8 *)gBattleEquipmentCursorYPositions + equipment_position_offset),
                BATTLE_SELECTION_CURSOR_TILE_OFFSET, BATTLE_SELECTION_CURSOR_PALETTE_BANK, 0x1060, 0);
        cursor_sprite_slot = gBattleSelectionCursorSprites;
        cursor_sprite_slot = (struct BattleDisplaySprite **)((cursor_slot << 2) + (s32)cursor_sprite_slot);
        *cursor_sprite_slot = cursor_sprite;
        target_positions[cursor_slot].x = cursor_sprite->x;
        {
            register u16 *target_y_positions asm("r0") = gBattleSelectionCursorTargetYPositions;
            register u16 *target_y_slot asm("r1");

            target_y_slot = (u16 *)((cursor_slot << 2) + (s32)target_y_positions);
            *target_y_slot = (*cursor_sprite_slot)->y;
        }
        start_positions[cursor_slot].x = (*cursor_sprite_slot)->x;
        {
            register u16 *start_y_slot asm("r2");

            start_y_slot = (u16 *)((u8 *)start_y_positions + (cursor_slot << 2));
            *start_y_slot = (*cursor_sprite_slot)->y;
        }
        gBattleSelectionCursorProgress[cursor_slot] = BATTLE_SELECTION_CURSOR_MOVE_UPDATES;
        cursor_slot++;
    } while (cursor_slot <= BATTLE_SELECTION_CURSOR_LAST);
    trail_mode = BATTLE_SELECTION_CURSOR_TRAIL;
    gBattleSelectionCursorMode = trail_mode;
}
