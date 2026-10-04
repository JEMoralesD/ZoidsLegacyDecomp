#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
#include "../graphics/screen_effects.h"
#define gBattlePaletteCyclingTrianglePolygonStream ((s16 *)0x02034B20)
#define gBattlePaletteCyclingTriangleVector (*(struct Vector3s *)0x087A2CEC)

extern int DestroySprite() asm("func_08094554");
extern int QueueCopy() asm("func_08095208");

struct BattlePaletteCyclingTriangleChargeGroupView {
    u32 group_flags;
    s32 x;
    s32 y;
    void *sprite_slots[BATTLE_ANIMATION_GROUP_SPRITE_COUNT];
    u32 phase;
    union BattleTriangleChargeParameters phase_parameters;
    u32 charge_particle_flags;
};

struct BattlePaletteCyclingTriangleChargeStack {
    s16 matrix[16];
    u32 angles[2];
    struct Vector3s transformed;
    s32 cosine_carrier;
    s16 *origin_y_vertex;
    s32 sine_carrier;
    s32 signed_cosine;
    u32 *narrowing_progress_slot;
    s32 *progress_slot;
    s16 *polygon_stream;
};

void DestroySpriteGroup(void *) asm("func_08095114");
void DisableDisplayWindows(void) asm("func_0809534C");
void ConfigurePolygonScanlineWindows(void *, s32, s32, s32) asm("func_080955A0");
void TransformVector3sFixed8(s16 *, void *, struct Vector3s *) asm("func_08093678");
void BuildRotationMatrixXYZ(u32 *, s16 *) asm("func_0809378C");
s32 Sin256(u32) asm("func_08092A90");
u16 Cos256(u32) asm("func_08092ADC");
void *CreateBattleAnimationSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2450");
void CreateBattleAngledProjectileSprite(void *, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2660");
void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
void PlayBattleAnimationSound(s32) asm("func_080D2790");
void SpawnBattleInwardChargeParticle(void *, s32) asm("func_080DCDC4");
s32 BiosArcTan2(s32, s32) asm("func_080ECD24");
u32 CallFunctionR0(u32) asm("func_080ECD5C");
s32 DivideSigned32(s32, s32) asm("func_080ECD98");
s32 ModuloUnsigned32(s32, s32) asm("func_080ECF78");

extern u32 gRandomNumberCallback asm("D_03000010");
extern s32 gBattleZoidScrollX asm("D_02034034");
extern s32 gBattleAnimationPaletteBanks asm("D_020348B4");
extern struct SpriteBackgroundScrollOffsets gSpriteBackgroundScroll[] asm("D_03000054");
extern volatile u16 gBattleBlendControl asm("D_0300004E");
extern volatile u16 gBattleBlendAlpha asm("D_03000050");
extern u8 gBattlePaletteCyclingBeamPaletteSequence[] asm("D_087A2CCC");
extern u8 gBattlePaletteCyclingBeamPaletteBlocks[] asm("D_087A2B0C");
extern u8 gSpritePaletteMemory[] asm("D_05000200");

void UpdateBattleRotatingTriangleChargePaletteCyclingBeamEffect(struct BattlePaletteCyclingTriangleChargeGroupView *effect_view) asm("func_080DEB84");

void UpdateBattleRotatingTriangleChargePaletteCyclingBeamEffect(struct BattlePaletteCyclingTriangleChargeGroupView *effect_view)
{
    struct BattlePaletteCyclingTriangleChargeStack stack;
    register u32 next_index_bits asm("r0");

    switch (effect_view->phase) {
    case BATTLE_PALETTE_CYCLING_TRIANGLE_CHARGE_CONFIGURE_WINDOWS:
        ConfigurePolygonScanlineWindows(gBattlePaletteCyclingTrianglePolygonStream, BATTLE_SECTOR_WINDOW_INSIDE_LAYERS, BATTLE_SECTOR_WINDOW_OUTSIDE_LAYERS, SCANLINE_WINDOW_HBLANK_CALLBACK);
        *(volatile u16 *)0x0300004E = 0x3757;
        *(volatile u16 *)0x05000000 = 0x7FFF;
        *(u8 *)0x03000075 = 2;
        *(u8 *)0x0300603D = 2;
        PlayBattleAnimationSound(0);
        effect_view->phase++;
        /* fall through */
    case BATTLE_PALETTE_CYCLING_TRIANGLE_CHARGE_ROTATE_TRIANGLES: {
        register s32 *progress_slot asm("r2");
        register u32 lower asm("r0");
        register u32 triangle_index asm("r8");

        if (!(effect_view->charge_particle_flags & BATTLE_PALETTE_CYCLING_TRIANGLE_CHARGE_DISABLE_PARTICLES)) {
            SpawnBattleInwardChargeParticle(effect_view, 2);
        }
        progress_slot = &effect_view->phase_parameters.window.rotation_angle;
        lower = *progress_slot + 4;

        lower <<= 16;
        lower >>= 16;
        {
        register u32 mask asm("r5") = 0xFFFF0000;
        register u32 packed asm("r4") = stack.angles[0];

        packed &= mask;
        packed |= lower;
        stack.angles[0] = packed;
        {
            register s32 upper asm("r3") = effect_view->phase_parameters.window.yaw_angle;
            register u32 shifted asm("r1");
            register u32 flags asm("r0") = effect_view->group_flags;
            register u32 bit asm("r1") = BATTLE_ANIMATION_GROUP_MIRRORED;

            flags &= bit;
            stack.progress_slot = progress_slot;
            if (flags != 0) {
                upper += 0x80;
            }
            shifted = upper << 16;
            {
                register u32 low asm("r0") = 0xFFFF;

                low &= packed;
                low |= shifted;
                stack.angles[0] = low;
            }
        }
        {
            register u32 *narrowing_progress_slot asm("r3") = &effect_view->phase_parameters.window.narrowing_progress;
            register u32 value asm("r2") = *(u16 *)narrowing_progress_slot;
            register u32 *angles asm("r1") = stack.angles;
            register u32 second asm("r0") = angles[1];
            register u32 zero asm("r4");

            second &= mask;
            second |= value;
            angles[1] = second;
            zero = 0;
            asm volatile("" : "+r"(zero));
            triangle_index = zero;
            stack.narrowing_progress_slot = narrowing_progress_slot;
        }
        }

        {
        register struct Vector3s *transformed asm("r9") =
            &stack.transformed;
        register s16 *polygon_stream asm("r3");

        for (;;) {
            register s32 polygon_halfword_offset_or_address asm("r5");
            register s16 *projected_vertex;
            register u32 transformed_or_rounding_work asm("r4");
            u16 direction;

            {
                register u32 *angles asm("r0");

                asm volatile("add %0, sp, #48" : "=r"(angles));
                BuildRotationMatrixXYZ(angles, stack.matrix);
            }
            TransformVector3sFixed8(stack.matrix, (void *)&gBattlePaletteCyclingTriangleVector,
                transformed);
            {
                register struct Vector3s *z_base asm("r2") =
                    transformed;
                register u32 z asm("r1") = (u16)z_base->z;
                register s32 new_z asm("r0") = 0x80 - z;

                z_base->z = new_z;
            }
            {
                register s32 projected_x asm("r0");
                register u32 x_offset asm("r3");
                register struct Vector3s *y_base asm("r2") =
                    transformed;
                register s32 projected_y asm("r1");
                register u32 y_offset asm("r4");

                asm volatile(
                    "add %0, sp, #56\n\t"
                    "mov %1, #0\n\t"
                    "ldrsh %0, [%0, %1]"
                    : "=r"(projected_x), "=r"(x_offset));
                asm volatile(
                    "mov %1, #2\n\t"
                    "ldrsh %0, [%2, %1]"
                    : "=r"(projected_y), "=r"(y_offset)
                    : "r"(y_base));
                direction = BiosArcTan2(projected_x, projected_y);
            }
            direction >>= 8;
            {
                register u16 sine_carrier asm("sl");
                register u32 sine_result asm("r0") =
                    Sin256(direction);
                register s32 projection asm("r0");

                sine_result <<= 16;
                sine_result >>= 16;
                sine_carrier = sine_result;
                stack.cosine_carrier = (u16)Cos256(direction);
                {
                    register u32 base asm("r3") = (u32)gBattlePaletteCyclingTrianglePolygonStream;
                    register u32 polygon_triangle_index asm("r1") = triangle_index;
                    register u32 polygon_offset_carrier asm("r0") = polygon_triangle_index << 3;
                    register u32 origin_x_address asm("r2");

                    polygon_offset_carrier -= polygon_triangle_index;
                    polygon_triangle_index = polygon_offset_carrier << 1;
                    polygon_triangle_index += base;
                    *(s16 *)polygon_triangle_index = 3;
                    polygon_offset_carrier += 1;
                    polygon_offset_carrier <<= 1;
                    origin_x_address = polygon_offset_carrier + base;
                    if (!(effect_view->group_flags & BATTLE_ANIMATION_GROUP_MIRRORED)) {
                        *(u16 *)origin_x_address = *(u16 *)&effect_view->x;
                    } else {
                        *(u16 *)origin_x_address = SCREEN_WIDTH - effect_view->x;
                    }
                }
                {
                    register u32 base asm("r1") = (u32)gBattlePaletteCyclingTrianglePolygonStream;
                    register u32 polygon_triangle_index asm("r2") = triangle_index;
                    register u32 scaled asm("r0") = polygon_triangle_index << 3;
                    register u32 address asm("r0");

                    polygon_halfword_offset_or_address = scaled - polygon_triangle_index;
                    address = polygon_halfword_offset_or_address + 2;
                    address <<= 1;
                    address += base;
                    stack.origin_y_vertex = (s16 *)address;
                    {
                        register s32 owner_y asm("r0") = effect_view->y;
                        register s16 *origin_y_store asm("r3") =
                            stack.origin_y_vertex;

                        *origin_y_store = owner_y;
                    }
                    address = polygon_halfword_offset_or_address + 3;
                    address <<= 1;
                    address += base;
                    projected_vertex = (s16 *)address;

                    {
                        register struct Vector3s *x_base asm("r4") =
                            transformed;
                        register u32 x_offset asm("r0");
                        register s32 projected_x asm("r2");
                        register s32 projection_numerator asm("r0");

                        asm volatile(
                            "mov %1, #0\n\t"
                            "ldrsh %0, [%2, %1]"
                            : "=r"(projected_x), "=r"(x_offset)
                            : "r"(x_base));
                        {
                            register s32 sine_source asm("r3") = sine_carrier;
                            register s32 signed_sine asm("r0") =
                                (s16)sine_source;

                            stack.sine_carrier = signed_sine;
                        }
                        {
                            register u32 *narrowing_progress_view asm("r4") = stack.narrowing_progress_slot;
                            register u32 narrowing_progress asm("r0") =
                                *(u16 *)narrowing_progress_view;
                            register s32 maximum_half_width asm("r3") = 0x40;

                            projection_numerator = maximum_half_width - narrowing_progress;
                        }
                        {
                            register s32 multiplier asm("r4") = stack.sine_carrier;

                            projection_numerator *= multiplier;
                        }
                        {
                            register u32 base_saved asm("r3") = base;

                            if (projection_numerator < 0) {
                                projection_numerator += 0x1FF;
                            }
                            projection_numerator >>= 9;
                            projection_numerator = projected_x + projection_numerator;
                            projection_numerator <<= 7;
                            {
                                register u32 z_offset asm("r2");
                                register s32 z asm("r1");

                                transformed_or_rounding_work = (u32)transformed;
                                asm volatile(
                                    "mov %1, #4\n\t"
                                    "ldrsh %0, [%2, %1]"
                                    : "=r"(z), "=r"(z_offset)
                                    : "r"(transformed_or_rounding_work));
                                stack.polygon_stream = (s16 *)base_saved;
                                projection = DivideSigned32(projection_numerator, z);
                            }
                        }
                    }
                }
                {
                    register s16 *origin_x_vertex asm("sl");
                    register u32 address asm("r1") = polygon_halfword_offset_or_address + 1;
                    register u32 base asm("r3");

                    address <<= 1;
                    base = (u32)stack.polygon_stream;
                    address += base;
                    origin_x_vertex = (s16 *)address;
                    {
                        register u32 value asm("r1") =
                            *(u16 *)origin_x_vertex;

                        value += projection;
                        asm volatile("strh %0, [%1]"
                            :
                            : "l"(value), "l"(projected_vertex)
                            : "memory");
                    }

            {
                register u32 address asm("r0") = polygon_halfword_offset_or_address + 4;

                address <<= 1;
                projected_vertex = (s16 *)(address + base);
                {
                    register u32 y_offset asm("r0");
                    register s32 projected_y asm("r1");
                    register s32 projection_numerator asm("r0");

                    asm volatile(
                        "mov %1, #2\n\t"
                        "ldrsh %0, [%2, %1]"
                        : "=r"(projected_y), "=r"(y_offset)
                        : "r"(transformed_or_rounding_work));
                    {
                        register s32 cosine_source asm("r2") = stack.cosine_carrier;
                        register s32 signed_cosine asm("r0") =
                            (s16)cosine_source;

                        stack.signed_cosine = signed_cosine;
                    }
                    {
                        register u32 narrowing_pointer_or_width asm("r2") =
                            (u32)stack.narrowing_progress_slot;
                        register u32 narrowing_progress asm("r0") =
                            *(u16 *)narrowing_pointer_or_width;

                        narrowing_pointer_or_width = 0x40;
                        projection_numerator = narrowing_pointer_or_width - narrowing_progress;
                    }
                    {
                        register s32 multiplier asm("r2") =
                            stack.signed_cosine;

                        projection_numerator *= multiplier;
                    }
                    if (projection_numerator < 0) {
                        projection_numerator += 0x1FF;
                    }
                    projection_numerator >>= 9;
                    projection_numerator = projected_y - projection_numerator;
                    projection_numerator <<= 7;
                    {
                        register u32 z_offset asm("r2");
                        register s32 z asm("r1");

                        asm volatile(
                            "mov %1, #4\n\t"
                            "ldrsh %0, [%2, %1]"
                            : "=r"(z), "=r"(z_offset)
                            : "r"(transformed_or_rounding_work));
                        stack.polygon_stream = (s16 *)base;
                        projection = DivideSigned32(projection_numerator, z);
                    }
                }
                {
                    register u16 *origin_y_load asm("r2") =
                        (u16 *)stack.origin_y_vertex;
                    register u32 value asm("r1") = *origin_y_load;

                    value += projection;
                    asm volatile("strh %0, [%1]"
                        :
                        : "l"(value), "l"(projected_vertex)
                        : "memory");
                }
            }

            {
                register u32 address asm("r0") =
                    (polygon_halfword_offset_or_address + 5) << 1;
                register u32 base asm("r3") = (u32)stack.polygon_stream;
                register s16 *second_tip_x_vertex asm("r7");

                second_tip_x_vertex = (s16 *)(address + base);
                {
                    register u32 x_offset asm("r0");
                    register s32 projected_x asm("r1");
                    register s32 projection_numerator asm("r0");

                    asm volatile(
                        "mov %1, #0\n\t"
                        "ldrsh %0, [%2, %1]"
                        : "=r"(projected_x), "=r"(x_offset)
                        : "r"(transformed_or_rounding_work));
                    {
                        register u32 narrowing_pointer_or_width asm("r2") =
                            (u32)stack.narrowing_progress_slot;
                        register u32 narrowing_progress asm("r0") =
                            *(u16 *)narrowing_pointer_or_width;

                        narrowing_pointer_or_width = 0x40;
                        projection_numerator = narrowing_pointer_or_width - narrowing_progress;
                    }
                    {
                        register s32 multiplier asm("r2") = stack.sine_carrier;

                        projection_numerator *= multiplier;
                    }
                    if (projection_numerator < 0) {
                        projection_numerator += 0x1FF;
                    }
                    projection_numerator >>= 9;
                    projection_numerator = projected_x - projection_numerator;
                    projection_numerator <<= 7;
                    {
                        register u32 z_offset asm("r2");
                        register s32 z asm("r1");

                        asm volatile(
                            "mov %1, #4\n\t"
                            "ldrsh %0, [%2, %1]"
                            : "=r"(z), "=r"(z_offset)
                            : "r"(transformed_or_rounding_work));
                        stack.polygon_stream = (s16 *)base;
                        projection = DivideSigned32(projection_numerator, z);
                    }
                }
                {
                    register u16 *origin_x_load asm("r2") =
                        (u16 *)origin_x_vertex;
                    register u32 value asm("r1") = *origin_x_load;

                    value += projection;
                    asm volatile("strh %0, [%1]"
                        :
                        : "l"(value), "l"(second_tip_x_vertex)
                        : "memory");
                }
            }
                }
            }

            {
                register u32 address asm("r0") =
                    (polygon_halfword_offset_or_address + 6) << 1;
                register u32 base asm("r3") = (u32)stack.polygon_stream;

                polygon_halfword_offset_or_address = address + base;
                {
                    register u32 y_offset asm("r0");
                    register s32 projected_y asm("r1");
                    register s32 projection_numerator asm("r0");
                    register s32 projection asm("r0");

                    asm volatile(
                        "mov %1, #2\n\t"
                        "ldrsh %0, [%2, %1]"
                        : "=r"(projected_y), "=r"(y_offset)
                        : "r"(transformed_or_rounding_work));
                    {
                        register u32 *narrowing_progress_view asm("r2") = stack.narrowing_progress_slot;
                        register u32 narrowing_progress asm("r0") =
                            *(u16 *)narrowing_progress_view;

                        transformed_or_rounding_work = 0x40;
                        projection_numerator = transformed_or_rounding_work - narrowing_progress;
                    }
                    {
                        register s32 multiplier asm("r2") =
                            stack.signed_cosine;

                        projection_numerator *= multiplier;
                    }
                    if (projection_numerator < 0) {
                        transformed_or_rounding_work = 0x1FF;
                        asm volatile("" : "+r"(transformed_or_rounding_work));
                        projection_numerator += transformed_or_rounding_work;
                    }
                    projection_numerator >>= 9;
                    projection_numerator = projected_y + projection_numerator;
                    projection_numerator <<= 7;
                    {
                        register struct Vector3s *z_base asm("r2") =
                            transformed;
                        register u32 z_offset asm("r4");
                        register s32 z asm("r1");

                        asm volatile(
                            "mov %1, #4\n\t"
                            "ldrsh %0, [%2, %1]"
                            : "=r"(z), "=r"(z_offset)
                            : "r"(z_base));
                        stack.polygon_stream = (s16 *)base;
                        projection = DivideSigned32(projection_numerator, z);
                    }
                    {
                        register u16 *origin_y_load asm("r2") =
                            (u16 *)stack.origin_y_vertex;
                        register u32 value asm("r1") = *origin_y_load;

                        value += projection;
                        *(s16 *)polygon_halfword_offset_or_address = value;
                    }
                }
            }

            {
                register u8 *angle_load_base asm("r4");
                register u32 triangle_rotation_angle asm("r0");
                register u16 *angle_store asm("r1");

                asm volatile("mov %0, sp" : "=r"(angle_load_base));
                triangle_rotation_angle = *(u16 *)(angle_load_base + 48);
                triangle_rotation_angle += 0x55;
                asm volatile("add %0, sp, #48" : "=r"(angle_store));
                *angle_store = triangle_rotation_angle;
            }
            next_index_bits = triangle_index + 1;
            next_index_bits <<= 24;
            next_index_bits >>= 24;
            triangle_index = next_index_bits;
            polygon_stream = stack.polygon_stream;
            if (next_index_bits > 2) {
                break;
            }
        }

        {
            register u32 completed_triangle_count asm("r1");

            next_index_bits <<= 3;
            completed_triangle_count = triangle_index;
            next_index_bits -= completed_triangle_count;
            next_index_bits <<= 1;
            next_index_bits += (u32)polygon_stream;
            *(s16 *)next_index_bits = 0;
        }
        }
        {
            register volatile u16 *blend asm("r3") =
                (volatile u16 *)0x03000050;
            register u32 *narrowing_progress_view asm("r4") = stack.narrowing_progress_slot;
            register u32 narrowing_progress asm("r2") = *narrowing_progress_view;

            *blend = ((narrowing_progress >> 2) << 8) | 0x10;
            if (narrowing_progress == 0x40) {
                goto advance;
            }
        }
        if (!(effect_view->group_flags & BATTLE_ANIMATION_GROUP_MIRRORED)) {
            register s32 *positive_progress_slot asm("r1") = stack.progress_slot;

            *positive_progress_slot += 8;
        } else {
            register s32 *negative_progress_slot asm("r2") = stack.progress_slot;

            *negative_progress_slot -= 8;
        }
        {
            register u32 *narrowing_progress_store asm("r3") = stack.narrowing_progress_slot;

            *narrowing_progress_store += 1;
        }
        break;
    }
    case BATTLE_PALETTE_CYCLING_TRIANGLE_CHARGE_CLEAR_PARTICLES_AND_START_LINE: {
        register u32 charge_particle_slot asm("r8");
        register u32 one asm("r4") = 1;
        register u32 *effect_state_slot asm("r5");

        asm volatile("" : "+r"(one));
        charge_particle_slot = one;
        effect_state_slot = &effect_view->phase;
        {
            register u32 *sprite_slots asm("r4") = effect_view->sprite_slots;

            do {
                register u32 sprite_slot_offset asm("r0");
                register u32 sprite_slot_index asm("r1");

                asm volatile(
                    "mov %1, r8\n\t"
                    "lsl %0, %1, #2"
                    : "=r"(sprite_slot_offset), "=r"(sprite_slot_index)
                    :
                    : "cc");
                if (*(u32 *)((u8 *)sprite_slots + sprite_slot_offset) != 0) {
                    /* Native R0 carries the particle pointer through this partial call signature. */
                    DestroySprite();
                }
                charge_particle_slot = (u8)(charge_particle_slot + 1);
            } while (charge_particle_slot < BATTLE_ANIMATION_GROUP_SPRITE_COUNT);
        }
        effect_view->phase_parameters.line.length_pixels = 0x80;
        *(u8 *)0x03000075 = 1;
        *(u8 *)0x0300603D = 1;
        (*effect_state_slot)++;
        /* fall through */
    }
    case BATTLE_PALETTE_CYCLING_TRIANGLE_CHARGE_RETRACT_LINE: {
        register s32 *progress_slot asm("r0") = &effect_view->phase_parameters.line.length_pixels;
        register s32 line_length asm("r4") = *progress_slot;

        stack.progress_slot = progress_slot;
        if (line_length == 0) {
            DisableDisplayWindows();
            {
                register volatile u16 *display_control asm("r0") =
                    (volatile u16 *)0x0300004E;

                *display_control = line_length;
            }
            goto advance;
        }
        {
            register s16 *line_polygon asm("r2");
            register s16 *line_polygon_view asm("r3");
            register s16 *line_polygon_vertices asm("r5");
            register u32 saved_flags asm("r4");
            s32 x;

            {
                register s16 *line_polygon_init asm("r1") =
                    gBattlePaletteCyclingTrianglePolygonStream;

                line_polygon_init[0] = 4;
                line_polygon = line_polygon_init;
                line_polygon_vertices = line_polygon;
            }
            {
                register u32 flags asm("r1") = effect_view->group_flags;
                register u32 masked asm("r0") = BATTLE_ANIMATION_GROUP_MIRRORED;

                masked &= flags;
                line_polygon_view = line_polygon;
                saved_flags = flags;
                if (masked == 0) {
                    x = (u16)effect_view->x;
                } else {
                    x = SCREEN_WIDTH - effect_view->x;
                }
            }
            line_polygon_vertices[7] = x;
            line_polygon[1] = x;
                {
                    register s16 *line_polygon_store asm("r7") = line_polygon_view;
                    register s32 line_origin_x asm("r1");
                register u32 zero asm("r2");
                register s32 line_end_x asm("r0");

                line_polygon_vertices = line_polygon_view;
                {
                    register u32 x_offset asm("r2");

                    asm volatile(
                        "mov %1, #2\n\t"
                        "ldrsh %0, [%2, %1]"
                        : "=r"(line_origin_x), "=r"(x_offset)
                        : "r"(line_polygon_view));
                }
                {
                    register u32 masked asm("r0") = BATTLE_ANIMATION_GROUP_MIRRORED;

                    masked &= saved_flags;
                    if (masked == 0) {
                        register s32 *negative_progress_slot asm("r4") = stack.progress_slot;
                        register s32 value asm("r0") = *negative_progress_slot;

                        line_end_x = line_origin_x - value;
                    } else {
                        register s32 *positive_progress_slot asm("r2") = stack.progress_slot;
                        register s32 value asm("r0") = *positive_progress_slot;

                        line_end_x = line_origin_x + value;
                    }
                    zero = 0;
                    line_polygon_vertices[5] = line_end_x;
                    asm volatile("strh %0, [%1, #6]"
                        :
                        : "l"(line_end_x), "l"(line_polygon_store)
                        : "memory");
                }
                {
                    register s32 y asm("r0") = effect_view->y;
                    register s32 y_minus_one asm("r1") = y - 1;

                    line_polygon_view[4] = y_minus_one;
                    line_polygon_view[2] = y_minus_one;
                    y += 1;
                    line_polygon_view[8] = y;
                    line_polygon_view[6] = y;
                    line_polygon_view[9] = zero;
                }
            }
            {
                register s32 *line_length_slot asm("r3") = stack.progress_slot;

                *line_length_slot -= 0x10;
            }
        }
        break;
    }
    case BATTLE_PALETTE_CYCLING_TRIANGLE_CHARGE_WAIT_FOR_BEAM_DELAY:
        effect_view->phase_parameters.beam.elapsed_updates++;
        if (effect_view->phase_parameters.beam.elapsed_updates != 0x1E) {
            break;
        }
        {
            register volatile u16 *display_control asm("r1") =
                &gBattleBlendControl;
            register u32 display_value asm("r4") = 0x740;
            register u32 store_value asm("r0");

            asm volatile("" : "+r"(display_value));
            store_value = display_value;
            *display_control = store_value;
        }
        {
            register volatile u16 *blend asm("r1") = &gBattleBlendAlpha;
            register u32 blend_value asm("r3") = 0x1010;
            register u32 store_value asm("r0");

            asm volatile("" : "+r"(blend_value));
            store_value = blend_value;
            *blend = store_value;
        }
        effect_view->phase_parameters.beam.elapsed_updates = 0;
        goto advance;
    case BATTLE_PALETTE_CYCLING_TRIANGLE_CHARGE_CREATE_RING_AND_BEAM: {
        s32 base_x;
        s32 base_y;
        s32 camera_x;
        s32 camera_y;
        s32 *camera_x_ptr;
        struct SpriteBackgroundScrollOffsets *background_scroll;
        s32 x;
        s32 y;
        register s32 first_x asm("r3");
        register u32 first_x_offset asm("r4");
        register s32 first_y asm("r0");
        register u32 first_y_offset asm("r1");

        asm volatile(
            "mov %1, #4\n\t"
            "ldrsh %0, [%2, %1]"
            : "=r"(first_x), "=r"(first_x_offset)
            : "r"(effect_view));
        asm volatile(
            "mov %1, #8\n\t"
            "ldrsh %0, [%2, %1]"
            : "=r"(first_y), "=r"(first_y_offset)
            : "r"(effect_view));
        effect_view->sprite_slots[0] = CreateBattleAnimationSprite(effect_view, 2, 0,
            first_x, first_y, BATTLE_SPRITE_SEMITRANSPARENT, 0, 0);
        base_x = effect_view->x;
        camera_x_ptr = &gBattleZoidScrollX;
        camera_x = *camera_x_ptr;
        if (camera_x < 0) {
            camera_x += 0xFF;
        }
        camera_x >>= 8;
        camera_x -= 0x80;
        asm volatile("add %0, %1, %0"
            : "+r"(camera_x)
            : "r"(base_x));
        x = (s16)camera_x;
        base_y = effect_view->y;
        background_scroll = gSpriteBackgroundScroll;
        camera_y = background_scroll[0].y_fixed8;
        if (camera_y < 0) {
            camera_y += 0xFF;
        }
        camera_y >>= 8;
        asm volatile("add %0, %1, %0"
            : "+r"(camera_y)
            : "r"(base_y));
        y = (s16)camera_y;
        effect_view->sprite_slots[1] =
            CreateBattleAnimationSprite(effect_view, 0, 0, x, y, (BATTLE_SPRITE_BACKGROUND_RELATIVE | BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), 0, 0);
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_RECOIL_SCROLL, 0);
        PlayBattleAnimationSound(1);
        goto advance;
    }
advance:
        effect_view->phase++;
        break;
    case BATTLE_PALETTE_CYCLING_TRIANGLE_CHARGE_CYCLE_BEAM_PALETTE: {
        register void *beam_sprite asm("r2") = effect_view->sprite_slots[1];

        if (beam_sprite == 0) {
            DestroySpriteGroup(effect_view);
        } else if (*(u16 *)((u8 *)beam_sprite + BATTLE_SPRITE_OFFSET(frame_timer)) == 0) {
            register u32 palette_block_offset_or_address asm("r0");

            {
                register u8 *palette_sequence asm("r1");

                asm volatile("ldr %0, [pc, #44]" : "=r"(palette_sequence));
                palette_block_offset_or_address = *(u16 *)((u8 *)beam_sprite + BATTLE_SPRITE_OFFSET(animation_step));
                palette_block_offset_or_address += (u32)palette_sequence;
                palette_block_offset_or_address = *(u8 *)palette_block_offset_or_address;
            }
            palette_block_offset_or_address <<= 5;
            {
                register u8 *palette_blocks asm("r1");

                asm volatile("ldr %0, [pc, #40]" : "=r"(palette_blocks));
                palette_block_offset_or_address += (u32)palette_blocks;
            }
            {
                register s32 sprite_palette_address asm("r1");
                register s32 *palette_bank_word_slot asm("r1");

                asm volatile("ldr %0, [pc, #40]" : "=r"(palette_bank_word_slot));
                sprite_palette_address = *palette_bank_word_slot;
                sprite_palette_address <<= 5;
                {
                    register u8 *sprite_palette_memory asm("r2");

                    asm volatile("ldr %0, [pc, #36]" : "=r"(sprite_palette_memory));
                    sprite_palette_address += (u32)sprite_palette_memory;
                }
                QueueCopy(palette_block_offset_or_address, sprite_palette_address, 0x20);
            }
        }
        break;
    }
    }
}
