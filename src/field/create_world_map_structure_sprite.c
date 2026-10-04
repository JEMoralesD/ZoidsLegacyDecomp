#include "m2c_prelude.h"
#include "../events/event_script.h"
extern void LoadSpriteGraphicsFromTable(void *, int, int, int) asm("func_0809AA64");
extern void *CreateSpriteFromTable(void *, int, int, int, int, int, int, int, int) asm("func_8094374");

void CreateWorldMapStructureSprite(void) asm("func_080A6148");

void CreateWorldMapStructureSprite(void) {
    void *graphics_table = (void *)WORLD_MAP_STRUCTURE_GRAPHICS_TABLE_ROM;
    u8 *field_save_state = (u8 *)WORLD_MAP_STRUCTURE_STATE_RAM;
    struct WorldMapStructureSpriteOffsetBitsView *structure_sprite;
    LoadSpriteGraphicsFromTable(graphics_table, (u8)(field_save_state[EVENT_COMMAND_OFFSET(EventFieldMapStateView, world_structure_kind)] + 11), WORLD_MAP_STRUCTURE_TILE_OFFSET, 10);
    structure_sprite = CreateSpriteFromTable((void *)WORLD_MAP_STRUCTURE_SPRITE_TABLE_ROM, 12, 4, field_save_state[EVENT_COMMAND_OFFSET(EventFieldMapStateView, world_structure_cell_x)] << 4, field_save_state[EVENT_COMMAND_OFFSET(EventFieldMapStateView, world_structure_cell_y)] << 4, WORLD_MAP_STRUCTURE_TILE_OFFSET, 10, 0x410c8, 0);
    *(struct WorldMapStructureSpriteOffsetBitsView **)WORLD_MAP_STRUCTURE_SPRITE_RAM = structure_sprite;
    structure_sprite->offset_y_bits = 0xfff8;
}
