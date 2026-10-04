#ifndef ZOID_BODY_SPRITES_H
#define ZOID_BODY_SPRITES_H

#include "../battle/battle_display.h"

enum ZoidBodySpritePhase {
    ZOID_BODY_SPRITES_IDLE = 0,
    ZOID_BODY_SPRITES_START = 1,
    ZOID_BODY_SPRITES_WAITING = 2,
    ZOID_BODY_SPRITES_READY = 0xFF
};

enum ZoidBodySpriteGroupFlags {
    ZOID_BODY_SPRITE_GROUP_ACTIVE = 1,
    ZOID_BODY_SPRITE_GROUP_MIRRORED = 2
};

enum ZoidBodySpriteModel {
    ZOID_BODY_RR_PILE_BUNKER = 73,
    ZOID_BODY_CANNONRY_MOLGA = 90,
    ZOID_BODY_BLITZ_TIGER = 116
};

enum BerserkFuryBodyAnimation {
    BERSERK_FURY_BODY_LONG_SEQUENCE = 0,
    BERSERK_FURY_BODY_SHORT_SEQUENCE = 1,
    BERSERK_FURY_BODY_PARTIAL_SEQUENCE = 2
};

enum BlitzTigerBodyAnimation {
    BLITZ_TIGER_BODY_FIRST_CLAW_STARTUP = 0,
    BLITZ_TIGER_BODY_SECOND_CLAW_STARTUP = 1,
    BLITZ_TIGER_BODY_FIRST_CLAW_WITHOUT_STARTUP = 2,
    BLITZ_TIGER_BODY_SECOND_CLAW_WITHOUT_STARTUP = 3
};

union ZoidBodySpriteAnimationSelector {
    u16 berserk_fury_animation_id;
    s32 blitz_tiger_skip_startup;
};

struct ZoidBodySpriteGroup {
    u32 flags;
    u8 data04[8];
    struct BattleDisplaySprite *sprites[32];
    u32 phase;
    union ZoidBodySpriteAnimationSelector animation;
    u8 data94[0x18];
    s32 initialize_callback;
    s32 update_callback;
};

#define ZOID_BODY_GROUP_FIELD(group, type, field) \
    M2C_FIELD(group, type *, (s32)&((struct ZoidBodySpriteGroup *)0)->field)

#define ZOID_BODY_GROUP_OFFSET(field) \
    ((s32)&((struct ZoidBodySpriteGroup *)0)->field)

#endif
