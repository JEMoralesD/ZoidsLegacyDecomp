#include "m2c_prelude.h"
#include "battle.h"
#include "battle_animation.h"

struct ZoidBodyAnimationState {
    u8 pad[0x8C];
    s32 animation_state;
    s32 data90;
};

extern u8 gZoidEquipmentAnimationZoidId asm("D_02034055");
extern u8 gZoidEquipmentAnimationSlot asm("D_02034056");
extern u8 gZoidEquipmentAnimationState asm("D_02034057");
extern u8 gZoidEquipmentAnimationReady asm("D_02034058");
extern u16 gZoidEquipmentAnimationCommandIndex asm("D_0203405A");
extern u16 gZoidEquipmentAnimationElapsedFrames asm("D_0203405C");
extern u8 gZoidEquipmentAnimationScriptBuffer asm("D_02034060");
extern s32 *D_02033F40[];
extern u8 D_087A773C[];
extern struct ZoidBodyAnimationState *D_02033F3C;
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
        if (zoid_id == 0x40 && animation_slot == 1) {
            D_02033F3C->animation_state = animation_slot;
            D_02033F3C->data90 = 0;
        } else if (zoid_id == 0x74 && animation_slot == 2) {
            D_02033F3C->animation_state = 1;
            D_02033F3C->data90 = 1;
        }
        gZoidEquipmentAnimationState = 1;
        gZoidEquipmentAnimationReady = 0;
        gZoidEquipmentAnimationCommandIndex = 0;
        gZoidEquipmentAnimationElapsedFrames = 0;
        return;
    }

    if (zoid_id == 0x1A && animation_slot == 1) {
        D_02033F3C->animation_state = animation_slot;
        gZoidEquipmentAnimationState = 2;
        gZoidEquipmentAnimationReady = (u8)(s32)entry;
        return;
    }
    if (zoid_id == 0x40) {
        if (animation_slot == 0) {
            D_02033F3C->animation_state = 1;
            D_02033F3C->data90 = 2;
            gZoidEquipmentAnimationState = 2;
            gZoidEquipmentAnimationReady = animation_slot;
            return;
        }
        if (animation_slot == 2) {
            D_02033F3C->animation_state = 1;
            D_02033F3C->data90 = 1;
            gZoidEquipmentAnimationState = animation_slot;
            gZoidEquipmentAnimationReady = 0;
            return;
        }
    }
    if (zoid_id == 0x41 && animation_slot == 0) {
        D_02033F3C->animation_state = 1;
        gZoidEquipmentAnimationState = 2;
        gZoidEquipmentAnimationReady = animation_slot;
        return;
    }

default_state:
    gZoidEquipmentAnimationState = 0;
    gZoidEquipmentAnimationReady = 1;
}
