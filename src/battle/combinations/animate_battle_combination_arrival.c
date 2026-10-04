#include "m2c_prelude.h"
#include "battle_combination.h"
#include "../popups/battle_popup.h"
#include "../../graphics/screen_effects.h"
#define NULL ((void *)0)

M2C_UNK InitializePerspectiveScanlineBuffers(M2C_UNK, s32) asm("func_08093AE8");                /* extern */
void *CreateSpriteFromTable() asm("func_08094374"); /* extern */
M2C_UNK DestroySprite() asm("func_08094554");                            /* extern */
M2C_UNK StartScreenTransition(s32, s32) asm("func_08096308");                    /* extern */
s32 IsScreenTransitionComplete() asm("func_0809669C");                                /* extern */
M2C_UNK ConfigureBattleUnitSprites() asm("func_080BAF2C"); /* extern */
M2C_UNK StartBattleCameraTransition(s32, u8, s32, s32) asm("func_080BB224");           /* extern */
s32 IsBattleCameraTransitionComplete() asm("func_080BB654");                                /* extern */
M2C_UNK LoadBattlePopupGraphics(u8, s32) asm("func_080C9F00");                     /* extern */
s32 IsBattleUnitActive(s32, void *) asm("func_080E9D88");                      /* extern */
u8 ModuloUnsigned32(void *, s32) asm("func_080ECF78");                      /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */

extern u8 gBattleCombinationPresentation[] asm("D_02032F7C");
extern s16 gBlendAlpha[] asm("D_03000050");
void AnimateBattleCombinationArrival(u8 side) asm("func_080C3050");

