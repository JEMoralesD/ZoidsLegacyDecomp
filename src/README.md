# Source layout

Start with [RunGameStateMachine](game/run_game_state_machine.c) to follow game modes.
See [RunEventScripts](events/run_event_scripts.c) for event commands and [battle.h](battle/battle.h) for battle layouts and effects.
See [window.h](ui/window.h) for text formats and window fields, and [tasks.h](engine/tasks.h) for scheduler contexts.
See [battle_display.h](battle/battle_display.h) for unit sprites and gauges, and [camera.h](graphics/camera.h) for projection.
Follow [queued battle results](battle/present_queued_battle_effect_results.c) for popups, camera positioning, and destruction handling.
[Story result checks](battle/check_battle_result_and_play_story_scenes.c) handle special endings and dialogue triggers.
[Pulse effect growth](zoids/update_pulse_effects_from_emotion_growth.c) uses four emotion tracks and [effect admission](zoids/add_pulse_effect.c).
See [battle_animation.h](battle/battle_animation.h) for animation scripts, resource headers, effect phases, and projectile fields.
Its effect callbacks also cover dust, shields, sweeping windows, falling streaks, and looping background sprites.
See [screen_effects.h](graphics/screen_effects.h) for scanline masks and screen transition state.
See [field_display.h](field/field_display.h) for map rendering, decoration sprites, encounters, and scanline state.
See [name_entry.h](ui/name_entry.h) for character tables, input, and the player name and battle quote editors.
See [player_selection.h](ui/player_selection.h) for team lists, equipment details, repairs, and menu phases.
[Zoid customization](ui/run_zoid_customization_menu.c) covers power, EP regeneration, defense bonuses, and palette selection.
[Battle combinations](events/commands/event_combine_battle_zoids.c) connect formation checks to departure, animation, and arrival.

| Directory | Contents |
| --- | --- |
| `game/` | Main state machine, mode IDs, and mode handlers in `modes/` |
| `battle/` | Unit state, effects, recovery items, and battle stat calculations |
| `events/` | Event interpreter, flags, opcode handlers in `commands/`, and scripted scenes in `scenes/` |
| `field/` | Map transitions, tile streaming, actors, decorations, and random encounters |
| `zoids/` | Zoid forms, base stats, equipment bonuses, and abilities |
| `ui/` | Windows, tile allocation, and menu scripts |
| `graphics/` | Sprites, scanline events, HBlank callbacks, and graphics transfers |
| `audio/` | Songs, music players, sequence commands, and sound driver assembly |
| `save/` | Save blocks and SRAM access |
| `engine/` | Startup, frame updates, task scheduling, input, deferred callbacks, and fixed-point math |
| `runtime/` | BIOS calls, startup, interrupt dispatch, and compiler support |
| `unnamed/` | Assembly fragments that preserve matching compiler output |

The [source map](../source-map.csv) links original filenames to current paths, ROM addresses, region IDs, and function names.
Headers have no ROM region. Assembly fragments list their owning region.
Each region keeps its own compilation unit to preserve its byte match.
The remaining assembly fragments preserve compiler output for menus and battle routines.
