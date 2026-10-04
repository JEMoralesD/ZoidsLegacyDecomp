#include "m2c_prelude.h"
#include "zoid_body_sprites.h"
extern void *CreateMirroredSpriteFromTable(void *, int, int, int, int, int, int, int, int, int) asm("func_080D22B4");
extern u8 gZoidBodyAnimationResources[] asm("D_087AA244");

void InitializeCannonryMolgaBodySprites(struct ZoidBodySpriteGroup *group) asm("func_080E0DE8");

void InitializeCannonryMolgaBodySprites(struct ZoidBodySpriteGroup *group) {
    group->phase = ZOID_BODY_SPRITES_READY;
    ZOID_BODY_GROUP_FIELD(group, void *, sprites[0]) = CreateMirroredSpriteFromTable(gZoidBodyAnimationResources, ZOID_BODY_CANNONRY_MOLGA, 0, 0xc8, 0x60, 0, 0,
                            (group->flags & ZOID_BODY_SPRITE_GROUP_MIRRORED) ? 0x9288 : 0x1288, 0, (group->flags >> 1) & 1);
}
