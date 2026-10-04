/* Zoids Legacy (USA) - function @ 0x080925A4.  MATCHING.
 * Sorted doubly-linked-list insert (descending key), walking the prev chain
 * from the head. -1 is the end sentinel. Matching required forcing the agbcc
 * register allocation (explicit `register ... asm("rN")`), a goto-rotated loop
 * to reproduce the block layout, and materializing -1 before caching the head
 * pointer so the `adds r5,r2,#0` schedules after `negs`. */

typedef unsigned char u8;
struct ScanlineEvent { u8 scanline; u8 pad[7]; struct ScanlineEvent *next; struct ScanlineEvent *prev; };

extern struct ScanlineEvent *gScanlineEventHead;   /* 0x03000880 */
extern unsigned char gScanlineEventsDirty; /* 0x03000888 */

void InsertScanlineEvent(struct ScanlineEvent *event)
{
    register struct ScanlineEvent *inserted_event asm("r3") = event;
    register struct ScanlineEvent **head_address asm("r2") = &gScanlineEventHead;
    register struct ScanlineEvent *current_event asm("r1") = *head_address;
    register struct ScanlineEvent *list_end asm("r0") = (struct ScanlineEvent *)-1;
    register struct ScanlineEvent **cached_head asm("r5") = head_address;
    register u8 scanline asm("r4");
    register struct ScanlineEvent *previous_event asm("r2");

    if (current_event != list_end) {
        scanline = inserted_event->scanline;
        goto compare_scanline;
    advance:
        current_event = previous_event;
    compare_scanline:
        if (current_event->scanline <= scanline) goto follow_previous;
        inserted_event->next = current_event->next;
        current_event->next = inserted_event;
        inserted_event->prev = current_event;
        if (*cached_head == current_event) *cached_head = inserted_event;
        goto done;
    follow_previous:
        previous_event = current_event->prev;
        if (previous_event != (struct ScanlineEvent *)-1) goto advance;
        current_event->prev = inserted_event;
        inserted_event->next = current_event;
        inserted_event->prev = previous_event;
    } else {
        *head_address = inserted_event;
        inserted_event->prev = current_event;
        inserted_event->next = current_event;
        gScanlineEventsDirty = 1;
    }
done: ;
}
