#include "m2c_prelude.h"
#include "battle.h"

extern s32 LoadZoidBodyGraphics(u32, u32, s32, s32, u32, s32) asm("func_0809A1F8");
extern void LoadSpriteGraphicsFromTable(u8 *, u32, u32, u32) asm("func_0809AA64");
extern s32 CreateMirroredSpriteFromTable(u8 *, u32, s32, s32, s32, u32, u32, u32, s32, u32) asm("func_080D22B4");

void LoadBattleSceneZoidGraphics(struct BattleUnit *unit, int mirrored) asm("func_080CB340");

void LoadBattleSceneZoidGraphics(struct BattleUnit *unit, int mirrored)
{
    register u8 *unit_record asm("r8");
    register u32 mirror_flag asm("r10");
    register u32 zoid_id asm("r9");
    register u32 mount_slot asm("r5");
    register s32 equipment_slot_offset asm("r6");
    volatile u32 sp18;

    unit_record = (u8 *)unit;
    mirrored = mirrored << 24;
    mirrored = (u32)mirrored >> 24;
    mirror_flag = mirrored;
    zoid_id = unit->zoid_id;
    {
        register u8 *rv0 asm("r2");
        rv0 = unit_record;
        asm volatile("" : "+r"(rv0));
        *(s32 *)0x02033F3C = LoadZoidBodyGraphics(zoid_id, M2C_FIELD(rv0, u8 *, (s32)&((struct BattleUnit *)0)->palette_variant), 1, 0,
            ({ register u32 t asm("r7"); t = mirror_flag; asm volatile("" : "+r"(t)); t; }),
            0x02002880);
    }
    mount_slot = 0;
loop_head:
    {
        register u32 t4 asm("r1");
        register u16 *slot asm("r2");
        register u32 vm1 asm("r0");

        t4 = mount_slot << 2;
        {
            register u8 *rv asm("r2");
            register u8 *ptmp asm("r0");
            rv = unit_record;
            asm volatile("" : "+r"(rv));
            ptmp = rv + t4;
            slot = (u16 *)(ptmp + 0x52);
        }
        vm1 = *slot;
        vm1 -= 1;
        vm1 = vm1 << 16;
        vm1 = vm1 >> 16;
        equipment_slot_offset = t4;
        if (vm1 <= 0x63) {
            register u8 *tbl asm("r3");
            register u32 v2 asm("r0");
            register u32 w asm("r1");
            register u32 x asm("r0");
            register u32 resource_id asm("r4");
            register u32 v23 asm("r4");
            register s32 mount_x asm("r3");
            register u32 kind32 asm("r2");
            register u8 *eckeep asm("ip");

            tbl = (u8 *)0x087ABC6C;
            asm volatile("" : "+r"(tbl));
            v2 = *slot;
            w = v2 << 1;
            if (mount_slot != 0) {
                x = (w + 1) << 24;
            } else {
                x = v2 << 25;
            }
            LoadSpriteGraphicsFromTable(tbl, x >> 24, (v23 = mount_slot << 23) >> 16, mount_slot);
            {
                register u32 v3 asm("r0");
                register u32 w2 asm("r1");
                register u32 yv asm("r0");

                {
                    register u8 *rv2 asm("r7");
                    rv2 = unit_record;
                    asm volatile("" : "+r"(rv2));
                    v3 = *(u16 *)(rv2 + equipment_slot_offset + 0x52);
                }
                w2 = v3 << 1;
                sp18 = v23;
                if (mount_slot != 0) {
                    yv = (w2 + 1) << 16;
                } else {
                    yv = v3 << 17;
                }
                resource_id = yv >> 16;
            }
            {
                register u8 *eclo asm("r0");
                register u32 kv asm("r1");
                eclo = (u8 *)0x087EC38C;
                asm volatile("" : "+r"(eclo));
                kv = zoid_id;
                kind32 = kv << 5;
                mount_x = *(s16 *)((equipment_slot_offset + (s32)kind32) + (s32)eclo);
                eckeep = eclo;
            }
            if (mount_slot == 2) {
                register u8 *acbase asm("r0");
                register u32 hv asm("r1");
                register s32 sv asm("r0");
                acbase = (u8 *)0x087AC90C;
                asm volatile("" : "+r"(acbase));
                hv = *(u16 *)(unit_record + 0x5A);
                hv <<= 1;
                hv += (u32)acbase;
                sv = *(s16 *)hv;
                mount_x = (s16)(mount_x - sv);
            }
            {
                register s32 res asm("r0");
                register u8 *outp asm("r1");
                res = CreateMirroredSpriteFromTable(
                (u8 *)0x087AC2BC,
                resource_id,
                0,
                mount_x,
                ({
                    register s32 esum asm("r0");
                    register u8 *eck2 asm("r1");
                    register s32 ev asm("r0");
                    esum = equipment_slot_offset + (s32)kind32;
                    eck2 = eckeep + 2;
                    asm volatile("" : "+r"(eck2));
                    esum += (s32)eck2;
                    ev = *(s16 *)esum;
                    ev;
                }),
                ({
                    register u32 sv18 asm("r2");
                    register u32 shv asm("r0");
                    sv18 = sp18;
                    asm volatile("" : "+r"(sv18));
                    shv = sv18 >> 16;
                    shv;
                }),
                mount_slot,
                ({
                    register u8 *t2 asm("r1");
                    register u32 k3 asm("r0");
                    register u32 fv asm("r0");
                    register u32 fl asm("r1");
                    register u32 kv2 asm("r7");
                    t2 = (u8 *)0x087ED68C;
                    asm volatile("" : "+r"(t2));
                    kv2 = zoid_id;
                    asm volatile("" : "+r"(kv2));
                    k3 = kv2 << 1;
                    k3 += (u32)zoid_id;
                    k3 = mount_slot + k3;
                    k3 += (u32)t2;
                    fv = *(u8 *)k3;
                    fl = fv << 6;
                    fl |= 0x1218;
                    {
                        register u32 flv asm("r0");
                        flv = mirror_flag;
                        asm volatile("" : "+r"(flv));
                        if (flv != 0) {
                            fl |= 0x8000;
                        }
                    }
                    fl;
                }),
                0,
                ({ register u32 t9 asm("r1"); t9 = mirror_flag; asm volatile("" : "+r"(t9)); t9; }));
                outp = (u8 *)0x02033F40;
                asm volatile("" : "+r"(outp));
                outp = (u8 *)(equipment_slot_offset + (s32)outp);
                *(s32 *)outp = res;
            }
        } else {
            register u8 *zp asm("r0");
            register u32 zv asm("r1");
            zp = (u8 *)0x02033F40;
            asm volatile("" : "+r"(zp));
            zp = (u8 *)(t4 + (s32)zp);
            zv = 0;
            *(s32 *)zp = zv;
        }
    }
    {
        register u32 tinc asm("r0");
        tinc = mount_slot + 1;
        tinc <<= 24;
        mount_slot = tinc >> 24;
    }
    if (mount_slot <= 2) {
        goto loop_head;
    }
}
