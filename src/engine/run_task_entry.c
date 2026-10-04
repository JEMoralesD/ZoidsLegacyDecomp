#include "m2c_prelude.h"
#include "tasks.h"
extern u8 gTaskArguments[] asm("D_03000E3C"), gTaskFunctions[] asm("D_03000E14");
void CallFunctionR1(s32, s32) asm("func_80ECD60");
void StopTask(u8) asm("func_08092E0C");
void YieldTaskForUpdates(s32) asm("func_080ED17C");
void RunTaskEntry(s32 task_slot) asm("func_08092C68");

void RunTaskEntry(s32 task_slot) {
    s32 shifted_slot = task_slot << 0x18;
    u8 *function_table = gTaskFunctions;
    s32 table_offset = ((u32)shifted_slot >> 0x16) - 4;
    s32 *function_entry = (s32 *)(function_table + table_offset), *argument_entry = (s32 *)(gTaskArguments + table_offset);
    CallFunctionR1(*argument_entry, *function_entry);
    StopTask(*(u8 *)0x03000A80);
loop_1:
    YieldTaskForUpdates(1);
    goto loop_1;
}
void RunTaskSlot1(void) asm("func_08092C9C");

void RunTaskSlot1(void) { RunTaskEntry(1); }
void RunTaskSlot2(void) asm("func_08092CA8");

void RunTaskSlot2(void) { RunTaskEntry(2); }
void RunTaskSlot3(void) asm("func_08092CB4");

void RunTaskSlot3(void) { RunTaskEntry(3); }
void RunTaskSlot4(void) asm("func_08092CC0");

void RunTaskSlot4(void) { RunTaskEntry(4); }
void RunTaskSlot5(void) asm("func_08092CCC");

void RunTaskSlot5(void) { RunTaskEntry(5); }
void RunTaskSlot6(void) asm("func_08092CD8");

void RunTaskSlot6(void) { RunTaskEntry(6); }
void RunTaskSlot7(void) asm("func_08092CE4");

void RunTaskSlot7(void) { RunTaskEntry(7); }
void RunTaskSlot8(void) asm("func_08092CF0");

void RunTaskSlot8(void) { RunTaskEntry(8); }
void RunTaskSlot9(void) asm("func_08092CFC");

void RunTaskSlot9(void) { RunTaskEntry(9); }
void RunTaskSlot10(void) asm("func_08092D08");

void RunTaskSlot10(void) { RunTaskEntry(10); }
void RunTaskSlot11(void) asm("func_08092D14");

void RunTaskSlot11(void) { RunTaskEntry(11); }
