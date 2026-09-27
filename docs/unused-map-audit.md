# Unused map cleanup

Removed seven retired map folders and their scripts, events and dedicated map graphics:

- Route12_FishingHouse
- Route16_House
- Route21_South
- SeafoamIslands_B1F
- SeafoamIslands_B3F
- PokemonLeague_LoreleisRoom
- PokemonLeague_LancesRoom

BattleColosseum_2P and BattleColosseum_4P remain intact. There are now 246 map folders.

The old numeric map slots are reserved in map_groups.json and redirect to active map headers, preserving subsequent IDs. Retired layout IDs alias existing layouts. Save loading migrates retired locations to their replacement areas; this migration has not been tested live. Route21_South remains an encounter-table key for the merged southern waters, and its active ferry scripts now live in Route21_North.

The main ROM builds successfully. Registered map indices, remaining layout file references and colosseum retention were checked. The earlier static reachability audit reached 244 maps; the two colosseums are entered through multiplayer code. This is not a live traversal audit.

An unrelated navigation check flags the user-edited Route 2 channel sign at x=38 outside its 38-tile-wide map. Its placement was left unchanged.

Route 21 connection/event checks pass. The older terrain hash check fails against the current Route 18 terrain; this cleanup did not edit its map binary. That terrain was preserved.