void AnimateBattleCombinationArrival(u8 side) {
    void *arrival_particle_sprites[6];
    s32 unused_stack04;
    s32 unused_stack08;
    s32 zero;
    s32 *changed_unit_sprite_flags;
    s32 *opaque_sprite_flags;
    s32 *restore_sprite_flags;
    s32 arrival_side_sprite_offset;
    s32 arrival_sprite_table;
    s32 opaque_side_sprite_offset;
    s32 opaque_sprite_table;
    s32 opponent_side_offset;
    s32 battle_state_address;
    s16 *blend_alpha;
    s32 restore_side_sprite_offset;
    s32 changed_sprite_table;
    s32 changed_sprite_offset;
    s32 restoration_sprite_table;
    s32 current_world_x;
    s32 camera_center_y;
    void *perspective_camera;
    register s32 position_byte_offset asm("r2");
    s32 restore_slot_times_four;
    s32 restore_stride_five;
    register s32 world_positions_x asm("r3");
    register s32 world_positions_y asm("r1");
    register s32 restore_slot_carrier asm("r8");
    register u8 model_id asm("r4");
    u8 palette_variant;
    u8 size_class;
    s32 changed_slot_times_four;
    s32 camera_center_y_delta;
    register s32 restored_sprite_flags asm("r0");
    s32 arrival_world_x;
    s32 unused_sprite_work;
    s32 unused_camera_work;
    u8 presentation_state;
    register s32 opponent_side asm("r4");
    u8 side_byte;
    u8 particle_unit_slot;
    u8 arrival_update;
    void *arrival_particle;
    register void *position_index asm("r1");
    void *restore_unit_offset;
    void *restore_unit_record;
    register void *arrival_unit_sprite asm("r1");
    void *changed_unit_record;
    void *unused_unit_work;
    void *opponent_unit_offset;
    void *opponent_unit_record;
    void *particle_resources;
    void *unused_projection_work;
    void *arrival_unit_slot;
    void *opaque_unit_slot;
    void *changed_unit_slot;
    void *restore_unit_slot;
    void *opponent_unit_slot;

    side_byte = side;
    LoadBattlePopupGraphics(side_byte, BATTLE_POPUP_GRAPHICS_BLUE_PARTICLES);
    changed_unit_slot = NULL;
loop_1:
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    asm("" :: "r"(changed_unit_slot));
    if ((IsBattleUnitActive(side_byte, changed_unit_slot) << 0x18) != 0) {
        goto block_3;
    }
    goto block_17;
block_3:
    if (gBattleCombinationPresentation[(s32) changed_unit_slot] == BATTLE_COMBINATION_CHANGED_MODEL) {
        goto block_5;
    }
    goto block_17;
block_5:
    changed_slot_times_four = (s32) changed_unit_slot * 4;
    changed_unit_record = (void *) (changed_slot_times_four + (s32) changed_unit_slot);
    changed_unit_record = (void *) ((s32) changed_unit_record * 8);
    asm("" : "+r"(changed_unit_record));
    changed_unit_record = (void *) ((s32) changed_unit_record - (s32) changed_unit_slot);
    changed_unit_record = (void *) ((s32) changed_unit_record * 0x10);
    asm("" : "+r"(changed_unit_record));
    changed_unit_record = (void *) ((s32) changed_unit_record + (side_byte * 0x1380));
    changed_unit_record = (void *) ((s32) changed_unit_record + 0x02034B4C);
    asm("" : "+r"(changed_unit_record));
    ConfigureBattleUnitSprites(((struct BattleUnit *)changed_unit_record)->zoid_id, ((struct BattleUnit *)changed_unit_record)->palette_variant, ((struct BattleUnit *)changed_unit_record)->size_class, side_byte, changed_unit_slot, (zero = 0), zero);
    changed_sprite_table = 0x02032E8C;
    asm("" : "+r"(changed_sprite_table));
    changed_sprite_offset = changed_slot_times_four + (side_byte * 0x18);
    changed_unit_sprite_flags = *(s32 **)(changed_sprite_offset + changed_sprite_table);
    *changed_unit_sprite_flags |= BATTLE_SPRITE_DOUBLE_CANVAS;
    particle_resources = (void *)BATTLE_POPUP_SPRITE_RESOURCE_TABLE_ROM;
    asm("" : "+r"(particle_resources));
    asm("" :: "r"(changed_slot_times_four));
    asm("" :: "r"(changed_slot_times_four));
    asm("" :: "r"(changed_slot_times_four));
    asm("" :: "r"(changed_slot_times_four));
    arrival_particle = CreateSpriteFromTable(particle_resources, BATTLE_POPUP_RESOURCE_BLUE_PARTICLES, 0, 0, zero,
                           (side_byte == 0 ? 0x180 : 0),
                           (side_byte != 0) ? 0 : 6,
                           ({ register s32 v asm("r4");
                              if (side_byte == 0) {
                                  v = 3;
                                  v -= ModuloUnsigned32(changed_unit_slot, 3);
                                  v <<= 6;
                              } else {
                                  v = ((u32) (ModuloUnsigned32(changed_unit_slot, 3) << 0x18) >> 0x12) + 0x40;
                              }
                              v |= BATTLE_SPRITE_DOUBLE_CANVAS | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_2 | BATTLE_SPRITE_LOOP_ANIMATION;
                              if (side_byte != 0) {
                                  v |= BATTLE_SPRITE_FLIP_X;
                              }
                              v; }),
                           BATTLE_COMBINATION_PROJECTED_SPRITE_CALLBACK);
    arrival_particle_sprites[(s32) changed_unit_slot] = arrival_particle;
    position_index = (void *) (side_byte * 6);
    position_index = (void *) ((s32) position_index + (s32) changed_unit_slot);
    position_byte_offset = (s32) position_index * 12;
    asm("" :: "r"(position_index));
    world_positions_x = 0x087A2790;
    BATTLE_SPRITE_FIELD(arrival_particle, s32, user_data.position.x) = (s32) M2C_FIELD(position_byte_offset, s32 *, world_positions_x);
    world_positions_y = world_positions_x;
    asm("" : "+r"(world_positions_y));
    world_positions_y += 4;
    BATTLE_SPRITE_FIELD(arrival_particle, s32, user_data.position.y) = (s32) (M2C_FIELD(position_byte_offset, s32 *, world_positions_y) + 0xFFFFE000);
    BATTLE_SPRITE_FIELD(arrival_particle, s32, user_data.position.z) = (s32) M2C_FIELD(position_byte_offset, s32 *, 0x087A2798);
    goto block_18;
block_17:
    arrival_particle_sprites[(s32) changed_unit_slot] = 0;
block_18:
    changed_unit_slot = (void *) (u8) (changed_unit_slot + 1);
    if ((u32) changed_unit_slot > 5U) {
        goto block_20;
    }
    goto loop_1;
block_20:
    InitializePerspectiveScanlineBuffers(0x0202F094, 1);
    *(u8 *)0x03000074 |= 1;
    StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 8);
    goto loop_22;
