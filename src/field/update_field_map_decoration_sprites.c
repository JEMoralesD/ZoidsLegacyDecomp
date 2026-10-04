#include "m2c_prelude.h"
#include "field_display.h"

extern u16 gFieldMapState asm("D_0202ECF4");
extern u8 gFieldDecorationsEnabled[] asm("D_020324B1");
extern u8 gFieldDecorationSpriteSlots[] asm("D_020324BB");
extern void *gFieldDecorationSprites[] asm("D_020324E8");
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern s16 gWorldMapDecorationPositions[] asm("D_087AFBB4");
extern s16 gAlternateWorldMapDecorationPositions[] asm("D_087AFCB0");
extern struct FieldDecorationResourcePointerView gFieldDecorationGraphicsPointers[] asm("D_087AFB54");
extern struct FieldDecorationResourcePointerView gFieldDecorationPalettePointers[] asm("D_087AFB58");

extern void QueueCopy(s32, s32, s32) asm("func_08095208");
extern void DestroySprite(void *) asm("func_08094554");
extern void *CreateSprite(s32, s32, u16, s32, s32, u16, u16, s32, s32) asm("func_08094484");

void UpdateFieldMapDecorationSprites(void) asm("func_0809D734");

void UpdateFieldMapDecorationSprites(void)
{
    s16 *decoration_position;
    u32 decoration_count;
    register u32 decoration_index asm("r9");

    if (gFieldMapState == 0) {
        decoration_position = gWorldMapDecorationPositions;
        decoration_count = FIELD_DECORATION_SLOT_COUNT;
    } else {
        decoration_position = gAlternateWorldMapDecorationPositions;
        decoration_count = FIELD_ALTERNATE_DECORATION_COUNT;
    }

    decoration_index = 0;
    if (decoration_index < decoration_count) {
        void **sprite_slots = gFieldDecorationSprites;
        for (;;) {
            s16 *next_decoration_position;
            u32 next_decoration_index;

            if (gFieldDecorationSpriteSlots[decoration_index] == 0xFF) {
                u8 decorations_enabled = gFieldDecorationsEnabled[0];
                next_decoration_position = decoration_position + 3;
                next_decoration_index = decoration_index + 1;
                if (decorations_enabled != 0 &&
                    (decoration_position[1] + 2) * 0x800 > gFieldCameraScrollOffsets[0] &&
                    (decoration_position[1] - 2) * 0x800 < gFieldCameraScrollOffsets[0] + 0xF000 &&
                    (decoration_position[2] + 2) * 0x800 > gFieldCameraScrollOffsets[1] &&
                    (decoration_position[2] - 2) * 0x800 < gFieldCameraScrollOffsets[1] + 0xA000) {
                    u8 sprite_slot;
                    u32 tile_offset;

                    sprite_slot = 0;
                    if (sprite_slots[sprite_slot] != 0) {
                        register void **sprite_slots_scan asm("r1") = gFieldDecorationSprites;
                        asm volatile("" : "+r"(sprite_slots_scan));
                        do {
                            sprite_slot++;
                            if (sprite_slot >= FIELD_DECORATION_SLOT_COUNT)
                                break;
                        } while (sprite_slots_scan[sprite_slot] != 0);
                    }

                    {
                        register s32 offset asm("r0");
                        register struct FieldDecorationResourcePointerView *base asm("r2");
                        offset = decoration_position[0] << 3;
                        asm volatile("" : "+r"(offset));
                        base = gFieldDecorationGraphicsPointers;
                        asm volatile("" : "+r"(base));
                        QueueCopy((s32)*(void **)(offset + (s32)base),
                            0x06010000 + ((sprite_slot * 16 + 0x200) << 5), 0x200);
                    }
                    {
                        register s32 offset asm("r0");
                        register struct FieldDecorationResourcePointerView *base asm("r2");
                        offset = decoration_position[0] << 3;
                        asm volatile("" : "+r"(offset));
                        base = gFieldDecorationPalettePointers;
                        asm volatile("" : "+r"(base));
                        QueueCopy((s32)*(void **)(offset + (s32)base),
                            ({
                                register s32 destination asm("r1");
                                register s32 destination_base asm("r3");
                                destination = (12 - sprite_slot) << 5;
                                destination_base = 0x05000200;
                                asm volatile("add %0, %0, %1"
                                    : "+r"(destination)
                                    : "r"(destination_base));
                                destination;
                            }), 0x20);
                    }
                    tile_offset = sprite_slot * 16 + 0x200;
                    {
                        register s32 x asm("r3");
                        register s32 y asm("r0");
                        asm volatile(
                            "mov r0, #2\n\t"
                            "ldrsh %0, [%1, r0]"
                            : "=r"(x)
                            : "r"(decoration_position)
                            : "r0");
                        x <<= 19;
                        x >>= 16;
                        asm volatile(
                            "mov r1, #4\n\t"
                            "ldrsh %0, [%1, r1]"
                            : "=r"(y)
                            : "r"(decoration_position)
                            : "r1");
                        y <<= 19;
                        y >>= 16;
                        sprite_slots[sprite_slot] = CreateSprite(0x0842558C, 0x08425598, 0,
                            x, y, tile_offset, 12 - sprite_slot, 0x13C8, 0);
                    }
                    gFieldDecorationSpriteSlots[decoration_index] = sprite_slot;
                }
            } else {
                register s16 *next_position_carrier asm("r8");
                register u32 next_index_carrier asm("r7");
                if (gFieldDecorationsEnabled[0] != 0 &&
                    (decoration_position[1] + 2) * 0x800 > gFieldCameraScrollOffsets[0] &&
                    (decoration_position[1] - 2) * 0x800 < gFieldCameraScrollOffsets[0] + 0xF000 &&
                    (decoration_position[2] + 2) * 0x800 > gFieldCameraScrollOffsets[1]) {
                    register s32 sprite_top_fixed8 asm("r0");
                    register s32 screen_bottom_fixed8 asm("r1");
                    sprite_top_fixed8 = (decoration_position[2] - 2) * 0x800;
                    screen_bottom_fixed8 = gFieldCameraScrollOffsets[1] + 0xA000;
                    asm volatile("" : "+r"(sprite_top_fixed8), "+r"(screen_bottom_fixed8));
                    asm volatile(
                        "add r3, %2, #6\n\t"
                        "mov %0, r3\n\t"
                        "mov %1, %3\n\t"
                        "add %1, #1"
                        : "=r"(next_position_carrier), "=r"(next_index_carrier)
                        : "r"(decoration_position), "r"(decoration_index)
                        : "r3");
                    if (sprite_top_fixed8 < screen_bottom_fixed8)
                        goto visible;
                }
                {
                    u8 *decoration_sprite_slot = gFieldDecorationSpriteSlots + decoration_index;
                    DestroySprite(sprite_slots[*decoration_sprite_slot]);
                    sprite_slots[*decoration_sprite_slot] = 0;
                    *decoration_sprite_slot = 0xFF;
                    asm volatile(
                        "add %2, #6\n\t"
                        "mov %0, %2\n\t"
                        "mov %1, %3\n\t"
                        "add %1, #1"
                        : "=r"(next_position_carrier), "=r"(next_index_carrier), "+r"(decoration_position)
                        : "r"(decoration_index));
                }
visible:
                next_decoration_position = next_position_carrier;
                next_decoration_index = next_index_carrier;
            }

            decoration_position = next_decoration_position;
            decoration_index = (u8)next_decoration_index;
            {
                register u32 decoration_count_carrier asm("r0") = decoration_count;
                asm volatile("" : "+r"(decoration_count_carrier));
                if (decoration_index >= decoration_count_carrier)
                    break;
            }
        }
    }
}
