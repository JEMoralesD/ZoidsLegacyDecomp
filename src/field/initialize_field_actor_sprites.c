#include "field_actor.h"

extern s32 GetFieldActorAnimationIndex(struct FieldActor *) asm("func_080A9A54");
extern struct FieldActorSpriteOffsetView *CreateSpriteFromTable(void *, u32, u32, s32,
    s32, s32, s32, void *, s32) asm("func_08094374");
extern struct FieldActorSpriteOffsetView *CreateSprite(s32, s32, s32, s32,
    s32, s32, s32, s32, s32) asm("func_08094484");

extern struct FieldActorSpriteDefinition gFieldActorSpriteDefinitions[] asm("D_087AD208");
extern s32 gFieldActorDirectionVectors[][2] asm("D_087A1B98");

void InitializeFieldActorSprites(struct FieldActor *actor) asm("func_080A9AFC");

void InitializeFieldActorSprites(struct FieldActor *actor)
{
    register u32 animation_index asm("r6");
    s32 world_x_pixels;

    if (actor->behavior != FIELD_ACTOR_MAP_INTERACTION) {
        s32 result = GetFieldActorAnimationIndex(actor);

        animation_index = (u32)(result << 16) >> 16;
    } else {
        register u32 bit_index asm("r2");
        register u32 bitset_base asm("r0") = 0x0202ECF4;
        u32 word_offset;
        u32 addend;
        u32 *word;
        s32 masked;

        asm volatile("" :: "r"(bitset_base));
        bit_index = actor->behavior_state.map_interaction.flag_id;
        word_offset = (bit_index >> 5) * 4;
        addend = 0x170;
        bitset_base += addend;
        word = (u32 *)(word_offset + bitset_base);
        bit_index &= 0x1F;
        bitset_base = 1;
        bitset_base <<= bit_index;
        masked = *word;
        masked &= bitset_base;
        bitset_base = -masked;
        bitset_base |= masked;
        animation_index = bitset_base >> 31;
    }

    {
    register u32 model_id_value asm("r0") = actor->model_id;

    if (model_id_value != 0x4B) {
        void *descriptor;
        register u32 model_id asm("r5");
        register s32 tile_offset asm("r2");
        register u32 model_id_copy asm("r2");
        register s32 x_work asm("r0");
        s32 world_y_pixels;
        u8 palette_bank;
        struct FieldActorSpriteDefinition *descriptor_base;
        struct FieldActorSpriteDefinition *entry;
        register u32 entry_addr asm("r0");
        void *frame_table;
        register void *comparison asm("r0");

        descriptor_base = gFieldActorSpriteDefinitions;
        model_id_copy = model_id_value;
        entry_addr = model_id_copy << 4;
        entry_addr += (u32)descriptor_base;
        entry = (struct FieldActorSpriteDefinition *)entry_addr;
        frame_table = entry->frame_table;

        comparison = (void *)0x0827A0C4;
        descriptor = descriptor_base;
        model_id = model_id_copy;
        palette_bank = model_id_value;
        if (frame_table == comparison) {
            tile_offset = 0xD0;
            palette_bank = actor->actor_id;
        } else {
            comparison = (void *)0x0827A28C;
            if (frame_table == comparison) {
                tile_offset = 0x110;
                palette_bank = actor->actor_id;
            } else {
                tile_offset = actor->actor_id << 4;
                palette_bank = actor->actor_id;
            }
        }

        x_work = actor->world_x_fixed8;
        if (x_work < 0) {
            x_work += 0xFF;
        }
        x_work <<= 8;
        world_x_pixels = x_work >> 16;
        world_y_pixels = actor->world_y_fixed8;
        if (world_y_pixels < 0) {
            world_y_pixels += 0xFF;
        }
        world_y_pixels = (world_y_pixels << 8) >> 16;
        actor->sprite = CreateSpriteFromTable(descriptor, model_id, animation_index, world_x_pixels, world_y_pixels,
            tile_offset, palette_bank,
            actor->behavior != FIELD_ACTOR_MAP_INTERACTION ? (void *)0x004410E0 : (void *)0x004410D0, 0);
    } else {
        register struct FieldActorSpriteDefinition *palette_override_definition asm("r2") =
            (struct FieldActorSpriteDefinition *)0x0203299C;
        register u8 *definition_bytes asm("r0") = (u8 *)gFieldActorSpriteDefinitions;
        register struct FieldActorSpriteDefinition *definition_copy_target asm("r1") = palette_override_definition;
        register u32 definition_offset asm("r5") = 150;
        register s32 x_work asm("r0");
        s32 world_y_pixels;

        definition_offset <<= 3;
        definition_bytes += definition_offset;
        *definition_copy_target = *(const struct FieldActorSpriteDefinition *)definition_bytes;
        {
            s32 *alternate_palettes = (s32 *)0x087A1C08;
            register u8 *selection_base asm("r0") = (u8 *)0x020218E4;
            register u32 selection_addend asm("r7") = 0x6809;

            asm volatile("" :: "r"(selection_base));
            selection_base += selection_addend;
            palette_override_definition->palette = (void *)alternate_palettes[*selection_base];
        }
        x_work = actor->world_x_fixed8;
        if (x_work < 0) {
            x_work += 0xFF;
        }
        x_work <<= 8;
        world_x_pixels = x_work >> 16;
        world_y_pixels = actor->world_y_fixed8;
        if (world_y_pixels < 0) {
            world_y_pixels += 0xFF;
        }
        world_y_pixels = (world_y_pixels << 8) >> 16;
        actor->sprite = CreateSpriteFromTable(palette_override_definition, 0, animation_index, world_x_pixels, world_y_pixels,
            actor->actor_id << 4, actor->actor_id,
            actor->behavior != FIELD_ACTOR_MAP_INTERACTION ? (void *)0x004410E0 : (void *)0x004410D0, 0);
    }
    }

    if (actor->behavior != FIELD_ACTOR_STATIONARY && actor->behavior != FIELD_ACTOR_DIRECTION_OFFSET && actor->behavior != FIELD_ACTOR_MAP_INTERACTION &&
        actor->model_id != FIELD_ACTOR_ALTERNATE_PALETTE_MODEL && actor->model_id != 0x49 && actor->model_id != 0x69 &&
        actor->model_id != 0x6A && actor->model_id != 0x6B &&
        (u8)(actor->model_id - 0x6E) > 0x1E) {
        struct FieldActorSpriteOffsetView *secondary_sprite;
        register s32 x_work asm("r0");
        s32 secondary_frame_table_addr = 0x0832BB9C;
        s32 secondary_animation_table_addr = 0x0832BBA8;
        s32 world_y_pixels;

        asm volatile("" :: "r"(secondary_frame_table_addr), "r"(secondary_animation_table_addr));
        x_work = actor->world_x_fixed8;
        if (x_work < 0) {
            x_work += 0xFF;
        }
        x_work <<= 8;
        world_x_pixels = x_work >> 16;
        world_y_pixels = actor->world_y_fixed8;
        if (world_y_pixels < 0) {
            world_y_pixels += 0xFF;
        }
        world_y_pixels = (world_y_pixels << 8) >> 16;
        secondary_sprite = CreateSprite(secondary_frame_table_addr, secondary_animation_table_addr, 0, world_x_pixels, world_y_pixels,
            0x3B2, 0xF, 0x11C8, 0);
        actor->secondary_sprite = secondary_sprite;
        secondary_sprite->offset_y += 0xE;
    } else {
        actor->secondary_sprite = 0;
    }

    if (actor->behavior != FIELD_ACTOR_STATIONARY) {
        if (actor->behavior == FIELD_ACTOR_MAP_INTERACTION) {
            actor->sprite->offset_y = 8;
        } else if ((u8)(actor->model_id - 0x69) > 2 && actor->model_id != 0x6E &&
                   actor->model_id != 0x6F) {
            if (actor->model_id == 0x6C || actor->model_id == 0x8D ||
                actor->model_id == 0x8E) {
                if (actor->behavior != FIELD_ACTOR_NO_REGULAR_UPDATES) {
                    actor->sprite->offset_y = 0xFFF0;
                }
            } else if (actor->behavior != FIELD_ACTOR_DIRECTION_OFFSET) {
                if (actor->model_id != FIELD_ACTOR_ALTERNATE_PALETTE_MODEL) {
                    actor->sprite->offset_y -= 2;
                } else {
                    actor->sprite->offset_y += 2;
                }
            } else {
                struct FieldActorSpriteOffsetView *sprite = actor->sprite;
                s32 direction_component_fixed8 = gFieldActorDirectionVectors[actor->requested_direction][0];

                if (direction_component_fixed8 < 0) {
                    direction_component_fixed8 += 0xF;
                }
                sprite->offset_x -= direction_component_fixed8 >> 4;
                sprite = actor->sprite;
                direction_component_fixed8 = gFieldActorDirectionVectors[actor->requested_direction][1];
                if (direction_component_fixed8 < 0) {
                    direction_component_fixed8 += 0xF;
                }
                sprite->offset_y -= direction_component_fixed8 >> 4;
            }
        }
    }
    actor->previous_movement_mode = 0;
}
