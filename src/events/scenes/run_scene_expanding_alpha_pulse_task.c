#include "m2c_prelude.h"
#include "../../graphics/screen_effects.h"
#include "../../battle/battle_display.h"

void PlaySong(s32) asm("func_08092E84");
struct BattleDisplaySprite *CreateSprite(s32, s32, s32, s32, s32,
    s32, s32, s32, s32) asm("func_08094484");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

void RunSceneExpandingAlphaPulseTask(void) asm("func_080A876C");

void RunSceneExpandingAlphaPulseTask(void)
{
    struct BattleDisplaySprite *pulse_sprite;
    register void **raw_slot asm("r1");
    register struct BattleDisplaySprite **slot asm("r5");
    register u32 pulse_update asm("r4");
    register volatile u16 *blend asm("r6");

    pulse_sprite = CreateSprite(0x0821024C, 0x08210258, 0, 0x78,
        0x58, 0, 0, 0x887C8, 0);
    raw_slot = (void **)0x02031C94;
    asm volatile("" : "+r"(raw_slot));
    *raw_slot = pulse_sprite;
    asm volatile("" : "+r"(raw_slot));
    pulse_update = 0;
    slot = (struct BattleDisplaySprite **)raw_slot;
    asm volatile("" : "+r"(slot));
    blend = (volatile u16 *)0x03000050;

loop:
    if (pulse_update <= 0xF) {
        struct BattleDisplaySprite *pulse_sprite_for_update;
        s32 value;

        if (pulse_update == 0) {
            PlaySong(0x71);
        }
        pulse_sprite_for_update = *slot;
        pulse_sprite_for_update->scale = (pulse_update << 4) + 0x100;
        value = pulse_sprite_for_update->scale;
        if (value < 0) {
            value += 7;
        }
        pulse_sprite_for_update->y = (value >> 3) + 0x38;
        *blend = (0x10 - pulse_update) | 0x1000;
    } else {
        (*slot)->scale = 0;
    }

    {
        register s32 next asm("r1") = pulse_update + 1;
        register s32 reduced asm("r0") = next;
        asm volatile("" : "+r"(reduced));
        reduced >>= 5;
        reduced <<= 5;
        reduced = next - reduced;
        pulse_update = (u8)reduced;
    }
    YieldTaskForUpdates(1);
    goto loop;
}
