#include "m2c_prelude.h"
#include "../game/game_state.h"

struct SceneFlagsA7D28 {
    u8 pad0;
    u8 field1;
    u8 screen;
};

struct ViewStateA7D28 {
    s32 x;
    s32 y;
    s32 z;
    s16 field0C;
    s16 field0E;
    s16 field10;
    u8 pad12[2];
    s32 field14;
    s32 field18;
    s32 field1C;
    u8 pad20[0x40];
    s32 flags;
};

struct SpriteA7D28 {
    s32 flags;
    u8 pad04[6];
    s16 y;
    u8 pad0C[0x1C];
    s32 x;
    s32 field2C;
    s32 field30;
};

struct TaskA7D28 {
    u8 type;
    u8 flags;
    s16 value;
    s32 callback;
    s32 field8;
    s32 fieldC;
};

extern s32 gGameMode;
extern s32 gBattlePhase;
extern struct SceneFlagsA7D28 gBattleSetup;
extern struct ViewStateA7D28 D_030033C4;
extern volatile u16 D_0300004E;
extern volatile u16 D_03000050;
extern s32 D_03000054[];
extern struct SpriteA7D28 *D_02032E8C[2];
extern u8 D_02032EEC[2];
extern struct TaskA7D28 D_020314A4;
extern struct TaskA7D28 D_020314B4;

void ResetScanlineEvents(void) asm("func_0809258C");
void InsertScanlineEvent(void) asm("func_080925A4");
void PlaySong(s32) asm("func_08092E84");
void ClearSpritePools(void) asm("func_08094330");
struct SpriteA7D28 *CreateSpriteFromTable(s32, s32, s32, s32, s32,
    s32, s32, s32, s32) asm("func_08094374");
struct SpriteA7D28 *CreateSprite(s32, s32, s32, s32, s32,
    s32, s32, s32, s32) asm("func_08094484");
void func_08096308(s32, s32);
s32 func_0809669C(void);
void RunMenuScript(s32) asm("func_08098BB4");
void func_0809A1F8(s32, s32, s32, s32, s32, s32);
void func_0809A4CC(s32, s32, s32, s32);
void func_0809A52C(s32, s32, s32, s32, s32);
void func_0809A5B4(u8, s32, s32, s32, s32);
void func_0809A9C8(s32, s32, s32, s32, s32, s32);
void func_0809AA64(s32, s32, s32, s32);
void ResetBattleAnimation(s32) asm("func_080D0AF0");
void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
void func_080ED17C(s32);

