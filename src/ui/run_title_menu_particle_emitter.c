#include "m2c_prelude.h"
#include "title_menu.h"


struct TitleParticlePositionView *CreateSprite(s32, s32, s32, s32) asm("func_08094484");
u32 CallFunctionR0(u32) asm("func_080ECD5C");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

extern u32 gTitleParticleSpawnInterval asm("D_020216F0");
extern struct TitleParticlePositionView *gTitleParticles[16] asm("D_020216B0");
extern u32 gRandomCallback asm("D_03000010");

void RunTitleMenuParticleEmitter(void) asm("func_0809B9E4");

void RunTitleMenuParticleEmitter(void)
{
    volatile s32 reserve0;
    volatile s32 reserve1;
    volatile s32 reserve2;
    volatile s32 reserve3;
    volatile s32 reserve4;
    register s32 next_spawn_tick asm("r6") = 0;
    register s32 elapsed_updates asm("r4") = 0;

    asm volatile("" : "=m"(reserve0), "=m"(reserve1),
                       "=m"(reserve2), "=m"(reserve3),
                       "=m"(reserve4));
    gTitleParticleSpawnInterval = 0x60;

update_emitter:
    {
        s32 next_update = elapsed_updates + 1;

        if (elapsed_updates == next_spawn_tick) {
            register u32 particle_slot_index asm("r2") = 0;
            register struct TitleParticlePositionView **particle_slots asm("r4") = gTitleParticles;
            register u32 active_flag asm("r3") = 1;

            do {
                register u32 slot_offset asm("r0") = particle_slot_index << 2;
                register struct TitleParticlePositionView **particle_slot asm("r0");
                struct TitleParticlePositionView *existing_particle;

                particle_slot = (struct TitleParticlePositionView **)(slot_offset + (u32)particle_slots);
                existing_particle = *particle_slot;
                if (existing_particle != 0) {
                    u32 active_bits = existing_particle->flags & active_flag;

                    if (active_bits == 0) {
                        *particle_slot = (struct TitleParticlePositionView *)active_bits;
                    }
                }
                {
                    register u32 next_slot_bits asm("r0") = particle_slot_index + 1;

                    next_slot_bits <<= 24;
                    particle_slot_index = next_slot_bits >> 24;
                }
            } while (particle_slot_index <= 15U);

            particle_slot_index = 0;
            {
                register struct TitleParticlePositionView **particle_slots_base asm("r1") = gTitleParticles;
                register struct TitleParticlePositionView **particle_slot asm("r4");
                register struct TitleParticlePositionView *existing_particle asm("r5");
find_free_slot:
                {
                    register u32 slot_offset asm("r0") = particle_slot_index << 2;

                    particle_slot = (struct TitleParticlePositionView **)(slot_offset + (u32)particle_slots_base);
                    existing_particle = *particle_slot;
                    if (existing_particle == 0) {
                        goto create_particle;
                    }
                    {
                        register u32 next_slot_bits asm("r0") = particle_slot_index + 1;

                        next_slot_bits <<= 24;
                        particle_slot_index = next_slot_bits >> 24;
                    }
                    if (particle_slot_index <= 15U) {
                        goto find_free_slot;
                    }
                }

schedule_next_particle:
                {
                    register u32 *spawn_interval_address asm("r1") = &gTitleParticleSpawnInterval;
                    register u32 spawn_interval asm("r0") = *spawn_interval_address;

                    if (spawn_interval > 0x10U) {
                        next_spawn_tick += spawn_interval;
                        spawn_interval -= 0x10;
                        *spawn_interval_address = spawn_interval;
                        goto advance_update;
                    }
                    goto schedule_minimum_interval;
                }

create_particle:
                {
                    register volatile s32 *sprite_constructor_stack asm("sp");

                    sprite_constructor_stack[0] = (s32)existing_particle;
                    sprite_constructor_stack[1] = TITLE_PARTICLE_TILE_OFFSET;
                    sprite_constructor_stack[2] = 0xB;
                    sprite_constructor_stack[3] = 0x440;
                    sprite_constructor_stack[4] = TITLE_PARTICLE_UPDATE_CALLBACK;
                    *particle_slot = CreateSprite(TITLE_PARTICLE_FRAMES_ROM, TITLE_PARTICLE_ANIMATIONS_ROM, 0, 0);
                    {
                        register u32 random asm("r0") = CallFunctionR0(gRandomCallback);
                        register struct TitleParticlePositionView *particle asm("r2") = *particle_slot;

                        particle->x_fixed8 = ((random * 0x401) >> 15) + 0x7A00;
                        particle->y_fixed8 = 0x5000;
                        particle->phase = (s32)existing_particle;
                    }
                    goto schedule_next_particle;
                }
            }
schedule_minimum_interval:
            next_spawn_tick += 0x10;
        }
advance_update:
        elapsed_updates = next_update;
    }
    YieldTaskForUpdates(1);
    goto update_emitter;
}
