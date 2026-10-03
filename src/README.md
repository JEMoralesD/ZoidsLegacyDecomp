# Source layout

Start with [RunGameStateMachine](game/run_game_state_machine.c) to follow game modes.
See [RunEventScripts](events/run_event_scripts.c) for event commands and [battle.h](battle/battle.h) for battle layouts and effects.

| Directory | Contents |
| --- | --- |
| `game/` | Main state machine, mode IDs, and mode handlers in `modes/` |
| `battle/` | Unit state, effects, recovery items, and battle stat calculations |
| `events/` | Event interpreter, flags, camera control, and opcode handlers in `commands/` |
| `zoids/` | Zoid forms, base stats, equipment bonuses, and abilities |
| `ui/` | Windows, tile allocation, and menu scripts |
| `graphics/` | Sprites, scanline events, HBlank callbacks, and graphics transfers |
| `audio/` | Songs, music players, sequence commands, and sound driver assembly |
| `save/` | Save blocks and SRAM access |
| `engine/` | Input, deferred callbacks, and fixed-point math |
| `runtime/` | BIOS calls, startup, interrupt dispatch, and compiler support |
| `unnamed/` | Address-named routines and their assembly fragments |

The [source map](../source-map.csv) links original filenames to current paths, ROM addresses, region IDs, and function names.
Headers have no ROM region. Assembly fragments list their owning region.
Each region keeps its own compilation unit to preserve its byte match.
Unnamed files keep their address names until their behavior is understood.
