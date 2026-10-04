#include "m2c_prelude.h"
#include "battle_popup.h"

extern s32 *gBattleUnitSprites[][6] asm("D_02032E8C");

s32 ProjectCameraPoint(s32 *, s16 *, s8 *) asm("func_08093E30");
struct BattleDisplaySprite *CreateSpriteFromTable(u32, u32, u32, u32, u32, u32, u32, u32,
    u32) asm("func_08094374");

struct BattleDisplaySprite *CreateBattleUnitPopupEffectSprite(struct BattleUnitPopupGroupView *popup,
    s32 resource_id, s32 animation_id, s32 x_offset_pixels, s32 y_offset_pixels,
    s32 palette_variant, s32 sprite_flags, s32 unused) asm("func_080C9024");

struct BattleDisplaySprite *CreateBattleUnitPopupEffectSprite(struct BattleUnitPopupGroupView *popup,
    s32 resource_id, s32 animation_id, s32 x_offset_pixels, s32 y_offset_pixels,
    s32 palette_variant, s32 sprite_flags, s32 unused)
{
    register struct BattleUnitPopupGroupView *popup_view asm("ip") = popup;
    register s32 narrowed_resource_id asm("r5");
    register u32 narrowed_animation_id asm("sl");
    register u32 narrowed_x_offset asm("r9");
    register u32 narrowed_y_offset asm("r0") = y_offset_pixels;
    register u32 narrowed_palette_variant asm("r4") = palette_variant;
    register s32 *unit_sprite asm("r8");
    register u32 tile_offset asm("r6");
    s32 *side_pointer;
    register struct BattleDisplaySprite *sprite asm("r4");
    struct {
        s8 visible;
        u8 pad[3];
        volatile s32 saved_y_offset;
    } locals;

    resource_id <<= 16;
    narrowed_resource_id = (u32)resource_id >> 16;
    animation_id <<= 16;
    animation_id = (u32)animation_id >> 16;
    narrowed_animation_id = animation_id;
    x_offset_pixels <<= 16;
    x_offset_pixels = (u32)x_offset_pixels >> 16;
    narrowed_x_offset = x_offset_pixels;
    narrowed_y_offset <<= 16;
    narrowed_y_offset >>= 16;
    locals.saved_y_offset = narrowed_y_offset;
    narrowed_palette_variant <<= 24;
    narrowed_palette_variant >>= 24;

    unit_sprite = gBattleUnitSprites[popup_view->side][popup_view->unit_index];

    if ((u32)narrowed_resource_id <= BATTLE_POPUP_RESOURCE_BLUE_PARTICLES) {
        tile_offset = 0;
    } else {
        switch (narrowed_resource_id) {
        case BATTLE_POPUP_RESOURCE_SPARKS:
            tile_offset = 0;
            break;
        case BATTLE_POPUP_RESOURCE_STREAKS:
            tile_offset = BATTLE_POPUP_STREAK_TILE_OFFSET;
            break;
        case BATTLE_POPUP_RESOURCE_DISSOLVING_ORBS:
            tile_offset = BATTLE_POPUP_ORB_TILE_OFFSET;
            break;
        case BATTLE_POPUP_RESOURCE_PURPLE_RING:
            tile_offset = BATTLE_POPUP_RING_TILE_OFFSET;
            break;
        }
    }

    {
        register u32 resource_table_address asm("r3") = BATTLE_POPUP_SPRITE_RESOURCE_TABLE_ROM;
        register s32 side_index asm("r2");
        register u32 tile_offset_value asm("r1");
        register s32 *side_address asm("r0");
        u32 tile_offset_argument;

        asm volatile("" : "+r"(resource_table_address));
        sprite = CreateSpriteFromTable(resource_table_address, narrowed_resource_id, narrowed_animation_id, 0, 0,
            ({
                tile_offset_value = tile_offset;
                side_address = &popup_view->side;
                side_index = *side_address;
                side_pointer = side_address;
                if (side_index == 0) {
                    register u32 side_tile_adjustment asm("r0") = 0xC0;

                    side_tile_adjustment <<= 1;
                    tile_offset_value = tile_offset + side_tile_adjustment;
                }
                {
                    register u32 narrowed_tile_offset asm("r0") = tile_offset_value << 16;

                    tile_offset_argument = narrowed_tile_offset >> 16;
                }
                tile_offset_argument;
            }),
            (narrowed_palette_variant <= BATTLE_POPUP_PALETTE_ORANGE_STREAKS && side_index != 0) ?
                ({ asm volatile("" : "+r"(narrowed_palette_variant)); narrowed_palette_variant; }) :
                ({
                    register u32 side_palette_variant asm("r0") = narrowed_palette_variant + BATTLE_POPUP_SIDE_ZERO_PALETTE_OFFSET;

                    asm volatile("" : "+r"(side_palette_variant));
                    side_palette_variant;
                }),
            ({
                register s32 *unit_sprite_view asm("r2") = unit_sprite;
                register s32 combined_sprite_flags asm("r1");

                asm volatile("" : "+r"(unit_sprite_view));
                combined_sprite_flags = *unit_sprite_view;
                combined_sprite_flags &= BATTLE_POPUP_HARDWARE_PRIORITY_MASK;
                asm volatile("" : "+r"(combined_sprite_flags));
                combined_sprite_flags |= sprite_flags;

                if (*side_pointer != 0) combined_sprite_flags |= BATTLE_SPRITE_FLIP_X;
                combined_sprite_flags;
            }), 0);
    }
    sprite->scale = ProjectCameraPoint(
        (s32 *)((u8 *)unit_sprite + BATTLE_SPRITE_OFFSET(user_data.position)),
        &BATTLE_SPRITE_FIELD(sprite, s16, x), &locals.visible);

    {
        register s32 scale asm("r3");
        register s32 projected_x asm("r1");
        register s32 register_r2_constraint asm("r2");
        s32 offset;
        s32 product;

        asm volatile("" : "=r"(projected_x));
        asm volatile("" : "=r"(register_r2_constraint));
        {
            register u32 field asm("r0") = BATTLE_SPRITE_OFFSET(scale);

            scale = *(s16 *)((u8 *)sprite + field);
        }
        asm volatile("" : : "r"(projected_x));
        asm volatile("" : : "r"(register_r2_constraint));
        {
            register u32 field asm("r2") = BATTLE_SPRITE_OFFSET(x);

            projected_x = *(s16 *)((u8 *)sprite + field);
        }

        if (*side_pointer == 0) {
            register u32 input asm("r2") = narrowed_x_offset;

            offset = input << 16;
            offset >>= 16;
        } else {
            register u32 input asm("r2") = narrowed_x_offset;

            offset = input << 16;
            offset >>= 16;
            offset = -offset;
        }
        product = offset * scale;
        if (product < 0) {
            product += 0xFF;
        }
        {
            register s32 adjusted_x asm("r0") =
                projected_x + (product >> 8);

            asm volatile("" : "+r"(adjusted_x));
            BATTLE_SPRITE_FIELD(sprite, s16, x) = adjusted_x;
        }
    }

    {
        register s32 saved_y_offset asm("r1") = locals.saved_y_offset;
        register s32 product asm("r0");

        product = saved_y_offset << 16;
        product >>= 16;
        {
            register u32 field asm("r2") = BATTLE_SPRITE_OFFSET(scale);
            register s32 scale asm("r1") =
                *(s16 *)((u8 *)sprite + field);

            product *= scale;
        }

        if (product < 0) {
            product += 0xFF;
        }
        sprite->y = sprite->y + (product >> 8);
    }
    asm volatile("" : : "r"(tile_offset));
    return sprite;
}
