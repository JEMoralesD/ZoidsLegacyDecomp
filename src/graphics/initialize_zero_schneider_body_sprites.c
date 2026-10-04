#include "m2c_prelude.h"
#include "zoid_body_sprites.h"
extern s32 CreateMirroredSpriteFromTable(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080D22B4");
extern u8 gZoidBodyAnimationResources[] asm("D_087AA244");

void InitializeZeroSchneiderBodySprites(struct ZoidBodySpriteGroup *group) asm("func_080E0B1C");

void InitializeZeroSchneiderBodySprites(struct ZoidBodySpriteGroup *group) {
    ZOID_BODY_GROUP_FIELD(group, s32, phase) = ZOID_BODY_SPRITES_IDLE;
    ZOID_BODY_GROUP_FIELD(group, s32, sprites[0]) = CreateMirroredSpriteFromTable(gZoidBodyAnimationResources, ZOID_ZERO_SCHNEIDER, 0, 0x80, 0x50, 0, 0,
        (ZOID_BODY_GROUP_FIELD(group, s32, flags) & ZOID_BODY_SPRITE_GROUP_MIRRORED) ? 0x9298 : 0x1298, 0, (ZOID_BODY_GROUP_FIELD(group, u32, flags) >> 1) & 1);
}
