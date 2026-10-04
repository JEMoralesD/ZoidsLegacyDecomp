#include "m2c_prelude.h"
#include "../link_battle.h"
extern u8 gLinkBattleStartLocalDecision asm("D_02032E57");
extern u8 gLinkBattleStartReceiveComplete asm("D_02032E56");
extern u8 gLinkBattleStartSendComplete asm("D_02032E55");
extern u8 gLinkBattleStartHandshakePhase asm("D_02032E54");
#include "../../graphics/screen_effects.h"
#include "../game_state.h"

M2C_UNK StartTask(s32, M2C_UNK) asm("func_08092D8C");                /* extern */
M2C_UNK StopTask(s32) asm("func_08092E0C");                         /* extern */
M2C_UNK PlayOrContinueSong(s32) asm("func_08092E74");                         /* extern */
M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
M2C_UNK ClearSpritePools() asm("func_08094330");                            /* extern */
s32 CreateSprite(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484"); /* extern */
M2C_UNK DestroySprite(s32) asm("func_08094554");                         /* extern */
M2C_UNK StartScreenTransition(s32, s32) asm("func_08096308");                    /* extern */
s32 IsScreenTransitionComplete() asm("func_0809669C");                                /* extern */
M2C_UNK InitializeWindowGraphics(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08096FBC"); /* extern */
void *GetWindow(s32) asm("func_0809716C");                           /* extern */
M2C_UNK RequestWindowRefresh() asm("func_080972C8");                            /* extern */
M2C_UNK PrintWindowTextAt(M2C_UNK, s32, s32, s32, s32) asm("func_080981F0"); /* extern */
M2C_UNK PrintWindowText(s32, s32, s32) asm("func_08098248");               /* extern */
M2C_UNK PrintWindowNumberAt(u32, s32, s32, s32, s32, s32, s32) asm("func_0809844C"); /* extern */
M2C_UNK ClearWindow(s32) asm("func_080986B4");                         /* extern */
M2C_UNK RunMenuScript(s32) asm("func_08098BB4");                         /* extern */
M2C_UNK LoadMenuGradientBackground(s32, s32, M2C_UNK, s32, s32) asm("func_0809AB44"); /* extern */
M2C_UNK ResetLinkTransferState() asm("func_0809AC98");                            /* extern */
s32 TryStartLinkConnection() asm("func_0809AE38");                                /* extern */
M2C_UNK StopLinkConnection() asm("func_0809AEA0");                            /* extern */
M2C_UNK BeginLinkSend(s32 *, s32, s32) asm("func_0809AEC0");             /* extern */
u8 PollLinkSend() asm("func_0809AEF4");                                 /* extern */
M2C_UNK BeginLinkReceive(s32 *, s32, s32) asm("func_0809B00C");             /* extern */
u8 PollLinkReceive() asm("func_0809B040");                                 /* extern */
M2C_UNK RunPlayerEquipmentMenu() asm("func_080B0D38");                            /* extern */
s32 RunPlayerPilotAssignmentMenu() asm("func_080B2108");                                /* extern */
M2C_UNK RunPlayerTeamFormationMenu(s32) asm("func_080B35D4");                         /* extern */
M2C_UNK ResetAllBattleEffects() asm("func_080BE600");                            /* extern */
M2C_UNK DrawEnabledBattleRules() asm("func_080E0F44");                            /* extern */
M2C_UNK BuildBattleRuleToggleTextList() asm("func_080E125C");                            /* extern */
s32 GetPlayerPartyScore() asm("func_080E13C4");                                /* extern */
s32 DoesPlayerTeamMeetBattleRule(u32) asm("func_080E60B0");                             /* extern */
M2C_UNK EnableBattleRule(u8) asm("func_080E65B4");                          /* extern */
M2C_UNK DisableBattleRule(s32) asm("func_080E664C");                         /* extern */
s32 TestBattleRuleFlag(u8) asm("func_080E6664");                              /* extern */
M2C_UNK InitializePlayerBattleUnits() asm("func_080E9128");                            /* extern */
M2C_UNK ExchangeLinkBattleUnitRecords() asm("func_080E96E8");                            /* extern */
M2C_UNK BiosLz77ToVram(M2C_UNK, M2C_UNK) asm("func_080ECD34");            /* extern */
s32 CallFunctionR0(s32) asm("func_080ECD5C");                             /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */
M2C_UNK jtbl_080E1DC4();                            /* static */

void RunLinkBattle(void) {
    u16 sp18;
    u32 sp1C;
    u32 sp20[2];
    u16 *fixed_r6;
    register u8 *output asm("r2");
    register u8 *lookup asm("r1");
    register s32 *menu_table asm("r1");
    u8 output_value;
    u8 *selection_dst;
    s32 zero;
    s32 temp_r0;
    s32 temp_r7;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r2;
    s32 var_r4_2;
    s32 var_r5;
    s32 var_r7;
    u16 var_r4;
    u32 var_r4_3;
    u8 temp_r0_2;
    u8 temp_r0_3;
    u8 temp_r0_4;
    u8 temp_r0_5;
    u8 temp_r0_6;
    u8 temp_r0_7;
    u8 temp_r2;
    u8 init_flag;
    u8 temp_r4;
    u8 temp_r4_2;
    u8 temp_r4_3;
    u8 temp_r4_4;
    u8 temp_r4_5;
    u8 temp_r4_6;
    u8 temp_r5;
    u8 temp_r5_2;
    u8 temp_r5_3;
    u8 temp_r5_4;
    u8 temp_r5_5;
    u8 temp_r5_6;
    u8 var_r0_3;
    u8 var_r9;

    *(s16 *)0x0300004C = 0x1840;
    InitializeWindowGraphics(3, 1, 0, 0x35C, 0x3C0, 0, 0xE, 0, 0x3E6, 0xF);
    BiosLz77ToVram(0x081046A8, 0x06015840);
    LoadMenuGradientBackground(2, 0, 0x1EF, 4, 4);
    ClearSpritePools();
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_REVEAL, 0x10);
    RunMenuScript(0x08028199);
    PrintWindowTextAt(0x02021774, 0, 2, 0, 0);
    fixed_r6 = (u16 *)0x0202F08C;
    PrintWindowNumberAt(fixed_r6[0], 5, 0, 2, 3, 0, 0);
    PrintWindowNumberAt(fixed_r6[1], 5, 0, 2, 3, 7, 0);
    temp_r0 = GetPlayerPartyScore();
    sp20[0] = temp_r0;
    PrintWindowNumberAt(temp_r0, 0xA, 0, 2, 6, 3, 0);
    RequestWindowRefresh();
    temp_r7 = CreateSprite(0x08105AFC, 0x08105B08, 0, 0xA8, 0x30, 0x350, 0xF, 8, 0);
    while ((IsScreenTransitionComplete() << 0x18) == 0) {
        YieldTaskForUpdates(1);
    }
    ResetLinkTransferState();
    while ((TryStartLinkConnection() << 0x18) == 0) {
        if (2 & *(u16 *)0x0300000E) {
            PlaySong(0x3F);
            StopLinkConnection();
            DestroySprite(temp_r7);
            RunMenuScript(0x08028250);
            StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0);
            while ((IsScreenTransitionComplete() << 0x18) == 0) {
                YieldTaskForUpdates(1);
            }
            *(s32 *)0x02021690 = GAME_MODE_OPTIONS;
            return;
        }
        YieldTaskForUpdates(1);
    }
        {
        register s32 left_ready asm("r5");
        register s32 right_ready asm("r4");

        BeginLinkSend((s32 *)0, 0, 0x081091FC);
        BeginLinkReceive((s32 *)0, 0, 0x081091FC);
        do {
loop_13:
            PollLinkSend();
            asm volatile("lsl r0, r0, #24\n\tlsr %0, r0, #24"
                         : "=r"(left_ready) : : "r0");
            PollLinkReceive();
            asm volatile("lsl r0, r0, #24\n\tlsr %0, r0, #24"
                         : "=r"(right_ready) : : "r0");
            YieldTaskForUpdates(1);
            if (left_ready == 0) {
                goto loop_13;
            }
        } while (right_ready == 0);
        BeginLinkSend((s32 *)0x02021774, 0x12, 0x0810920C);
        BeginLinkReceive((s32 *)0x02030564, 0x12, 0x0810920C);
        do {
loop_16:
            PollLinkSend();
            asm volatile("lsl r0, r0, #24\n\tlsr %0, r0, #24"
                         : "=r"(left_ready) : : "r0");
            PollLinkReceive();
            asm volatile("lsl r0, r0, #24\n\tlsr %0, r0, #24"
                         : "=r"(right_ready) : : "r0");
            YieldTaskForUpdates(1);
            if (left_ready == 0) {
                goto loop_16;
            }
        } while (right_ready == 0);
        BeginLinkSend((s32 *)0x0202F08C, 4, 0x08109218);
        BeginLinkReceive(&sp1C, 4, 0x08109218);
        do {
loop_19:
            PollLinkSend();
            asm volatile("lsl r0, r0, #24\n\tlsr %0, r0, #24"
                         : "=r"(left_ready) : : "r0");
            PollLinkReceive();
            asm volatile("lsl r0, r0, #24\n\tlsr %0, r0, #24"
                         : "=r"(right_ready) : : "r0");
            YieldTaskForUpdates(1);
            if (left_ready == 0) {
                goto loop_19;
            }
        } while (right_ready == 0);
        BeginLinkSend((s32 *)&sp20[0], 4, 0x08109220);
        BeginLinkReceive((s32 *)&sp20[1], 4, 0x08109220);
        do {
loop_22:
            PollLinkSend();
            asm volatile("lsl r0, r0, #24\n\tlsr %0, r0, #24"
                         : "=r"(left_ready) : : "r0");
            PollLinkReceive();
            asm volatile("lsl r0, r0, #24\n\tlsr %0, r0, #24"
                         : "=r"(right_ready) : : "r0");
            YieldTaskForUpdates(1);
            if (left_ready == 0) {
                goto loop_22;
            }
        } while (right_ready == 0);
        }
        RunMenuScript(0x080282AA);
        PrintWindowTextAt(0x02030564, 0, 4, 0, 0);
        PrintWindowNumberAt(((u16 *)&sp1C)[0], 5, 0, 2, 5, 0, 0);
        PrintWindowNumberAt(((u16 *)&sp1C)[1], 5, 0, 2, 5, 7, 0);
        PrintWindowNumberAt(sp20[1], 0xA, 0, 2, 7, 3, 0);
        RequestWindowRefresh();
        DestroySprite(temp_r7);
        var_r4 = 0;
        do {
            YieldTaskForUpdates(1);
            var_r4 += 1;
        } while ((u32) var_r4 <= 0x12BU);
        StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0x10);
        while ((IsScreenTransitionComplete() << 0x18) == 0) {
            YieldTaskForUpdates(1);
        }
        RunMenuScript(0x08028378);
        StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_REVEAL, 0x10);
        var_r5 = 0;
        var_r7 = 0;
