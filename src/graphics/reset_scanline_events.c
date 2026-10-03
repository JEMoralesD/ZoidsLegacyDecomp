/* Zoids Legacy (USA) - function @ 0x0809258C.
 * Resets an IWRAM long to -1 and sets an IWRAM flag byte to 1. */

extern int gScanlineEventHead;
extern unsigned char gScanlineEventsDirty;

/* 0x0809258C */
void ResetScanlineEvents(void)
{
    gScanlineEventHead = -1;
    gScanlineEventsDirty = 1;
}
