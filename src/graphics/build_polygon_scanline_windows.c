#include "m2c_prelude.h"
#include "screen_effects.h"

M2C_UNK BiosCpuFastSet(M2C_UNK *, M2C_UNK, M2C_UNK) asm("func_080ECD28"); /* extern */
extern u8 gScanlineWindowBuffers[] asm("D_02000000");

void BuildPolygonScanlineWindows(void) asm("func_08095864");

void BuildPolygonScanlineWindows(void) {
    s32 scratch_fill;
    s32 y_decreases;
    u32 next_scanline;
    s32 edge_step;
    register s16 *polygon_stream asm("r9");
    register s32 vertex_count asm("r0");
    s32 var_r0_2;
    s32 var_r0_5;
    s32 var_r0_7;
    register s32 edge_y asm("r1");
    register s32 edge_x asm("r3");
    s32 temp_r0_3;
    s32 var_r0_3;
    s32 var_r0_4;
    s32 var_r0_6;
    register s32 edge_or_bounds_work asm("r4");
    register s32 edge_dx asm("r5");
    register s32 edge_error asm("r6");
    register s32 direction_or_row_cursor asm("r8");
    register u16 *output_bounds asm("r1");
    register u32 packed_bounds asm("r0");
    register u32 vertex_index asm("ip");
    register u32 scanline_or_bounds asm("ip");
    register u32 window_id asm("r7");
    register u32 output_right asm("r0");
    register u32 raster_right asm("r3");
    register u32 output_left asm("r5");
    register u32 raster_left asm("r6");
    register u8 *scratch_row asm("r2");
    void *steep_scratch_row;
    register u16 *raster_bounds asm("r2");
    register s16 *vertex_or_empty_interval asm("sl");

    asm volatile("" : "=m"(scratch_fill), "=m"(y_decreases), "=m"(next_scanline), "=m"(edge_step));
    {
        register u32 clear_index asm("ip") = 0;
        register s16 **polygon_source asm("r7") = (s16 **)SCANLINE_WINDOW_ADDRESS(polygon_stream);
        register u8 *scanline_buffers asm("r6") = gScanlineWindowBuffers;
        register u8 *displayed_bank asm("r5") = (u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
        register u32 one asm("r4") = 1;
        register s32 empty asm("r3") = -1;
        asm volatile(
            "1:\n\t"
            "mov r1, ip\n\t"
            "lsl r2, r1, #3\n\t"
            "ldrb r0, [r5]\n\t"
            "mov r1, r4\n\t"
            "eor r1, r0\n\t"
            "lsl r0, r1, #2\n\t"
            "add r0, r1\n\t"
            "lsl r0, r0, #8\n\t"
            "add r2, r0\n\t"
            "add r2, r6\n\t"
            "str r3, [r2]\n\t"
            "mov r2, #1\n\t"
            "add ip, r2\n\t"
            "mov r0, ip\n\t"
            "cmp r0, #159\n\t"
            "bls 1b"
            : "+r"(clear_index)
            : "r"(polygon_source), "r"(scanline_buffers), "r"(displayed_bank),
              "r"(one), "r"(empty)
            : "r0", "r1", "r2", "cc", "memory");
        polygon_source = (s16 **)*polygon_source;
        polygon_stream = (s16 *)polygon_source;
    }
    {
        register s16 *shape asm("r1") = polygon_stream;
        register s32 zero asm("r2") = 0;
        asm volatile("ldrsh %0, [%1, %2]"
            : "=r"(vertex_count) : "r"(shape), "r"(zero));
        if (vertex_count == 0) {
            return;
        }
    }
loop_4:
    scratch_fill = 0xFF00FF00;
    {
        register M2C_UNK *call_arg0 asm("r0") = &scratch_fill;
        register u32 call_arg1 asm("r1") = 0x02000A00;
        register u32 call_arg2 asm("r2") = 0x01000050;
        asm volatile("" : "+r"(call_arg0), "+r"(call_arg1), "+r"(call_arg2));
        BiosCpuFastSet(call_arg0, call_arg1, call_arg2);
    }
    {
        register u32 zero asm("r4") = 0;
        asm volatile("" : "+r"(zero));
        vertex_index = zero;
    }
    {
        register s16 *shape asm("r7") = polygon_stream;
        register s32 zero asm("r1") = 0;
        asm volatile("ldrsh %0, [%1, %2]"
            : "=r"(vertex_count) : "r"(shape), "r"(zero));
    }
    if (vertex_index >= (u32)vertex_count) {
        goto raster_done;
    }
    {
        asm volatile(
            "mov r2, #2\n\t"
            "add r2, r9\n\t"
            "mov sl, r2"
            : "=r"(vertex_or_empty_interval) : "r"(polygon_stream) : "r2");
    }
loop_7:
        vertex_count -= 1;
        asm volatile(
            "mov r7, #0\n\t"
            "cmp ip, r0\n\t"
            "bcs 1f\n\t"
            "mov r7, ip\n\t"
            "add r7, #1\n"
            "1:\n\t"
            "mov r4, sl\n\t"
            "mov r0, #0\n\t"
            "ldrsh r3, [r4, r0]\n\t"
            "mov r2, #2\n\t"
            "ldrsh r1, [r4, r2]\n\t"
            "lsl r0, r7, #2\n\t"
            "add r0, r9\n\t"
            "mov r4, #2\n\t"
            "ldrsh r5, [r0, r4]\n\t"
            "mov r7, #4\n\t"
            "ldrsh r4, [r0, r7]"
            : "+r"(vertex_count), "=r"(edge_x), "=r"(edge_y),
              "=r"(edge_dx), "=r"(edge_or_bounds_work)
            : "r"(vertex_index), "r"(vertex_or_empty_interval), "r"(polygon_stream)
            : "r2", "r7", "cc");
        if ((edge_x == edge_dx) && (edge_y == edge_or_bounds_work)) {

        } else {
            edge_dx -= edge_x;
            edge_or_bounds_work -= edge_y;
            if (edge_dx >= 0) {
                asm volatile(
                    "mov r0, #0\n\t"
                    "mov r8, r0"
                    : "=r"(direction_or_row_cursor) : : "r0");
            } else {
                edge_dx = 0 - edge_dx;
                asm volatile(
                    "mov r2, #1\n\t"
                    "mov r8, r2"
                    : "=r"(direction_or_row_cursor) : : "r2");
            }
            if (edge_or_bounds_work >= 0) {
                register s32 y_forward asm("r7") = 0;
                asm volatile("str r7, %0" : "=m"(y_decreases) : "r"(y_forward));
            } else {
                edge_or_bounds_work = 0 - edge_or_bounds_work;
                y_decreases = 1;
            }
            edge_error = 0;
            if (edge_dx >= edge_or_bounds_work) {
                if (edge_or_bounds_work == 0) {
                    if ((u32) edge_y > 0x9FU) {

                    } else {
                        register s32 row_offset asm("r0") = edge_y << 1;
                        register u8 *scratch_base asm("r1") = (u8 *)0x02000A00;
                        asm volatile("" : "+r"(row_offset), "+r"(scratch_base));
                        asm volatile("add r2, r0, r1"
                            : "=r"(scratch_row) : "r"(row_offset), "r"(scratch_base));
                        asm volatile("mov r4, r8" : "=r"(edge_or_bounds_work) : "r"(direction_or_row_cursor));
                        if (edge_or_bounds_work == 0) {
                            var_r0_2 = edge_x;
                            if ((s32) var_r0_2 < 0) {
                                var_r0_2 = 0;
                            } else if ((s32) var_r0_2 > 0xEF) {
                                var_r0_2 = 0xEF;
                            }
                            *(u16 *)scratch_row = (u16) (var_r0_2 << 8);
                            {
                                register s32 forward_end asm("r0") = edge_x + edge_dx;
                                asm volatile("" : "+&r"(forward_end), "+r"(edge_x));
                                var_r0_3 = forward_end;
                            }
                            if (var_r0_3 < 0) {
                                goto block_35;
                            }
                            asm volatile("" : : : "memory");
                            goto block_36;
                        }
                        var_r0_4 = edge_x - edge_dx;
                        if (var_r0_4 < 0) {
                            var_r0_4 = 0;
                        } else if (var_r0_4 > 0xEF) {
                            var_r0_4 = 0xEF;
                        }
                        *(u16 *)scratch_row = (u16) (var_r0_4 << 8);
                        {
                            register s32 final_x asm("r0") = edge_x;
                            asm volatile("" : "+r"(final_x));
                            var_r0_3 = final_x;
                        }
                        if (var_r0_3 < 0) {
block_35:
                            var_r0_3 = 0;
                        } else {
block_36:
                            if (var_r0_3 > 0xEF) {
                                var_r0_3 = 0xEF;
                            }
                        }
                        *(u16 *)scratch_row = (u16) ((var_r0_3 + 1) | *(u16 *)scratch_row);
                    }
                } else {
                    {
                        register s32 row_offset asm("r0") = edge_y << 1;
                        register u8 *scratch_base asm("r7") = (u8 *)0x02000A00;
                        asm volatile("" : "+r"(row_offset), "+r"(scratch_base));
                        asm volatile("add r2, r0, r7"
                            : "=r"(scratch_row) : "r"(row_offset), "r"(scratch_base));
                    }
                    edge_step = 0;
                    if (edge_error < edge_dx) {
loop_40:
                        if ((u32) edge_y <= 0x9FU) {
                            var_r0_5 = edge_x;
                            if ((s32) var_r0_5 < 0) {
                                var_r0_5 = 0;
                            } else if ((s32) var_r0_5 > 0xEF) {
                                var_r0_5 = 0xEF;
                            }
                            if ((s32) var_r0_5 < (s32) M2C_FIELD(scratch_row, u8 *, 1)) {
                                M2C_FIELD(scratch_row, u8 *, 1) = (u8) var_r0_5;
                            }
                            if ((s32) var_r0_5 >= (s32) M2C_FIELD(scratch_row, u8 *, 0)) {
                                M2C_FIELD(scratch_row, u8 *, 0) = (u8) (var_r0_5 + 1);
                            }
                        }
                        if (direction_or_row_cursor == 0) {
                            edge_x += 1;
                        } else {
                            edge_x -= 1;
                        }
                        edge_error += edge_or_bounds_work;
                        if (edge_error >= edge_dx) {
                            register s32 y_direction asm("r7") = y_decreases;
                            asm volatile("" : "+r"(y_direction));
                            if (y_direction == 0) {
                                if ((s32) edge_y <= 0x9E) {
                                    edge_y += 1;
                                    scratch_row += 2;
                                    goto block_58;
                                }
                            } else if ((s32) edge_y > 0) {
                                edge_y -= 1;
                                scratch_row -= 2;
block_58:
                                edge_error -= edge_dx;
                                goto block_59;
                            }
                        } else {
block_59:
                            temp_r0_3 = edge_step + 1;
                            edge_step = temp_r0_3;
                            if (temp_r0_3 >= edge_dx) {

                            } else {
                                goto loop_40;
                            }
                        }
                    }
                }
            } else {
                steep_scratch_row = (edge_y * 2) + 0x02000A00;
                var_r0_6 = 0;
                goto steep_test;
steep_up:
                edge_y += 1;
                steep_scratch_row += 2;
                goto steep_after_y;
steep_down:
                if (edge_y <= 0) {
                    goto segment_done;
                }
                edge_y -= 1;
                steep_scratch_row -= 2;
steep_after_y:
                edge_error += edge_dx;
                if (edge_error >= edge_or_bounds_work) {
                    register s32 x_direction asm("r7") = direction_or_row_cursor;
                    asm volatile("" : "+r"(x_direction));
                    if (x_direction == 0) {
                        edge_x += 1;
                    } else {
                        edge_x -= 1;
                    }
                    edge_error -= edge_or_bounds_work;
                }
                var_r0_6 = edge_step + 1;
steep_test:
                edge_step = var_r0_6;
                asm volatile("" : : : "memory");
                {
                    register s32 steep_count asm("r7") = edge_step;
                    if (steep_count >= edge_or_bounds_work) {
                        goto segment_done;
                    }
                }
                if ((u32) edge_y <= 0x9FU) {
                    var_r0_7 = edge_x;
                    if ((s32) var_r0_7 < 0) {
                        var_r0_7 = 0;
                    } else if ((s32) var_r0_7 > 0xEF) {
                        var_r0_7 = 0xEF;
                    }
                    if ((s32) var_r0_7 < (s32) M2C_FIELD(steep_scratch_row, u8 *, 1)) {
                        M2C_FIELD(steep_scratch_row, u8 *, 1) = (u8) var_r0_7;
                    }
                    if ((s32) var_r0_7 >= (s32) M2C_FIELD(steep_scratch_row, u8 *, 0)) {
                        M2C_FIELD(steep_scratch_row, u8 *, 0) = (u8) (var_r0_7 + 1);
                    }
                }
                if (y_decreases != 0) {
                    goto steep_down;
                }
                if (edge_y <= 0x9E) {
                    goto steep_up;
                }
                goto segment_done;
            }
        }
segment_done:
        asm volatile(
            "mov r1, #4\n\t"
            "add sl, r1\n\t"
            "mov r2, #1\n\t"
            "add ip, r2"
            : "+r"(vertex_or_empty_interval), "+r"(vertex_index) : : "r1", "r2");
        asm volatile(
            "mov r4, r9\n\t"
            "mov r7, #0\n\t"
            "ldrsh r0, [r4, r7]"
            : "=r"(vertex_count) : "r"(polygon_stream) : "r4", "r7");
        if (vertex_index < (u32) vertex_count) {
            goto loop_7;
        }
raster_done:
    output_bounds = (u16 *)(gScanlineWindowBuffers + ((1 ^ *(u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank)) * SCANLINE_WINDOW_BANK_BYTES));
    raster_bounds = (u16 *)0x02000A00;
    {
        register u32 zero asm("r0");
        register s16 *empty_interval asm("r4");
        asm volatile(
            "mov r0, #0\n\t"
            "mov ip, r0"
            : "=r"(zero), "=r"(scanline_or_bounds));
        empty_interval = (s16 *)0xFFFF;
        asm volatile("" : "+r"(empty_interval));
        vertex_or_empty_interval = empty_interval;
    }
merge_scanline:
    {
        window_id = 0;
        packed_bounds = M2C_FIELD(raster_bounds, u16 *, 0);
        raster_right = packed_bounds;
        next_scanline = scanline_or_bounds + 1;
        scanline_or_bounds = packed_bounds;
        direction_or_row_cursor = (s32)(raster_bounds + 1);
        {
            register u32 invalid_interval asm("r4") = 0xFF00;
            if ((raster_right != invalid_interval) && (raster_right != 1) && (raster_right != 0xEFF0)) {
            if (*output_bounds == (u32)vertex_or_empty_interval) {
                asm volatile(
                    "mov r0, ip\n\t"
                    "strh r0, %0"
                    : "=m"(*output_bounds) : "r"(scanline_or_bounds) : "r0");
                goto merge_done;
            }
            goto merge_values;
merge_word:
            asm volatile(
                "mov r2, ip\n\t"
                "strh r2, %0"
                : "=m"(*output_bounds) : "r"(scanline_or_bounds) : "r2");
            goto merge_done;
merge_low:
            M2C_FIELD(output_bounds, u8 *, 0) = raster_right;
            goto merge_done;
merge_high:
            M2C_FIELD(output_bounds, u8 *, 1) = raster_left;
            goto merge_done;
merge_values:
loop_96:
                raster_left = M2C_FIELD(raster_bounds, u8 *, 1);
                output_left = M2C_FIELD(output_bounds, u8 *, 1);
                edge_or_bounds_work = raster_left;
                raster_right = M2C_FIELD(raster_bounds, u8 *, 0);
                output_right = M2C_FIELD(output_bounds, u8 *, 0);
                if ((output_left > (u32)edge_or_bounds_work) || (raster_right > output_right)) {
                    if (((u32)edge_or_bounds_work > output_left) || (output_right > raster_right)) {
                        if (((u32)edge_or_bounds_work > output_right) || (raster_right <= output_right)) {
                            if ((raster_right < output_left) || ((u32)edge_or_bounds_work >= output_left)) {
                                output_bounds += 1;
                                window_id += 1;
                                if (window_id <= 1U) {
                                    if (*output_bounds == (u32)vertex_or_empty_interval) {
                                        asm volatile(
                                            "mov r4, ip\n\t"
                                            "strh r4, %0"
                                            : "=m"(*output_bounds) : "r"(scanline_or_bounds) : "r4");
                                    } else {
                                        goto loop_96;
                                    }
                                }
                            } else {
                                goto merge_high;
                            }
                        } else {
                            goto merge_low;
                        }
                    } else {
                        goto merge_word;
                    }
                }
        }
        }
merge_done:
        output_bounds = (u16 *)((u8 *)output_bounds + ((4 - window_id) * 2));
        raster_bounds = (u16 *)direction_or_row_cursor;
        {
            register u32 next_row asm("r7") = next_scanline;
            register u32 row_test asm("r0");
            scanline_or_bounds = next_row;
            asm volatile("mov %0, ip" : "=r"(row_test));
            if (row_test <= 0x9F) {
                goto merge_scanline;
            }
        }
    }
    asm volatile(
        "mov r1, r9\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]"
        : "=r"(vertex_count) : "r"(polygon_stream) : "r1", "r2");
    vertex_count <<= 2;
    vertex_count += 2;
    polygon_stream = (s16 *)((u8 *)polygon_stream + vertex_count);
    asm volatile(
        "mov r4, r9\n\t"
        "mov r7, #0\n\t"
        "ldrsh r0, [r4, r7]"
        : "=r"(vertex_count) : "r"(polygon_stream) : "r4", "r7");
    if (vertex_count != 0) {
        goto loop_4;
    }
}
