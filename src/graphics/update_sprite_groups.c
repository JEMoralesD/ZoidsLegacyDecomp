#include "m2c_prelude.h"
extern u8 gSpriteGroupPool[];
typedef void (*SpriteGroupCallback)(void *);
void CallFunctionR1(void *, SpriteGroupCallback) asm("func_080ECD60");

void UpdateSpriteGroups(void) {
    u8 group_index;
    u8 *group;
    u8 *sprite_slots;
    u8 sprite_index;
    u32 *sprite_address;

    for (group_index = 0; group_index <= 15; group_index++) {
        group = gSpriteGroupPool + group_index * 180;
        if (*(u32 *)group & 1) {
            sprite_index = 0;
            sprite_slots = group + 12;
            do {
                sprite_address = (u32 *)(sprite_slots + sprite_index * 4);
                if (*sprite_address != 0 && !(*(u32 *)*sprite_address & 1)) {
                    *sprite_address = 0;
                }
                sprite_index++;
            } while (sprite_index <= 31);
        }
    }
    for (group_index = 0; group_index <= 15; group_index++) {
        group = gSpriteGroupPool + group_index * 180;
        if (*(u32 *)group & 1) {
            {
                SpriteGroupCallback callback;
                callback = (SpriteGroupCallback)*(u32 *)(group + 172);
                if (callback != 0) {
                    CallFunctionR1(group, callback);
                    *(u32 *)(group + 172) = 0;
                }
            }
            {
                SpriteGroupCallback callback;
                callback = (SpriteGroupCallback)*(u32 *)(group + 176);
                if (callback != 0) {
                    CallFunctionR1(group, callback);
                }
            }
        }
    }
}
