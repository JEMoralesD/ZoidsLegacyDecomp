#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern s32 gRandomNumberCallback asm("D_03000010");
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");                      /* extern */
u32 CreateBattleAnimationSprite(void *, s32, u32, s16, s32, s32, u32, u32) asm("func_080D2450"); /* extern */
void *CreateBattleAngledProjectileSprite(void *, s32, s32, s16, s32, s32, s32, s32, s32) asm("func_080D2660"); /* extern */
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");                         /* extern */
u32 CallFunctionR0(s32) asm("func_080ECD5C");                             /* extern */
s32 DivideSigned32(s32, s32) asm("func_080ECD98");                        /* extern */
s32 ModuloUnsigned32(u32, s32) asm("func_080ECF78");                        /* extern */

void UpdateBattleChargedBeamSparkEffect(void *group) asm("func_080D5BAC");

void UpdateBattleChargedBeamSparkEffect(void *group) {
    s32 emission_phase;
    register s32 charge_y asm("r4");
    register s32 spark_angle asm("r5");
    u32 phase;
    u32 charge_elapsed_frames;
    u32 pause_elapsed_frames;
    u32 segment_count;
    register u32 segment_index asm("r4");
    register u32 random_angle asm("r4");
    u32 random;
    u32 charge_sprite;
    u8 spark_index;
    void *spark_sprite;
    void *sprite_slots;
    void *beam_segment;

    phase = BATTLE_ANIMATION_FIELD(group, u32, state);
    switch (phase) {                              /* irregular */
    case BATTLE_CHARGED_BEAM_START_CHARGE:
        BATTLE_ANIMATION_FIELD(group, u32, sprites[0]) = CreateBattleAnimationSprite(
            group,
            0,
            0U,
            BATTLE_ANIMATION_FIELD(group, s16, x),
            (s32) BATTLE_ANIMATION_FIELD(group, s16, y),
            BATTLE_SPRITE_SEMITRANSPARENT,
            0U,
            0U
        );
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, u32, effect.charged_beam.progress_counter) = 0U;
        BATTLE_ANIMATION_FIELD(group, u32, state) = (u32) (BATTLE_ANIMATION_FIELD(group, u32, state) + 1);
        /* fallthrough */
    case BATTLE_CHARGED_BEAM_EMIT_CHARGE_PARTICLES:
        charge_elapsed_frames = BATTLE_ANIMATION_FIELD(group, u32, effect.charged_beam.progress_counter);
        if (charge_elapsed_frames <= 0x57U) {
            emission_phase = 3 & charge_elapsed_frames;
            if (emission_phase == 0) {
                random = CallFunctionR0(gRandomNumberCallback);
                charge_y = BATTLE_ANIMATION_FIELD(group, s32, y);
                CreateBattleAngledProjectileSprite(
                    group,
                    1,
                    0,
                    BATTLE_ANIMATION_FIELD(group, s16, x),
                    (s32) (s16) (charge_y + (ModuloUnsigned32(BATTLE_ANIMATION_FIELD(group, u32, effect.charged_beam.progress_counter), 6) + (random >> 0xE) + 0xFFFE)),
                    BATTLE_SPRITE_SEMITRANSPARENT,
                    0x80,
                    0x100,
                    emission_phase
                );
            }
            BATTLE_ANIMATION_FIELD(group, u32, effect.charged_beam.progress_counter) = (u32) (BATTLE_ANIMATION_FIELD(group, u32, effect.charged_beam.progress_counter) + 1);
            return;
        }
        BATTLE_ANIMATION_FIELD(group, u32, effect.charged_beam.progress_counter) = 0U;
        BATTLE_ANIMATION_FIELD(group, u32, state) = (u32) (BATTLE_ANIMATION_FIELD(group, u32, state) + 1);
    case BATTLE_CHARGED_BEAM_WAIT_FOR_CHARGE:
        charge_sprite = BATTLE_ANIMATION_FIELD(group, u32, sprites[0]);
        if (charge_sprite != 0) {
            return;
        }
        pause_elapsed_frames = BATTLE_ANIMATION_FIELD(group, u32, effect.charged_beam.progress_counter);
        if (pause_elapsed_frames <= 0x1DU) {
            BATTLE_ANIMATION_FIELD(group, u32, effect.charged_beam.progress_counter) = (u32) (pause_elapsed_frames + 1);
            return;
        }
        PlayBattleAnimationSound(1);
        BATTLE_ANIMATION_FIELD(group, u32, effect.charged_beam.progress_counter) = charge_sprite;
        BATTLE_ANIMATION_FIELD(group, u32, state) = (u32) (BATTLE_ANIMATION_FIELD(group, u32, state) + 1);
    case BATTLE_CHARGED_BEAM_CREATE_SEGMENTS:
        {
        u32 *progress_counter;
        register u32 *progress_counter_init asm("r0") = (u32 *)((s32)group + BATTLE_ANIMATION_OFFSET(effect.charged_beam.progress_counter));
        register u32 segment_count_init asm("r1") = *progress_counter_init;
        asm volatile("" : "+r"(segment_count_init));
        progress_counter = progress_counter_init;
        segment_count = segment_count_init;
        if (segment_count <= 7U) {
            beam_segment = (void *)CreateBattleAnimationSprite(
                group,
                3,
                (u32) ((0 - segment_count) | segment_count) >> 0x1F,
                (s16) (BATTLE_ANIMATION_FIELD(group, s32, x) - (segment_count << 5)),
                (s32) BATTLE_ANIMATION_FIELD(group, s16, y),
                BATTLE_SPRITE_SEMITRANSPARENT,
                0U,
                0U
            );
            segment_index = *progress_counter;
            {
                register u32 sprite_offset asm("r1") = segment_index;
                register char *sprite_slots_init asm("r2") = group;

                sprite_offset <<= 2;
                sprite_slots_init += 0xC;
                *(void **)(sprite_slots_init + sprite_offset) = beam_segment;
                sprite_slots = sprite_slots_init;
            }
            if (segment_index == 0) {
                BATTLE_ANIMATION_FIELD(group, u32, sprites[8]) = CreateBattleAnimationSprite(
                    group,
                    2,
                    0U,
                    BATTLE_ANIMATION_FIELD(group, s16, x),
                    (s32) BATTLE_ANIMATION_FIELD(group, s16, y),
                    BATTLE_SPRITE_SEMITRANSPARENT,
                    segment_index,
                    segment_index
                );
                spark_index = 0;
                do {
                    random_angle = CallFunctionR0(gRandomNumberCallback);
                    spark_angle = DivideSigned32(spark_index << 5, 7);
                    asm volatile("" : "+r"(spark_angle));
                    {
                        register u32 angle_jitter asm("r0") = random_angle * 9;

                        angle_jitter >>= 0xF;
                        angle_jitter += 0x6C;
                        spark_angle += angle_jitter;
                    }
                    spark_sprite = CreateBattleAngledProjectileSprite(
                        group,
                        1,
                        0,
                        BATTLE_ANIMATION_FIELD(group, s16, x),
                        (s32) BATTLE_ANIMATION_FIELD(group, s16, y),
                        (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1),
                        spark_angle,
                        ((u32) (CallFunctionR0(gRandomNumberCallback) * 0x201) >> 0xF) + 0x200,
                        0
                    );
                    {
                        register u32 sprite_offset asm("r1") = spark_index;

                        sprite_offset += 9;
                        sprite_offset <<= 2;
                        asm volatile("" : "+r"(sprite_offset));
                        sprite_offset += (u32)sprite_slots;
                        *(void **)sprite_offset = spark_sprite;
                    }
                    spark_index += 1;
                } while ((u32) spark_index <= 7U);
            }
            *progress_counter = *progress_counter + 1;
            return;
        }
        BATTLE_ANIMATION_FIELD(group, u32, state) = (u32) (BATTLE_ANIMATION_FIELD(group, u32, state) + 1);
        }
    case BATTLE_CHARGED_BEAM_WAIT_FOR_SPRITES:
        spark_index = 0;
        if (BATTLE_ANIMATION_FIELD(group, u32, sprites[0]) == 0) {
            register char *sprite_slots asm("r1") = group + BATTLE_ANIMATION_OFFSET(sprites[0]);
scan_finished_sprites:
            spark_index += 1;
            if ((u32) spark_index <= 0x10U) {
                register u32 sprite_offset asm("r0") = spark_index;
                u32 sprite_address;

                sprite_offset <<= 2;
                sprite_address = (u32)sprite_slots;
                sprite_address += sprite_offset;
                if (*(s32 *)sprite_address == 0) {
                    goto scan_finished_sprites;
                }
            }
        }
        if (spark_index == 0x11) {
            DestroySpriteGroup(group);
        }
        return;
    }
}
