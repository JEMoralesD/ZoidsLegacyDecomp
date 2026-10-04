#include "m2c_prelude.h"
#include "battle.h"

extern u8 RemoveBattleActionCandidate(u32, u32) asm("func_080CA570");

void KeepBattleCandidatesTargetingPilot(int pilot_id) asm("func_080CB194");

void KeepBattleCandidatesTargetingPilot(int pilot_id)
{
    register u32 choice_index asm("r8");
    register u32 next_target_side asm("r9");
    register u8 *battle_state asm("sl");
    register s32 equipment_offset asm("ip");
    register u32 pilot_found asm("r5");
    register u32 target_side asm("r4");
    register u32 target_unit_index asm("r3");
    u32 candidate_index;
    volatile u32 target_pilot_id;
    u8 *volatile equipment_slot_ptr;
    volatile u32 candidate_choice_offset;
    volatile u32 action_offset;

    target_pilot_id = (u8)pilot_id;
    candidate_index = 0;
    goto outer_test;
outer_loop:
    {
        register u32 z asm("r0");
        z = 0;
        choice_index = z;
    }
    {
        register u8 *c0 asm("r0");
        register u8 *cs asm("r1");
        register u32 cnt asm("r1");
        c0 = ((struct BattleActionCandidates *)0x0203EF70)->target_choice_counts;
        asm volatile("" : "+r"(c0));
        cs = (u8 *)(candidate_index + (u32)c0);
        cnt = *cs;
        if (choice_index >= cnt) goto outer_next;
    }
mid_loop:
    pilot_found = 0;
    target_side = 0;
    {
        register u8 *p1 asm("r1");
        p1 = ((struct BattleActionCandidates *)0x0203EF70)->equipment_slots;
        asm volatile("" : "+r"(p1));
        p1 = (u8 *)(candidate_index + (u32)p1);
        equipment_slot_ptr = p1;
    }
    {
        register u32 e6 asm("r0");
        e6 = candidate_index << 1;
        e6 += candidate_index;
        e6 <<= 1;
        e6 += choice_index;
        candidate_choice_offset = e6;
    }
sub_loop:
    target_unit_index = 0;
    {
        register u32 tn asm("r2");
        tn = target_side + 1;
        asm volatile("" : "+r"(tn));
        next_target_side = tn;
    }
    if (pilot_found == 0) {
        {
            register u8 *t6 asm("r6");
            register u32 ri asm("r1");
            register s32 stride asm("r0");
            register s32 ro asm("r2");
            t6 = equipment_slot_ptr;
            ri = *t6;
            asm volatile("" : "+r"(ri));
            stride = 0xA8C;
            ro = ri;
            ro *= stride;
            equipment_offset = ro;
        }
        {
            register u8 *b6 asm("r6");
            b6 = (u8 *)0x02034B4C;
            asm volatile("" : "+r"(b6));
            battle_state = b6;
        }
        {
            register u8 *rp asm("r0");
            register u32 action_index asm("r1");
            register u32 t asm("r0");
            rp = (u8 *)0x0203ECFB;
            asm volatile("" : "+r"(rp));
            action_index = *rp;
            t = action_index << 3;
            t -= action_index;
            t <<= 5;
            t += action_index;
            t <<= 2;
            action_offset = t;
        }
inner_loop:
        asm volatile("" : "+r"(target_side));
        {
            register u32 w asm("r1");
            register u32 t12 asm("r2");
            register u32 e asm("r2");
            register s32 m asm("r0");
            register u32 h asm("r1");
            register u32 mask asm("r0");

            w = 0x27C8;
            asm volatile("" : "+r"(w));
            w += (u32)battle_state;
            t12 = action_offset;
            w = t12 + w;
            w += (u32)equipment_offset;
            {
                register u32 t8 asm("r6");
                register u8 *et asm("r2");
                register u8 *es asm("r0");
                t8 = candidate_choice_offset;
                et = ((struct BattleActionCandidates *)0x0203EF70)->target_choices[0];
                asm volatile("" : "+r"(et));
                es = (u8 *)(t8 + (u32)et);
                e = *es;
            }
            m = 0x94;
            m *= e;
            m += 12;
            w += m;
            {
                register u32 s72 asm("r0");
                s72 = target_side << 3;
                s72 += target_side;
                s72 <<= 3;
                s72 += 4;
                w += s72;
            }
            {
                register u32 i12 asm("r0");
                i12 = target_unit_index << 1;
                i12 += target_unit_index;
                i12 <<= 2;
                w += i12;
            }
            h = *(u16 *)w;
            mask = 1;
            mask &= h;
            if (mask != 0) {
                register u32 q asm("r0");
                register u32 s13 asm("r1");
                q = target_unit_index << 2;
                q += target_unit_index;
                q <<= 3;
                q -= target_unit_index;
                q <<= 4;
                s13 = target_side << 2;
                s13 += target_side;
                s13 <<= 3;
                s13 -= target_side;
                s13 <<= 7;
                q += s13;
                q += (u32)battle_state;
                q += 112;
                {
                    register u32 val asm("r0");
                    register u32 a0v asm("r6");
                    val = *(u8 *)q;
                    a0v = target_pilot_id;
                    if (val == a0v) {
                        pilot_found = 1;
                    }
                }
            }
        }
        {
            register u32 ti asm("r0");
            ti = target_unit_index + 1;
            ti <<= 24;
            target_unit_index = ti >> 24;
        }
        if (target_unit_index <= 5 && pilot_found == 0) goto inner_loop;
    }
    {
        register u32 t1 asm("r1");
        register u32 t0 asm("r0");
        t1 = next_target_side;
        t0 = t1 << 24;
        target_side = t0 >> 24;
    }
    if (target_side <= 1 && pilot_found == 0) goto sub_loop;
    if (pilot_found == 0) {
        if (RemoveBattleActionCandidate(candidate_index, choice_index) == 0) {
            register u32 td asm("r0");
            td = candidate_index - 1;
            td <<= 24;
            candidate_index = td >> 24;
            goto outer_next;
        }
        {
            register u32 tm asm("r0");
            tm = choice_index;
            tm -= 1;
            tm <<= 24;
            tm >>= 24;
            choice_index = tm;
        }
    }
    {
        register u32 tp asm("r0");
        tp = choice_index;
        tp += 1;
        tp <<= 24;
        tp >>= 24;
        choice_index = tp;
    }
    {
        register u8 *c2 asm("r2");
        register u8 *cs2 asm("r0");
        register u32 cnt2 asm("r0");
        c2 = ((struct BattleActionCandidates *)0x0203EF70)->target_choice_counts;
        asm volatile("" : "+r"(c2));
        cs2 = (u8 *)(candidate_index + (u32)c2);
        cnt2 = *cs2;
        if (choice_index < cnt2) goto mid_loop;
    }
outer_next:
    {
        register u32 tu asm("r0");
        tu = candidate_index + 1;
        tu <<= 24;
        candidate_index = tu >> 24;
    }
outer_test:
    {
        register u8 *cc asm("r0");
        register u32 ct asm("r0");
        cc = &((struct BattleActionCandidates *)0x0203EF70)->equipment_count;
        asm volatile("" : "+r"(cc));
        ct = *cc;
        if (candidate_index < ct) goto outer_loop;
    }
}
