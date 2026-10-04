#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleAngledProjectileSprite(void *) asm("func_080D2570");


void *CreateMirroredSpriteFromTable(void *, s32, s32, s32) asm("func_080D22B4");
extern u16 gBattleAnimationResourceIds[][2] asm("D_02034874");
extern u16 gBattleAnimationTileOffsets[][2] asm("D_02034894");
extern u16 gBattleAnimationPaletteBanks[][2] asm("D_020348B4");

void *CreateBattleAngledProjectileSprite(struct BattleAnimationGroup *group, s32 resource_slot, s32 animation_id, s32 x, s32 y, s32 sprite_flags, s32 angle, s32 speed_fixed8, s32 mirroring_flags) asm("func_080D2660");

void *CreateBattleAngledProjectileSprite(struct BattleAnimationGroup *group, s32 resource_slot, s32 animation_id, s32 x, s32 y, s32 sprite_flags, s32 angle, s32 speed_fixed8, s32 mirroring_flags)
{
    /* Stack reads preserve the original call frame and register allocation. */
    volatile s32 reserve0;
    volatile s32 reserve1;
    volatile s32 reserve2;
    volatile s32 reserve3;
    volatile s32 reserve4;
    volatile s32 reserve5;
    volatile s32 saved_x;
    register struct BattleAnimationGroup *saved_group asm("r6");
    register s32 saved_resource_slot asm("r8");
    register u32 saved_animation_id asm("r9");
    register u32 saved_y asm("ip");
    register s32 saved_sprite_flags asm("r4");
    register s32 saved_mirroring_flags asm("r5");
    register u32 facing_flags asm("r2");
    register u32 group_flags asm("r3");
    register volatile s32 *outgoing asm("sp");

    asm volatile("" : "=m"(reserve0), "=m"(reserve1), "=m"(reserve2),
                       "=m"(reserve3), "=m"(reserve4), "=m"(reserve5));
    saved_group = group;
    saved_resource_slot = resource_slot;
    {
        register u32 y_input asm("r0") = outgoing[15];

        saved_sprite_flags = outgoing[16];
        saved_mirroring_flags = outgoing[19];
        asm volatile("" : "+r"(y_input), "+r"(saved_sprite_flags),
                           "+r"(saved_mirroring_flags));
        animation_id <<= 16;
        saved_animation_id = (u32)animation_id >> 16;
        x <<= 16;
        x = (u32)x >> 16;
        saved_x = x;
        y_input <<= 16;
        y_input >>= 16;
        saved_y = y_input;
    }

    if ((saved_mirroring_flags & BATTLE_ANIMATION_SPRITE_REVERSE_FACING) == 0) {
        register u32 flags asm("r0") = saved_group->flags;
        register s32 test asm("r1") = 2;
        register s32 sign asm("r2");
        register u32 mask asm("r1");

        test &= flags;
        test = -test;
        sign = test >> 31;
        mask = 0x80;
        mask <<= 8;
        facing_flags = sign & mask;
        group_flags = flags;
    } else {
        register u32 flags asm("r1") = saved_group->flags;
        register u32 test asm("r0") = 2;

        test &= flags;
        facing_flags = 0;
        group_flags = flags;
        if (test == 0) {
            facing_flags = 0x80;
            facing_flags <<= 8;
        }
    }

    {
        register void *sprite_resource_table asm("sl") = (void *)0x087ACDD8;
        register u8 *resource_ids asm("r0") = (u8 *)gBattleAnimationResourceIds;
        register s32 offset asm("r1");
        register s32 resource_id asm("r8");
        register void *result;

        asm volatile("" : "+r"(resource_ids));
        offset = saved_resource_slot << 2;
        resource_id = *(u16 *)(offset + (s32)resource_ids);
        {
            register s32 x_reload asm("r7") = saved_x;
            register s32 normalized asm("r0");

            asm volatile("" : "+r"(x_reload));
            normalized = x_reload << 16;
            normalized >>= 16;
            saved_x = normalized;
        }
        {
            register s32 y_reload asm("r7") = saved_y;
            register s32 normalized asm("r0");

            asm volatile("" : "+r"(y_reload));
            normalized = y_reload << 16;
            normalized >>= 16;
            outgoing[0] = normalized;
        }
        outgoing[1] = *(u16 *)((u8 *)gBattleAnimationTileOffsets + offset);
        outgoing[2] = *(u16 *)((u8 *)gBattleAnimationPaletteBanks + offset);
        {
            register u8 *group_view asm("r0") = (u8 *)saved_group;

            group_view += BATTLE_ANIMATION_OFFSET(sprite_flags);
            saved_sprite_flags |= *(u32 *)group_view;
        }
        saved_sprite_flags |= facing_flags;
        outgoing[3] = saved_sprite_flags;
        outgoing[4] = (s32)UpdateBattleAngledProjectileSprite;
        {
            register s32 mirror_coordinates asm("r1") = 0;
            register s32 two asm("r0") = 2;

            group_flags &= two;
            if (group_flags != 0) {
                saved_mirroring_flags &= two;
                if (saved_mirroring_flags == 0) {
                    mirror_coordinates = 1;
                }
            }
            outgoing[5] = mirror_coordinates;
        }
        {
            register void *call0 asm("r0") = sprite_resource_table;
            register s32 call1 asm("r1") = resource_id;
            register s32 call2 asm("r2") = saved_animation_id;
            register s32 call3 asm("r3");

            asm volatile("" : "+r"(call0), "+r"(call1), "+r"(call2));
            call3 = saved_x;
            result = CreateMirroredSpriteFromTable(call0, call1, call2, call3);
        }
        {
            register s32 half_offset = 4;
            register s32 value = *(s16 *)((u8 *)result + half_offset);

            value <<= 8;
            *(s32 *)((u8 *)result + (s32)&((struct BattleDisplaySprite *)0)->user_data.angled_projectile.x_fixed8) = value;
        }
        {
            register s32 value = *(s16 *)((u8 *)result + 6);

            value <<= 8;
            *(s32 *)((u8 *)result + 0x2C) = value;
        }
        *(s32 *)((u8 *)result + (s32)&((struct BattleDisplaySprite *)0)->user_data.angled_projectile.angle) = outgoing[17];
        *(s32 *)((u8 *)result + (s32)&((struct BattleDisplaySprite *)0)->user_data.angled_projectile.speed_fixed8) = outgoing[18];
        return result;
    }
}
