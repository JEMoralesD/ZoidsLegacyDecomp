#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s16 Sin256(s16) asm("func_08092A90");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideUnsigned32(s32, s32) asm("func_080ECF00");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleScreenWidthBeamImpactEffect(void *group) asm("func_080D6050");

void UpdateBattleScreenWidthBeamImpactEffect(void *group) {
    char *group_bytes = group;
    register s32 *beam_segment_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 beam_segment_index asm("r3") = *beam_segment_slot;

    if (beam_segment_index <= 7) {
        register s32 one asm("r6");
        register s32 x_value asm("r3") = (s16)(beam_segment_index << 5);
        register volatile s32 *outgoing asm("sp");
        void *created;

        outgoing[0] = *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y));
        outgoing[1] = (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1);
        outgoing[2] = 0;
        one = 1;
        outgoing[3] = one;
        created = CreateBattleAnimationSprite(group_bytes, 0, 1, x_value);
        beam_segment_index = *beam_segment_slot;
        {
            register s32 offset asm("r2") = beam_segment_index << 2;
            register char *slot asm("r1") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
            asm volatile("add %0, %1" : "+r"(slot) : "r"(offset));
            *(void **)slot = created;
        }
        if (beam_segment_index == 0) {
            PlayBattleAnimationSound(0);
        }
        {
            register s32 *impact_started asm("r5") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.screen_width_beam_impact.impact_started));
            /* Unsigned subtraction starts the impact at segment 0 for screen positions. */
            if (*impact_started == 0 &&
                    (u32)((*beam_segment_slot << 5) - 0x20) >
                    (u32)*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x))) {
                SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
                PlayBattleAnimationSound(1);
                *impact_started = one;
            }
        }
        {
            register s32 *slot asm("r1") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
            *slot = *slot + 1;
        }
    }

    {
        register s32 *impact_count_slot asm("sl");
        register s32 impact_started asm("r0") = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.screen_width_beam_impact.impact_started));
        register s32 initial_impact_count asm("r0");
        register s32 *impact_count_init asm("r1") = (s32 *)BATTLE_ANIMATION_OFFSET(effect.screen_width_beam_impact.impact_count);
        asm volatile("add %0, %0, %1"
                     : "+r"(impact_count_init) : "r"(group_bytes));
        impact_count_slot = impact_count_init;

        if (impact_started != 0 &&
                (initial_impact_count = *impact_count_init, (u32)initial_impact_count <= 0xF)) {
            register s32 spread_bits asm("r5");
            register s32 spread asm("r8");
            register s32 random asm("r9");
            register s32 y_value asm("r4");
            register s32 impact_index asm("r6");
            register s32 spread_phase asm("r0");
            register s32 x_value asm("r3");
            s32 spread_sine;
            void *created;

            spread_sine = Sin256((s16)DivideUnsigned32(
                initial_impact_count << 7, 0xF));
            if (spread_sine < 0) {
                spread_sine += 0xF;
            }
            spread_bits = spread_sine >> 4;
            spread_bits <<= 24;
            {
                register s32 spread_init asm("r0") = (u32)spread_bits >> 24;
                spread = spread_init;
            }
            random = CallFunctionR0(gRandomNumberCallback);
            y_value = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y));
            {
                register s32 *impact_count_view asm("r1") = impact_count_slot;
                impact_index = *impact_count_view;
            }
            spread_phase = 3 & impact_index;
            spread_phase *= spread;
            y_value += DivideUnsigned32(spread_phase, 3);
            y_value -= (u32)spread_bits >> 25;
            {
                register s32 y_jitter asm("r0") =
                    (u32)(random * 9) >> 15;
                y_jitter += 0xFFFC;
                y_value += y_jitter;
            }
            y_value = (s16)y_value;
            x_value = (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)) + (impact_index << 2) - 0x20);
            created = CreateBattleAnimationSprite(group_bytes, 1, 0, x_value, y_value, 0, 0, 0);
            {
                register s32 next_impact_count asm("r3") = *impact_count_slot;
                register s32 offset asm("r2") = next_impact_count + 8;
                register char *slot asm("r1");
                offset <<= 2;
                slot = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                asm volatile("add %0, %1" : "+r"(slot) : "r"(offset));
                *(void **)slot = created;
                next_impact_count += 1;
                *impact_count_slot = next_impact_count;
            }
        }

        {
            register s32 *impact_count_check asm("r1") = impact_count_slot;
            initial_impact_count = *impact_count_check;
        }
        if (initial_impact_count == 0x10) {
            u8 live_sprite_index = 0;
            if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
                register char *sprite_slots asm("r2") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                do {
                    live_sprite_index = (u8)(live_sprite_index + 1);
                } while (live_sprite_index <= 0x17 &&
                    *(s32 *)(sprite_slots + (live_sprite_index << 2)) == 0);
            }
            if (live_sprite_index == 0x18) {
                DestroySpriteGroup(group_bytes);
            }
        }
    }
}
