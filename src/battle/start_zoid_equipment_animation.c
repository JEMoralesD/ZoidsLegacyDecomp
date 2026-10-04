#include "m2c_prelude.h"
#include "battle.h"
#include "battle_animation.h"
#include "../graphics/zoid_body_sprites.h"

extern u8 gZoidEquipmentAnimationZoidId asm("D_02034055");
extern u8 gZoidEquipmentAnimationSlot asm("D_02034056");
extern u8 gZoidEquipmentAnimationState asm("D_02034057");
extern u8 gZoidEquipmentAnimationReady asm("D_02034058");
extern u16 gZoidEquipmentAnimationCommandIndex asm("D_0203405A");
extern u16 gZoidEquipmentAnimationElapsedFrames asm("D_0203405C");
extern u8 gZoidEquipmentAnimationScriptBuffer asm("D_02034060");
extern s32 *D_02033F40[];
extern u8 D_087A773C[];
extern struct ZoidBodySpriteGroup *gZoidBodySpriteGroup asm("D_02033F3C");
extern void BiosLz77ToWram(s32, void *) asm("func_080ECD38");

void StartZoidEquipmentAnimation(u8 zoid_id, u8 animation_slot) asm("func_080D04A0");

void StartZoidEquipmentAnimation(u8 zoid_id, u8 animation_slot) {
    s32 *entry;

    gZoidEquipmentAnimationZoidId = zoid_id;
    gZoidEquipmentAnimationSlot = animation_slot;
    if (animation_slot <= 3) {
        if (animation_slot > 2) {
            goto default_state;
        }
        entry = D_02033F40[animation_slot];
        if (entry == 0) {
            goto default_state;
        }
        *entry &= ~ZOID_MOUNT_ANIMATION_PAUSED;
        gZoidEquipmentAnimationState = 2;
        gZoidEquipmentAnimationReady = 0;
        return;
    }

    animation_slot -= 4;
    {
        register u8 *base asm("r2") = D_087A773C;
        register s32 index asm("r1") = animation_slot << 2;
        register s32 offset asm("r0") = zoid_id << 2;
        asm volatile("" : "+r"(base));
        offset += zoid_id;
        offset <<= 3;
        index += offset;
        index += (s32)base;
        entry = *(s32 **)index;
    }
    if (entry != 0) {
        BiosLz77ToWram((s32)entry, &gZoidEquipmentAnimationScriptBuffer);
        if (zoid_id == ZOID_BERSERK_FURY && animation_slot == 1) {
            gZoidBodySpriteGroup->phase = animation_slot;
            ZOID_BODY_GROUP_FIELD(gZoidBodySpriteGroup, s32, animation.berserk_fury_animation_id) = BERSERK_FURY_BODY_LONG_SEQUENCE;
        } else if (zoid_id == ZOID_BODY_BLITZ_TIGER && animation_slot == 2) {
            gZoidBodySpriteGroup->phase = ZOID_BODY_SPRITES_START;
            gZoidBodySpriteGroup->animation.blitz_tiger_skip_startup = 1;
        }
        gZoidEquipmentAnimationState = 1;
        gZoidEquipmentAnimationReady = 0;
        gZoidEquipmentAnimationCommandIndex = 0;
        gZoidEquipmentAnimationElapsedFrames = 0;
        return;
    }

    if (zoid_id == ZOID_ZERO_SCHNEIDER && animation_slot == 1) {
        gZoidBodySpriteGroup->phase = animation_slot;
        gZoidEquipmentAnimationState = 2;
        gZoidEquipmentAnimationReady = (u8)(s32)entry;
        return;
    }
    if (zoid_id == ZOID_BERSERK_FURY) {
        if (animation_slot == 0) {
            gZoidBodySpriteGroup->phase = ZOID_BODY_SPRITES_START;
            ZOID_BODY_GROUP_FIELD(gZoidBodySpriteGroup, s32, animation.berserk_fury_animation_id) = BERSERK_FURY_BODY_PARTIAL_SEQUENCE;
            gZoidEquipmentAnimationState = 2;
            gZoidEquipmentAnimationReady = animation_slot;
            return;
        }
        if (animation_slot == 2) {
            gZoidBodySpriteGroup->phase = ZOID_BODY_SPRITES_START;
            ZOID_BODY_GROUP_FIELD(gZoidBodySpriteGroup, s32, animation.berserk_fury_animation_id) = BERSERK_FURY_BODY_SHORT_SEQUENCE;
            gZoidEquipmentAnimationState = animation_slot;
            gZoidEquipmentAnimationReady = 0;
            return;
        }
    }
    if (zoid_id == ZOID_STRUM_FURY && animation_slot == 0) {
        gZoidBodySpriteGroup->phase = ZOID_BODY_SPRITES_START;
        gZoidEquipmentAnimationState = 2;
        gZoidEquipmentAnimationReady = animation_slot;
        return;
    }

default_state:
    gZoidEquipmentAnimationState = 0;
    gZoidEquipmentAnimationReady = 1;
}
