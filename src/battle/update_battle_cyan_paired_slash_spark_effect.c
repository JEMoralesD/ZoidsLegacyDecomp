#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleCyanPairedSlashSparkEffect(struct BattleAnimationGroup *group) asm("func_080DB1D8");

void UpdateBattleCyanPairedSlashSparkEffect(struct BattleAnimationGroup *group)
{
    register char *group_bytes asm("r5") = (char *)group;
    register s32 *effect_state asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 phase asm("r0") = *effect_state;
    register s32 sprite_index asm("r6");

    switch (phase) {
    case BATTLE_PAIRED_SLASH_CREATE:
        goto create_slash;
    case BATTLE_PAIRED_SLASH_WAIT_FOR_SPARK_STEP:
        goto emit_sparks_on_animation_step;
    case BATTLE_PAIRED_SLASH_WAIT_FOR_SPRITES:
        goto wait_for_sprites;
    default:
        return;
    }

create_slash:
    *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(group_bytes, 0, 0,
        *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), BATTLE_SPRITE_SEMITRANSPARENT, 0, 1);
    *effect_state = *effect_state + 1;
    return;

emit_sparks_on_animation_step:
    if (BATTLE_SPRITE_FIELD(*(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])), u16, animation_step) == BATTLE_PAIRED_SLASH_SPARK_ANIMATION_STEP) {
        register s32 *saved_effect_state asm("r9");
        register s32 *sprite_slots asm("r8");
        register u32 *random_callback_slot;

        sprite_index = 0;
        saved_effect_state = effect_state;
        sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        random_callback_slot = &gRandomNumberCallback;
        do {
            register s32 sprite_angle asm("r4");
            register s32 random asm("r0");
            register s32 speed_fixed8 asm("r1");
            register s32 minimum_speed_fixed8 asm("r0");
            register s32 next_sprite_index_bits asm("r2");
            register s32 sprite_offset asm("r1");
            void *effect_sprite;

            random = CallFunctionR0(*random_callback_slot);
            sprite_angle = random << 1;
            sprite_angle += random;
            sprite_angle <<= 4;
            sprite_angle += random;
            sprite_angle = (u32)sprite_angle >> 15;
            sprite_angle += 0x80;
            random = CallFunctionR0(*random_callback_slot);
            speed_fixed8 = random << 9;
            speed_fixed8 += random;
            speed_fixed8 = (u32)speed_fixed8 >> 15;
            minimum_speed_fixed8 = 0x200;
            speed_fixed8 += minimum_speed_fixed8;
            effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0,
                *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), sprite_angle, speed_fixed8, 0);
            next_sprite_index_bits = sprite_index + 1;
            sprite_offset = next_sprite_index_bits << 2;
            *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)effect_sprite;
            next_sprite_index_bits <<= 24;
            sprite_index = (u32)next_sprite_index_bits >> 24;
        } while ((u32)sprite_index <= 7);
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE_AND_RECOIL_SCROLL, 0);
        PlayBattleAnimationSound(0);
        {
            register s32 *phase_slot asm("r1") = saved_effect_state;

            *phase_slot = *phase_slot + 1;
        }
    }
    return;

wait_for_sprites:
    sprite_index = 0;
    if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
        s32 *sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        s32 sprite_offset;

        do {
            register s32 next_sprite_index_bits asm("r0") = sprite_index + 1;

            next_sprite_index_bits <<= 24;
            sprite_index = (u32)next_sprite_index_bits >> 24;
        } while ((u32)sprite_index <= 8 &&
            (sprite_offset = sprite_index << 2,
             *(s32 *)((char *)sprite_slots + sprite_offset)) == 0);
    }
    if (sprite_index == 9) {
        DestroySpriteGroup(group_bytes);
    }
}
