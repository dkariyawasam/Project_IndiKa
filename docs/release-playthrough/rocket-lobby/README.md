# Rocket League lobby live check — 11 October 2026

Added three stationary visitors (ten NPCs total), each with unique dialogue: a Rocket tactician, a Porygon prize seeker, and a Game Corner spectator. They occupy side areas without changing service positions, warps or the arena movement scripts.

Tested in mGBA using the rebuilt ROM and a disposable copy of the earned save. Debug placement was used to refresh the lobby and position the fixture between checks; interactions, walking and transitions used normal inputs. This is feature QA, not earned campaign progress.

Verified:
- All three new conversations open, finish and release controls.
- Item/TM and Pokémon coin shops open and close normally. Purchases were not repeated in this pass.
- Nurse healing completes and returns control.
- PC selection menu opens and closes.
- Receptionist entry and the automatic entry prompt both lead into the arena; walking out returns to the lobby.
- Stairs connect to the Game Corner and back using normal movement.
- Lounge furniture, plants, floor emblem and visible NPCs render correctly after entering the map.

Loading an old save directly in this room initially retained the saved map-view tiles; re-entering refreshed the new layout. The earned campaign save SHA-256 was unchanged. Screenshots record the sampled checks, not exhaustive frame-by-frame coverage.

## Arena return walk

The lobby recognises arrival at the north arena exit (9,2), suppresses the entry trigger during a scripted walk, and now stops the player at (10,8), facing the receptionist, to the right of the rules/betting board. It opens no dialogue and re-enables normal arena entry afterwards. The earlier nurse destination was live-tested, then corrected at the user's request. The corrected six-down, one-right route passed live on 11 October: normal movement out of the arena ended at (10,8) with controls unlocked and no dialogue; a subsequent down input reached (10,9).
