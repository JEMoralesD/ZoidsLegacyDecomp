#include "m2c_prelude.h"
#include "event_script.h"
void *CreateSpriteFromTable(s32, u8, u8, s16, s32, s32, s32, s32, s32) asm("func_08094374");
extern void *gEventSprites[] asm("D_02031940");
extern u16 gEventSpriteResourceIds[] asm("D_020317DC");
extern u16 gEventSpriteTileOffsets[] asm("D_020317FC");
extern u16 gEventSpritePaletteBanks[] asm("D_0203181C");
extern s32 gEventSpriteDefinitions asm("D_087AFA94");

void CreateEventSprite(u8 sprite_slot, u8 resource_id, u8 animation_id, u16 x_bits, u16 y_bits, u8 playback_mode) asm("func_0809F94C");

void CreateEventSprite(u8 sprite_slot, u8 resource_id, u8 animation_id, u16 x_bits, u16 y_bits, u8 playback_mode) {
    u8 resource_index;
    s32 sprite_flags;
    s32 playback_mode_value = playback_mode;

    if (gEventSprites[sprite_slot] == 0) {
        resource_index = 0;
        do {
            if (gEventSpriteResourceIds[resource_index] == resource_id) {
                sprite_flags = 0x10C0;
                if (playback_mode_value == EVENT_SPRITE_PLAY_LOOP) goto loop_animation;
                if (playback_mode_value <= EVENT_SPRITE_PLAY_LOOP) goto create_sprite;
                if (playback_mode_value == EVENT_SPRITE_PLAY_AND_HOLD) goto hold_last_frame;
                goto create_sprite;
loop_animation:
                sprite_flags |= 0x20;
                goto create_sprite;
hold_last_frame:
                sprite_flags |= 0x10;
create_sprite:
                gEventSprites[sprite_slot] = CreateSpriteFromTable((s32)&gEventSpriteDefinitions,
                    resource_id, animation_id, (s16)x_bits, (s16)y_bits,
                    gEventSpriteTileOffsets[resource_index], gEventSpritePaletteBanks[resource_index], sprite_flags, 0);
                return;
            }
            resource_index += 1;
        } while ((u32)resource_index <= EVENT_SPRITE_LAST_SLOT);
    }
}
