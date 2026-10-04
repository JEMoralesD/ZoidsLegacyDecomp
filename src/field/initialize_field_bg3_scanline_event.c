#include "m2c_prelude.h"
#include "field_display.h"

extern struct FieldBg3ScanlineEventView gFieldBg3ScanlineEvent asm("D_020314A4");
extern u8 gDisplayUpdateFlags asm("D_03000074");
extern s8 gFieldBg3ScanlineState asm("D_020324B8");
extern void InsertScanlineEvent(void) asm("func_80925A4");

void InitializeFieldBg3ScanlineEvent(void) asm("func_0809EAE0");

void InitializeFieldBg3ScanlineEvent(void) {
    register s32 scanline_or_flag_mask asm("r1");
    struct FieldBg3ScanlineEventView *scanline_event;
    s32 zero;
    s32 event_flags;
    u8 display_flags;
    scanline_event = &gFieldBg3ScanlineEvent;
    zero = 0;
    scanline_or_flag_mask = 0x5F;
    scanline_event->scanline = scanline_or_flag_mask;
    event_flags = scanline_event->flags;
    scanline_or_flag_mask -= 0x61;
    scanline_or_flag_mask &= event_flags;
    scanline_event->flags = scanline_or_flag_mask;
    scanline_event->reserved02 = zero;
    scanline_event->callback = 0x0809EA8D;
    scanline_event->previous = zero;
    scanline_event->next = zero;
    InsertScanlineEvent();
    display_flags = gDisplayUpdateFlags;
    display_flags |= 0x10;
    gDisplayUpdateFlags = display_flags;
    gFieldBg3ScanlineState = FIELD_BG3_SCANLINE_READY;
}
