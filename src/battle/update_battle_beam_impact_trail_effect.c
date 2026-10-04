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

void UpdateBattleBeamImpactTrailEffect(void *group) asm("func_080D3158");

void UpdateBattleBeamImpactTrailEffect(void *group) {
    char *owner = group;
    register s32 *state_slot asm("r4") = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(state));
    register u32 state asm("r3") = *state_slot;

    if (state <= 7) {
        register s32 one asm("r6");
        register s32 x_value asm("r3") = (s16)(state << 5);
        register volatile s32 *outgoing asm("sp");
        void *created;

        outgoing[0] = *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y));
        outgoing[1] = 0x500;
        outgoing[2] = 0;
        one = 1;
        outgoing[3] = one;
        created = CreateBattleAnimationSprite(owner, 0, 0, x_value);
        state = *state_slot;
        {
            register s32 offset asm("r2") = state << 2;
            register char *slot asm("r1") = owner + BATTLE_ANIMATION_OFFSET(sprites[0]);
            asm volatile("add %0, %1" : "+r"(slot) : "r"(offset));
            *(void **)slot = created;
        }
        if (state == 0) {
            PlayBattleAnimationSound(0);
        }
        {
            register s32 *started asm("r5") = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.beam_trail.started));
            if (*started == 0 &&
                    (u32)((*state_slot << 5) - 0x20) >
                    (u32)*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(x))) {
                SetBattleAnimationCameraMode(6, 0);
                PlayBattleAnimationSound(1);
                *started = one;
            }
        }
        {
            register s32 *slot asm("r1") = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(state));
            *slot = *slot + 1;
        }
    }

    {
        register s32 *count_slot asm("sl");
        register s32 started asm("r0") = *(s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.beam_trail.started));
        register s32 initial_count asm("r0");
        register s32 *count_init asm("r1") = (s32 *)BATTLE_ANIMATION_OFFSET(effect.beam_trail.sprite_count);
        asm volatile("add %0, %0, %1"
                     : "+r"(count_init) : "r"(owner));
        count_slot = count_init;

        if (started != 0 &&
                (initial_count = *count_init, (u32)initial_count <= 0xF)) {
            register s32 scaled asm("r5");
            register s32 scale_byte asm("r8");
            register s32 random asm("r9");
            register s32 y_value asm("r4");
            register s32 count asm("r6");
            register s32 phase asm("r0");
            register s32 x_value asm("r3");
            s32 trig;
            void *created;

            trig = Sin256((s16)DivideUnsigned32(
                initial_count << 7, 0xF));
            if (trig < 0) {
                trig += 0xF;
            }
            scaled = trig >> 4;
            scaled <<= 24;
            {
                register s32 scale_init asm("r0") = (u32)scaled >> 24;
                scale_byte = scale_init;
            }
            random = CallFunctionR0(gRandomNumberCallback);
            y_value = *(s32 *)(owner + BATTLE_ANIMATION_OFFSET(y));
            {
                register s32 *count_view asm("r1") = count_slot;
                count = *count_view;
            }
            phase = 3 & count;
            phase *= scale_byte;
            y_value += DivideUnsigned32(phase, 3);
            y_value -= (u32)scaled >> 25;
            {
                register s32 perturb asm("r0") =
                    (u32)(random * 9) >> 15;
                perturb += 0xFFFC;
                y_value += perturb;
            }
            y_value = (s16)y_value;
            x_value = (s16)(*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(x)) + (count << 2) - 0x20);
            created = CreateBattleAnimationSprite(owner, 1, 0, x_value, y_value, 0, 0, 0);
            {
                register s32 count_after asm("r3") = *count_slot;
                register s32 offset asm("r2") = count_after + 8;
                register char *slot asm("r1");
                offset <<= 2;
                slot = owner + BATTLE_ANIMATION_OFFSET(sprites[0]);
                asm volatile("add %0, %1" : "+r"(slot) : "r"(offset));
                *(void **)slot = created;
                count_after += 1;
                *count_slot = count_after;
            }
        }

        {
            register s32 *count_check asm("r1") = count_slot;
            initial_count = *count_check;
        }
        if (initial_count == 0x10) {
            u8 index = 0;
            if (*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
                register char *children asm("r2") = owner + BATTLE_ANIMATION_OFFSET(sprites[0]);
                do {
                    index = (u8)(index + 1);
                } while (index <= 0x17 &&
                    *(s32 *)(children + (index << 2)) == 0);
            }
            if (index == 0x18) {
                DestroySpriteGroup(owner);
            }
        }
    }
}
