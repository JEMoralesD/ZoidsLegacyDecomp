#include "m2c_prelude.h"
#include "screen_effects.h"

s16 Sin256(s32) asm("func_08092A90");                             /* extern */
s16 Cos256(s32) asm("func_08092ADC");                             /* extern */
extern u8 gScanlineWindowBuffers[] asm("D_02000000");

void BuildRotatingSplitScanlineWindows(void) asm("func_08095B84");

void BuildRotatingSplitScanlineWindows(void) {
    volatile s32 x_decreases;
    volatile s32 y_decreases;
    register s32 shallow_error asm("ip");
    register s32 steep_error asm("ip");
    s32 cosine_delta_fixed8;
    s32 sine_delta_fixed8;
    register s32 line_y asm("r6");
    s32 line_x;
    register s32 line_dy asm("r8");
    register s32 line_dx asm("r9");
    register s32 shallow_steps asm("sl");
    register s32 steep_steps asm("sl");
    u32 temp_r0;
    u32 temp_r1;

    {
        register u32 clear_index asm("r3") = 0;
        register u8 *rotation_angle asm("r8") = (u8 *)SCANLINE_WINDOW_ADDRESS(rotation_angle);
        register u8 *scanline_buffers asm("r6") = gScanlineWindowBuffers;
        register u8 *displayed_bank asm("r5") = (u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
        register u32 one asm("r4") = 1;

        do {
            register u32 destination asm("r2");
            register u32 value asm("r0");
            if (clear_index <= 0x4F) {
                register u32 page asm("r0");
                register u32 bank asm("r1");
                destination = clear_index << 3;
                asm volatile("" : "+r"(destination));
                page = *displayed_bank;
                bank = one;
                bank ^= page;
                page = bank << 2;
                page += bank;
                page <<= 8;
                destination += page;
                destination += (u32)scanline_buffers;
                value = 0x78F00000;
            } else {
                register u32 page asm("r0");
                register u32 bank asm("r1");
                destination = clear_index << 3;
                asm volatile("" : "+r"(destination));
                page = *displayed_bank;
                bank = one;
                bank ^= page;
                page = bank << 2;
                page += bank;
                page <<= 8;
                destination += page;
                destination += (u32)scanline_buffers;
                value = 0x00780000;
            }
            *(u32 *)destination = value;
            {
                register u32 next asm("r0") = clear_index + 1;
                next <<= 24;
                clear_index = next >> 24;
            }
        } while (clear_index <= 0x9F);
        asm volatile("" : : "r"(rotation_angle));
    line_x = 0;
    line_y = 0;
    {
        register u8 *call_ptr asm("r1") = rotation_angle;
        register u32 call_arg asm("r0") = *call_ptr;
        asm volatile("" : "+r"(call_ptr), "+r"(call_arg));
        cosine_delta_fixed8 = 0 - (Cos256(call_arg) * 0xA0);
    }
    if (cosine_delta_fixed8 < 0) {
        cosine_delta_fixed8 += 0xFF;
    }
    line_dx = cosine_delta_fixed8 >> 8;
    {
        register u8 *call_ptr asm("r3") = rotation_angle;
        register u32 call_arg asm("r0") = *call_ptr;
        asm volatile("" : "+r"(call_ptr), "+r"(call_arg));
        sine_delta_fixed8 = 0 - (Sin256(call_arg) * 0xA0);
    }
    if (sine_delta_fixed8 < 0) {
        sine_delta_fixed8 += 0xFF;
    }
    line_dy = sine_delta_fixed8 >> 8;
    }
    {
    register s32 sign_test asm("r4") = line_dx;
    asm volatile("" : "+r"(sign_test));
    if (sign_test >= 0) {
        register s32 zero asm("r0") = 0;
        x_decreases = zero;
    } else {
        register s32 magnitude asm("r1") = line_dx;
        register s32 one asm("r3");
        magnitude = 0 - magnitude;
        line_dx = magnitude;
        one = 1;
        x_decreases = one;
    }
    }
    {
    register s32 sign_test asm("r4") = line_dy;
    asm volatile("" : "+r"(sign_test));
    if (sign_test >= 0) {
        register s32 zero asm("r0") = 0;
        y_decreases = zero;
    } else {
        register s32 magnitude asm("r1") = line_dy;
        register s32 one asm("r3");
        asm volatile("" : "+r"(magnitude));
        magnitude = 0 - magnitude;
        line_dy = magnitude;
        one = 1;
        y_decreases = one;
    }
    }
    if (line_dx > line_dy) {
        register s32 zero asm("r4") = 0;
        asm volatile("" : "+r"(zero));
        shallow_error = zero;
        if (shallow_error > line_dx) {
            return;
        }
        {
            register s32 steps asm("r0") = 1;
            asm volatile("" : "+r"(steps));
            steps += line_dx;
            shallow_steps = steps;
        }
loop_56:
        {
        register u8 *screen_0 asm("r2");
        register u8 *screen_2 asm("r3");
        {
        register u32 row asm("r0") = line_y + 0x4F;
        temp_r0 = row;
        if (temp_r0 <= 0x4FU) {
            register s32 clamp asm("r4") = line_x + 0x77;
            register u32 row_index asm("r5");
            asm volatile("" : "+r"(clamp));
            row_index = row;
            if (clamp < 0) {
                clamp = 0;
            } else if (clamp > 0xEF) {
                clamp = 0xEF;
            }
            {
                register u8 *page_ptr asm("r1") = (u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
                register u32 page asm("r0") = *page_ptr;
                register u32 bank asm("r1") = 1;
                register u32 address asm("r0");
                register u32 offset asm("r1");
                register u32 value asm("r1");
                bank ^= page;
                address = bank << 2;
                address += bank;
                address <<= 8;
                offset = row_index << 3;
                address += offset;
                screen_0 = gScanlineWindowBuffers;
                address += (u32)screen_0;
                value = (u32)clamp + 1;
                *(u16 *)address = value;
            }
            {
            register s32 clamp2 asm("r4");
            register u32 opposite_row asm("r5");
            register u32 start_x asm("r0") = 0x78;
            clamp2 = start_x - line_x;
            start_x = 0x50;
            opposite_row = start_x - line_y;
            if (clamp2 < 0) {
                clamp2 = 0;
            } else if (clamp2 > 0xEF) {
                clamp2 = 0xEF;
            }
            {
                register u8 *page_ptr asm("r3") = (u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
                register u32 page asm("r0") = *page_ptr;
                register u32 bank asm("r1") = 1;
                register u32 address asm("r0");
                register u32 offset asm("r1");
                register u32 value asm("r1");
                bank ^= page;
                address = bank << 2;
                address += bank;
                address <<= 8;
                offset = opposite_row << 3;
                address += offset;
                address += (u32)screen_0;
                value = (u32)clamp2 << 8;
                screen_0 = (u8 *)0xF0;
                value |= (u32)screen_0;
                *(u16 *)address = value;
            }
            }
        }
        }
        {
        register u32 column asm("r1") = line_x + 0x4F;
        temp_r1 = column;
        if (temp_r1 <= 0x4FU) {
            register s32 clamp3 asm("r4");
            register u32 column_index asm("r5");
            register u32 start_y asm("r0") = 0x77;
            asm volatile("" : "+r"(start_y));
            clamp3 = start_y - line_y;
            asm volatile("" : "+r"(clamp3));
            column_index = column;
            if (clamp3 < 0) {
                clamp3 = 0;
            } else if (clamp3 > 0xEF) {
                clamp3 = 0xEF;
            }
            {
                register u8 *page_ptr asm("r1") = (u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
                register u32 page asm("r0") = *page_ptr;
                register u32 bank asm("r1") = 1;
                register u32 address asm("r0");
                register u32 offset asm("r1");
                register u32 value asm("r1");
                register u32 high asm("r4");
                register u32 high_copy asm("r2");
                bank ^= page;
                address = bank << 2;
                address += bank;
                address <<= 8;
                offset = column_index << 3;
                address += offset;
                screen_2 = (u8 *)0x02000002;
                address += (u32)screen_2;
                value = (u32)clamp3 + 1;
                high = 0x7800;
                asm volatile("" : "+r"(high));
                high_copy = high;
                asm volatile("" : "+r"(high_copy));
                value |= high_copy;
                *(u16 *)address = value;
            }
            {
            register s32 clamp4 asm("r4") = line_y;
            register u32 opposite_column asm("r5");
            register u32 start asm("r0");
            clamp4 += 0x78;
            start = 0x50;
            opposite_column = start - line_x;
            if (clamp4 < 0) {
                clamp4 = 0;
            } else if (clamp4 > 0xEF) {
                clamp4 = 0xEF;
            }
            {
                register u8 *page_ptr asm("r1") = (u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
                register u32 page asm("r0") = *page_ptr;
                register u32 bank asm("r1") = 1;
                register u32 address asm("r0");
                register u32 offset asm("r1");
                register u32 value asm("r1");
                register u32 low asm("r2");
                bank ^= page;
                address = bank << 2;
                address += bank;
                address <<= 8;
                offset = opposite_column << 3;
                address += offset;
                address += (u32)screen_2;
                value = (u32)clamp4 << 8;
                low = 0x78;
                value |= low;
                *(u16 *)address = value;
            }
            }
        }
        }
        }
        {
        register s32 flag asm("r3") = x_decreases;
        asm volatile("" : "+r"(flag));
        if (flag == 0) {
            line_x += 1;
        } else {
            line_x -= 1;
        }
        }
        shallow_error += line_dy;
        if (shallow_error >= line_dx) {
            register s32 flag asm("r4") = y_decreases;
            asm volatile("" : "+r"(flag));
            if (flag == 0) {
                line_y += 1;
            } else {
                line_y -= 1;
            }
            {
                register s32 acc asm("r0") = shallow_error;
                register s32 major asm("r1") = line_dx;
                asm volatile("" : "+r"(acc), "+r"(major));
                asm volatile(".syntax unified\n\tsubs %0, %0, %1\n\t.syntax divided"
                             : "+r"(acc) : "r"(major) : "cc");
                shallow_error = acc;
            }
        }
        {
        register s32 decrement asm("r3") = 1;
        register s32 test asm("r4");
        asm volatile("" : "+r"(decrement));
        asm volatile(".syntax unified\n\tnegs %0, %0\n\tadd %1, %0\n\t.syntax divided"
                     : "+r"(decrement), "+r"(shallow_steps) : : "cc");
        test = shallow_steps;
        asm volatile("" : "+r"(test));
        if (test != 0) {
            goto loop_56;
        }
        }
        return;
    }
    {
    register s32 zero asm("r0") = 0;
    register s32 major_test asm("r1");
    asm volatile("" : "+r"(zero));
    steep_error = zero;
    asm volatile("" : "+r"(steep_error));
    major_test = line_dy;
    asm volatile("" : "+r"(major_test));
    if (major_test < 0) {
        return;
    }
    }
    {
    register s32 steps asm("r3") = 1;
    asm volatile("" : "+r"(steps));
    steps += line_dy;
    steep_steps = steps;
    }
loop_21:
    {
    register u8 *screen_0_first asm("r3");
    register u8 *screen_0_second asm("r2");
    {
    register u32 row asm("r0") = line_y + 0x4F;
    if (row <= 0x4FU) {
        register s32 clamp asm("r4") = line_x + 0x77;
        register u32 row_index asm("r5") = row;
        asm volatile("" : "+r"(clamp), "+r"(row_index));
        if (clamp < 0) {
            clamp = 0;
        } else if (clamp > 0xEF) {
            clamp = 0xEF;
        }
        {
        register u8 *page_ptr asm("r1") = (u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
        register u32 page asm("r0") = *page_ptr;
        register u32 bank asm("r1") = 1;
        register u32 address asm("r0");
        register u32 offset asm("r1");
        register u32 value asm("r1");
        bank ^= page;
        address = bank << 2;
        address += bank;
        address <<= 8;
        offset = row_index << 3;
        address += offset;
        screen_0_first = gScanlineWindowBuffers;
        address += (u32)screen_0_first;
        value = (u32)clamp + 1;
        *(u16 *)address = value;
        }
        {
        register s32 clamp2 asm("r4");
        register u32 opposite_row asm("r5");
        register u32 start asm("r0") = 0x78;
        asm volatile("" : "+r"(start));
        clamp2 = start - line_x;
        start = 0x50;
        opposite_row = start - line_y;
        screen_0_second = screen_0_first;
        asm volatile("" : "+r"(screen_0_second));
        if (clamp2 < 0) {
            clamp2 = 0;
        } else if (clamp2 > 0xEF) {
            clamp2 = 0xEF;
        }
        {
        register u8 *page_ptr asm("r1") = (u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
        register u32 page asm("r0") = *page_ptr;
        register u32 bank asm("r1") = 1;
        register u32 address asm("r0");
        register u32 offset asm("r1");
        register u32 value asm("r1");
        bank ^= page;
        address = bank << 2;
        address += bank;
        address <<= 8;
        offset = opposite_row << 3;
        address += offset;
        address += (u32)screen_0_second;
        value = (u32)clamp2 << 8;
        screen_0_second = (u8 *)0xF0;
        value |= (u32)screen_0_second;
        *(u16 *)address = value;
        }
        }
    }
    }
    {
    register u32 column asm("r1") = line_x + 0x4F;
    if (column <= 0x4FU) {
        register s32 clamp3 asm("r4");
        register u32 column_index asm("r5");
        register u32 start_y asm("r0") = 0x77;
        asm volatile("" : "+r"(start_y));
        clamp3 = start_y - line_y;
        column_index = column;
        asm volatile("" : "+r"(column_index));
        if (clamp3 < 0) {
            clamp3 = 0;
        } else if (clamp3 > 0xEF) {
            clamp3 = 0xEF;
        }
        {
        register u8 *page_ptr asm("r3") = (u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
        register u32 page asm("r0") = *page_ptr;
        register u32 bank asm("r1") = 1;
        register u32 address asm("r0");
        register u32 offset asm("r1");
        register u8 *base asm("r1");
        register u32 value asm("r1");
        register u32 high asm("r3");
        register u32 high_copy asm("r2");
        bank ^= page;
        address = bank << 2;
        address += bank;
        address <<= 8;
        offset = column_index << 3;
        address += offset;
        base = (u8 *)0x02000002;
        address += (u32)base;
        value = (u32)clamp3 + 1;
        high = 0x7800;
        asm volatile("" : "+r"(high));
        high_copy = high;
        asm volatile("" : "+r"(high_copy));
        value |= high_copy;
        *(u16 *)address = value;
        }
        {
        register s32 clamp4 asm("r4") = line_y;
        register u32 opposite_column asm("r5");
        register u32 start asm("r0");
        register u8 *base asm("r3");
        clamp4 += 0x78;
        start = 0x50;
        opposite_column = start - line_x;
        base = (u8 *)0x02000002;
        asm volatile("" : "+r"(base));
        if (clamp4 < 0) {
            clamp4 = 0;
        } else if (clamp4 > 0xEF) {
            clamp4 = 0xEF;
        }
        {
        register u8 *page_ptr asm("r1") = (u8 *)SCANLINE_WINDOW_ADDRESS(displayed_bank);
        register u32 page asm("r0") = *page_ptr;
        register u32 bank asm("r1") = 1;
        register u32 address asm("r0");
        register u32 offset asm("r1");
        register u32 value asm("r1");
        register u32 low asm("r2");
        bank ^= page;
        address = bank << 2;
        address += bank;
        address <<= 8;
        offset = opposite_column << 3;
        address += offset;
        address += (u32)base;
        value = (u32)clamp4 << 8;
        low = 0x78;
        value |= low;
        *(u16 *)address = value;
        }
        }
    }
    }
    }
    {
    register s32 flag asm("r3") = y_decreases;
    asm volatile("" : "+r"(flag));
    if (flag == 0) {
        line_y += 1;
    } else {
        line_y -= 1;
    }
    }
    steep_error += line_dx;
    if (steep_error >= line_dy) {
        register s32 flag asm("r4") = x_decreases;
        asm volatile("" : "+r"(flag));
        if (flag == 0) {
            line_x += 1;
        } else {
            line_x -= 1;
        }
        {
        register s32 acc asm("r0") = steep_error;
        register s32 major asm("r1") = line_dy;
        asm volatile("" : "+r"(acc), "+r"(major));
        asm volatile(".syntax unified\n\tsubs %0, %0, %1\n\t.syntax divided"
                     : "+r"(acc) : "r"(major) : "cc");
        steep_error = acc;
        }
    }
    {
    register s32 decrement asm("r3") = 1;
    register s32 test asm("r4");
    asm volatile("" : "+r"(decrement));
    asm volatile(".syntax unified\n\tnegs %0, %0\n\tadd %1, %0\n\t.syntax divided"
                 : "+r"(decrement), "+r"(steep_steps) : : "cc");
    test = steep_steps;
    asm volatile("" : "+r"(test));
    if (test != 0) {
        goto loop_21;
    }
    }
}
