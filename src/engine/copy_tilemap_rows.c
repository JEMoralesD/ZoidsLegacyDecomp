#include "m2c_prelude.h"
M2C_UNK BiosCpuSet(s32, s32, u16) asm("func_080ECD2C");
void CopyTilemapRows(s32 destination, s32 source, s32 row_width, s32 row_count) asm("func_08092B44");

void CopyTilemapRows(s32 destination, s32 source, s32 row_width, s32 row_count) {
    s32 source_row;
    s32 destination_row;
    u16 width_halfwords;
    u16 rows;
    u16 row;
    destination_row = destination;
    source_row = source;
    row_width = row_width << 16;
    row_width = (u32)row_width >> 16;
    width_halfwords = row_width;
    row_count = row_count << 16;
    row_count = (u32)row_count >> 16;
    rows = row_count;
    row = 0;
    if (row < rows) do {
        BiosCpuSet(source_row, destination_row, width_halfwords);
        source_row += width_halfwords * 2;
        destination_row += 0x40;
        row++;
    } while (row < rows);
}
