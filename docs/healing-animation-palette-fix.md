# Healing animation palette budget

The monitor and Poké Balls both use palette tag 0x1007. Removed the unused GeneralFieldEffect0 load from the healing script. Field-effect palette loaders now skip tint/weather processing if allocation fails instead of using palette index 255.

| Map | Dynamic NPC palettes | Including healing |
|---|---:|---:|
| CeladonCity_PokemonCenter_1F | 3 | 4/4 |
| CeruleanCity_PokemonCenter_1F | 2 | 3/4 |
| CinnabarIsland_PokemonCenter_1F | 3 | 4/4 |
| FuchsiaCity_PokemonCenter_1F | 2 | 3/4 |
| IndigoPlateau_PokemonCenter_1F | 2 | 3/4 |
| PewterCity_PokemonCenter_1F | 2 | 3/4 |
| RocketLeague_Lobby | 0 | 1/4 |
| SaffronCity_PokemonCenter_1F | 2 | 3/4 |
| VermilionCity_PokemonCenter_1F | 3 | 4/4 |
| ViridianCity_PokemonCenter_1F | 2 | 3/4 |

Main ROM built and overworld palette audit passed. This is a whole-map static budget check; live healing playback remains to be checked.
