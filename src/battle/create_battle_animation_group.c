#include "m2c_prelude.h"
#include "battle.h"
#include "battle_animation.h"


extern u8 gBattleAnimationState[] asm("D_02033FD0");
extern s32 gBattleZoidScrollX asm("D_02034034");
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern s16 D_087EB08C[][16];
extern s16 gBattleEquipmentCursorPositions[] asm("D_087EC38C");

void ResetBattleAnimationResourceAllocation(void) asm("func_080D2328");
struct BattleAnimationGroup *CreateSpriteGroup(s32, u32, u32) asm("func_08095098");

struct BattleAnimationGroup *CreateBattleAnimationGroup(u32 zoid_id, u32 callback_kind, u32 x,
                                    u32 y, s32 sprite_flags) asm("func_080E083C");

struct BattleAnimationGroup *CreateBattleAnimationGroup(u32 zoid_id, u32 callback_kind, u32 x,
                                    u32 y, s32 sprite_flags)
{
    volatile u32 saved_arg3;
    register u32 kind asm("r4");
    u32 saved_arg0;
    register u32 saved_arg2 asm("r6");
    register u32 saved_arg4 asm("r8");
    register struct BattleAnimationGroup *result asm("r4");
    register u8 *state asm("r5");
    register u32 saved_y asm("r3");
    register u32 arg4_temp asm("r1");
    s32 flag;
    u8 mode;

    kind = callback_kind;
    arg4_temp = sprite_flags;
    saved_arg0 = (u8)zoid_id;
    kind = (u16)kind;
    saved_arg2 = (u16)x;
    saved_y = (u16)y;
    saved_arg4 = (u16)arg4_temp;
    saved_arg3 = saved_y;

    ResetBattleAnimationResourceAllocation();
    state = gBattleAnimationState;
    flag = state[1];
    flag = ((-flag | flag) >> 31) & 2;
    {
        register u8 *pair_base asm("r2");
        register u8 *first_address asm("r1");
        register u32 first asm("r1");
        register u32 second asm("r2");

        pair_base = (u8 *)0x087A2D1C;
        kind <<= 3;
        first_address = (u8 *)kind + (u32)pair_base;
        first = *(u32 *)first_address;
        pair_base += 4;
        kind += (u32)pair_base;
        second = *(u32 *)kind;
        result = CreateSpriteGroup(flag, first, second);
    }

    mode = state[5];
    saved_y = saved_arg3;
    switch (mode) {
    case BATTLE_ANIMATION_ANCHOR_SCREEN:
        {
            register s32 x asm("r0") = (s16)saved_arg2;
            register s32 y asm("r0");

            result->x = x;
            y = (s16)saved_y;
            result->y = y;
        }
        break;
    case BATTLE_ANIMATION_ANCHOR_WORLD:
        {
            register s32 narrow asm("r0");
            register s32 x asm("r1");
            register s32 y asm("r1");
            register s32 adjusted asm("r0");

            narrow = saved_arg2 << 16;
            x = narrow >> 16;
            adjusted = x - gBattleZoidScrollX / 0x100;
            result->x = adjusted;
            narrow = saved_y << 16;
            y = narrow >> 16;
            adjusted = y - gFieldCameraScrollOffsets[1] / 0x100;
            result->y = adjusted;
        }
        break;
    case BATTLE_ANIMATION_ANCHOR_EQUIPMENT:
        {
            register s32 x asm("r2") = (s16)saved_arg2;
            register const s16 *values asm("r6");
            register u8 *case_state asm("r5");

            values = gBattleEquipmentCursorPositions;
            case_state = gBattleAnimationState;
            {
                register u32 part3 asm("r0");
                register u32 part2 asm("r1");
                register s32 table_value asm("r0");
                register s32 sum asm("r1");
                register s32 adjusted asm("r0");

                part3 = case_state[3];
                part3 <<= 2;
                part2 = case_state[2];
                part2 <<= 5;
                part3 += part2;
                part3 += (u32)values;
                part2 = 0;
                table_value = *(s16 *)((u8 *)part3 + part2);
                sum = x + table_value;
                adjusted = sum - gBattleZoidScrollX / 0x100;
                result->x = adjusted;
            }
            {
                register s32 y asm("r2") = (s16)saved_y;
                register u32 offset asm("r1");
                register u32 part asm("r0");
                register s32 table_value asm("r0");
                register s32 sum asm("r1");
                register s32 adjusted asm("r0");

                offset = case_state[3];
                offset <<= 2;
                part = case_state[2];
                part <<= 5;
                offset += part;
                part = (u32)(values + 1);
                offset += part;
                table_value = *(s16 *)offset;
                sum = y + table_value;
                adjusted = sum - gFieldCameraScrollOffsets[1] / 0x100;
                result->y = adjusted;
            }
        }
        break;
    case BATTLE_ANIMATION_ANCHOR_ZOID_POINT_0:
        {
            register s32 x asm("r1") = (s16)saved_arg2;
            register const u8 *values asm("r5");
            register u32 row asm("r2");
            register const u8 *field asm("r0");
            register s32 table_value asm("r0");
            register s32 y asm("r1");
            register s32 adjusted asm("r0");

            values = (const u8 *)D_087EB08C;
            row = saved_arg0 << 5;
            field = (const u8 *)row + (u32)values;
            table_value = *(s16 *)field;
            x += table_value;
            adjusted = x - gBattleZoidScrollX / 0x100;
            result->x = adjusted;
            y = (s16)saved_y;
            field = values + 2;
            field = (const u8 *)row + (u32)field;
            table_value = *(s16 *)field;
            y += table_value;
            adjusted = y - gFieldCameraScrollOffsets[1] / 0x100;
            result->y = adjusted;
        }
        break;
    case BATTLE_ANIMATION_ANCHOR_ZOID_POINT_1:
        {
            register s32 x asm("r1") = (s16)saved_arg2;
            register const u8 *values asm("r5");
            register u32 row asm("r2");
            register const u8 *field asm("r0");
            register s32 table_value asm("r0");
            register s32 y asm("r1");
            register s32 adjusted asm("r0");

            values = (const u8 *)D_087EB08C;
            row = saved_arg0 << 5;
            field = values + 4;
            field = (const u8 *)row + (u32)field;
            table_value = *(s16 *)field;
            x += table_value;
            adjusted = x - gBattleZoidScrollX / 0x100;
            result->x = adjusted;
            y = (s16)saved_y;
            field = values + 6;
            field = (const u8 *)row + (u32)field;
            table_value = *(s16 *)field;
            y += table_value;
            adjusted = y - gFieldCameraScrollOffsets[1] / 0x100;
            result->y = adjusted;
        }
        break;
    case BATTLE_ANIMATION_ANCHOR_ZOID_POINT_2:
        {
            register s32 x asm("r1") = (s16)saved_arg2;
            register const u8 *values asm("r5");
            register u32 row asm("r2");
            register const u8 *field asm("r0");
            register s32 table_value asm("r0");
            register s32 y asm("r1");
            register s32 adjusted asm("r0");

            values = (const u8 *)D_087EB08C;
            row = saved_arg0 << 5;
            field = values + 8;
            field = (const u8 *)row + (u32)field;
            table_value = *(s16 *)field;
            x += table_value;
            adjusted = x - gBattleZoidScrollX / 0x100;
            result->x = adjusted;
            y = (s16)saved_y;
            field = values + 10;
            field = (const u8 *)row + (u32)field;
            table_value = *(s16 *)field;
            y += table_value;
            adjusted = y - gFieldCameraScrollOffsets[1] / 0x100;
            result->y = adjusted;
        }
        break;
    case BATTLE_ANIMATION_ANCHOR_ZOID_POINT_3:
        {
            register s32 x asm("r1") = (s16)saved_arg2;
            register const u8 *values asm("r5");
            register u32 row asm("r2");
            register const u8 *field asm("r0");
            register s32 table_value asm("r0");
            register s32 y asm("r1");
            register s32 adjusted asm("r0");

            values = (const u8 *)D_087EB08C;
            row = saved_arg0 << 5;
            field = values + 12;
            field = (const u8 *)row + (u32)field;
            table_value = *(s16 *)field;
            x += table_value;
            adjusted = x - gBattleZoidScrollX / 0x100;
            result->x = adjusted;
            y = (s16)saved_y;
            field = values + 14;
            field = (const u8 *)row + (u32)field;
            table_value = *(s16 *)field;
            y += table_value;
            adjusted = y - gFieldCameraScrollOffsets[1] / 0x100;
            result->y = adjusted;
        }
        break;
    case BATTLE_ANIMATION_ANCHOR_ZOID_POINT_4:
        {
            register s32 x asm("r1") = (s16)saved_arg2;
            register const u8 *values asm("r5");
            register u32 row asm("r2");
            register const u8 *field asm("r0");
            register s32 table_value asm("r0");
            register s32 y asm("r1");
            register s32 adjusted asm("r0");

            values = (const u8 *)D_087EB08C;
            row = saved_arg0 << 5;
            field = values + 16;
            field = (const u8 *)row + (u32)field;
            table_value = *(s16 *)field;
            x += table_value;
            adjusted = x - gBattleZoidScrollX / 0x100;
            result->x = adjusted;
            y = (s16)saved_y;
            field = values + 18;
            field = (const u8 *)row + (u32)field;
            table_value = *(s16 *)field;
            y += table_value;
            adjusted = y - gFieldCameraScrollOffsets[1] / 0x100;
            result->y = adjusted;
        }
        break;
    case BATTLE_ANIMATION_ANCHOR_ZOID_POINT_5:
        {
            register s32 x asm("r1") = (s16)saved_arg2;
            register const u8 *values asm("r5");
            register u32 row asm("r2");
            register const u8 *field asm("r0");
            register s32 table_value asm("r0");
            register s32 y asm("r1");
            register s32 adjusted asm("r0");

            values = (const u8 *)D_087EB08C;
            row = saved_arg0 << 5;
            field = values + 20;
            field = (const u8 *)row + (u32)field;
            table_value = *(s16 *)field;
            x += table_value;
            adjusted = x - gBattleZoidScrollX / 0x100;
            result->x = adjusted;
            y = (s16)saved_y;
            field = values + 22;
            field = (const u8 *)row + (u32)field;
            table_value = *(s16 *)field;
            y += table_value;
            adjusted = y - gFieldCameraScrollOffsets[1] / 0x100;
            result->y = adjusted;
        }
        break;
    }

    {
        register u32 *sprite_flags_slot asm("r0") = &result->sprite_flags;
        register u32 sprite_flags_value asm("r3");

        sprite_flags_value = saved_arg4;
        *sprite_flags_slot = sprite_flags_value;
    }
    return result;
}