void sub_080A7D28(void)
{
    struct SpriteA7D28 *sprites[8];
    s32 task_mask;
    s32 camera_step;
    s32 ramp;
    s32 velocity;
    s32 fall_speed;
    u8 counter;
    u8 index;

    gGameMode = GAME_MODE_BATTLE;
    gBattlePhase = 0xFF10;
    gBattleSetup.field1 = 0;
    D_030033C4.x = 0xFFFFC000;
    D_030033C4.y = 0;
    D_030033C4.z = 0x8000;
    D_030033C4.field0C = 0x20;
    D_030033C4.field10 = 0;
    D_030033C4.field0E = 0;
    D_030033C4.field14 = 0x78;
    D_030033C4.field18 = 0x78;
    D_030033C4.field1C = 0x80;
    D_030033C4.flags = 0x20000;
    func_080ED17C(1);
    func_0809A4CC(0x95, 0, 0, 0);

    {
        struct SpriteA7D28 *sprite = CreateSprite(
            0x0821024C, 0x08210258, 0, 0, 0, 0, 0, 0x2C8, 0x080BADD5);
        D_02032E8C[0] = sprite;
        sprite->x = 0;
        sprite->field2C = 0;
        sprite->field30 = 0;
        sprite->y = -0x20;
    }
    D_02032EEC[0] = 0;
    func_0809AA64(0x087AF9D4, 7, 0x40, 1);

    func_08096308(1, 0x10);
    while ((func_0809669C() << 24) == 0) {
        func_080ED17C(1);
    }

    D_0300004E = 0x740;
    D_03000050 = 0x810;
    counter = 0;
    do {
        func_080ED17C(1);
        counter++;
    } while (counter <= 0xF);

    sprites[0] = CreateSpriteFromTable(0x087AFA94, 7, 3, 0, 0, 0x40, 1,
        0x7C0, 0x080BADD5);
    sprites[0]->x = 0x4000;
    sprites[0]->field2C = 0;
    sprites[0]->field30 = 0;
    sprites[0]->y = -0x40;
    camera_step = -0x1000;
    PlaySong(0x44);
    while (sprites[0]->x > -0x10000) {
        D_030033C4.x += camera_step;
        sprites[0]->x -= 0x2000;
        func_080ED17C(1);
    }

    func_08096308(2, 0x10);
    while ((func_0809669C() << 24) == 0) {
        camera_step = (s32)(camera_step + ((u32)camera_step >> 31)) >> 1;
        D_030033C4.x += camera_step;
        sprites[0]->x -= 0x2000;
        func_080ED17C(1);
    }

    counter = 0;
    do {
        func_080ED17C(1);
        counter++;
    } while (counter <= 0x1D);

    gBattleSetup.screen = 0xFF;
    gGameMode = GAME_MODE_BATTLE_SCENE;
    func_080ED17C(1);
    ClearSpritePools();
    func_0809A1F8(0x1C, 0, 1, 0, 0, 0x02002880);
    ResetBattleAnimation(0xFFFFFF00);
    SetBattleAnimationCameraMode(0xE, 0);
    func_0809A5B4(gBattleSetup.screen, 2, 3, 2, 1);

    {
        register struct TaskA7D28 *task asm("r0") = &D_020314A4;
        register u8 flags asm("r2");

        task->type = 0x5F;
        flags = task->flags;
        task_mask = -2;
        task->flags = task_mask & flags;
        task->value = 0;
        task->callback = 0x080A0099;
        task->fieldC = 0;
        task->field8 = 0;
    }
    InsertScanlineEvent();

    D_020314B4.type = 0xA0;
    D_020314B4.flags &= task_mask;
    D_020314B4.value = 0;
    D_020314B4.callback = 0x080A00D5;
    D_020314B4.fieldC = 0;
    D_020314B4.field8 = 0;
    InsertScanlineEvent();

    func_08096308(1, 0x10);
    {
        s32 *scroll = D_03000054;
        s32 finished = 0x1000;
wait_scroll:
        if (*scroll != finished) {
            func_080ED17C(1);
            goto wait_scroll;
        }
    }

    ResetScanlineEvents();
    CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0x3C2,
        0xD, 8, 0);
    RunMenuScript(0x08017BD3);
    func_0809A9C8(0x1E, 3, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x08018969);
    func_08096308(2, 0x10);
    while ((func_0809669C() << 24) == 0) {
        func_080ED17C(1);
    }

    gGameMode = GAME_MODE_BATTLE;
    gBattlePhase = 0xFF10;
    gBattleSetup.field1 = 0;
    D_030033C4.x = 0;
    D_030033C4.y = 0;
    D_030033C4.z = 0x8000;
    D_030033C4.field0C = 0x20;
    D_030033C4.field10 = 0;
    D_030033C4.field0E = 0;
    D_030033C4.field14 = 0x78;
    D_030033C4.field18 = 0x78;
    D_030033C4.field1C = 0x80;
    D_030033C4.flags = 0x20000;
    func_080ED17C(1);
    func_0809A4CC(0x95, 0, 0, 0);

    {
        struct SpriteA7D28 *sprite = CreateSprite(
            0x0821024C, 0x08210258, 0, 0, 0, 0, 0, 0x2C8, 0x080BADD5);
        D_02032E8C[0] = sprite;
        sprite->x = 0;
        sprite->field2C = 0;
        sprite->field30 = 0;
        sprite->y = -0x20;
    }
    D_02032EEC[0] = 0;
    func_0809AA64(0x087AF9D4, 4, 0x80, 2);

    func_08096308(1, 0x10);
    while ((func_0809669C() << 24) == 0) {
        func_080ED17C(1);
    }

    counter = 0;
    do {
        func_080ED17C(1);
        counter++;
    } while (counter <= 0xF);

    func_0809AA64(0x087AF9D4, 8, 0x40, 1);
    {
        struct SpriteA7D28 *sprite = CreateSprite(
            0x0821024C, 0x08210258, 0, 0, 0, 0x40, 1,
            0x3C8, 0x080BAE61);
        D_02032E8C[1] = sprite;
        sprite->x = 0;
        sprite->field2C = 0;
        sprite->field30 = 0;
    }
    D_02032EEC[1] = 0;

    ramp = -0x3000;
    velocity = 0;
    do {
        velocity += 0x80;
        ramp += velocity;
        if (ramp > 0) {
            ramp = 0;
        }
        {
            struct SpriteA7D28 *sprite = D_02032E8C[1];
            register s32 rounded asm("r0") = ramp;

            if (ramp < 0) {
                rounded += 0xFF;
            }
            sprite->y = rounded >> 8;
        }
        func_080ED17C(1);
    } while (ramp != 0);

    func_0809A52C(0x1C, 0, 0x40, 1, 0x02002880);
    index = 0;
    do {
        sprites[index] = CreateSpriteFromTable(0x087AFA94, 4, 0, 0, 0,
            0x80, 2, 0x3C0, 0x080BAE61);
        sprites[index]->x = (index << 11) - 0x1000;
        sprites[index]->field2C = 0;
        sprites[index]->field30 = 0;
        index++;
    } while (index <= 4);

    PlaySong(0x4E);
    do {
        func_080ED17C(1);
        index = 0;
        while (index <= 3 && !(sprites[index]->flags & 1)) {
            index++;
        }
    } while (index <= 3);

    PlaySong(0x7E);
    fall_speed = 0;
    while (D_02032E8C[0]->x > -0x10000) {
        if (fall_speed > -0x200) {
            fall_speed -= 0x10;
        }
        D_02032E8C[0]->x += fall_speed;
        func_080ED17C(1);
    }

    func_08096308(2, 0x10);
    while ((func_0809669C() << 24) == 0) {
        func_080ED17C(1);
    }

    gGameMode = -1;
    func_080ED17C(1);
}
