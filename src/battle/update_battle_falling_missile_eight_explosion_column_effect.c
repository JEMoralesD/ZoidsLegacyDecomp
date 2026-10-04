#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedFallingMissileTrailSprite(void *) asm("func_080D6E6C");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleFallingMissileEightExplosionColumnEffect(void *group) asm("func_080D887C");

void UpdateBattleFallingMissileEightExplosionColumnEffect(void *group) {
    char *group_bytes = group;
    register u32 state asm("r0") = *(u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));

    switch (state) {
    case BATTLE_MISSILE_EXPLOSION_COLUMN_LAUNCH: {
        register s32 *impact_x_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.impact_x));
        register s32 x asm("r3") = *impact_x_slot - 0x100;
        void *created;

        created = CreateBattleAnimationSprite(group_bytes, 0, 0, (s16)x,
            -0x10, BATTLE_SPRITE_LOOP_ANIMATION, (s32)UpdateBattleTargetedFallingMissileTrailSprite, 1);
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

            outgoing[0] = 0x70;
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
        register s32 *burst_elapsed_slot asm("r5") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.burst_elapsed_frames));
        *burst_elapsed_slot = *burst_elapsed_slot + 1;
        if (*burst_elapsed_slot != BATTLE_MISSILE_EXPLOSION_COLUMN_BURST_DELAY_FRAMES) {
            break;
        }
        {
            register s32 *impact_x_slot asm("r0") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.impact_x));
            register s32 x asm("r3") = *impact_x_slot + 1;
            register s32 zero asm("r4");
            register volatile s32 *outgoing asm("sp");
            void *created;

            x = (s16)x;
            outgoing[0] = 0x62;
            outgoing[1] = (BATTLE_SPRITE_DOUBLE_CANVAS | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1);
            zero = 0;
            outgoing[2] = zero;
            outgoing[3] = zero;
            created = CreateBattleAnimationSprite(group_bytes, 2, 1, x);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[2])) = created;
            {
                register u32 scale_fixed8 asm("r1") = 0x140;
                BATTLE_SPRITE_FIELD(created, u16, scale) = scale_fixed8;
            }
            *burst_elapsed_slot = zero;
            asm volatile("");
            goto advance_column_phase;
        }
    }
    case BATTLE_MISSILE_EXPLOSION_COLUMN_SECOND_BURST: {
        register s32 *burst_elapsed_slot asm("r5") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.burst_elapsed_frames));
        *burst_elapsed_slot = *burst_elapsed_slot + 1;
        if (*burst_elapsed_slot != BATTLE_MISSILE_EXPLOSION_COLUMN_BURST_DELAY_FRAMES) {
            break;
        }
        {
            register s32 *impact_x_slot asm("r0") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.impact_x));
            register s32 x asm("r3") = *impact_x_slot - 0xB;
            register s32 zero asm("r4");
            register volatile s32 *outgoing asm("sp");
            void *created;

            x = (s16)x;
            outgoing[0] = 0x44;
            outgoing[1] = BATTLE_SPRITE_DOUBLE_CANVAS;
            zero = 0;
            outgoing[2] = zero;
            outgoing[3] = zero;
            created = CreateBattleAnimationSprite(group_bytes, 2, 0, x);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[3])) = created;
            {
                register u32 scale_fixed8 asm("r1") = 0x180;
                BATTLE_SPRITE_FIELD(created, u16, scale) = scale_fixed8;
            }
            *burst_elapsed_slot = zero;
            goto advance_column_phase;
        }
    }
    case BATTLE_MISSILE_EXPLOSION_COLUMN_THIRD_BURST: {
        register s32 *burst_elapsed_slot asm("r9") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.burst_elapsed_frames));
        register s32 elapsed_burst_frames asm("r0") = *burst_elapsed_slot;
        elapsed_burst_frames += 1;
        *burst_elapsed_slot = elapsed_burst_frames;
        if (elapsed_burst_frames != BATTLE_MISSILE_EXPLOSION_COLUMN_BURST_DELAY_FRAMES) {
            break;
        }
        {
            register s32 *impact_x_slot asm("r8");
            register s32 impact_x_address asm("r0") = BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.impact_x);
            register s32 sprite_flags asm("r6");
            register u32 scale_fixed8 asm("r5");
            register s32 zero asm("r4");
            register s32 x asm("r3");
            register volatile s32 *outgoing asm("sp");
            register void *created asm("r0");

            asm volatile("" : "+r"(impact_x_address));
            impact_x_address += (s32)group_bytes;
            impact_x_slot = (s32 *)impact_x_address;
            x = (s16)(*(s32 *)impact_x_address + 0x12);
            outgoing[0] = 0x3E;
            sprite_flags = (BATTLE_SPRITE_DOUBLE_CANVAS | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1);
            outgoing[1] = sprite_flags;
            zero = 0;
            outgoing[2] = zero;
            outgoing[3] = zero;
            created = CreateBattleAnimationSprite(group_bytes, 2, 1, x);
            asm volatile("" : "+r"(created));
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[4])) = created;
            scale_fixed8 = 0x140;
            BATTLE_SPRITE_FIELD(created, u16, scale) = scale_fixed8;

            {
                register s32 *impact_x_view asm("r1") = impact_x_slot;
                x = (s16)(*impact_x_view - 7);
            }
            outgoing[0] = 0x31;
            outgoing[1] = sprite_flags;
            outgoing[2] = zero;
            outgoing[3] = zero;
            created = CreateBattleAnimationSprite(group_bytes, 2, 1, x);
            asm volatile("" : "+r"(created));
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[5])) = created;
            BATTLE_SPRITE_FIELD(created, u16, scale) = scale_fixed8;
            *burst_elapsed_slot = zero;
            goto advance_column_phase;
        }
    }
    case BATTLE_MISSILE_EXPLOSION_COLUMN_FOURTH_BURST: {
        register s32 *burst_elapsed_slot asm("r8");
        register s32 burst_elapsed_address asm("r1") = BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.burst_elapsed_frames);
        register s32 elapsed_burst_frames asm("r0");
        asm volatile("" : "+r"(burst_elapsed_address));
        burst_elapsed_address += (s32)group_bytes;
        burst_elapsed_slot = (s32 *)burst_elapsed_address;
        elapsed_burst_frames = *(s32 *)burst_elapsed_address;
        elapsed_burst_frames += 1;
        *(s32 *)burst_elapsed_address = elapsed_burst_frames;
        if (elapsed_burst_frames != BATTLE_MISSILE_EXPLOSION_COLUMN_BURST_DELAY_FRAMES) {
            break;
        }
        {
            register s32 *impact_x_slot asm("r6") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.impact_x));
            register s32 sprite_flags asm("r5");
            register s32 zero asm("r4");
            register s32 x asm("r3") = *impact_x_slot - 0x1E;
            register volatile s32 *outgoing asm("sp");
            void *created;

            x = (s16)x;
            outgoing[0] = 0x41;
            sprite_flags = (BATTLE_SPRITE_DOUBLE_CANVAS | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_2);
            outgoing[1] = sprite_flags;
            zero = 0;
            outgoing[2] = zero;
            outgoing[3] = zero;
            created = CreateBattleAnimationSprite(group_bytes, 2, 1, x);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[6])) = created;
            {
                register u32 scale_fixed8 asm("r1") = 0x140;
                BATTLE_SPRITE_FIELD(created, u16, scale) = scale_fixed8;
            }

            x = *impact_x_slot + 0x14;
            x = (s16)x;
            outgoing[0] = 0x28;
            outgoing[1] = sprite_flags;
            outgoing[2] = zero;
            outgoing[3] = zero;
            created = CreateBattleAnimationSprite(group_bytes, 2, 0, x);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[7])) = created;
            {
                register u32 scale_fixed8 asm("r1") = 0x1C0;
                BATTLE_SPRITE_FIELD(created, u16, scale) = scale_fixed8;
            }
            *burst_elapsed_slot = zero;
            goto advance_column_phase;
        }
    }
    case BATTLE_MISSILE_EXPLOSION_COLUMN_FIFTH_BURST: {
        register s32 *burst_elapsed_slot asm("r1") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.burst_elapsed_frames));
        *burst_elapsed_slot = *burst_elapsed_slot + 1;
        if (*burst_elapsed_slot != BATTLE_MISSILE_EXPLOSION_COLUMN_BURST_DELAY_FRAMES) {
            break;
        }
        {
            register s32 x asm("r3");
            register volatile s32 *outgoing asm("sp");
            void *created;

            {
                register s32 *impact_x_slot asm("r0") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.impact_x));
                x = *impact_x_slot - 0xD;
            }
            x = (s16)x;
            outgoing[0] = 0x1E;
            outgoing[1] = (BATTLE_SPRITE_DOUBLE_CANVAS | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_2);
            {
                register s32 zero asm("r0") = 0;
                outgoing[2] = zero;
                outgoing[3] = zero;
            }
            created = CreateBattleAnimationSprite(group_bytes, 2, 0, x);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[8])) = created;
            {
                register u32 scale_fixed8 asm("r1") = 0x200;
                BATTLE_SPRITE_FIELD(created, u16, scale) = scale_fixed8;
            }
            goto advance_column_phase;
        }
    }
advance_column_phase:
    {
        register s32 *state_slot asm("r1") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
        *state_slot = *state_slot + 1;
    }
    break;

    case BATTLE_MISSILE_EIGHT_EXPLOSION_COLUMN_WAIT: {
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
                } while (live_sprite_index <= 8 &&
                    *(s32 *)(sprite_slots + (live_sprite_index << 2)) == 0);
            }
            if (live_sprite_index == 9) {
                DestroySpriteGroup(group_bytes);
            }
        }
        break;
    }
    }
    return;
}
