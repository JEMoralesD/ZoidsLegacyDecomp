#include "m2c_prelude.h"

extern void func_080EB084(void);
extern void CopyShadowOamToHardware(void) asm("func_080922A4");
extern void UpdatePerspectiveScanlineRegisters(u8) asm("func_08093D9C");
extern void UpdateDisplayWindowRegisters(u8) asm("func_08096080");
extern void UpdateBattleScanlineWindowDisplay(u8) asm("func_080D2050");
extern void UpdateSceneBg0ScanlineScroll(u8) asm("func_080A7860");
extern void UpdateFieldBg3ScanlineEvent(u8) asm("func_0809EB68");
extern void UpdateSceneBg1ScanlineScroll(u8) asm("func_080A8ED4");
extern void ArmScanlineEvents(void) asm("func_08092348");
extern void RunDeferredCallbacks(void) asm("func_0809290C");
extern void ProcessTransferQueue(void) asm("func_080952AC");
extern s32 CallFunctionR0(s32) asm("func_080ECD5C");
extern void UpdateScreenTransitionRegisters(u8) asm("func_08096DB0");
extern void UpdateLinkDriverPackets(void) asm("func_08092834");
extern void UpdateAudioAndVSyncState(void) asm("func_08092FA0");
extern void VBlankNoOp(void) asm("func_080923E0");
extern void CompleteVBlankUpdate(void) asm("func_080923E4");

void HandleVBlank(void) asm("func_08092410");

void HandleVBlank(void)
{
    u8 active;
    u8 *flags;

    active = *(u8 *)0x0300067C & 1;
    func_080EB084();
    CopyShadowOamToHardware();

    flags = (u8 *)0x03000074;
    if (*flags != 0) {
        if (*flags & 1)
            UpdatePerspectiveScanlineRegisters(active);
        if (*(u8 *)0x03000074 & 2)
            UpdateDisplayWindowRegisters(active);
        if (*(u8 *)0x03000074 & 4)
            UpdateBattleScanlineWindowDisplay(active);
        if (*(u8 *)0x03000074 & 8)
            UpdateSceneBg0ScanlineScroll(active);
        if (*(u8 *)0x03000074 & 0x10)
            UpdateFieldBg3ScanlineEvent(active);
        if (*(u8 *)0x03000074 & 0x20)
            UpdateSceneBg1ScanlineScroll(active);
    }

    ArmScanlineEvents();
    if (*(u8 *)0x0300067C != 0)
        RunDeferredCallbacks();

    if (active != 0) {
        register volatile u16 *display asm("r2") = (volatile u16 *)0x04000010;
        register s32 *state asm("r1") = (s32 *)0x03000054;
        *display++ = state[0] >> 8;
        *display++ = state[1] >> 8;
        *display++ = state[2] >> 8;
        *display++ = state[3] >> 8;
        *display++ = state[4] >> 8;
        *display++ = state[5] >> 8;
        *display++ = state[6] >> 8;
        *display++ = state[7] >> 8;

        {
            register volatile u16 *display asm("r1") = (volatile u16 *)0x04000000;
            *display = *(u16 *)0x0300004C;
            display += 0x28;
            *display++ = *(u16 *)0x0300004E;
            *display++ = *(u16 *)0x03000050;
            *display = *(u16 *)0x03000052;
        }
        ProcessTransferQueue();
        CallFunctionR0(*(s32 *)0x03000010);
    }

    UpdateScreenTransitionRegisters(active);
    if (active == 0) {
        UpdateLinkDriverPackets();
        UpdateAudioAndVSyncState();
    }
    VBlankNoOp();
    CompleteVBlankUpdate();
}
