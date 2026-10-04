#include "m2c_prelude.h"
#include "battle_combination.h"
#include "../../graphics/screen_effects.h"


extern volatile u16 gBlendControl asm("D_0300004E");
extern volatile u16 gBlendAlpha asm("D_03000050");
extern struct BattleDisplaySprite *gBattleUnitSprites[2][6] asm("D_02032E8C");
extern u8 gBattleCombinationPresentation[6] asm("D_02032F7C");
extern u8 gBattleState[];
extern s32 gBattleUnitWorldPositions[][3] asm("D_087A2790");
extern s32 gBattleUnitWorldPositionsZColumn[][3] asm("D_087A2798");
extern u8 gFieldEventActive asm("D_02030664");
extern struct PerspectiveCamera gPerspectiveCamera asm("D_030033C4");

u8 IsBattleUnitActive(u8 side, u8 unit_slot) asm("func_080E9D88");
void YieldTaskForUpdates(s32 frames) asm("func_080ED17C");
void HideBattleUnitSprites(u8 side, u8 unit_slot) asm("func_080BB05C");
void StartBattleCameraTransition(s32 kind, u8 side, u8 unit_slot, s32 value) asm("func_080BB224");
void StartScreenTransition(s32 kind, s32 duration) asm("func_08096308");

void AnimateBattleCombinationDeparture(u8 side) asm("func_080C2DD0");

void AnimateBattleCombinationDeparture(u8 side)
{
    u8 unit_slot;
    u8 departure_update;
    u8 merge_update;
    u32 next_merge_update;
    u32 side_sprite_table_offset;
    u32 side_position_index_base;
    struct BattleDisplaySprite **unit_sprite_table;

    gBlendControl = BATTLE_COMBINATION_BLEND_CONTROL;
    gBlendAlpha = 0x10;

    unit_slot = 0;
    do {
        if (IsBattleUnitActive(side, unit_slot) && gBattleCombinationPresentation[unit_slot] == BATTLE_COMBINATION_UNCHANGED_UNIT) {
            gBattleUnitSprites[side][unit_slot]->flags |= BATTLE_SPRITE_SEMITRANSPARENT;
        }
        unit_slot++;
    } while (unit_slot <= 5);

    asm volatile("" : "+r"(side));
    departure_update = 0;
    unit_sprite_table = &gBattleUnitSprites[0][0];
    side_sprite_table_offset = side * 24;
    asm volatile("" : "+r"(side_sprite_table_offset));
    asm volatile("" : "+r"(side_sprite_table_offset));
    do {
        asm volatile("" ::: "r2", "r3");
        unit_slot = 0;
        do {
            if (IsBattleUnitActive(side, unit_slot) && gBattleCombinationPresentation[unit_slot] == BATTLE_COMBINATION_UNCHANGED_UNIT) {
                register struct BattleDisplaySprite *unit_sprite asm("r1") = *(struct BattleDisplaySprite **)((u8 *)unit_sprite_table + (unit_slot * 4 + side_sprite_table_offset));
                s32 x = unit_sprite->user_data.position.x;
                if (side == 0) {
                    x += 0x200;
                } else {
                    x -= 0x200;
                }
                unit_sprite->user_data.position.x = x;
            }
            unit_slot++;
        } while (unit_slot <= 5);
        gBlendAlpha = (departure_update << 8) | (0x10 - departure_update);
        YieldTaskForUpdates(1);
        departure_update++;
    } while (departure_update <= 0xF);

    unit_slot = 0;
    do {
        if (IsBattleUnitActive(side, unit_slot) && gBattleCombinationPresentation[unit_slot] == BATTLE_COMBINATION_UNCHANGED_UNIT) {
            HideBattleUnitSprites(side, unit_slot);
        }
        unit_slot++;
    } while (unit_slot <= 5);

    gBlendControl = 0;

    unit_slot = 0;
    do {
        if (gBattleCombinationPresentation[unit_slot] != BATTLE_COMBINATION_UNCHANGED_UNIT) {
            gBattleUnitSprites[side][unit_slot]->flags |= BATTLE_SPRITE_DOUBLE_CANVAS;
        }
        unit_slot++;
    } while (unit_slot <= 5);

    unit_slot = 0;
    if (gBattleCombinationPresentation[0] != BATTLE_COMBINATION_CHANGED_MODEL) {
        do {
            unit_slot++;
            if (unit_slot > 5) {
                break;
            }
        } while (gBattleCombinationPresentation[unit_slot] != BATTLE_COMBINATION_CHANGED_MODEL);
    }

    gBattleState[BATTLE_COMBINATION_OFFSET(active_unit_slot)] = unit_slot;
    StartBattleCameraTransition(9, side, unit_slot, 0);
    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_WHITE, 0x10);

    asm volatile("" : "+r"(side));
    merge_update = 0;
    side_position_index_base = side * 3;
    asm volatile("");
    asm volatile("");
    asm volatile("");
    asm volatile("");
    asm volatile("");
    do {
        unit_slot = 0;
        next_merge_update = merge_update + 1;
        asm volatile("" : : "r"(next_merge_update));
        do {
            if (gBattleCombinationPresentation[unit_slot] != BATTLE_COMBINATION_UNCHANGED_UNIT && gBattleCombinationPresentation[unit_slot] != BATTLE_COMBINATION_CHANGED_MODEL) {
                struct BattleDisplaySprite *unit_sprite;
                s32 position_index;
                register s32 side6 asm("r3");
                u32 slot4;
                s32 *source_world_x;
                s32 *source_world_z;
                s32 source_z;
                s32 target_z;
                register u32 destination_unit_slot asm("r1");
                s32 delta;

                slot4 = unit_slot * 4;
                unit_sprite = *(struct BattleDisplaySprite **)((u8 *)&gBattleUnitSprites[0][0] + (slot4 + (side_position_index_base << 3)));
                side6 = side_position_index_base * 2;
                position_index = side6 + unit_slot;
                source_world_x = &gBattleUnitWorldPositions[position_index][0];

                destination_unit_slot = gBattleCombinationPresentation[unit_slot];
                delta = merge_update * (gBattleUnitWorldPositions[side6 + destination_unit_slot][0] - *source_world_x);
                if (delta < 0) {
                    delta += 0xF;
                }
                unit_sprite->user_data.position.x = *source_world_x + (delta >> 4);

                source_world_z = &gBattleUnitWorldPositionsZColumn[position_index][0];
                destination_unit_slot = gBattleCombinationPresentation[unit_slot];
                target_z = gBattleUnitWorldPositionsZColumn[side6 + destination_unit_slot][0];
                source_z = *source_world_z;
                delta = merge_update * (target_z - source_z);
                if (delta < 0) {
                    delta += 0xF;
                }
                unit_sprite->user_data.position.z = source_z + (delta >> 4);
            }
            unit_slot++;
        } while (unit_slot <= 5);

        if (gFieldEventActive == 1 && gPerspectiveCamera.projection.screen_center_y <= 0x78) {
            s32 delta = 0x78 - gPerspectiveCamera.projection.screen_center_y;
            if (delta < 0) {
                delta += 3;
            }
            gPerspectiveCamera.projection.screen_center_y += delta >> 2;
        }
        YieldTaskForUpdates(1);
        merge_update = next_merge_update;
    } while (merge_update <= 0xF);
}
