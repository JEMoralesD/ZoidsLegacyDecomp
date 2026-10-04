#include "m2c_prelude.h"
#include "../../battle/battle_display.h"
#include "../event_script.h"
#include "../../game/player_state.h"

M2C_UNK ClearSpritePools() asm("func_08094330");                            /* extern */
s32 CreateSpriteFromTable(s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094374"); /* extern */
M2C_UNK DestroySprite() asm("func_08094554");                            /* extern */
void *CreateSpriteGroup(s32, M2C_UNK, M2C_UNK) asm("func_08095098");         /* extern */
M2C_UNK RequestWindowRefresh() asm("func_080972C8");                            /* extern */
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");                     /* extern */
M2C_UNK LoadZoidBodyGraphics(u8, u8, s32, s32, s32, s32) asm("func_0809A1F8");  /* extern */
M2C_UNK LoadSceneBackgroundGraphics(u8, u8, s32, s32, s32) asm("func_0809A5B4");       /* extern */
M2C_UNK LoadSpriteGraphicsFromTable(M2C_UNK, s32, s32, s32) asm("func_0809AA64");      /* extern */
M2C_UNK SeekEventCommand(u8, s32, s32) asm("func_080A016C");                /* extern */
M2C_UNK ResetBattleAnimation(s32) asm("func_080D0AF0");                         /* extern */
M2C_UNK SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");                    /* extern */

extern u8 gEventBattleZoidModelId asm("D_020317D6");
extern u8 gEventBattleImpactFlags asm("D_020317D7");
extern u8 gEventBattlePhalanxOpcode asm("D_020317D8");
extern u8 gBattleSceneSide asm("D_02033F36");
extern u8 gBattleState[];
extern struct ZoidBaseRecordView gZoidBaseStatTable[];
extern struct BattleBodyAttachmentView gBattleBodyAttachmentOffsets[] asm("D_087EB08C");

s32 EventLoadBattleSceneModel(u32 script_slot, void **script_cursor) asm("func_080A2F68");

s32 EventLoadBattleSceneModel(u32 script_slot, void **script_cursor) {
    register u32 saved_script_slot asm("r9");
    u8 *pending_phalanx_opcode;
    s32 shield_x;
    s32 shield_enabled;
    u8 message_window_state;
    register s32 phalanx_opcode_carrier asm("r1");
    void *phalanx_group;
    void *command;
    register void *pilot_or_model_address_carrier asm("r5");

    saved_script_slot = (u8)script_slot;
    ClearSpritePools();
    *(s32 *)0x02031744 = 0;
    command = *script_cursor;
    LoadZoidBodyGraphics(M2C_FIELD(command, u8 *, EVENT_BATTLE_SCENE_MODEL_OFFSET(model_id)), M2C_FIELD(command, u8 *, EVENT_BATTLE_SCENE_MODEL_OFFSET(palette_variant)), 1, 0, (s32) M2C_FIELD(command, u8 *, EVENT_BATTLE_SCENE_MODEL_OFFSET(side)), 0x02002880);
    gBattleSceneSide = M2C_FIELD(*script_cursor, u8 *, EVENT_BATTLE_SCENE_MODEL_OFFSET(side));
    gEventBattleZoidModelId = M2C_FIELD(*script_cursor, u8 *, EVENT_BATTLE_SCENE_MODEL_OFFSET(model_id));
    {
        u8 *flags_destination = &gEventBattleImpactFlags;
        register u32 flags_test asm("r0");

        asm volatile("" : : "r"(flags_destination)
            : "r0", "r1", "r3");
        asm volatile(
            "ldr r0, [r4, #0]\n\t"
            "ldrb r1, [r0, #4]\n\t"
            "strb r1, [r2, #0]\n\t"
            "mov r0, #2\n\t"
            "and r0, r1"
            : "=r"(flags_test)
            : "r"(script_cursor)
            : "r1", "r2", "r3", "cc", "memory");
        if (flags_test == 0) {
            ResetBattleAnimation(0x10);
        } else {
            ResetBattleAnimation(0xFFFFFF00);
            SetBattleAnimationCameraMode(2, 0);
        }
    }
    {
        register u8 *state asm("r0");
        register u8 *graphics asm("r3");
        u8 state_value;
        state = (u8 *)0x0203055C;
        state_value = state[2];
        graphics = (u8 *)0x087AFCC4;
        asm volatile("" : "+r"(graphics));
        pilot_or_model_address_carrier = (void *)0x020317D6;
        LoadSceneBackgroundGraphics(state_value,
            graphics[*(u8 *)pilot_or_model_address_carrier * 0x38 + ZOID_BASE_RECORD_OFFSET(size_class)], 3, 2, 1);
    }
    shield_enabled = 1 & *(u8 *)0x020317D7;
    if (shield_enabled != 0) {
        register u8 *effect_sprite_definitions asm("r4");
        register u8 *attachment_offsets asm("r2");
        s32 shield_y;
        LoadSpriteGraphicsFromTable(0x087ACCB8, 0, 0x380, 0xE);
        effect_sprite_definitions = (u8 *)0x087ACDD8;
        if (*(u8 *)0x02033F36 == 0) {
            register u8 *coordinate_seed asm("r0") = (u8 *)0x087EB08C;
            register s32 shield_x_carrier asm("r3");
            asm volatile(
                "ldrb r1, [r5, #0]\n\t"
                "lsl r1, r1, #5\n\t"
                "add r2, r0, #4\n\t"
                "add r1, r1, r2\n\t"
                "mov r2, #0\n\t"
                "ldrsh r3, [r1, r2]\n\t"
                "add r2, r0, #0"
                : "=r"(shield_x_carrier), "=r"(attachment_offsets), "+r"(coordinate_seed)
                : "r"(pilot_or_model_address_carrier)
                : "r1", "cc", "memory");
            shield_x = shield_x_carrier;
        } else {
            register u8 *coordinate_seed asm("r2") = (u8 *)0x087EB08C;
            register s32 shield_x_carrier asm("r3");
            asm volatile(
                "ldrb r0, [r5, #0]\n\t"
                "lsl r0, r0, #5\n\t"
                "add r1, r2, #4\n\t"
                "add r0, r0, r1\n\t"
                "ldrh r1, [r0, #0]\n\t"
                "mov r3, #128\n\t"
                "lsl r3, r3, #1\n\t"
                "add r0, r3, #0\n\t"
                "sub r0, r0, r1\n\t"
                "lsl r0, r0, #16\n\t"
                "asr r3, r0, #16"
                : "=r"(shield_x_carrier), "+r"(coordinate_seed)
                : "r"(pilot_or_model_address_carrier)
                : "r0", "r1", "cc", "memory");
            shield_x = shield_x_carrier;
            attachment_offsets = coordinate_seed;
        }
        {
            register u32 y_carrier asm("r0") = 0x020317D6;
            asm volatile(
                "ldrb r0, [r0, #0]\n\t"
                "lsl r0, r0, #5\n\t"
                "add r1, r2, #6\n\t"
                "add r0, r0, r1\n\t"
                "mov r1, #0\n\t"
                "ldrsh r0, [r0, r1]"
                : "+r"(y_carrier)
                : "r"(attachment_offsets)
                : "r1", "cc", "memory");
            shield_y = (s32)y_carrier;
        }
        *(s32 *)0x02033F4C = CreateSpriteFromTable(
            (s32)effect_sprite_definitions, 0x58, 0, shield_x,
            shield_y,
            0x380, 0xE,
            ({
                register s32 script asm("r0");
                if (*(u8 *)0x02033F36 != 0) {
                    script = 0x9248;
                } else {
                    script = 0x1248;
                }
                asm volatile("" : "+r"(script));
                script;
            }),
            BATTLE_SHIELD_SPRITE_VISIBILITY_CALLBACK);
    } else {
        *(s32 *)0x02033F4C = shield_enabled;
    }
    if (*(u8 *)0x02031748 == 0) {
        RunMenuScript(0x08004057);
        if (gBattleSceneSide == 0) {
            RunMenuScript(0x08004037);
        } else {
            RunMenuScript(0x08004047);
        }
    } else {
        if (*(s32 *)0x02031744 != 0) {
            DestroySprite();
            *(s32 *)0x02031744 = 0;
        }
        message_window_state = *(u8 *)0x02030666;
        if (message_window_state != 0) {
            if (message_window_state == 1) {
                RunMenuScript(0x080177F5);
            } else {
                RunMenuScript(0x080177FA);
            }
            RequestWindowRefresh();
            *(u8 *)0x02030666 = 0U;
        }
    }
    {
        register u32 pending_value asm("r2");
        pending_phalanx_opcode = (u8 *)0x020317D8;
        asm volatile("" : "+r"(pending_phalanx_opcode));
        phalanx_opcode_carrier = *pending_phalanx_opcode;
        pending_value = phalanx_opcode_carrier;
        asm volatile("" : "+r"(pending_value));
        if (pending_value != 0) {
            register u8 *work asm("r5") = (u8 *)0x02034B4C;
            register u8 *side asm("r6") = (u8 *)0x02033F36;
            register u8 *destination asm("r0");
            register u8 *coordinate_base asm("r3");
            register u8 *coordinate_index asm("r4");
            register void *object asm("r0");
            register s32 zero asm("r8");

            asm volatile("" : "+r"(work), "+r"(side));
            destination = (u8 *)(u32)*side;
            work += 0x27BE;
            asm volatile(
                "add r0, r0, r5"
                : "+r"(destination)
                : "r"(work)
                : "cc");
            phalanx_opcode_carrier += 0xCC;
            asm volatile(
                "mov r3, #0\n\t"
                "mov r8, r3"
                : "=r"(zero)
                :
                : "r3");
            *destination = phalanx_opcode_carrier;
            phalanx_group = CreateSpriteGroup(0, 0x080CCF3D, 0x080CCF61);
            *(void **)0x020316F8 = phalanx_group;
            object = phalanx_group;
            coordinate_base = (u8 *)0x087EB08C;
            coordinate_index = (u8 *)0x020317D6;
            asm volatile(
                "ldrb r1, [r4, #0]\n\t"
                "lsl r1, r1, #5\n\t"
                "add r2, r3, #0\n\t"
                "add r2, #16\n\t"
                "add r1, r1, r2\n\t"
                "mov r2, #0\n\t"
                "ldrsh r1, [r1, r2]\n\t"
                "str r1, [r0, #4]\n\t"
                "ldrb r1, [r4, #0]\n\t"
                "lsl r1, r1, #5\n\t"
                "add r3, #18\n\t"
                "add r1, r1, r3\n\t"
                "mov r3, #0\n\t"
                "ldrsh r1, [r1, r3]\n\t"
                "str r1, [r0, #8]"
                : "+r"(object), "+r"(coordinate_base),
                  "+r"(coordinate_index)
                :
                : "r1", "r2", "cc", "memory");
            object = (u8 *)object + 0x8C;
            asm volatile(
                "ldrb r1, [r6, #0]\n\t"
                "add r1, r1, r5\n\t"
                "ldrb r1, [r1, #0]\n\t"
                "sub r1, #46\n\t"
                "str r1, [r0, #0]"
                : "+r"(object)
                : "r"(side), "r"(work)
                : "r1", "cc", "memory");
            *pending_phalanx_opcode = zero;
        } else {
            register u8 *work asm("r0") = (u8 *)0x02034B4C;
            register u8 *side asm("r1") = (u8 *)0x02033F36;
            register u32 work_offset asm("r3") = 0x27BE;

            asm volatile("" : "+r"(work), "+r"(side),
                "+r"(work_offset));
            work += work_offset;
            work += *side;
            *work = (u8)pending_value;
        }
    }
    {
        s32 neg_one = -1;
        asm volatile("" : "+r"(neg_one));
        SeekEventCommand(saved_script_slot, neg_one, 0);
    }
    return 0;
}
