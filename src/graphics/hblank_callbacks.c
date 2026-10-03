/* Zoids Legacy (USA) decompilation - first decompiled functions.
 *
 * These two live at 0x08092554..0x0809256C. NoOp is a do-nothing leaf;
 * InitHBlankCallbacks fills three consecutive IWRAM pointers with NoOp's address.
 * Compiled with agbcc -O2 they reproduce the original bytes exactly.
 */

typedef void (*HBlankCallback)(void);

/* 0x08092554 */
void NoOp(void)
{
}

/* IWRAM base treated as an array of function pointers (filled by InitHBlankCallbacks). */
extern HBlankCallback gHBlankCallbacks[];

/* 0x08092558 */
void InitHBlankCallbacks(void)
{
    gHBlankCallbacks[0] = NoOp;
    gHBlankCallbacks[1] = NoOp;
    gHBlankCallbacks[2] = NoOp;
}
