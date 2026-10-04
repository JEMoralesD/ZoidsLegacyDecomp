#include "m2c_prelude.h"
#include "battle_display.h"

extern u8 gBattleSetup[];
extern u16 D_0300004E;
extern u16 D_03000050;
extern u32 D_03000010;
extern u8 gBattleSceneSide asm("D_02033F36");

void UpdateBattleAnimation(void) asm("func_080D1B78");
void UpdateBattleAnimationCamera(void) asm("func_080D12DC");
void UpdateBattleScanlineWindow(void) asm("func_080D1E58");
void UpdateBattleBackgroundShake(void) asm("func_080D2244");
void UpdateZoidEquipmentAnimation(void) asm("func_080D0644");
u32 CallFunctionR0(u32) asm("func_080ECD5C");
void *CreateSpriteFromTable(u32, u32, u32, s32, s32, s32, s32, s32, s32) asm("func_08094374");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

void RunBattleSceneAnimationUpdates(void) asm("func_080CD2DC");

void RunBattleSceneAnimationUpdates(void) {
    void *cloud_sprites[BATTLE_DIAGONAL_CLOUD_COUNT];
    u8 slot_index;

    for (slot_index = 0; slot_index <= 7; slot_index++) {
        cloud_sprites[slot_index] = 0;
    }

    for (;;) {
        UpdateBattleAnimation();
        UpdateBattleAnimationCamera();
        UpdateBattleScanlineWindow();
        UpdateBattleBackgroundShake();
        UpdateZoidEquipmentAnimation();

        if (gBattleSetup[2] == BATTLE_DIAGONAL_CLOUD_SCENE_ID) {
            D_0300004E = 0x740;
            D_03000050 = 0x810;

            for (slot_index = 0; slot_index <= 7; slot_index++) {
                if (cloud_sprites[slot_index] != 0 && (*(u32 *)cloud_sprites[slot_index] & 1) == 0) {
                    cloud_sprites[slot_index] = 0;
                }
            }

            for (slot_index = 0; slot_index <= 7; slot_index++) {
                register s32 slot_offset asm("r5");
                register s32 slot_offset_source asm("r0");
                register void *cloud_sprite asm("r1");

                slot_offset_source = slot_index * 4;
                cloud_sprite = *(void **)((u8 *)cloud_sprites + slot_offset_source);
                slot_offset = slot_offset_source;
                if (cloud_sprite == 0) {
                    register s32 spawn_x asm("r4");
                    register s32 random_x_offset asm("r1");
                    register u32 random asm("r0");
                    register s32 hardware_priority_flags asm("r2");
                    register s32 sprite_flags asm("r1");
                    register s32 spawn_x_halfword_bits asm("r0");

                    random = CallFunctionR0(D_03000010);
                    asm volatile(".short 0x0041, 0x1809, 0x0109"
                                 : "=r"(random_x_offset) : "r"(random));
                    random_x_offset -= random;
                    random_x_offset <<= 4;
                    random_x_offset = (u32)random_x_offset >> 15;
                    if (gBattleSceneSide == 0) {
                        spawn_x_halfword_bits = (random_x_offset - 0x200) << 16;
                    } else {
                        spawn_x_halfword_bits = random_x_offset << 16;
                    }
                    spawn_x = spawn_x_halfword_bits >> 16;

                    random = CallFunctionR0(D_03000010);
                    random >>= 14;
                    hardware_priority_flags = 0xC8;
                    if (random != 0) {
                        hardware_priority_flags = 0x48;
                    }
                    if (gBattleSceneSide != 0) {
                        sprite_flags = 0x8400;
                    } else {
                        sprite_flags = 0x400;
                    }
                    sprite_flags |= hardware_priority_flags;

                    cloud_sprite = CreateSpriteFromTable(BATTLE_DIAGONAL_CLOUD_SPRITE_TABLE_ROM, 9, 0, spawn_x,
                        0xC0, BATTLE_DIAGONAL_CLOUD_TILE_OFFSET, BATTLE_DIAGONAL_CLOUD_PALETTE_BANK, sprite_flags, BATTLE_DIAGONAL_CLOUD_UPDATE_CALLBACK);
                    *(void **)((u8 *)cloud_sprites + slot_offset) = cloud_sprite;
                    asm volatile("" : : "r"(slot_offset));
                    break;
                }
            }
        }

        YieldTaskForUpdates(1);
    }
}
