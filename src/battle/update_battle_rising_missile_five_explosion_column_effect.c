#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedRisingMissileTrailSprite(void *) asm("func_080D6D84");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleRisingMissileFiveExplosionColumnEffect(void *group) asm("func_080D7D08");

void UpdateBattleRisingMissileFiveExplosionColumnEffect(void *group) {
    register char *group_bytes asm("r6") = group;
    register u32 state asm("r0") = *(u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));

    switch (state) {
    case BATTLE_MISSILE_EXPLOSION_COLUMN_LAUNCH: {
        register s32 *impact_x_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.impact_x));
        register s32 x asm("r3") = *impact_x_slot - 0x100;
        void *created;

        created = CreateBattleAnimationSprite(group_bytes, 0, 0, (s16)x,
            0xE0, BATTLE_SPRITE_LOOP_ANIMATION, (s32)UpdateBattleTargetedRisingMissileTrailSprite, 1);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = created;
        BATTLE_SPRITE_FIELD(created, void *, user_data.missile_trail.group) = group_bytes;
        BATTLE_SPRITE_FIELD(created, s32, user_data.missile_trail.elapsed_frames) = 0;
        BATTLE_SPRITE_FIELD(created, s32, user_data.missile_trail.impact_x) = *impact_x_slot;
        PlayBattleAnimationSound(0);
        goto advance_column_phase;
    }
    case BATTLE_MISSILE_EXPLOSION_COLUMN_START:
        if ((**(u32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) & BATTLE_SPRITE_HIDDEN) == 0) {
            break;
        }
        {
            register s16 *impact_x_slot asm("r0") = (s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.impact_x));
            register s32 halfword_index asm("r1") = 0;
            register s32 x asm("r3") = impact_x_slot[halfword_index];
            register volatile s32 *outgoing asm("sp");
            register s32 zero asm("r4");

            outgoing[0] = 0x60;
            outgoing[1] = BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_2;
            zero = 0;
            outgoing[2] = zero;
            outgoing[3] = zero;
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = CreateBattleAnimationSprite(group_bytes, 2, 1, x);
            SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
            PlayBattleAnimationSound(1);
            *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.burst_elapsed_frames)) = zero;
            goto advance_column_phase;
    }
    case BATTLE_MISSILE_EXPLOSION_COLUMN_FIRST_BURST: {
        s32 *burst_elapsed_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.burst_elapsed_frames));
        *burst_elapsed_slot = *burst_elapsed_slot + 1;
        if (*burst_elapsed_slot != BATTLE_MISSILE_EXPLOSION_COLUMN_BURST_DELAY_FRAMES) {
            break;
        }
        {
            register s32 *impact_x_slot asm("r5") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.impact_x));
            register s32 zero asm("r4");
            register s32 x asm("r3") = *impact_x_slot - 0x10;
            register volatile s32 *outgoing asm("sp");

            x = (s16)x;
            outgoing[0] = 0x5C;
            zero = 0;
            outgoing[1] = zero;
            outgoing[2] = zero;
            outgoing[3] = zero;
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[2])) = CreateBattleAnimationSprite(group_bytes, 2, 2, x);
            x = *impact_x_slot + 0x14;
            x = (s16)x;
            outgoing[0] = 0x48;
            outgoing[1] = zero;
            outgoing[2] = zero;
            outgoing[3] = zero;
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[3])) = CreateBattleAnimationSprite(group_bytes, 2, 2, x);
            *burst_elapsed_slot = zero;
            goto advance_column_phase;
        }
    }
    case BATTLE_MISSILE_EXPLOSION_COLUMN_SECOND_BURST: {
        register s32 *burst_elapsed_slot asm("r1") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.burst_elapsed_frames));
        *burst_elapsed_slot = *burst_elapsed_slot + 1;
        if (*burst_elapsed_slot != BATTLE_MISSILE_EXPLOSION_COLUMN_BURST_DELAY_FRAMES) {
            break;
        }
        {
            register s32 *impact_x_slot asm("r5") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.impact_x));
            register s32 zero asm("r4");
            register s32 x asm("r3") = *impact_x_slot - 8;
            register volatile s32 *outgoing asm("sp");
            void *created;

            x = (s16)x;
            outgoing[0] = 0x38;
            outgoing[1] = (BATTLE_SPRITE_DOUBLE_CANVAS | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1);
            zero = 0;
            outgoing[2] = zero;
            outgoing[3] = zero;
            created = CreateBattleAnimationSprite(group_bytes, 2, 0, x);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[4])) = created;
            {
                register u32 scale_fixed8 asm("r1") = 0x200;
                BATTLE_SPRITE_FIELD(created, u16, scale) = scale_fixed8;
            }
            x = *impact_x_slot - 8;
            x = (s16)x;
            outgoing[0] = 0x30;
            outgoing[1] = zero;
            outgoing[2] = zero;
            outgoing[3] = zero;
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[5])) = CreateBattleAnimationSprite(group_bytes, 2, 2, x);
            goto advance_column_phase;
        }
    }
advance_column_phase:
    {
        register s32 *state_slot asm("r1") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
        *state_slot = *state_slot + 1;
    }
    break;

    case BATTLE_MISSILE_FIVE_EXPLOSION_COLUMN_WAIT: {
        register u32 live_sprite_index asm("r2") = 0x18;
        register char *missile_bytes asm("r1") = *(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        register u32 trail_slot_count asm("r0") = *(u32 *)(missile_bytes + 0x2C) >> 1;
        register u32 trail_slot_end asm("r3") = trail_slot_count;
        asm volatile("" : "+r"(trail_slot_count));
        trail_slot_end += 0x19;

        if (live_sprite_index < trail_slot_end && *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0) {
            register u32 trail_scan_end asm("r4") = trail_slot_end;
            register char *sprite_slots asm("r3") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
            do {
                register u32 next asm("r0") = live_sprite_index + 1;
                next <<= 24;
                live_sprite_index = next >> 24;
            } while (live_sprite_index < trail_scan_end &&
                *(s32 *)(sprite_slots + (live_sprite_index << 2)) == 0);
        }
        if (live_sprite_index == ((*(u32 *)(missile_bytes + 0x2C) >> 1) + 0x19)) {
            live_sprite_index = 1;
            if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
                register char *sprite_slots asm("r1") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                do {
                    register u32 next asm("r0") = live_sprite_index + 1;
                    next <<= 24;
                    live_sprite_index = next >> 24;
                } while (live_sprite_index <= 5 &&
                    *(s32 *)(sprite_slots + (live_sprite_index << 2)) == 0);
            }
            if (live_sprite_index == 6) {
                DestroySpriteGroup(group_bytes);
            }
        }
        break;
    }
    }
    return;
}
