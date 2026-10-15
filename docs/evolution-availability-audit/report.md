# Evolution and required-item availability audit

399 active species; 206 configured evolution links. All configured endpoints are active and methods have engine support.

## Restored this pass

- Tynamo → Eelektrik at level 39; Eelektrik → Eelektross with Thunder Stone.
- Mime Jr. → Mr. Mime when levelling up knowing Mimic; added Mimic at level 15 alongside its existing move.
- Female Combee → Vespiquen at level 21; male/genderless Combee do not evolve.
- Alolan Rattata → Raticate at level 20 at night; Alolan Cubone → Marowak at level 28 at night.
- Alolan Pichu → Pikachu and Alolan Meowth → Persian through friendship.
- Alolan Diglett → Dugtrio (26), Geodude → Graveler (25), Graveler → Golem (trade), Grimer → Muk (38).
- Galarian Meowth → Perrserker (28), Corsola → Cursola (38), Koffing → Weezing (35).

Custom regional pre-evolutions stay in their own form lines. Standard methods cross-checked against the [PokeAPI evolution dataset](https://raw.githubusercontent.com/PokeAPI/pokeapi/master/data/v2/csv/pokemon_evolution.csv). Friendship uses the existing project threshold of 220.

## Remaining decisions and limitations

- Piloswine → Mamoswine remains deliberately disabled in Kanto. Apex discovery, habitat and Mewtwo conditions remain unchanged.
- Ordinary Feebas evolves into Milotic on level-up with friendship at least 220, at any time of day. This replaces the inaccessible Beauty requirement; Deep Form Feebas remains unchanged.
- Galarian Farfetch’d has no Sirfetch’d species in this roster; adding that evolution is a roster/design decision.
- Custom species relationships beyond configured links cannot be inferred from official ancestry data.
- Item evidence below includes scripts, map items, configured in-game trades and wild held items. Unreachable scripts, prerequisite flags, trade partners, transfer access and limited item quantities still need live verification.
- This is not a claim that every active species is obtainable. Non-native species can be intentionally unavailable; trainer appearances are not acquisition routes.

## Required items with no acquisition evidence

5 items: DAWN STONE, DUSK STONE, ICE STONE, RAZOR CLAW, SHINY STONE.

No new item placements or shops were added. Newer evolution items also retain question-mark menu icons.

## Item-source evidence

### DAWN_STONE

No acquisition source found.

### DEEP_SEA_SCALE

- `src/data/ingame_trades.h:236: .heldItem = ITEM_DEEP_SEA_SCALE,`

### DEEP_SEA_TOOTH

- `src/data/ingame_trades.h:252: .heldItem = ITEM_DEEP_SEA_TOOTH,`

### DRAGON_SCALE

- `src/data/ingame_trades.h:140: .heldItem = ITEM_DRAGON_SCALE,`

### DUBIOUS_DISC

- `data/maps/RocketLeague_Lobby/scripts.inc:249: giveitem ITEM_DUBIOUS_DISC`

### DUSK_STONE

No acquisition source found.

### ELECTIRIZER

- `src/data/ingame_trades.h:108: .heldItem = ITEM_ELECTIRIZER,`

### FIRE_STONE

- `data/maps/CeladonCity_DepartmentStore_4F/scripts.inc:30: .2byte ITEM_FIRE_STONE`

### ICE_STONE

No acquisition source found.

### KINGS_ROCK

- `src/data/ingame_trades.h:43: .heldItem = ITEM_KINGS_ROCK,`
- `src/data/ingame_trades.h:172: .heldItem = ITEM_KINGS_ROCK,`

### LEAF_STONE

- `data/scripts/item_ball_scripts.inc:138: finditem ITEM_LEAF_STONE`
- `data/maps/CeladonCity_DepartmentStore_4F/scripts.inc:33: .2byte ITEM_LEAF_STONE`
- `data/maps/SafariZone_Center/events.inc:27: bg_hidden_item_event 27, 15, 3, ITEM_LEAF_STONE, FLAG_HIDDEN_ITEM_SAFARI_ZONE_CENTER_LEAF_STONE, 1, FALSE`
- `data/maps/SafariZone_Center/map.json:141: "item": "ITEM_LEAF_STONE",`

### MAGMARIZER

- `src/data/ingame_trades.h:188: .heldItem = ITEM_MAGMARIZER,`

### METAL_COAT

- `src/data/ingame_trades.h:91: .heldItem = ITEM_METAL_COAT,`
- `src/data/ingame_trades.h:124: .heldItem = ITEM_METAL_COAT,`

### MOON_STONE

- `data/scripts/item_ball_scripts.inc:42: finditem ITEM_MOON_STONE`
- `data/maps/MtMoon_B2F/events.inc:21: bg_hidden_item_event 20, 16, 3, ITEM_MOON_STONE, FLAG_HIDDEN_ITEM_MT_MOON_B2F_MOON_STONE, 1, FALSE`
- `data/maps/PokemonMansion_1F/events.inc:29: bg_hidden_item_event 2, 21, 0, ITEM_MOON_STONE, FLAG_HIDDEN_ITEM_POKEMON_MANSION_1F_MOON_STONE, 1, FALSE`
- `data/maps/MtMoon_B2F/map.json:126: "item": "ITEM_MOON_STONE",`
- `data/maps/PokemonMansion_1F/map.json:190: "item": "ITEM_MOON_STONE",`
- `Wild held item: CLEFAIRY`
- `Wild held item: CLEFABLE`
- `Wild held item: CLEFFA`

### PROTECTOR

- `src/data/ingame_trades.h:59: .heldItem = ITEM_PROTECTOR,`

### RAZOR_CLAW

No acquisition source found.

### REAPER_CLOTH

- `src/data/ingame_trades.h:204: .heldItem = ITEM_REAPER_CLOTH,`

### SHINY_STONE

No acquisition source found.

### SUN_STONE

- `data/maps/Route1/events.inc:18: bg_hidden_item_event 5, 7, 3, ITEM_SUN_STONE, FLAG_HIDDEN_ITEM_PEWTER_CITY_POKE_BALL, 1, FALSE`
- `data/maps/Route1/map.json:144: "item": "ITEM_SUN_STONE",`
- `Wild held item: SOLROCK`

### THUNDER_STONE

- `data/scripts/item_ball_scripts.inc:234: finditem ITEM_THUNDER_STONE`
- `data/maps/CeladonCity_DepartmentStore_4F/scripts.inc:31: .2byte ITEM_THUNDER_STONE`
- `data/maps/PowerPlant/events.inc:34: bg_hidden_item_event 8, 12, 3, ITEM_THUNDER_STONE, FLAG_HIDDEN_ITEM_POWER_PLANT_THUNDER_STONE, 1, FALSE`
- `data/maps/PowerPlant/map.json:279: "item": "ITEM_THUNDER_STONE",`

### UP_GRADE

- `data/maps/CinnabarIsland_PokemonLab_ResearchRoom/scripts.inc:29: giveitem ITEM_UP_GRADE`

### WATER_STONE

- `data/scripts/item_ball_scripts.inc:426: finditem ITEM_WATER_STONE`
- `data/maps/CeladonCity_DepartmentStore_4F/scripts.inc:32: .2byte ITEM_WATER_STONE`
- `data/maps/SeafoamIslands_B4F/events.inc:15: bg_hidden_item_event 13, 8, 0, ITEM_WATER_STONE, FLAG_HIDDEN_ITEM_SEAFOAM_ISLANDS_B4F_WATER_STONE, 1, FALSE`
- `data/maps/SeafoamIslands_B4F/map.json:64: "item": "ITEM_WATER_STONE",`

## Standard ancestry pairs without a configured link

- PILOSWINE → MAMOSWINE

## Validation

Host-side tests execute the production evolution function and table, checking thresholds, time, gender, all four move slots, Everstone, stones and trades, plus existing Eevee/Apex regression cases. Main-ROM compilation is separate from this static report.