block_21:
    YieldTaskForUpdates(1);
loop_22:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto block_21;
    }
    StartBattleCameraTransition(2, side_byte, 0, 0);
    goto loop_30;
block_24:
    if (*(u8 *)0x02030664 != 1) {
        goto block_29;
    }
    perspective_camera = (void *)0x030033C4;
    camera_center_y = CAMERA_FIELD(perspective_camera, s32, projection.screen_center_y);
    if (camera_center_y <= 0x58) {
        goto block_29;
    }
    camera_center_y_delta = 0x58 - camera_center_y;
    if (camera_center_y_delta >= 0) {
        goto block_28;
    }
    camera_center_y_delta += 3;
block_28:
    CAMERA_FIELD(perspective_camera, s32, projection.screen_center_y) = (s32) (camera_center_y + (camera_center_y_delta >> 2));
block_29:
    YieldTaskForUpdates(1);
loop_30:
    if ((IsBattleCameraTransitionComplete() << 0x18) == 0) {
        goto block_24;
    }
    particle_unit_slot = 0;
loop_32:
    if (((s32) arrival_particle_sprites[particle_unit_slot]) == 0) {
        goto block_34;
    }
    DestroySprite();
block_34:
    particle_unit_slot += 1;
    if ((u32) particle_unit_slot <= 5U) {
        goto loop_32;
    }
    asm("" : "+r"(side_byte));
    restore_unit_slot = NULL;
    restoration_sprite_table = 0x02032E8C;
    asm("" : "+r"(restoration_sprite_table));
    restore_side_sprite_offset = side_byte * 0x18;
loop_36:
    if ((IsBattleUnitActive(side_byte, restore_unit_slot) << 0x18) == 0) {
        goto block_46;
    }
    presentation_state = gBattleCombinationPresentation[(s32) restore_unit_slot];
    if (presentation_state != BATTLE_COMBINATION_UNCHANGED_UNIT) {
        goto block_43;
    }
    restore_slot_times_four = (s32) restore_unit_slot * 4;
    restore_stride_five = restore_slot_times_four + (s32) restore_unit_slot;
    restore_unit_offset = (((restore_stride_five * 8) - (s32) restore_unit_slot) * 0x10) + (side_byte * 0x1380);
    restore_unit_record = restore_unit_offset + 0x02034B4C;
    asm("" : "+r"(restore_unit_record));
    model_id = ((struct BattleUnit *)restore_unit_record)->zoid_id;
    palette_variant = ((struct BattleUnit *)restore_unit_record)->palette_variant;
    size_class = ((struct BattleUnit *)restore_unit_record)->size_class;
    ConfigureBattleUnitSprites(model_id, palette_variant, size_class, side_byte, restore_unit_slot,
                  ((restore_slot_carrier = restore_slot_times_four, side_byte) == 0) ? (void *)0x2000 : (void *)0xFFFFE000, 0);
    restore_sprite_flags = M2C_FIELD((restore_slot_carrier + restore_side_sprite_offset), s32 **, restoration_sprite_table);
    restored_sprite_flags = *restore_sprite_flags | BATTLE_SPRITE_SEMITRANSPARENT;
    goto block_45;
block_43:
    if (presentation_state != BATTLE_COMBINATION_CHANGED_MODEL) {
        goto block_46;
    }
    restore_sprite_flags = M2C_FIELD((((s32) restore_unit_slot * 4) + restore_side_sprite_offset), s32 **, restoration_sprite_table);
    restored_sprite_flags = *restore_sprite_flags & ~(u32)BATTLE_SPRITE_DOUBLE_CANVAS;
block_45:
    *restore_sprite_flags = restored_sprite_flags;
