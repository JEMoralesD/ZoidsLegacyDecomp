#include "m2c_prelude.h"
#include "field_display.h"

extern u16 GetFieldBackgroundTile(u8, s32, s32) asm("func_0809D5F4");
extern u16 gFieldMapIdAtColumnStart asm("D_0202ECF4_init");
extern u16 gFieldMapIdAfterColumnUpdate asm("D_0202ECF4_middle");
extern u16 gFieldMapIdAfterRowUpdate asm("D_0202ECF4_final");
extern struct FieldStreamedTileOrigin gFieldTileOriginsAtColumnStart[] asm("D_0203249C_init");
extern struct FieldStreamedTileOrigin gFieldTileOriginsAfterColumnUpdate[] asm("D_0203249C_middle_a");
extern struct FieldStreamedTileOrigin gWorldMapTileOriginsAfterColumnUpdate[] asm("D_0203249C_middle_b");
extern struct FieldStreamedTileOrigin gFieldTileOriginsAfterRowUpdate[] asm("D_0203249C_final");
extern struct FieldStreamedTileYView gFieldTileYForDelta[] asm("D_0203249E_delta_a");
extern struct FieldStreamedTileYView gWorldMapTileYForDelta[] asm("D_0203249E_delta_b");
extern struct FieldStreamedTileYView gFieldTileYForRowUpdate[] asm("D_0203249E_tile_inc");
extern struct FieldStreamedTileYView gWorldMapTileYAfterRowUpdate[] asm("D_0203249E_final_b");
extern s32 gFieldBackgroundTilemaps[] asm("D_02032E88");
extern s32 gFieldScrollAtColumnStart[] asm("D_03000054_init");
extern s32 gFieldScrollAfterColumnUpdate[] asm("D_03000054_middle_a");
extern s32 gWorldMapScrollAfterColumnUpdate[] asm("D_03000054_middle_b");
extern s32 gFieldScrollAfterRowUpdate[] asm("D_03000054_final_a");
extern s32 gWorldMapScrollAfterRowUpdate[] asm("D_03000054_final_b");

void StreamFieldBackgroundTilemaps(void) asm("func_0809DD80");

