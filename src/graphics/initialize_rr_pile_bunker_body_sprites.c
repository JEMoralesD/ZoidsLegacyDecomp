#include "m2c_prelude.h"
#include "zoid_body_sprites.h"
extern u8 gZoidBodyAnimationResources[] asm("D_087AA244");
int CreateMirroredSpriteFromTable(int, int, int, int, int, int, int, int, int, int) asm("func_080D22B4");

void InitializeRRPileBunkerBodySprites(struct ZoidBodySpriteGroup *group) asm("func_080E0D8C");

void InitializeRRPileBunkerBodySprites(struct ZoidBodySpriteGroup *group) {
    register int animation_table_address asm("r2");
    group->phase = ZOID_BODY_SPRITES_READY;
    animation_table_address = (s32)gZoidBodyAnimationResources;
    ZOID_BODY_GROUP_FIELD(group, int, sprites[0]) = CreateMirroredSpriteFromTable(
        animation_table_address, ZOID_BODY_RR_PILE_BUNKER, 0, 0xC0,
        0x40, 0, 0,
        (group->flags & ZOID_BODY_SPRITE_GROUP_MIRRORED) ? 0x9288 : 0x1288,
        0,
        (group->flags >> 1) & 1);
}