block_46:
    restore_unit_slot = (void *) (u8) (restore_unit_slot + 1);
    if ((u32) restore_unit_slot <= 5U) {
        goto loop_36;
    }
    *(s16 *)0x0300004E = 0x540;
    *(s16 *)gBlendAlpha = 0x1000;
    arrival_update = 0;
    arrival_sprite_table = 0x02032E8C;
    asm("" : "+r"(arrival_sprite_table));
    arrival_side_sprite_offset = side_byte * 0x18;
    blend_alpha = (s16 *)gBlendAlpha;
loop_48:
    arrival_unit_slot = NULL;
loop_49:
    if ((IsBattleUnitActive(side_byte, arrival_unit_slot) << 0x18) == 0) {
        goto block_56;
    }
    if (gBattleCombinationPresentation[(s32) arrival_unit_slot] != BATTLE_COMBINATION_UNCHANGED_UNIT) {
        goto block_56;
    }
    arrival_unit_sprite = M2C_FIELD((((s32) arrival_unit_slot * 4) + arrival_side_sprite_offset), void **, arrival_sprite_table);
    current_world_x = BATTLE_SPRITE_FIELD(arrival_unit_sprite, s32, user_data.position.x);
    if (side_byte != 0) {
        goto block_54;
    }
    arrival_world_x = current_world_x + 0xFFFFFE00;
    goto block_55;
block_54:
    arrival_world_x = current_world_x + 0x200;
block_55:
    BATTLE_SPRITE_FIELD(arrival_unit_sprite, s32, user_data.position.x) = arrival_world_x;
block_56:
    arrival_unit_slot = (void *) (u8) (arrival_unit_slot + 1);
    if ((u32) arrival_unit_slot <= 5U) {
        goto loop_49;
    }
    *blend_alpha = ((0x10 - arrival_update) << 8) | arrival_update;
    YieldTaskForUpdates(1);
    arrival_update += 1;
    if ((u32) arrival_update <= 0xFU) {
        goto loop_48;
    }
    *(u16 *)0x0300004E = 0;
    opaque_unit_slot = NULL;
    asm("" : "+r"(side_byte));
    opaque_sprite_table = 0x02032E8C;
    opaque_side_sprite_offset = side_byte * 0x18;
loop_59:
    if ((IsBattleUnitActive(side_byte, opaque_unit_slot) << 0x18) == 0) {
        goto block_62;
    }
    if (gBattleCombinationPresentation[(s32) opaque_unit_slot] != BATTLE_COMBINATION_UNCHANGED_UNIT) {
        goto block_62;
    }
    opaque_sprite_flags = M2C_FIELD((((s32) opaque_unit_slot * 4) + opaque_side_sprite_offset), s32 **, opaque_sprite_table);
    *opaque_sprite_flags &= ~(u32)BATTLE_SPRITE_SEMITRANSPARENT;
block_62:
    opaque_unit_slot = (void *) (u8) (opaque_unit_slot + 1);
    if ((u32) opaque_unit_slot <= 5U) {
        goto loop_59;
    }
    opponent_unit_slot = NULL;
    opponent_side = side_byte;
    opponent_side ^= 1;
    battle_state_address = 0x02034B4C;
    opponent_side_offset = opponent_side * 0x1380;
loop_64:
    if ((IsBattleUnitActive(opponent_side, opponent_unit_slot) << 0x18) == 0) {
        goto block_66;
    }
    opponent_unit_offset = ((((s32) opponent_unit_slot * 5 * 8) - (s32) opponent_unit_slot) * 0x10) + opponent_side_offset;
    opponent_unit_record = opponent_unit_offset + battle_state_address;
    asm("" : "+r"(opponent_unit_record));
    ConfigureBattleUnitSprites(((struct BattleUnit *)opponent_unit_record)->zoid_id, ((struct BattleUnit *)opponent_unit_record)->palette_variant, ((struct BattleUnit *)opponent_unit_record)->size_class, opponent_side, opponent_unit_slot, 0, 0);
block_66:
    opponent_unit_slot = (void *) (u8) (opponent_unit_slot + 1);
    if ((u32) opponent_unit_slot <= 5U) {
        goto loop_64;
    }
    return;
}
