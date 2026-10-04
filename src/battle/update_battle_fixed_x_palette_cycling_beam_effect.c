#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
M2C_UNK QueueCopy(s32, s32, s32) asm("func_08095208");
void *CreateBattleAnimationSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2450");
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");

extern u8 gBattlePaletteCyclingBeamPaletteSequence[] asm("D_087A2CCC");
extern u8 gBattlePaletteCyclingBeamPaletteBlocks[] asm("D_087A2B0C");
extern s32 gBattleAnimationPaletteBanks asm("D_020348B4");
extern u8 gSpritePaletteMemory[] asm("D_05000200");

void UpdateBattleFixedXPaletteCyclingBeamEffect(struct BattleAnimationGroup *group) asm("func_080DF288");

void UpdateBattleFixedXPaletteCyclingBeamEffect(struct BattleAnimationGroup *group) {
    s32 phase;
    register void *beam_sprite asm("r2");

    phase = BATTLE_ANIMATION_FIELD(group, s32, state);
    if (phase == BATTLE_CONTACT_EFFECT_CREATE) {
        BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = CreateBattleAnimationSprite(group, 0, 1, 0x80, (s32) BATTLE_ANIMATION_FIELD(group, s16, y), BATTLE_SPRITE_SEMITRANSPARENT, phase, 1);
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, s32, state) = (s32) (BATTLE_ANIMATION_FIELD(group, s32, state) + 1);
        return;
    }
    beam_sprite = BATTLE_ANIMATION_FIELD(group, void *, sprites[0]);
    if (beam_sprite == 0) {
        DestroySpriteGroup(group);
        return;
    }
    if (BATTLE_SPRITE_FIELD(beam_sprite, u16, frame_timer) == 0) {
        QueueCopy(
            ((s32)gBattlePaletteCyclingBeamPaletteSequence[BATTLE_SPRITE_FIELD(beam_sprite, u16, animation_step)] << 5) +
                (s32)gBattlePaletteCyclingBeamPaletteBlocks,
            (gBattleAnimationPaletteBanks << 5) + (s32)gSpritePaletteMemory,
            0x20);
    }
}
