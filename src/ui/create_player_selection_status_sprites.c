#include "player_selection.h"

void *CreateSprite(s32, s32, s32, s32) asm("func_08094484");

extern struct PlayerSelectionSpriteFlagsView *gPlayerSelectionTeamMarkerSprites[] asm("D_02032A88");
extern struct PlayerSelectionSpriteFlagsView *gPlayerSelectionNewMarkerSprites[] asm("D_02032AA8");

void CreatePlayerSelectionStatusSprites(s32 visible_rows, s32 marker_column, s32 first_row) asm("func_080ACA8C");

void CreatePlayerSelectionStatusSprites(s32 visible_rows, s32 marker_column, s32 first_row)
{
    u32 visible_row_count;
    u32 visible_row;
    register s32 palette_bank asm("r9");
    register s32 zero asm("r8");
    register s32 team_marker_x_shifted16 asm("sl");
    volatile s32 reserve0;
    volatile s32 reserve1;
    volatile s32 reserve2;
    volatile s32 reserve3;
    volatile s32 reserve4;
    volatile s32 first_row_unsigned;
    volatile s32 new_marker_x_shifted16;

    asm volatile("" : "=m"(reserve0), "=m"(reserve1), "=m"(reserve2),
                       "=m"(reserve3), "=m"(reserve4));

    visible_rows <<= 24;
    visible_row_count = (u32)visible_rows >> 24;
    marker_column <<= 16;
    marker_column = (u32)marker_column >> 16;
    first_row <<= 16;
    first_row_unsigned = (u32)first_row >> 16;
    visible_row = 0;

    if (visible_row < visible_row_count) {
        {
            register s32 signed_marker_column asm("r1") = marker_column;

            signed_marker_column <<= 16;
            signed_marker_column >>= 16;
            marker_column = signed_marker_column;
        }
        palette_bank = PLAYER_SELECTION_MARKER_PALETTE;
        zero = visible_row;
        new_marker_x_shifted16 = (marker_column - 1) << 19;
        team_marker_x_shifted16 = marker_column << 19;
        do {
            register s32 marker_y_pixels asm("r4");
            register s32 sprite_slot_offset asm("r5");
            register volatile s32 *outgoing asm("sp");

            {
                register s32 first_row_value asm("r5") = first_row_unsigned;

                marker_y_pixels = (s16)(((s32)(first_row_value << 16) >> 13) + (visible_row << 4));
            }
            outgoing[0] = marker_y_pixels;
            outgoing[1] = PLAYER_SELECTION_TEAM_MARKER_TILE_OFFSET;
            {
                register s32 value asm("r0") = palette_bank;

                outgoing[2] = value;
            }
            outgoing[3] = PLAYER_SELECTION_TEAM_MARKER_FLAGS;
            {
                register s32 value asm("r5") = zero;

                outgoing[4] = value;
            }
            {
                register s32 frame_table_argument asm("r0") = PLAYER_SELECTION_TEAM_MARKER_FRAMES_ROM;
                register s32 animation_table_argument asm("r1") = PLAYER_SELECTION_TEAM_MARKER_ANIMATIONS_ROM;
                register s32 animation_id_argument asm("r2") = 0;
                register s32 marker_x_argument asm("r3");
                register s32 value asm("r5") = team_marker_x_shifted16;

                asm volatile("" : "+r"(value));
                marker_x_argument = value >> 16;
                asm volatile("" : "+r"(frame_table_argument), "+r"(animation_table_argument),
                                   "+r"(animation_id_argument), "+r"(marker_x_argument));
                gPlayerSelectionTeamMarkerSprites[(sprite_slot_offset = visible_row << 2) >> 2] =
                    CreateSprite(frame_table_argument, animation_table_argument, animation_id_argument, marker_x_argument);
            }
            outgoing[0] = marker_y_pixels;
            outgoing[1] = PLAYER_SELECTION_NEW_MARKER_TILE_OFFSET;
            outgoing[2] = palette_bank;
            outgoing[3] = PLAYER_SELECTION_NEW_MARKER_FLAGS;
            {
                register s32 value asm("r4") = zero;

                outgoing[4] = value;
            }
            {
                register s32 frame_table_argument asm("r0") = PLAYER_SELECTION_NEW_MARKER_FRAMES_ROM;
                register s32 animation_table_argument asm("r1") = PLAYER_SELECTION_NEW_MARKER_ANIMATIONS_ROM;
                register s32 animation_id_argument asm("r2") = 0;
                register s32 marker_x_argument asm("r3");
                register s32 value asm("r4") = new_marker_x_shifted16;

                asm volatile("" : "+r"(value));
                marker_x_argument = value >> 16;
                asm volatile("" : "+r"(frame_table_argument), "+r"(animation_table_argument),
                                   "+r"(animation_id_argument), "+r"(marker_x_argument));
                *(struct PlayerSelectionSpriteFlagsView **)(sprite_slot_offset + (s32)gPlayerSelectionNewMarkerSprites) =
                    CreateSprite(frame_table_argument, animation_table_argument, animation_id_argument, marker_x_argument);
            }
            visible_row = (u8)(visible_row + 1);
        } while (visible_row < visible_row_count);
    }
}
