#include "m2c_prelude.h"
#include "battle.h"

s32 CreateSprite(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484"); /* extern */
asm(".set func_08094484_4, func_08094484");
s32 CreateSprite_4(M2C_UNK, M2C_UNK, s32, s32) asm("func_08094484_4");       /* extern */
M2C_UNK DestroySprite(s32) asm("func_08094554");                         /* extern */
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");                     /* extern */
M2C_UNK QueuePilotPortraitGraphics(s32, s32, s32, s32, s32, s32) asm("func_0809A9C8"); /* extern */
s32 IsBattleUnitActive(u8, u32) asm("func_080E9D88");                         /* extern */
M2C_UNK jtbl_080C6858();                            /* static */
asm(".set sub_080C682C_state, 0x0203055C");
extern u8 gBattleStorySetup[] asm("sub_080C682C_state");

u8 CheckBattleResultAndPlayStoryScenes(void) asm("func_080C682C");

u8 CheckBattleResultAndPlayStoryScenes(void) {
    s16 gard_end_max_hp;
    s32 gard_end_half_max_hp;
    s32 gard_end_hp;
    s16 gard_end_scene_max_hp;
    s32 gard_end_scene_half_max_hp;
    s32 gard_end_scene_hp;
    s16 gard_scene_max_hp;
    s32 gard_scene_half_max_hp;
    s32 gard_scene_hp;
    register s32 gard_scene_sprite asm("r8");
    s32 bit_first_damage_scene_sprite;
    s32 bit_damage_200_scene_sprite;
    s32 leon_damage_200_scene_sprite;
    register u8 *gard_scene_state_tail asm("r1");
    register u8 *gard_scene_record_base asm("r2");
    register u8 *gard_scene_record asm("r1");
    register u8 *gard_scene_health_address asm("r2");
    register u8 *gard_scene_max_address asm("r0");
    register s32 gard_portrait_tile_offset asm("r6");
    register s32 gard_portrait_palette_bank asm("r4");
    register void *gard_portrait_graphics_buffer asm("r7");
    register s32 *gard_scene_outgoing_arguments asm("sp");
    register u8 *enemy_scan_state_base asm("r3");
    register u8 *gard_end_record asm("r1");
    register u32 gard_end_record_offset asm("r0");
    register u8 *gard_end_health_address asm("r2");
    register u8 *gard_end_max_address asm("r0");
    register u8 *gard_end_scene_health_address asm("r2");
    register u8 *gard_end_scene_max_address asm("r0");
    register u8 *gard_end_scene_record_base asm("r3");
    register u8 *gard_one_hp_record_base asm("r2");
    register u8 *bit_vs_stoller_state_or_player_record asm("r1");
    register u32 bit_vs_stoller_damage_offset asm("r3");
    register u8 *bit_first_damage_state asm("r0");
    register u32 bit_first_damage_offset asm("r2");
    register u8 *bit_damage_200_state asm("r0");
    register u32 bit_damage_200_offset asm("r1");
    register u8 *leon_damage_200_state asm("r0");
    register u32 leon_damage_200_offset asm("r4");
    register u8 *bit_vs_leon_player_records asm("r4");
    register u8 *leon_enemy_slot_base asm("r6");
    register u8 *bit_vs_stoller_player_records asm("r4");
    register u8 *stoller_enemy_slot_base asm("r6");
    u32 scenario_dispatch_index;
    u32 active_unit_slot;
    u32 leon_unit_slot;
    u32 stoller_unit_slot;
    u32 bit_vs_leon_player_slot;
    u32 bit_vs_stoller_player_slot;
    u8 gard_one_hp_pilot_id;
    u8 gard_end_pilot_id;
    u8 gard_end_scene_pilot_id;
    u8 gard_scene_pilot_id;
    u8 bit_vs_leon_pilot_id;
    u8 bit_vs_stoller_pilot_id;
    u8 bit_first_damage_scene_played;
    u8 bit_damage_200_scene_played;
    u8 leon_damage_200_scene_played;
    u8 unit_slot_or_side;
    register u8 *story_setup_bytes asm("r2");
    register u8 *story_setup_address asm("r1");
    u8 result_flags;
    void *leviathe_enemy_slot_base;
    void *gard_end_scene_enemy_slot_base;
    void *bit_player_slot_base;
    void *gard_one_hp_enemy_slot_base;

    story_setup_address = (u8 *) 0x0203055C;
    scenario_dispatch_index = M2C_FIELD(story_setup_address, u8 *, BATTLE_STORY_SETUP_OFFSET(story_scenario)) - 1;
    story_setup_bytes = story_setup_address;
    switch (scenario_dispatch_index) {
    case BATTLE_STORY_LEVIATHE_FIRST_DAMAGE - 1:
        unit_slot_or_side = 0;
        enemy_scan_state_base = (u8 *)0x02034B4C;
        do {
            leviathe_enemy_slot_base = (void *)((unit_slot_or_side * (s32)sizeof(struct BattleUnit)) - (0U - (u32)enemy_scan_state_base));
            if (((u32) (u8) (M2C_FIELD(leviathe_enemy_slot_base, u8 *, (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(pilot_id)) - BATTLE_STORY_LEVIATHE) <= BATTLE_STORY_LEVIATHE_VARIANT - BATTLE_STORY_LEVIATHE) && ((s32) M2C_FIELD(leviathe_enemy_slot_base, s16 *, (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(hp)) < (s32) M2C_FIELD(leviathe_enemy_slot_base, s16 *, (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(max_hp)))) {
                goto end_scripted_battle;
            }
            unit_slot_or_side += 1;
        } while ((u32) unit_slot_or_side <= BATTLE_ACTIVE_UNIT_COUNT - 1);
        goto check_empty_sides;
    case BATTLE_STORY_GARD_HALF_HP_END - 1:
        unit_slot_or_side = 0;
        enemy_scan_state_base = (u8 *)0x02034B4C;
loop_10:
        gard_end_record_offset = unit_slot_or_side * (s32)sizeof(struct BattleUnit);
        gard_end_record = (u8 *)(gard_end_record_offset - (0U - (u32)enemy_scan_state_base));
        gard_end_pilot_id = M2C_FIELD(gard_end_record, u8 *, (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(pilot_id));
        if ((gard_end_pilot_id == BATTLE_STORY_GARD) || (gard_end_pilot_id == BATTLE_STORY_GARD_VARIANT)) {
            gard_end_health_address = gard_end_record + (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(hp);
            gard_end_max_address = gard_end_record + (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(max_hp);
            gard_end_max_hp = *(s16 *)gard_end_max_address;
            gard_end_half_max_hp = gard_end_max_hp + ((u32) gard_end_max_hp >> 0x1F);
            gard_end_hp = *(s16 *)gard_end_health_address;
            gard_end_half_max_hp >>= 1;
            if (gard_end_hp < gard_end_half_max_hp) {
                goto end_scripted_battle;
            }
        }
        unit_slot_or_side += 1;
        asm volatile(
            ".macro bls target\n\t"
            ".purgem bls\n\t"
            "bls .L14\n\t"
            ".endm\n\t"
            ".macro b target\n\t"
            ".purgem b\n\t"
            ".endm");
        if ((u32) unit_slot_or_side > BATTLE_ACTIVE_UNIT_COUNT - 1) {
            goto check_empty_sides;
        }
        goto loop_10;
    case BATTLE_STORY_GARD_HALF_HP_END_SCENE - 1:
        unit_slot_or_side = 0;
        gard_end_scene_record_base = (u8 *)0x02034B4C;
        do {
            gard_end_scene_enemy_slot_base = (void *)((unit_slot_or_side * (s32)sizeof(struct BattleUnit)) - (0U - (u32)gard_end_scene_record_base));
            gard_end_scene_pilot_id = M2C_FIELD(gard_end_scene_enemy_slot_base, u8 *, (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(pilot_id));
            if ((gard_end_scene_pilot_id == BATTLE_STORY_GARD) || (gard_end_scene_pilot_id == BATTLE_STORY_GARD_VARIANT)) {
                gard_end_scene_health_address = gard_end_scene_enemy_slot_base + (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(hp);
                gard_end_scene_max_address = gard_end_scene_enemy_slot_base + (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(max_hp);
                gard_end_scene_max_hp = *(s16 *)gard_end_scene_max_address;
                gard_end_scene_half_max_hp = gard_end_scene_max_hp + ((u32) gard_end_scene_max_hp >> 0x1F);
                gard_end_scene_hp = *(s16 *)gard_end_scene_health_address;
                gard_end_scene_half_max_hp >>= 1;
                if (gard_end_scene_hp < gard_end_scene_half_max_hp) {
                    goto play_gard_ending_scene;
                }
            }
            unit_slot_or_side += 1;
        } while ((u32) unit_slot_or_side <= BATTLE_ACTIVE_UNIT_COUNT - 1);
        goto check_empty_sides;
    case BATTLE_STORY_GARD_HALF_HP_SCENE - 1:
        if (M2C_FIELD(story_setup_bytes, u8 *, BATTLE_STORY_SETUP_OFFSET(played_scene_flags)) != 0) {

        } else {
            unit_slot_or_side = 0;
            gard_scene_record_base = (u8 *)0x02034B4C;
            gard_portrait_tile_offset = 0x3C2;
            gard_portrait_palette_bank = 0xD;
            gard_portrait_graphics_buffer = (void *)0x02002880;
loop_27:
            {
                register u32 gard_scene_record_offset asm("r0");

                gard_scene_record_offset = unit_slot_or_side * (s32)sizeof(struct BattleUnit);
                gard_scene_record = (u8 *)(gard_scene_record_offset - (0U - (u32)gard_scene_record_base));
            }
            gard_scene_pilot_id = M2C_FIELD(gard_scene_record, u8 *, (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(pilot_id));
            if ((gard_scene_pilot_id != BATTLE_STORY_GARD) && (gard_scene_pilot_id != BATTLE_STORY_GARD_VARIANT)) {
                goto gard_scene_retry;
            } else {
                gard_scene_health_address = gard_scene_record + (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(hp);
                gard_scene_max_address = gard_scene_record + (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(max_hp);
                gard_scene_max_hp = *(s16 *)gard_scene_max_address;
                gard_scene_half_max_hp = gard_scene_max_hp + ((u32) gard_scene_max_hp >> 0x1F);
                gard_scene_hp = *(s16 *)gard_scene_health_address;
                gard_scene_half_max_hp >>= 1;
                if (gard_scene_hp >= gard_scene_half_max_hp) {

                } else {
                    gard_scene_outgoing_arguments[0] = 0x68;
                    gard_scene_outgoing_arguments[1] = gard_portrait_tile_offset;
                    gard_scene_outgoing_arguments[2] = gard_portrait_palette_bank;
                    gard_scene_outgoing_arguments[3] = 8;
                    gard_scene_outgoing_arguments[4] = 0;
                    gard_scene_sprite = CreateSprite_4(0x08359850, 0x0835985C, 0, 8);
                    RunMenuScript(0x08017BD3);
                    QueuePilotPortraitGraphics(0x46, 0, 0, gard_portrait_tile_offset, gard_portrait_palette_bank, gard_portrait_graphics_buffer);
                    RunMenuScript(0x08017BEC);
                    QueuePilotPortraitGraphics(9, 6, 0, gard_portrait_tile_offset, gard_portrait_palette_bank, gard_portrait_graphics_buffer);
                    RunMenuScript(0x08018021);
                    QueuePilotPortraitGraphics(3, 1, 0, gard_portrait_tile_offset, gard_portrait_palette_bank, gard_portrait_graphics_buffer);
                    RunMenuScript(0x0801805A);
                    QueuePilotPortraitGraphics(0x34, 2, 0, gard_portrait_tile_offset, gard_portrait_palette_bank, gard_portrait_graphics_buffer);
                    RunMenuScript(0x08018093);
                    QueuePilotPortraitGraphics(0x34, 3, 0, gard_portrait_tile_offset, gard_portrait_palette_bank, gard_portrait_graphics_buffer);
                    RunMenuScript(0x08018121);
                    QueuePilotPortraitGraphics(1, 3, 0, gard_portrait_tile_offset, gard_portrait_palette_bank, gard_portrait_graphics_buffer);
                    RunMenuScript(0x08018184);
                    QueuePilotPortraitGraphics(0x34, 3, 0, gard_portrait_tile_offset, gard_portrait_palette_bank, gard_portrait_graphics_buffer);
                    RunMenuScript(0x080181E6);
                    DestroySprite(gard_scene_sprite);
                    RunMenuScript(0x08017BE6);
                    gard_scene_state_tail = (u8 *)0x0203055C;
                    M2C_FIELD(gard_scene_state_tail, u8 *, BATTLE_STORY_SETUP_OFFSET(played_scene_flags)) = (u8) (M2C_FIELD(gard_scene_state_tail, u8 *, BATTLE_STORY_SETUP_OFFSET(played_scene_flags)) + 1);
                }
            }
        }
        goto check_empty_sides;
gard_scene_retry:
        unit_slot_or_side += 1;
        if ((u32) unit_slot_or_side <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
            goto loop_27;
        }
        goto check_empty_sides;
    case BATTLE_STORY_BIT_VERSUS_LEON - 1:
        bit_vs_leon_player_slot = 0;
        bit_vs_leon_player_records = (u8 *)0x02034B4C;
loop_38:
        if (((IsBattleUnitActive(0U, bit_vs_leon_player_slot) << 0x18) == 0) || ((bit_vs_leon_pilot_id = M2C_FIELD(((bit_vs_leon_player_slot * (s32)sizeof(struct BattleUnit)) + bit_vs_leon_player_records), u8 *, BATTLE_UNIT_OFFSET(pilot_id)), (bit_vs_leon_pilot_id != BATTLE_STORY_BIT)) && (bit_vs_leon_pilot_id != BATTLE_STORY_BIT_VARIANT))) {
            bit_vs_leon_player_slot = (u32) (u8) (bit_vs_leon_player_slot + 1);
            if (bit_vs_leon_player_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                goto loop_38;
            }
        }
        leon_unit_slot = 0;
        leon_enemy_slot_base = (u8 *)0x02034B4C;
        while ((leon_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) && (((IsBattleUnitActive(1U, leon_unit_slot) << 0x18) == 0) || (M2C_FIELD(((leon_unit_slot * (s32)sizeof(struct BattleUnit)) + leon_enemy_slot_base), u8 *, (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(pilot_id)) != BATTLE_STORY_LEON))) {
            leon_unit_slot = (u32) (u8) (leon_unit_slot + 1);
        }
        if (bit_vs_leon_player_slot > BATTLE_ACTIVE_UNIT_COUNT - 1) {

        } else if (leon_unit_slot > BATTLE_ACTIVE_UNIT_COUNT - 1) {

        } else {
            bit_first_damage_scene_played = BATTLE_STORY_BIT_FIRST_DAMAGE_SCENE & gBattleStorySetup[BATTLE_STORY_SETUP_OFFSET(played_scene_flags)];
            if (bit_first_damage_scene_played != 0) {

            } else if (({
                bit_first_damage_state = (u8 *)0x02034B4C;
                bit_first_damage_offset = BATTLE_STORY_DAMAGE_OFFSET(bit_damage_taken);
                asm volatile(".include \"src/unnamed/sub_080C682C_fix_score2.inc\""
                    : "+r"(bit_first_damage_state), "+r"(bit_first_damage_offset));
                *(s16 *)(bit_first_damage_state + bit_first_damage_offset);
            }) == 0) {

            } else {
                bit_first_damage_scene_sprite = CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0x3C2, 0xD, 8, (s32) bit_first_damage_scene_played);
                RunMenuScript(0x08017BD3);
                QueuePilotPortraitGraphics(0x22, 3, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x08018ED2);
                QueuePilotPortraitGraphics(0x21, 6, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x08018F1E);
                QueuePilotPortraitGraphics(0x31, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x08018F89);
                QueuePilotPortraitGraphics(0x31, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x08018FFC);
                QueuePilotPortraitGraphics(0x21, 7, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x08019052);
                QueuePilotPortraitGraphics(0x31, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x080190B0);
                QueuePilotPortraitGraphics(0x21, 7, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x080190FA);
                QueuePilotPortraitGraphics(0x20, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x08019119);
                QueuePilotPortraitGraphics(0x31, 7, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x0801913A);
                QueuePilotPortraitGraphics(0x21, 4, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x08019177);
                DestroySprite(bit_first_damage_scene_sprite);
                RunMenuScript(0x08017BE6);
                {
                    register u32 bit_first_damage_old_scene_flags asm("r1") = gBattleStorySetup[BATTLE_STORY_SETUP_OFFSET(played_scene_flags)];
                    register u32 bit_first_damage_new_scene_flags asm("r0") = BATTLE_STORY_BIT_FIRST_DAMAGE_SCENE;

                    bit_first_damage_new_scene_flags |= bit_first_damage_old_scene_flags;
                    gBattleStorySetup[BATTLE_STORY_SETUP_OFFSET(played_scene_flags)] = (u8)bit_first_damage_new_scene_flags;
                }
            }
            bit_damage_200_scene_played = BATTLE_STORY_BIT_DAMAGE_200_SCENE & gBattleStorySetup[BATTLE_STORY_SETUP_OFFSET(played_scene_flags)];
            if (bit_damage_200_scene_played != 0) {

            } else if ((s32) ({
                bit_damage_200_state = (u8 *)0x02034B4C;
                bit_damage_200_offset = BATTLE_STORY_DAMAGE_OFFSET(bit_damage_taken);
                asm volatile(".include \"src/unnamed/sub_080C682C_fix_score4.inc\""
                    : "+r"(bit_damage_200_state), "+r"(bit_damage_200_offset));
                *(s16 *)(bit_damage_200_state + bit_damage_200_offset);
            }) <= BATTLE_STORY_DAMAGE_THRESHOLD - 1) {

            } else {
                bit_damage_200_scene_sprite = CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0x3C2, 0xD, 8, (s32) bit_damage_200_scene_played);
                RunMenuScript(0x08017BD3);
                QueuePilotPortraitGraphics(0x22, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x0801919E);
                QueuePilotPortraitGraphics(0x22, 3, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x080191F8);
                QueuePilotPortraitGraphics(0x22, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x0801925B);
                QueuePilotPortraitGraphics(0x22, 3, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x080192F5);
                QueuePilotPortraitGraphics(0x1E, 3, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x080193A6);
                QueuePilotPortraitGraphics(0x22, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x080193DB);
                QueuePilotPortraitGraphics(0x22, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x08019480);
                QueuePilotPortraitGraphics(0x22, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x080194D0);
                QueuePilotPortraitGraphics(0x22, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x0801954D);
                QueuePilotPortraitGraphics(0x22, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x08019617);
                QueuePilotPortraitGraphics(0x22, 3, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x0801963A);
                QueuePilotPortraitGraphics(0x22, 3, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x0801969D);
                QueuePilotPortraitGraphics(0x1E, 3, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x0801972B);
                QueuePilotPortraitGraphics(0x1E, 3, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x08019756);
                DestroySprite(bit_damage_200_scene_sprite);
                RunMenuScript(0x08017BE6);
                gBattleStorySetup[BATTLE_STORY_SETUP_OFFSET(played_scene_flags)] = (u8) (BATTLE_STORY_BIT_DAMAGE_200_SCENE | gBattleStorySetup[BATTLE_STORY_SETUP_OFFSET(played_scene_flags)]);
            }
            leon_damage_200_scene_played = BATTLE_STORY_LEON_DAMAGE_200_SCENE & gBattleStorySetup[BATTLE_STORY_SETUP_OFFSET(played_scene_flags)];
            if (leon_damage_200_scene_played != 0) {

            } else if ((s32) ({
                leon_damage_200_state = (u8 *)0x02034B4C;
                leon_damage_200_offset = BATTLE_STORY_DAMAGE_OFFSET(leon_damage_taken_from_bit);
                asm volatile("" : "+r"(leon_damage_200_state), "+r"(leon_damage_200_offset));
                *(s16 *)(leon_damage_200_state + leon_damage_200_offset);
            }) <= BATTLE_STORY_DAMAGE_THRESHOLD - 1) {

            } else {
                leon_damage_200_scene_sprite = CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0x3C2, 0xD, 8, (s32) leon_damage_200_scene_played);
                RunMenuScript(0x08017BD3);
                QueuePilotPortraitGraphics(0x1E, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x080197BB);
                QueuePilotPortraitGraphics(0x20, 6, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x08019815);
                QueuePilotPortraitGraphics(0x21, 6, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x0801986B);
                QueuePilotPortraitGraphics(0x23, 0, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x080198A0);
                QueuePilotPortraitGraphics(0x1E, 3, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x080198C3);
                QueuePilotPortraitGraphics(0x22, 3, 0, 0x3C2, 0xD, 0x02002880);
                RunMenuScript(0x08019930);
                DestroySprite(leon_damage_200_scene_sprite);
                RunMenuScript(0x08017BE6);
                gBattleStorySetup[BATTLE_STORY_SETUP_OFFSET(played_scene_flags)] = (u8) (BATTLE_STORY_LEON_DAMAGE_200_SCENE | gBattleStorySetup[BATTLE_STORY_SETUP_OFFSET(played_scene_flags)]);
            }
        }
        goto check_empty_sides;
    case BATTLE_STORY_BIT_VERSUS_STOLLER - 1:
        bit_vs_stoller_player_slot = 0;
        bit_vs_stoller_player_records = (u8 *)0x02034B4C;
loop_68:
        if (((IsBattleUnitActive(0U, bit_vs_stoller_player_slot) << 0x18) == 0) || ((bit_vs_stoller_pilot_id = M2C_FIELD(((bit_vs_stoller_player_slot * (s32)sizeof(struct BattleUnit)) + bit_vs_stoller_player_records), u8 *, BATTLE_UNIT_OFFSET(pilot_id)), (bit_vs_stoller_pilot_id != BATTLE_STORY_BIT)) && (bit_vs_stoller_pilot_id != BATTLE_STORY_BIT_VARIANT))) {
            bit_vs_stoller_player_slot = (u32) (u8) (bit_vs_stoller_player_slot + 1);
            if (bit_vs_stoller_player_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                goto loop_68;
            }
        }
        stoller_unit_slot = 0;
        stoller_enemy_slot_base = (u8 *)0x02034B4C;
        while ((stoller_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) && (((IsBattleUnitActive(1U, stoller_unit_slot) << 0x18) == 0) || (M2C_FIELD(((stoller_unit_slot * (s32)sizeof(struct BattleUnit)) + stoller_enemy_slot_base), u8 *, (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(pilot_id)) != BATTLE_STORY_STOLLER))) {
            stoller_unit_slot = (u32) (u8) (stoller_unit_slot + 1);
        }
        if (bit_vs_stoller_player_slot > BATTLE_ACTIVE_UNIT_COUNT - 1) {
            goto check_empty_sides;
        }
        if (stoller_unit_slot > BATTLE_ACTIVE_UNIT_COUNT - 1) {
            goto check_empty_sides;
        }
        bit_vs_stoller_state_or_player_record = (u8 *)0x02034B4C;
        bit_vs_stoller_damage_offset = BATTLE_STORY_DAMAGE_OFFSET(bit_damage_taken);
        if ((s32) *(s16 *)(bit_vs_stoller_state_or_player_record + bit_vs_stoller_damage_offset) <= BATTLE_STORY_DAMAGE_THRESHOLD - 1) {
            bit_vs_stoller_state_or_player_record = (u8 *)((bit_vs_stoller_player_slot * (s32)sizeof(struct BattleUnit)) - (0U - (u32)bit_vs_stoller_state_or_player_record));
            bit_player_slot_base = bit_vs_stoller_state_or_player_record;
            if (M2C_FIELD(bit_player_slot_base, s16 *, BATTLE_UNIT_OFFSET(hp)) != 1) {
                if (M2C_FIELD(bit_player_slot_base, s16 *, (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(hp)) != 1) {
                    goto check_empty_sides;
                }
                goto end_scripted_battle;
            }
        }
        goto end_scripted_battle;
play_gard_ending_scene:
        CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0x3C2, 0xD, 8, 0);
        RunMenuScript(0x08017BD3);
        QueuePilotPortraitGraphics(0x34, 2, 0, 0x3C2, 0xD, 0x02002880);
        RunMenuScript(0x08018369);
        QueuePilotPortraitGraphics(1, 1, 0, 0x3C2, 0xD, 0x02002880);
        RunMenuScript(0x080183F1);
        QueuePilotPortraitGraphics(3, 1, 0, 0x3C2, 0xD, 0x02002880);
        RunMenuScript(0x08018440);
end_scripted_battle:
        return BATTLE_RESULT_SCRIPTED_END;
    case BATTLE_STORY_GARD_ONE_HP_END - 1:
        unit_slot_or_side = 0;
        gard_one_hp_record_base = (u8 *)0x02034B4C;
loop_90:
        gard_one_hp_enemy_slot_base = (void *)((unit_slot_or_side * (s32)sizeof(struct BattleUnit)) - (0U - (u32)gard_one_hp_record_base));
        gard_one_hp_pilot_id = M2C_FIELD(gard_one_hp_enemy_slot_base, u8 *, (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(pilot_id));
        if (((gard_one_hp_pilot_id != BATTLE_STORY_GARD) && (gard_one_hp_pilot_id != BATTLE_STORY_GARD_VARIANT)) || (M2C_FIELD(gard_one_hp_enemy_slot_base, s16 *, (s32)sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(hp)) != 1)) {
            unit_slot_or_side += 1;
            if ((u32) unit_slot_or_side > BATTLE_ACTIVE_UNIT_COUNT - 1) {
                goto check_empty_sides;
            }
            goto loop_90;
        }
        goto end_scripted_battle;
    default:
check_empty_sides:
        result_flags = 0;
        unit_slot_or_side = 0;
        do {
            active_unit_slot = 0;
            while ((active_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) && ((IsBattleUnitActive(unit_slot_or_side, active_unit_slot) << 0x18) == 0)) {
                active_unit_slot = (u32) (u8) (active_unit_slot + 1);
            }
            if (active_unit_slot == BATTLE_ACTIVE_UNIT_COUNT) {
                result_flags |= 1 << unit_slot_or_side;
            }
            unit_slot_or_side += 1;
        } while ((u32) unit_slot_or_side <= 1U);
        return result_flags;
    }
}