void StreamFieldBackgroundTilemaps(void)
{
    u8 layer;
    u16 *map_id_address;
    register s32 *camera asm("r2");
    register struct FieldStreamedTileOrigin *scroll asm("r5");
    volatile u32 screen_base;
    register s32 mask asm("sl");

    layer = 0;
    map_id_address = &gFieldMapIdAtColumnStart;
    camera = gFieldScrollAtColumnStart;
    scroll = gFieldTileOriginsAtColumnStart;
    mask = 0x1FF;
    do {
        if (gFieldBackgroundTilemaps[layer] != 0) {
            register s32 delta asm("r0");

            if (*map_id_address != 0 || layer == 0) {
                register u32 screen_offset asm("r0");
                register u32 screen_value asm("r1");
                register u32 camera_offset asm("r0");
                register s32 camera_value asm("r1");
                register struct FieldStreamedTileOrigin *scroll_ptr asm("r0");
                register s32 mask_value asm("r2");

                screen_offset = layer << 11;
                asm volatile("" : "+r"(screen_offset));
                screen_value = 0xC0;
                screen_value <<= 19;
                asm volatile("" : "+r"(screen_value));
                screen_value = screen_offset + screen_value;
                screen_base = screen_value;
                camera_offset = layer << 3;
                camera_value = *(s32 *)(camera_offset + (u32)camera) >> 11;
                scroll_ptr = (struct FieldStreamedTileOrigin *)((layer << 2) + (u32)scroll);
                delta = camera_value - scroll_ptr->x;
                mask_value = mask;
                asm volatile("" : "+r"(mask_value));
                delta &= mask_value;
            } else {
                register struct FieldStreamedTileOrigin *scroll_ptr asm("r1");
                register s32 mask_value asm("r3");
                register u32 screen_value asm("r4");

                delta = camera[6] >> 11;
                scroll_ptr = (struct FieldStreamedTileOrigin *)((layer << 2) + (u32)scroll);
                delta -= scroll_ptr->x;
                mask_value = mask;
                asm volatile("" : "+r"(mask_value));
                delta &= mask_value;
                screen_value = 0x0600F800;
                screen_base = screen_value;
            }

            if (delta != 0) {
                register s32 tile_x_value asm("r0");
                register s32 tile_x_home asm("r3");
                s32 tile_x;
                s16 edge_cell;
                register s32 scroll_y asm("r8");
                register u32 column_base asm("r6");
                register s32 tile_mask asm("r9");
                register s32 i_value asm("r4");
                register s32 tile_y asm("r5");

                if (delta == 0x1FF) {
                    tile_x_value = scroll[layer].x - 1;
                } else {
                    tile_x_value = scroll[layer].x + 0x1F;
                }
                asm volatile("lsl %1, %1, #16\n\t"
                             "lsr %0, %1, #16"
                             : "=r"(tile_x_home), "+r"(tile_x_value));

                {
                    register s16 *scroll_y_ptr asm("r0");
                    register s32 zero asm("r1");
                    register s32 loaded_y asm("r4");

                    scroll_y_ptr = (s16 *)((u8 *)scroll + 2);
                    scroll_y_ptr = (s16 *)((layer << 2) + (u32)scroll_y_ptr);
                    edge_cell = 0;
                    {
                        register s32 sign_tmp asm("r1");
                        sign_tmp = tile_x_home << 16;
                        tile_x = sign_tmp >> 16;
                    }
                    zero = 0;
                    asm volatile("ldrsh %0, [%1, %2]"
                                 : "=r"(loaded_y)
                                 : "r"(scroll_y_ptr), "r"(zero));
                    scroll_y = loaded_y;
                }
                {
                    register s32 low_mask asm("r4") = 0x1F;
                    register u32 column_offset asm("r0");
                    register u32 screen_tmp asm("r1");
                    tile_mask = low_mask;
                    column_offset = (tile_x & low_mask) * 2;
                    asm volatile("" : "+r"(column_offset));
                    screen_tmp = screen_base;
                    column_base = column_offset + screen_tmp;
                }
                do {
                    u16 tile;
                    register s32 signed_tile_y asm("r2");

                    i_value = (s16)edge_cell;
                    signed_tile_y = scroll_y;
                    asm volatile("" : "+r"(signed_tile_y));
                    tile_y = signed_tile_y + i_value;
                    signed_tile_y = (s16)tile_y;
                    tile = GetFieldBackgroundTile(layer, tile_x, signed_tile_y);
                    tile_y = (tile_y & tile_mask) << 6;
                    tile_y += column_base;
                    *(u16 *)tile_y = tile;
                    i_value = (s16)(u16)(i_value + 1);
                    edge_cell = (u16)i_value;
                } while ((s16)i_value <= 0x14);
            }
        }

        {
            register u16 *next_mode asm("r0") = &gFieldMapIdAfterColumnUpdate;
            register u16 mode_value asm("r1") = *next_mode;
            map_id_address = next_mode;
            if (mode_value != 0 || layer == 0) {
                register struct FieldStreamedTileOrigin *next_scroll asm("r3");
                register u32 scroll_offset asm("r1");
                struct FieldStreamedTileOrigin *out;

                next_scroll = gFieldTileOriginsAfterColumnUpdate;
                scroll_offset = layer << 2;
                out = (struct FieldStreamedTileOrigin *)(scroll_offset + (u32)next_scroll);
                camera = gFieldScrollAfterColumnUpdate;
                out->x = camera[layer * 2] >> 11;
                scroll = next_scroll;
            } else {
                register struct FieldStreamedTileOrigin *next_scroll asm("r3");
                register u32 scroll_offset asm("r1");
                struct FieldStreamedTileOrigin *out;

                next_scroll = gWorldMapTileOriginsAfterColumnUpdate;
                scroll_offset = layer << 2;
                out = (struct FieldStreamedTileOrigin *)(scroll_offset + (u32)next_scroll);
                camera = gWorldMapScrollAfterColumnUpdate;
                out->x = camera[6] >> 11;
                scroll = next_scroll;
            }
        }
        layer++;
    } while (layer <= 1);

    layer = 0;
    do {
        register s32 next_layer asm("r9");
        register s32 active asm("r0");
        register s32 *active_base asm("r0");
        register u32 active_offset asm("r1");
        register u32 layer_offset asm("r6");
        register s32 initial_next asm("r3");

        active_base = gFieldBackgroundTilemaps;
        active_offset = layer << 2;
        active = *(s32 *)(active_offset + (u32)active_base);
        layer_offset = active_offset;
        asm volatile("" : "+r"(layer_offset));
        initial_next = layer + 1;
        asm volatile("" : "+r"(initial_next));
        next_layer = initial_next;
        if (active != 0) {
            register s32 raw_delta asm("r0");
            register s32 delta asm("r1");

            if (*map_id_address != 0 || layer == 0) {
                register u32 camera_offset asm("r0");
                register u32 screen_offset asm("r0");
                register u32 screen_value asm("r4");
                register s32 camera_y asm("r1");
                register u16 *y_base asm("r2");
                register u16 *y_ptr asm("r0");

                screen_offset = layer << 11;
                screen_value = 0xC0;
                screen_value <<= 19;
                asm volatile("" : "+r"(screen_value));
                screen_value = screen_offset + screen_value;
                screen_base = screen_value;
                camera_offset = (layer * 2 + 1) * 4;
                asm volatile("" : "+r"(camera_offset));
                camera_y = *(s32 *)(camera_offset + (u32)camera) >> 11;
                y_base = &gFieldTileYForDelta[0].y;
                y_ptr = (u16 *)(layer_offset + (u32)y_base);
                raw_delta = camera_y - *y_ptr;
            } else {
                register u32 screen_value asm("r4") = 0x0600F800;
                register u16 *y_base asm("r2");
                register u16 *y_ptr asm("r1");

                screen_base = screen_value;
                raw_delta = camera[7] >> 11;
                y_base = &gWorldMapTileYForDelta[0].y;
                y_ptr = (u16 *)(layer_offset + (u32)y_base);
                raw_delta -= *y_ptr;
            }

            {
                register s32 second_mask asm("r3");
                second_mask = 0x1FF;
                asm volatile("" : "+r"(second_mask));
                raw_delta &= second_mask;
                asm volatile("" : "+r"(raw_delta));
            }
            delta = raw_delta;
            {
                register s32 recomputed_next asm("r4");
                asm volatile("add %0, %1, #1"
                             : "=r"(recomputed_next)
                             : "r"(layer));
                next_layer = recomputed_next;
            }
            if (delta != 0) {
                register s32 tile_y_value asm("r0");
                s32 tile_y;
                s16 edge_cell;
                register u32 row_offset asm("r8");
                register u16 scroll_x_raw asm("r3");
                register s32 i_value asm("r5");
                register s32 tile_x asm("r4");

                {
                    register struct FieldStreamedTileOrigin *scroll_ptr asm("r0");
                    scroll_ptr = (struct FieldStreamedTileOrigin *)(layer_offset +
                                                          (u32)scroll);
                    scroll_x_raw = scroll_ptr->x;
                }
                asm volatile("" : "+r"(scroll_x_raw));
                if (delta == 0x1FF) {
                    register struct FieldStreamedTileYView *y_base asm("r1");
                    register u16 *y_ptr asm("r0");
                    y_base = gWorldMapTileYForDelta;
                    y_ptr = (u16 *)(layer_offset + (u32)y_base);
                    tile_y_value = *y_ptr - 1;
                } else {
                    register struct FieldStreamedTileYView *y_base asm("r2");
                    register u16 *y_ptr asm("r0");
                    y_base = gFieldTileYForRowUpdate;
                    y_ptr = (u16 *)(layer_offset + (u32)y_base);
                    tile_y_value = *y_ptr + 0x15;
                }

                asm volatile("lsl %0, %0, #16\n\t"
                             "lsr %0, %0, #16"
                             : "+r"(tile_y_value));
                edge_cell = 0;
                {
                    register s32 signed_scroll_x asm("r1");
                    register s32 recomputed_next asm("r3");
                    register s32 shifted_tile_y asm("r0");

                    signed_scroll_x = scroll_x_raw << 16;
                    shifted_tile_y = tile_y_value << 16;
                    asm volatile("add %0, %1, #1"
                                 : "=r"(recomputed_next)
                                 : "r"(layer));
                    next_layer = recomputed_next;
                    mask = signed_scroll_x >> 16;
                    tile_y = shifted_tile_y >> 16;
                }
                {
                    register s32 row_value asm("r0");
                    register s32 tile_mask asm("r4");
                    row_value = tile_y;
                    asm volatile("" : "+r"(row_value));
                    tile_mask = 0x1F;
                    row_offset = (row_value & tile_mask) << 6;
                }
                do {
                    register s32 base_x asm("r0");
                    register s32 signed_tile_x asm("r1");
                    u16 tile;

                    i_value = (s16)edge_cell;
                    base_x = mask;
                    asm volatile("" : "+r"(base_x));
                    tile_x = base_x + i_value;
                    signed_tile_x = (s16)tile_x;
                    tile = GetFieldBackgroundTile(layer, signed_tile_x, tile_y);
                    {
                        register s32 tile_mask asm("r1") = 0x1F;
                        register u32 screen_tmp asm("r2");

                        tile_x &= tile_mask;
                        tile_x *= 2;
                        screen_tmp = screen_base;
                        tile_x += screen_tmp;
                        tile_x += row_offset;
                        *(u16 *)tile_x = tile;
                    }
                    i_value = (s16)(u16)(i_value + 1);
                    edge_cell = (u16)i_value;
                } while ((s16)i_value <= 0x1E);
            }
        }

        {
            register u16 *next_mode asm("r0") = &gFieldMapIdAfterRowUpdate;
            register u16 mode_value asm("r1") = *next_mode;
            map_id_address = next_mode;
            if (mode_value != 0 || layer == 0) {
                register struct FieldStreamedTileYView *out_base asm("r3");
                register u16 *out_ptr asm("r0");
                register u32 camera_offset asm("r1");
                register s32 camera_value asm("r1");

                out_base = gFieldTileYForRowUpdate;
                out_ptr = (u16 *)(layer_offset + (u32)out_base);
                camera_offset = (layer * 2 + 1) * 4;
                camera = gFieldScrollAfterRowUpdate;
                camera_value = *(s32 *)(camera_offset + (u32)camera) >> 11;
                *out_ptr = camera_value;
            } else {
                register struct FieldStreamedTileYView *out_base asm("r3");
                register u16 *out_ptr asm("r1");

                out_base = gWorldMapTileYAfterRowUpdate;
                out_ptr = (u16 *)(layer_offset + (u32)out_base);
                camera = gWorldMapScrollAfterRowUpdate;
                *out_ptr = camera[7] >> 11;
            }
        }
        scroll = gFieldTileOriginsAfterRowUpdate;
        {
            register s32 next_copy asm("r3") = next_layer;
            register s32 trunc_tmp asm("r0");
            asm volatile("" : "+r"(next_copy));
            trunc_tmp = next_copy << 24;
            asm volatile("" : "+r"(trunc_tmp));
            layer = (u32)trunc_tmp >> 24;
        }
    } while (layer <= 1);
}
