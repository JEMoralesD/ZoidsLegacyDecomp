/* Zoids Legacy (USA) - function @ 0x0809256C.
 * Resets two IWRAM longs to -1 and an IWRAM byte to 0. */

extern int gScanlineEventHead;
extern int gCurrentScanlineEvent;
extern unsigned char gScanlineEventsDirty;

/* 0x0809256C */
void InitScanlineEvents(void)
{
    gScanlineEventHead = gCurrentScanlineEvent = -1;
    gScanlineEventsDirty = 0;
}
