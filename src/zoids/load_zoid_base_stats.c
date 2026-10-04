#include "m2c_prelude.h"
#include "../battle/battle.h"

struct ZoidBaseStats {
    u8 bytes[4];
    u16 stats[10];
    u32 words[8];
};

struct ZoidStatState {
    u8 zoid_id;
    u8 pad01[0x0F];
    s16 level;
    u8 bonus_a[6];
    u8 bonus_b[0x1E];
    u8 base_flags[4];
    u16 stats[10];
    u32 words[8];
};

extern struct ZoidBaseStats gZoidBaseStatTable[];

u16 ScaleByPercent(s16, s32) asm("func_080E522C");
u8 GetZoidFormIndex(u8) asm("func_080E523C");

void LoadZoidBaseStats(struct ZoidStatState *zoid)
{
    struct ZoidBaseStats *config;
    u8 i;

    config = &gZoidBaseStatTable[zoid->zoid_id];
    asm volatile("" : : "r"(config));
    asm volatile("" : : "r"(config));
    asm volatile("" : : "r"(config));
    zoid->base_flags[0] = config->bytes[0];
    zoid->base_flags[1] = config->bytes[1];
    zoid->base_flags[2] = config->bytes[2];
    zoid->base_flags[3] = config->bytes[3];

    i = 4;
    do {
        zoid->words[i] = config->words[i];
        i++;
    } while (i <= 7);

    zoid->stats[ZOID_STAT_MAX_HP] = config->stats[ZOID_STAT_MAX_HP];
    zoid->stats[ZOID_STAT_DCP] = config->stats[ZOID_STAT_DCP];
    zoid->stats[ZOID_STAT_MAX_EP] = config->stats[ZOID_STAT_MAX_EP];
    {
        register u16 *ep_regen asm("r5");
        register u16 *speed asm("r9");
        register u16 *mobility asm("r8");
        register u16 *defense asm("r4");
        register u16 *load_capacity asm("r6");
        register s32 scale asm("r4");
        u8 bonus_index;
        u16 value;
        register u16 captured_value asm("r0");

        value = config->stats[ZOID_STAT_EP_REGEN];
        ep_regen = &zoid->stats[ZOID_STAT_EP_REGEN];
        asm volatile("" : "+r"(ep_regen));
        *ep_regen = value;
        captured_value = config->stats[ZOID_STAT_SPEED];
        asm volatile("" : "+r"(captured_value));
        speed = &zoid->stats[ZOID_STAT_SPEED];
        *speed = captured_value;
        captured_value = config->stats[ZOID_STAT_MOBILITY];
        asm volatile("" : "+r"(captured_value));
        mobility = &zoid->stats[ZOID_STAT_MOBILITY];
        *mobility = captured_value;
        value = config->stats[ZOID_STAT_DEFENSE];
        defense = &zoid->stats[ZOID_STAT_DEFENSE];
        asm volatile("" : "+r"(defense));
        *defense = value;
        zoid->stats[ZOID_STAT_ARMOR_RATE] = config->stats[ZOID_STAT_ARMOR_RATE];
        zoid->stats[ZOID_STAT_SENSOR_ACCURACY] = config->stats[ZOID_STAT_SENSOR_ACCURACY];
        value = config->stats[ZOID_STAT_LOAD_CAPACITY];
        load_capacity = &zoid->stats[ZOID_STAT_LOAD_CAPACITY];
        asm volatile("" : "+r"(load_capacity));
        *load_capacity = value;

        bonus_index = GetZoidFormIndex(zoid->zoid_id);
        *ep_regen += zoid->bonus_a[bonus_index];
        {
            register u32 bonus asm("r0") =
                zoid->bonus_b[bonus_index] * 5;
            register u32 current asm("r2") = *defense;

            asm volatile("" : "+r"(bonus));
            asm volatile("" : "+r"(current));
            *defense = bonus + current;
        }

        {
            register s32 phase1 asm("r1");
            register s32 phase2 asm("r2");

            asm volatile("" : "=r"(phase1));
            asm volatile("" : "=r"(phase2));
            scale = zoid->level;
            asm volatile("" : : "r"(phase1), "r"(phase2));
        }
        scale = (s32)(scale + ((u32)scale >> 31)) >> 1;
        scale += 100;
        asm volatile("" : "+r"(scale));

        zoid->stats[ZOID_STAT_MAX_HP] = ScaleByPercent((s16)zoid->stats[ZOID_STAT_MAX_HP], scale);
        {
            register s32 phase asm("r1");

            asm volatile("" : "=r"(phase));
            zoid->stats[ZOID_STAT_DCP] = ScaleByPercent((s16)zoid->stats[ZOID_STAT_DCP],
                ({ asm volatile("" : : "r"(phase)); scale; }));
        }
        zoid->stats[ZOID_STAT_MAX_EP] = ScaleByPercent((s16)zoid->stats[ZOID_STAT_MAX_EP], scale);
        *ep_regen = ScaleByPercent((s16)*ep_regen, scale);
        *speed = ScaleByPercent(
            ({
                register u16 *view asm("r1") = speed;

                asm volatile("" : "+r"(view));
                *(s16 *)view;
            }), scale);
        {
            register u16 result asm("r0");

            result = ScaleByPercent(
                ({
                    register u16 *view asm("r2") = mobility;

                    asm volatile("" : "+r"(view));
                    *(s16 *)view;
                }), scale);
            {
                register u16 *view asm("r2") = mobility;

                asm volatile("" : "+r"(view));
                *view = result;
            }
        }
        {
            register s32 phase asm("r2");

            asm volatile("" : "=r"(phase));
            *load_capacity = ScaleByPercent((s16)*load_capacity,
                ({ asm volatile("" : : "r"(phase)); scale; }));
        }
    }
}
