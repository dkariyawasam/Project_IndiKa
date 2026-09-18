# Pokémon overworld usage audit

Audited 18 September 2026. Updated after requested removal of 16 unused overworlds.

There are now **39 Pokémon overworld sheets: 33 used and 6 unused**. Removed Voltorb, Lapras, Mew and the 13 recently imported overworlds. Battle assets and menu icons were preserved.

## Unused overworlds

Entei, Suicune, Raikou, Lugia, Ho-Oh, Celebi.

## Scope and checks

Scanned map JSON, generated map events, scripts and C source for exact graphics IDs. Excluded asset declarations, graphics tables and text-colour metadata from usage counts. Checked direct graphics-symbol references, numeric map graphics assignments, dynamic graphics-variable setters and object-graphics changes. No numeric map placements or numeric script assignments were found hiding additional uses. This is a source-reference audit, not a live campaign playthrough.

Omanyte and Kabuto are selected dynamically in Fuchsia City and are used. Voltorb and Lapras picture-display scripts use battle pictures, not these overworld sprites. The separate Lapras doll is not part of the Lapras Pokémon sheet. The three leftover Deoxys graphics IDs point to ItemBall; no Deoxys Pokémon overworld sheet remains, so they are excluded. Battle sprites, menu icons, encounters and gifts do not themselves count as overworld usage.

## Used sprites and evidence

| Pokémon | Placement or dynamic reference |
|---|---|
| Spearow | `data/maps/ViridianCity_House/map.json` |
| Cubone | `data/maps/SaffronCity/map.json`, `data/maps/LavenderTown_House1/map.json` |
| Poliwrath | `data/maps/CeladonCity/map.json` |
| Clefairy | `data/maps/Route25_SeaCottage/map.json`, `data/maps/CeladonCity_Condominiums_1F/map.json` |
| Pidgeot | `data/maps/SaffronCity/map.json` |
| Jigglypuff | `data/maps/PewterCity_PokemonCenter_1F/map.json`, `data/maps/SaffronCity_CopycatsHouse_2F/map.json` |
| Pidgey | `data/maps/VermilionCity_House3/map.json`, `data/maps/SaffronCity_House/map.json` |
| Chansey | `data/maps/FuchsiaCity/map.json`, `data/maps/SaffronCity_CopycatsHouse_1F/map.json` |
| Omanyte | `data/maps/FuchsiaCity/scripts.inc` |
| Kangaskhan | `data/maps/FuchsiaCity/map.json` |
| Pikachu | `data/maps/VermilionCity_PokemonFanClub/map.json`, `data/maps/SaffronCity_CopycatsHouse_2F/map.json` |
| Psyduck | `data/maps/LavenderTown_VolunteerPokemonHouse/map.json` |
| Nidoran F | `data/maps/CeladonCity_Condominiums_1F/map.json` |
| Nidoran M | `data/maps/PewterCity_House1/map.json` |
| Nidorino | `data/maps/LavenderTown_VolunteerPokemonHouse/map.json` |
| Meowth | `data/maps/CeladonCity_Condominiums_1F/map.json` |
| Seel | `data/maps/VermilionCity_PokemonFanClub/map.json` |
| Slowpoke | `data/maps/FuchsiaCity/map.json` |
| Slowbro | `data/maps/CeruleanCity/map.json` |
| Machop | `data/maps/VermilionCity/map.json` |
| Wigglytuff | `data/maps/SSAnne_1F_Room3/map.json` |
| Doduo | `data/maps/SaffronCity_CopycatsHouse_2F/map.json` |
| Fearow | `data/maps/Route16_House/map.json` |
| Machoke | `data/maps/SSAnne_B1F_Room5/map.json` |
| Zapdos | `data/maps/PowerPlant/map.json` |
| Moltres | `data/maps/CinnabarVolcano_3F/map.json` |
| Articuno | `data/maps/SeafoamIslands_B4F/map.json` |
| Mewtwo | `data/maps/CeladonCave/map.json`, `data/maps/CeruleanCave_B1F/map.json`, `data/maps/Route7/map.json` |
| Kabuto | `data/maps/FuchsiaCity/scripts.inc` |
| Tangrowth | `data/maps/ViridianForest/scripts.inc`, `data/maps/ViridianForest/map.json` |
| Osscythe | `data/maps/PokemonTower_4F/map.json` |
| Annihilape | `data/maps/MtMoon_B2F/map.json` |
| Mime Sr. | `data/maps/DiglettsCave_B2F/map.json` |
