#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
#include "../graphics/screen_effects.h"

s16 Sin256(s32) asm("func_08092A90");
s16 Cos256(s32) asm("func_08092ADC");
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
M2C_UNK DisableDisplayWindows() asm("func_0809534C");
s32 DivideSigned32(s32, s32) asm("func_080ECD98");
s32 DivideUnsigned32(s32, s32) asm("func_080ECF00");
extern s16 gBattleSectorWindowPolygonStream[] asm("D_02034910");

void UpdateBattleRedSweepingSectorWindowEffect(struct BattleAnimationGroup *group) asm("func_080DBAE8");

void UpdateBattleRedSweepingSectorWindowEffect(struct BattleAnimationGroup *group) {
    register void *effect_owner asm("r8") = group;
    void *effect = effect_owner;
    u32 *volatile sweep_count_slot;
    s32 retracting_line_start_x;
    s32 line_start_x;
    s32 line_y;
    s32 retracting_line_y;
    s32 sweep_coordinate_index;
    s32 opening_coordinate_index;
    s32 sweep_arc_angle;
    s32 closing_coordinate_index;
    s32 arc_angle;
    s32 mirrored_vertex_x;
    u32 *progress_increment_slot;
    u32 effect_phase;
    u32 increased_sweep_angle;
    u32 decreased_sweep_angle;
    u32 line_length;
    u32 opening_width_or_center_angle;
    u32 opening_width;
    u32 closing_width;
    u32 closing_width_reload;
    u32 retracting_line_length;
    u32 sweep_count;
    s32 next_opening_vertex;
    s32 next_sweep_vertex;
    s32 next_closing_vertex;
    u8 arc_vertex_index;

    effect_phase = BATTLE_ANIMATION_FIELD(effect, u32, state);
    if (effect_phase > BATTLE_SECTOR_WINDOW_RETRACT_LINE) {

    } else {
        register u32 *shape_progress_slot asm("r9");
        /* Insert the shared phase pointer setup into agbcc's table jump. */
        asm volatile(
            ".macro mov dst, src\n\t"
            ".hword 0x2190, 0x4441, 0x4689, 0x4687\n\t"
            ".endm");
        switch (effect_phase) {
        case BATTLE_SECTOR_WINDOW_EXTEND_LINE:
            asm volatile(".purgem mov");
            line_length = BATTLE_ANIMATION_FIELD(effect, u32, effect.sector_window.shape_progress);
            shape_progress_slot = effect + BATTLE_ANIMATION_OFFSET(effect.sector_window.shape_progress);
            if (line_length <= (BATTLE_SECTOR_WINDOW_RADIUS_PIXELS - 1U)) {
                gBattleSectorWindowPolygonStream[0] = 2;
                line_start_x = BATTLE_ANIMATION_FIELD(effect, s32, x);
                gBattleSectorWindowPolygonStream[1] = (s16) line_start_x;
                line_y = BATTLE_ANIMATION_FIELD(effect, s32, y);
                gBattleSectorWindowPolygonStream[2] = (s16) line_y;
                gBattleSectorWindowPolygonStream[3] = (s16) (line_start_x - line_length);
                gBattleSectorWindowPolygonStream[4] = (s16) line_y;
                gBattleSectorWindowPolygonStream[5] = 0;
                {
                    register u32 *progress_store asm("r5");
                    register u32 next_progress asm("r0") = line_length;
                    next_progress += 8;
                    progress_store = shape_progress_slot;
                    *progress_store = next_progress;
                }
            } else {
                {
                    register u32 *progress_store asm("r6");
                    register u32 zero_progress asm("r0") = 0;
                    progress_store = shape_progress_slot;
                    *progress_store = zero_progress;
                }
                BATTLE_ANIMATION_FIELD(effect, u32, state) = (u32) (BATTLE_ANIMATION_FIELD(effect, u32, state) + 1);
            case BATTLE_SECTOR_WINDOW_OPEN_SECTOR:
                {
                register u32 *progress_test asm("r1") = shape_progress_slot;
                if ((u32) *progress_test <= (BATTLE_SECTOR_WINDOW_ANGLE_WIDTH - 1U)) {
                    register s32 opening_cosine_carrier asm("r0");
                    register s32 opening_sine_carrier asm("r0");
                    register void *origin_group asm("r2");
                    gBattleSectorWindowPolygonStream[0] = BATTLE_SECTOR_WINDOW_VERTEX_COUNT;
                    origin_group = effect;
                    gBattleSectorWindowPolygonStream[1] = (s16) BATTLE_ANIMATION_FIELD(origin_group, s32, x);
                    gBattleSectorWindowPolygonStream[2] = (s16) BATTLE_ANIMATION_FIELD(origin_group, s32, y);
                    arc_vertex_index = 0;
                    {
                    register u32 *loop_progress asm("sl") = shape_progress_slot;
                    do {
                        {
                            register u32 *progress_read asm("r5") = loop_progress;
                            opening_width_or_center_angle = *progress_read;
                        }
                        arc_angle = DivideUnsigned32(arc_vertex_index * opening_width_or_center_angle, 6);
                        opening_width_or_center_angle >>= 1;
                        opening_width_or_center_angle -= 0x80;
                        Cos256((s16)(arc_angle - opening_width_or_center_angle));
                        asm volatile("" : "=r"(opening_cosine_carrier));
                        next_opening_vertex = arc_vertex_index + 1;
                        opening_coordinate_index = next_opening_vertex * 2;
                        {
                            register s32 vertex_coordinate_address asm("r2") = (opening_coordinate_index + 1) << 1;
                            register s32 origin_coordinate asm("r3");
                            register s32 radius_component asm("r1");
                            {
                                register s16 *polygon_stream_base asm("r1") = gBattleSectorWindowPolygonStream;
                                vertex_coordinate_address += (s32)polygon_stream_base;
                            }
                            origin_coordinate = BATTLE_ANIMATION_FIELD(effect, s32, x);
                            opening_cosine_carrier = (u32)opening_cosine_carrier << 16;
                            radius_component = opening_cosine_carrier >> 16;
                            opening_cosine_carrier = (u32)opening_cosine_carrier >> 31;
                            radius_component += opening_cosine_carrier;
                            radius_component >>= 1;
                            origin_coordinate += radius_component;
                            *(s16 *)vertex_coordinate_address = (s16)origin_coordinate;
                        }
                        {
                            register u32 *progress_read asm("r2") = loop_progress;
                            opening_width = *progress_read;
                        }
                        Sin256((s16) (DivideUnsigned32(arc_vertex_index * opening_width, 6) - ((opening_width >> 1) - 0x80)));
                        asm volatile("" : "=r"(opening_sine_carrier));
                        {
                            register s32 vertex_coordinate_address asm("r5") = (opening_coordinate_index + 2) << 1;
                            register s32 origin_coordinate asm("r2");
                            register s32 radius_component asm("r1");
                            {
                                register s16 *polygon_stream_base asm("r1") = gBattleSectorWindowPolygonStream;
                                vertex_coordinate_address += (s32)polygon_stream_base;
                            }
                            origin_coordinate = BATTLE_ANIMATION_FIELD(effect, s32, y);
                            opening_sine_carrier = (u32)opening_sine_carrier << 16;
                            radius_component = opening_sine_carrier >> 16;
                            opening_sine_carrier = (u32)opening_sine_carrier >> 31;
                            radius_component += opening_sine_carrier;
                            radius_component >>= 1;
                            origin_coordinate += radius_component;
                            *(s16 *)vertex_coordinate_address = (s16)origin_coordinate;
                        }
                        arc_vertex_index = next_opening_vertex;
                    } while ((u32) arc_vertex_index <= (BATTLE_SECTOR_WINDOW_ARC_VERTEX_COUNT - 1U));
                    }
                    gBattleSectorWindowPolygonStream[17] = 0;
                    progress_increment_slot = shape_progress_slot;
                    goto increment_progress_or_sweep_count;
                }
                {
                    register s32 *sweep_count_or_state_slot asm("r1") = effect + BATTLE_ANIMATION_OFFSET(effect.sector_window.sweep_count);
                    register u32 zero asm("r0") = 0;
                    register u32 *progress_store asm("r5");
                    *sweep_count_or_state_slot = zero;
                    progress_store = shape_progress_slot;
                    *progress_store = zero;
                    sweep_count_or_state_slot -= 2;
                    *sweep_count_or_state_slot += 1;
                }
                }
            case BATTLE_SECTOR_WINDOW_SWEEP_SECTOR:
                {
                    register u32 *sweep_count_init asm("r0") = effect + BATTLE_ANIMATION_OFFSET(effect.sector_window.sweep_count);
                    register u32 sweep_count_value asm("r1") = *sweep_count_init;
                    sweep_count_slot = sweep_count_init;
                    sweep_count = sweep_count_value;
                }
                if ((sweep_count <= 3U) ||
                    (*({
                        register u32 *progress_test asm("r6") = shape_progress_slot;
                        progress_test;
                    }) != 0)) {
                    register s32 sweep_cosine_carrier asm("r0");
                    register s32 sweep_sine_carrier asm("r0");
                    register s16 *polygon_stream_tail asm("r5");
                    gBattleSectorWindowPolygonStream[0] = BATTLE_SECTOR_WINDOW_VERTEX_COUNT;
                    gBattleSectorWindowPolygonStream[1] = (s16) BATTLE_ANIMATION_FIELD(effect, s32, x);
                    gBattleSectorWindowPolygonStream[2] = (s16) BATTLE_ANIMATION_FIELD(effect, s32, y);
                    arc_vertex_index = 0;
                    {
                    register u32 *loop_progress asm("sl") = shape_progress_slot;
                    do {
                        sweep_arc_angle = DivideSigned32(arc_vertex_index << 5, 6) + 0x70;
                        {
                            register u32 *progress_read asm("r6") = loop_progress;
                            Cos256((s16) (*progress_read + sweep_arc_angle));
                        }
                        asm volatile("" : "=r"(sweep_cosine_carrier));
                        next_sweep_vertex = arc_vertex_index + 1;
                        sweep_coordinate_index = next_sweep_vertex * 2;
                        {
                            register s32 vertex_coordinate_address asm("r2") = (sweep_coordinate_index + 1) << 1;
                            register s32 origin_coordinate asm("r3");
                            register s32 radius_component asm("r1");
                            {
                                register s16 *polygon_stream_base asm("r1") = gBattleSectorWindowPolygonStream;
                                vertex_coordinate_address += (s32)polygon_stream_base;
                            }
                            origin_coordinate = BATTLE_ANIMATION_FIELD(effect, s32, x);
                            sweep_cosine_carrier = (u32)sweep_cosine_carrier << 16;
                            radius_component = sweep_cosine_carrier >> 16;
                            sweep_cosine_carrier = (u32)sweep_cosine_carrier >> 31;
                            radius_component += sweep_cosine_carrier;
                            radius_component >>= 1;
                            origin_coordinate += radius_component;
                            *(s16 *)vertex_coordinate_address = (s16)origin_coordinate;
                        }
                        {
                            register u32 *progress_read asm("r2") = loop_progress;
                            Sin256((s16) (*progress_read + sweep_arc_angle));
                        }
                        asm volatile("" : "=r"(sweep_sine_carrier));
                        {
                            register s32 vertex_coordinate_address asm("r4") = (sweep_coordinate_index + 2) << 1;
                            register s32 origin_coordinate asm("r2");
                            register s32 radius_component asm("r1");
                            asm volatile(""
                                         : "=r"(polygon_stream_tail)
                                         : "0"(gBattleSectorWindowPolygonStream));
                            vertex_coordinate_address += (s32)polygon_stream_tail;
                            origin_coordinate = BATTLE_ANIMATION_FIELD(effect, s32, y);
                            sweep_sine_carrier = (u32)sweep_sine_carrier << 16;
                            radius_component = sweep_sine_carrier >> 16;
                            sweep_sine_carrier = (u32)sweep_sine_carrier >> 31;
                            radius_component += sweep_sine_carrier;
                            radius_component >>= 1;
                            origin_coordinate += radius_component;
                            *(s16 *)vertex_coordinate_address = (s16)origin_coordinate;
                        }
                        arc_vertex_index = next_sweep_vertex;
                    } while ((u32) arc_vertex_index <= (BATTLE_SECTOR_WINDOW_ARC_VERTEX_COUNT - 1U));
                    }
                    {
                        register s16 *polygon_terminator_view asm("r1");
                        asm volatile("mov %0, %1"
                                     : "=r"(polygon_terminator_view)
                                     : "r"(polygon_stream_tail));
                        polygon_terminator_view[17] = 0;
                    }
                    progress_increment_slot = sweep_count_slot;
                    if (!(*progress_increment_slot & 1)) {
                        register u32 *progress_slot asm("r5") = shape_progress_slot;
                        increased_sweep_angle = *progress_slot + 2;
                        *progress_slot = increased_sweep_angle;
                        if (increased_sweep_angle != BATTLE_SECTOR_WINDOW_ANGLE_WIDTH) {

                        } else {
increment_progress_or_sweep_count:
                            *progress_increment_slot += 1;
                        }
                    } else {
                        register u32 *progress_slot asm("r6") = shape_progress_slot;
                        decreased_sweep_angle = *progress_slot - 2;
                        *progress_slot = decreased_sweep_angle;
                        if (decreased_sweep_angle != -(u32)BATTLE_SECTOR_WINDOW_ANGLE_WIDTH) {

                        } else {
                            *sweep_count_slot += 1;
                        }
                    }
                } else {
                    register u32 *progress_slot asm("r2");
                    register u32 reset_progress asm("r0") = BATTLE_SECTOR_WINDOW_ANGLE_WIDTH;
                    progress_slot = shape_progress_slot;
                    *progress_slot = reset_progress;
                    BATTLE_ANIMATION_FIELD(effect, u32, state) = (u32) (BATTLE_ANIMATION_FIELD(effect, u32, state) + 1);
                case BATTLE_SECTOR_WINDOW_CLOSE_SECTOR:
                    {
                    register u32 *progress_test asm("r5") = shape_progress_slot;
                    if (*progress_test != 0) {
                        register s32 closing_cosine_carrier asm("r0");
                        register s32 closing_sine_carrier asm("r0");
                        register void *origin_group asm("r6");
                        gBattleSectorWindowPolygonStream[0] = BATTLE_SECTOR_WINDOW_VERTEX_COUNT;
                        origin_group = effect;
                        gBattleSectorWindowPolygonStream[1] = (s16) BATTLE_ANIMATION_FIELD(origin_group, s32, x);
                        gBattleSectorWindowPolygonStream[2] = (s16) BATTLE_ANIMATION_FIELD(origin_group, s32, y);
                        arc_vertex_index = 0;
                        {
                        register u32 *loop_progress asm("sl") = shape_progress_slot;
                        do {
                            closing_width = *loop_progress;
                            Cos256((s16) (DivideUnsigned32(arc_vertex_index * closing_width, 6) - ((closing_width >> 1) - 0x80)));
                            asm volatile("" : "=r"(closing_cosine_carrier));
                            next_closing_vertex = arc_vertex_index + 1;
                            closing_coordinate_index = next_closing_vertex * 2;
                            {
                                register s32 vertex_coordinate_address asm("r2") = (closing_coordinate_index + 1) << 1;
                                register s32 origin_coordinate asm("r3");
                                register s32 radius_component asm("r1");
                                {
                                    register s16 *polygon_stream_base asm("r1") = gBattleSectorWindowPolygonStream;
                                    vertex_coordinate_address += (s32)polygon_stream_base;
                                }
                                origin_coordinate = BATTLE_ANIMATION_FIELD(effect, s32, x);
                                closing_cosine_carrier = (u32)closing_cosine_carrier << 16;
                                radius_component = closing_cosine_carrier >> 16;
                                closing_cosine_carrier = (u32)closing_cosine_carrier >> 31;
                                radius_component += closing_cosine_carrier;
                                radius_component >>= 1;
                                origin_coordinate += radius_component;
                                *(s16 *)vertex_coordinate_address = (s16)origin_coordinate;
                            }
                            {
                                register u32 *progress_read asm("r2") = loop_progress;
                                closing_width_reload = *progress_read;
                            }
                            Sin256((s16) (DivideUnsigned32(arc_vertex_index * closing_width_reload, 6) - ((closing_width_reload >> 1) - 0x80)));
                            asm volatile("" : "=r"(closing_sine_carrier));
                            {
                                register s32 vertex_coordinate_address asm("r5") = (closing_coordinate_index + 2) << 1;
                                register s32 origin_coordinate asm("r2");
                                register s32 radius_component asm("r1");
                                {
                                    register s16 *polygon_stream_base asm("r1") = gBattleSectorWindowPolygonStream;
                                    vertex_coordinate_address += (s32)polygon_stream_base;
                                }
                                origin_coordinate = BATTLE_ANIMATION_FIELD(effect, s32, y);
                                closing_sine_carrier = (u32)closing_sine_carrier << 16;
                                radius_component = closing_sine_carrier >> 16;
                                closing_sine_carrier = (u32)closing_sine_carrier >> 31;
                                radius_component += closing_sine_carrier;
                                radius_component >>= 1;
                                origin_coordinate += radius_component;
                                *(s16 *)vertex_coordinate_address = (s16)origin_coordinate;
                            }
                            arc_vertex_index = next_closing_vertex;
                        } while ((u32) arc_vertex_index <= (BATTLE_SECTOR_WINDOW_ARC_VERTEX_COUNT - 1U));
                        }
                        gBattleSectorWindowPolygonStream[17] = 0;
                        {
                            register u32 *progress_slot asm("r2") = shape_progress_slot;
                            register u32 next_progress asm("r0") = *progress_slot;
                            next_progress -= 1;
                            *progress_slot = next_progress;
                        }
                    } else {
                        register u32 *progress_slot asm("r5");
                        register u32 reset_progress asm("r0") = BATTLE_SECTOR_WINDOW_RADIUS_PIXELS;
                        progress_slot = shape_progress_slot;
                        *progress_slot = reset_progress;
                        BATTLE_ANIMATION_FIELD(effect, u32, state) = (u32) (BATTLE_ANIMATION_FIELD(effect, u32, state) + 1);
                    case BATTLE_SECTOR_WINDOW_RETRACT_LINE:
                        {
                        register u32 *progress_slot asm("r6") = shape_progress_slot;
                        retracting_line_length = *progress_slot;
                        if (retracting_line_length != 0) {
                            gBattleSectorWindowPolygonStream[0] = 2;
                            retracting_line_start_x = BATTLE_ANIMATION_FIELD(effect, s32, x);
                            gBattleSectorWindowPolygonStream[1] = (s16) retracting_line_start_x;
                            retracting_line_y = BATTLE_ANIMATION_FIELD(effect, s32, y);
                            gBattleSectorWindowPolygonStream[2] = (s16) retracting_line_y;
                            gBattleSectorWindowPolygonStream[3] = (s16) (retracting_line_start_x - retracting_line_length);
                            gBattleSectorWindowPolygonStream[4] = (s16) retracting_line_y;
                            gBattleSectorWindowPolygonStream[5] = 0;
                            *progress_slot = retracting_line_length - 8;
                        } else {
                            DisableDisplayWindows();
                            *(s16 *)0x0300004E = (s16) retracting_line_length;
                            DestroySpriteGroup(effect);
                        }
                        }
                    }
                    }
                }
            }
            break;
        }
    }
    {
        arc_vertex_index = 0;
        {
            register s16 *polygon_stream asm("r2") = gBattleSectorWindowPolygonStream;
            register s32 zero asm("r5") = 0;
            register s32 vertex_count asm("r0") = *(s16 *)((u8 *)polygon_stream + zero);
        if ((s32) arc_vertex_index < vertex_count) {
            register s16 *polygon_stream_base asm("r5") = polygon_stream;
            register s32 mirroring_mask asm("r4") = BATTLE_ANIMATION_GROUP_MIRRORED;
            register s32 screen_width asm("r3") = SCREEN_WIDTH;
            register s32 vertex_x_offset asm("r0");
            register u16 *vertex_coordinate_address asm("r1");
            do {
                vertex_x_offset = ((arc_vertex_index * 2) + 1) * 2;
                asm volatile("add %0, %1, %2"
                             : "=r"(vertex_coordinate_address)
                             : "r"(vertex_x_offset), "r"(polygon_stream_base));
                if (!(BATTLE_ANIMATION_FIELD(effect, volatile s32, flags) & mirroring_mask)) {
                    mirrored_vertex_x = *vertex_coordinate_address;
                } else {
                    mirrored_vertex_x = screen_width - *vertex_coordinate_address;
                }
                *vertex_coordinate_address = mirrored_vertex_x;
                arc_vertex_index += 1;
                {
                    register s32 reload_zero asm("r1") = 0;
                    vertex_count = *(s16 *)((u8 *)polygon_stream + reload_zero);
                }
            } while ((s32) arc_vertex_index < vertex_count);
        }
        }
    }
}