loop_31:
        var_r4_2 = 0x1400;
        switch (var_r5) {                           /* switch 1; irregular */
        case 0x0:                                   /* switch 1 */
            init_flag = *(u8 *)0x0300603C;
            var_r5 = 0x2000;
            if (init_flag == 0) {

            } else {
                var_r5 = 0x1000;
            }
            break;
        case 0x1000:                                /* switch 1 */
            RunMenuScript(0x0802843B);
            DrawEnabledBattleRules();
            M2C_FIELD(&sp18, u8 *, 0) = 0U;
            M2C_FIELD(&sp18, u8 *, 1) = (u8) ((u32) (CallFunctionR0(*(s32 *)0x03000010) * 0xE) >> 0xF);
            menu_table = (s32 *)0x087A3624;
            asm volatile("" : "+r"(menu_table));
            PrintWindowText(*(s32 *)(M2C_FIELD(&sp18, u8 *, 0) * 4 + (u32)menu_table), 0, 7);
            var_r5 = 0x1010;
            break;
        case 0x1010:                                /* switch 1 */
            RunMenuScript(0x080284B4);
            temp_r0_3 = *(u8 *)0x0200A880;
            switch (temp_r0_3) {                    /* switch 3; irregular */
            case 0:                                 /* switch 3 */
                var_r5 = 0x1100;
                break;
            case 1:                                 /* switch 3 */
                var_r5 = 0x1600;
                break;
            case 2:                                 /* switch 3 */
                var_r5 = 0x1F00;
                break;
            }
            break;
        case 0x1100:                                /* switch 1 */
            var_r0_2 = 0x080284BC;
            goto block_149;
        case 0x1110:                                /* switch 1 */
            RunMenuScript(0x0802853E);
            if (*(u8 *)0x0200A882 == 1) {
                temp_r0_2 = *(u8 *)0x0200A880;
                switch (temp_r0_2) {                /* switch 4; irregular */
                case 0:                             /* switch 4 */
                    var_r5 = 0x1200;
                    break;
                case 1:                             /* switch 4 */
                    var_r5 = 0x1300;
                    break;
                case 3:                             /* switch 4 */
                    var_r5 = 0x1500;
                    break;
                case 2:                             /* switch 4 */
block_115:
                    var_r5 = var_r4_2;
                    break;
                }
            } else {
                var_r0 = 0x08028542;
                goto block_156;
            }
            break;
        case 0x1200:                                /* switch 1 */
                RunMenuScript(0x08028545);
                var_r5 = 0x1210;
                break;
        case 0x1210:                                /* switch 1 */
                RunMenuScript(0x08028755);
                if (*(u8 *)0x0200A882 != 1) {
                    goto block_148;
                }
                temp_r0_4 = *(u8 *)0x0200A880;
                if ((u32) temp_r0_4 <= 3U) {
                    var_r0_3 = temp_r0_4 + 1;
                    goto block_145;
                } else {
                    *(s32 *)0x0202F090 &= -0x1F;
                    goto block_147;
                }
                break;
        case 0x1300:                                /* switch 1 */
                RunMenuScript(0x080285CA);
                var_r5 = 0x1310;
                break;
        case 0x1310: {                              /* switch 1 */
                register s32 state_1110 asm("r6");

                state_1110 = 0x1110;
                asm volatile("" : "+r"(state_1110));
                RunMenuScript(0x08028755);
                if (*(u8 *)0x0200A882 == 1) {
                    temp_r0_5 = *(u8 *)0x0200A880;
                    if ((u32) temp_r0_5 <= 5U) {
                        var_r0_3 = temp_r0_5 + 5;
                        goto block_145;
                    }
                    *(s32 *)0x0202F090 &= 0xFFFFF81F;
                    goto block_147;
                }
                var_r0_2 = 0x08028759;
                RunMenuScript(var_r0_2);
                asm volatile("" ::: "memory");
                var_r5 = state_1110;
                break;
        }
        case 0x1400:                                /* switch 1 */
            RunMenuScript(0x0802869D);
            var_r5 = 0x1410;
            break;
        case 0x1410:                                /* switch 1 */
            RunMenuScript(0x08028755);
            if (*(u8 *)0x0200A882 == 1) {
                temp_r0_6 = *(u8 *)0x0200A880;
                if ((u32) temp_r0_6 <= 4U) {
                    var_r0_3 = temp_r0_6 + 0xB;
                    goto block_145;
                }
                *(s32 *)0x0202F090 &= 0xFFFE07FF;
                goto block_147;
            }
            goto block_148;
        case 0x1500:                                /* switch 1 */
            RunMenuScript(0x0802874D);
            var_r5 = 0x1510;
            break;
        case 0x1510:                                /* switch 1 */
            BuildBattleRuleToggleTextList();
            RunMenuScript(0x08028755);
            if (*(u8 *)0x0200A882 == 1) {
                if ((TestBattleRuleFlag((u8) (*(u8 *)0x0200A880 + 0x11)) << 0x18) == 0) {
                    var_r0_3 = *(u8 *)0x0200A880 + 0x11;
block_145:
                    EnableBattleRule(var_r0_3);
                } else {
                    DisableBattleRule(*(u8 *)0x0200A880 + 0x11);
                }
block_147:
                DrawEnabledBattleRules();
                break;
            }
block_148:
            var_r0_2 = 0x08028759;
block_149:
            RunMenuScript(var_r0_2);
            var_r5 = 0x1110;
            break;
        case 0x1600:                                /* switch 1 */
            RunMenuScript(0x0802875C);
            RunMenuScript(0x08028871);
            if (*(u8 *)0x0200A882 == 1) {
                selection_dst = (u8 *)&sp18;
                asm volatile("" : "+r"(selection_dst));
                temp_r2 = *(u8 *)0x0200A880;
                selection_dst[0] = temp_r2;
                asm volatile("" : "+r"(temp_r2));
                if ((temp_r2 << 0x18) == 0) {
                    M2C_FIELD(&sp18, u8 *, 1) = (u8) ((u32) (CallFunctionR0(*(s32 *)0x03000010) * 0xE) >> 0xF);
                } else {
                    M2C_FIELD(&sp18, u8 *, 1) = (u8) (temp_r2 - 1);
                }
                ClearWindow(7);
                menu_table = (s32 *)0x087A3624;
                asm volatile("" : "+r"(menu_table));
                PrintWindowText(*(s32 *)(M2C_FIELD(&sp18, u8 *, 0) * 4 + (u32)menu_table), 0, 7);
            }
            var_r0 = 0x08028875;
block_156:
            RunMenuScript(var_r0);
            var_r5 = 0x1010;
            break;
        case 0x1F00: {                              /* switch 1 */
            register s32 receive_ready asm("r5");

            RunMenuScript(0x080284B8);
            BeginLinkSend((s32 *)0x0202F090, 4, 0x0810922C);
            do {
                PollLinkSend();
                asm volatile("lsl r0, r0, #24\n\tlsr %0, r0, #24"
                             : "=r"(receive_ready) : : "r0");
                YieldTaskForUpdates(1);
            } while (receive_ready == 0);
            BeginLinkSend(&sp18, 2, 0x08109238);
            do {
                PollLinkSend();
                asm volatile("lsl r0, r0, #24\n\tlsr %0, r0, #24"
                             : "=r"(receive_ready) : : "r0");
                YieldTaskForUpdates(1);
            } while (receive_ready == 0);
            var_r5 = 0x3000;
            break;
        }
        case 0x2000: {                              /* switch 1 */
            register s32 send_ready asm("r4");

            RunMenuScript(0x08028983);
            BeginLinkReceive((s32 *)0x0202F090, 4, 0x0810922C);
            do {
                PollLinkReceive();
                asm volatile("lsl r0, r0, #24\n\tlsr %0, r0, #24"
                             : "=r"(send_ready) : : "r0");
                YieldTaskForUpdates(1);
            } while (send_ready == 0);
            BeginLinkReceive(&sp18, 2, 0x08109238);
            do {
                PollLinkReceive();
                asm volatile("lsl r0, r0, #24\n\tlsr %0, r0, #24"
                             : "=r"(send_ready) : : "r0");
                YieldTaskForUpdates(1);
            } while (send_ready == 0);
            RunMenuScript(0x080289DE);
            DrawEnabledBattleRules();
            menu_table = (s32 *)0x087A3624;
            asm volatile("" : "+r"(menu_table));
            PrintWindowText(*(s32 *)(M2C_FIELD(&sp18, u8 *, 0) * 4 + (u32)menu_table), 0, 7);
            var_r5 = 0x3000;
            break;
        }
        case 0x3000:                                /* switch 1 */
            M2C_FIELD((void *)0x0203055C, s8 *, 0) = 1;
            RunMenuScript(0x08028A0F);
            StartTask(2, LINK_BATTLE_START_HANDSHAKE_TASK);
            var_r5 = 0x3020;
            break;
        case 0x3010:                                /* switch 1 */
            RunMenuScript(0x08028A69);
            DrawEnabledBattleRules();
            menu_table = (s32 *)0x087A3624;
            asm volatile("" : "+r"(menu_table));
            PrintWindowText(*(s32 *)(M2C_FIELD(&sp18, u8 *, 0) * 4 + (u32)menu_table), 0, 7);
            M2C_FIELD(GetWindow(0), u8 *, 0x16) = var_r9;
            var_r5 = 0x3020;
            break;
        case 0x3020:                                /* switch 1 */
            PrintWindowNumberAt(GetPlayerPartyScore(), 0xA, 0, 2, 2, 4, 0);
            RunMenuScript(0x08028AF1);
            temp_r0_7 = *(u8 *)0x0200A880;
            var_r9 = temp_r0_7;
            switch ((u32) temp_r0_7) {              /* switch 5; irregular */
            case 0:                                 /* switch 5 */
                RunMenuScript(0x08028AF5);
                RunPlayerEquipmentMenu();
                var_r5 = 0x3010;
                break;
            case 1:                                 /* switch 5 */
                RunMenuScript(0x08028AF5);
                if (RunPlayerPilotAssignmentMenu() == 2) {
                    RequestWindowRefresh();
                    YieldTaskForUpdates(1);
                    RunPlayerTeamFormationMenu(0);
                }
                var_r5 = 0x3010;
                break;
            case 2:                                 /* switch 5 */
                RunMenuScript(0x08028AF5);
                RunPlayerTeamFormationMenu(0);
                var_r5 = 0x3010;
                break;
            case 3:                                 /* switch 5 */
                for (var_r4_3 = 1; var_r4_3 <= 0x10U;
                     var_r4_3 = (u32)(u8)(var_r4_3 + 1)) {
                    if (((TestBattleRuleFlag((u8) var_r4_3) << 0x18) != 0) &&
                        ((DoesPlayerTeamMeetBattleRule(var_r4_3) << 0x18) == 0)) {
                        PlaySong(0x58);
                        RunMenuScript(0x08028AFE);
                        break;
                    }
                }
                if (var_r4_3 == 0x11) {
                    *(s8 *)&gLinkBattleStartLocalDecision = LINK_BATTLE_START_READY;
                    RunMenuScript(0x08028B33);
                    var_r5 = 0x7000;
                }
                break;
            case 4:                                 /* switch 5 */
                *(s8 *)&gLinkBattleStartLocalDecision = LINK_BATTLE_START_CANCEL;
                var_r5 = 0x7000;
                break;
            }
            break;
        case 0x7000:                                /* switch 1 */
            *(s8 *)&gLinkBattleStartHandshakePhase = LINK_BATTLE_START_SEND_DECISION;
            while (gLinkBattleStartSendComplete == 0 ||
                   gLinkBattleStartReceiveComplete == 0) {
                YieldTaskForUpdates(1);
            }
            RunMenuScript(0x08028B7E);
            StopTask(2);
            var_r7 = 1;
            break;
        }
        if (var_r7 == 0) {
            goto loop_31;
        }
        ResetAllBattleEffects();
        InitializePlayerBattleUnits();
        ExchangeLinkBattleUnitRecords();
        StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0x10);
        while ((IsScreenTransitionComplete() << 0x18) == 0) {
            YieldTaskForUpdates(1);
        }
        *(s8 *)0x02032EF8 = 0x1D;
        PlayOrContinueSong(0x1D);
        output = (u8 *)0x0203055C;
        lookup = (u8 *)0x087A3614;
        asm volatile("" : "+r"(lookup));
        output_value = *(u8 *)(M2C_FIELD(&sp18, u8 *, 1) + (u32)lookup);
        zero = 0;
        output[1] = output_value;
        output[5] = zero;
        *(s32 *)0x02030558 = zero;
        *(s32 *)0x02021690 = GAME_MODE_BATTLE;
        return;
}
