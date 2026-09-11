# Trainer class placement audit

Counts represent script-reachable encounter sites in compiled maps, grouped by the project's region-map sections (with the S.S. Anne, Silph Co. and Pokémon/Rocket Leagues separated). Map objects, coordinate/background triggers and map callbacks are traversed through call/goto branches.

Trainer aliases are resolved. Double-battle partners sharing a fight count once. Badge/starter/story-dependent rosters at the same trigger count as one site per class/variant. Rematch commands are excluded. Conditional appearances are potential encounters, not a claim that every branch is available simultaneously. Script commands without a literal trainer ID are reported separately; battles selected solely in C/procedural facilities are outside this static script audit.

Rows follow the displayed trainer class, with M/F battle-sprite variants separated for shared classes. Sprite gender falls back to the female trainer flag where the picture name has no gender suffix. Original encoded class IDs remain in each encounter record. Duplicate legacy class labels are merged. Zero rows include declared classes with no script-reachable encounter. Only areas containing an encounter appear in the heatmap; empty areas are listed in the JSON.

| Class | Sites | Areas |
| --- | ---: | --- |
| Ace Trainer F | 7 | Route 23 (3), Kanto Victory Road (4) |
| Ace Trainer M | 7 | Route 23 (3), Kanto Victory Road (4) |
| Aces | 3 | Route 23 (1), Kanto Victory Road (2) |
| Aroma Lady | 11 | Route 5 (1), Route 8 (1), Route 11 (2), Route 25 (3), Fuchsia Forest (1), Viridian Forest (3) |
| Biker | 15 | Route 13 (2), Route 14 (2), Route 15 (2), Route 16 (2), Route 17 (5), Route 18 (2) |
| Bird Keeper | 10 | Route 13 (2), Route 14 (2), Route 15 (4), Route 22 (1), Route 23 (1) |
| Black Belt F | 7 | Route 9 (1), Route 23 (1), Cinnabar Volcano (1), Kanto Victory Road (1), Rock Tunnel (1), Saffron City (2) |
| Black Belt M | 7 | Route 9 (1), Cinnabar Volcano (1), Kanto Victory Road (1), Rock Tunnel (1), Saffron City (3) |
| Boss | 1 | Silph Co. (1) |
| Bug Catcher | 14 | Route 2 (3), Route 5 (2), Fuchsia Forest (1), Mt. Moon (2), Viridian Forest (6) |
| Bug Maniac | 8 | Route 2 (1), Route 5 (2), Route 11 (3), Fuchsia Forest (1), Viridian Forest (1) |
| Burglar | 7 | Pokémon Mansion (7) |
| Champion | 2 | Pokémon League (1), Rocket League (1) |
| Channeler | 9 | Route 3 (1), Route 4 (1), Route 7 (1), Route 8 (1), Pokémon Tower (5) |
| Crush Kin | 0 |  |
| Electrician | 4 | Power Plant (4) |
| Elite Four | 3 | Pokémon League (3) |
| Engineer | 3 | Power Plant (3) |
| Expert F | 3 | Cinnabar Volcano (1), Kanto Victory Road (1), Saffron City (1) |
| Expert M | 3 | Cinnabar Volcano (1), Kanto Victory Road (1), Saffron City (1) |
| Fisherman | 20 | Route 6 (1), Route 10 (2), Route 12 (6), Route 21 (4), Route 24 (1), Route 25 (4), Vermilion Harbor (2) |
| Gambler | 0 |  |
| Gentleman | 4 | S.S. Anne (4) |
| Hiker | 20 | Route 3 (1), Route 9 (3), Route 10 (2), Route 13 (1), Route 14 (1), Route 15 (2), Route 22 (1), Route 23 (1), Cinnabar Volcano (2), Rock Tunnel (6) |
| Kindler | 4 | Cinnabar Volcano (4) |
| Lady | 2 | S.S. Anne (2) |
| Leader | 10 | Cerulean City (1), Cinnabar Island (1), Cinnabar Volcano (1), Pewter City (1), Pokémon League (1), Pokémon Tower (1), Saffron City (1), Vermilion City (1), Viridian City (1), Viridian Forest (1) |
| Ninja | 0 |  |
| Player | 0 |  |
| Pokéfan F | 3 | Underground Path (3) |
| Pokéfan M | 3 | Underground Path (3) |
| Pokémaniac | 4 | Underground Path (4) |
| Pokémon Breeder F | 7 | Route 5 (1), Route 8 (1), Route 11 (1), Route 25 (3), Fuchsia Forest (1) |
| Pokémon Breeder M | 2 | Route 2 (1), Viridian Forest (1) |
| Pokémon Ranger F | 3 | Route 2 (1), Route 5 (1), Viridian Forest (1) |
| Pokémon Ranger M | 2 | Route 11 (1), Fuchsia Forest (1) |
| Psychic F | 0 |  |
| Psychic M | 4 | Route 5 (1), Route 6 (1), Route 7 (1), Route 8 (1) |
| Rival Early | 2 | Celadon City (1), Pallet Town (1) |
| Rival Late | 3 | Pokémon Mansion (1), Pokémon Tower (1), Silph Co. (1) |
| Rocket Ace F | 1 | Rocket League (1) |
| Rocket Ace M | 1 | Rocket League (1) |
| Rocket Admin | 1 | Rocket League (1) |
| Roughneck | 10 | Route 15 (1), Route 16 (2), Route 17 (5), Route 18 (2) |
| Ruin Maniac | 15 | Route 4 (1), Route 9 (1), Route 11 (1), Cerulean Cave (3), Cinnabar Volcano (1), Kanto Victory Road (4), Mt. Moon (2), Rock Tunnel (2) |
| Sailor | 16 | Route 6 (1), Route 10 (2), Route 12 (1), Route 20 (2), S.S. Anne (7), Vermilion Harbor (3) |
| Scientist | 9 | Pokémon Mansion (7), Power Plant (2) |
| Scout F | 16 | Route 1 (1), Route 2 (2), Route 4 (1), Route 5 (1), Route 7 (1), Route 8 (2), Route 9 (1), Route 15 (1), Route 22 (1), Route 24 (1), Route 25 (2), Cinnabar Volcano (1), Fuchsia Forest (1) |
| Scout M | 19 | Route 1 (2), Route 2 (2), Route 5 (2), Route 8 (2), Route 9 (1), Route 11 (1), Route 13 (1), Route 14 (1), Route 15 (1), Route 22 (1), Route 25 (2), Cinnabar Volcano (1), Fuchsia Forest (1), Viridian Forest (1) |
| Super Nerd | 2 | Underground Path (2) |
| Swim Sibs | 3 | Route 19 (1), Route 20 (1), Route 21 (1) |
| Swimmer F | 15 | Route 10 (3), Route 12 (3), Route 19 (2), Route 20 (3), Route 21 (2), Route 24 (1), Vermilion Harbor (1) |
| Swimmer M | 15 | Route 10 (1), Route 19 (4), Route 20 (5), Route 21 (4), Vermilion Harbor (1) |
| Team Rocket F | 0 |  |
| Team Rocket M | 20 | Celadon City (1), Silph Co. (19) |
| Trendsetter F | 3 | Route 7 (1), Celadon City (1), S.S. Anne (1) |
| Trendsetter M | 3 | Route 7 (1), S.S. Anne (2) |
| Trendsetters | 3 | Route 16 (1), Celadon City (1), S.S. Anne (1) |
| Triathlete F | 24 | Route 12 (2), Route 13 (2), Route 14 (2), Route 15 (2), Route 16 (2), Route 17 (5), Route 18 (2), Route 20 (2), Route 22 (1), Route 24 (1), Route 25 (1), Vermilion Harbor (2) |
| Triathlete M | 8 | Route 17 (5), Route 22 (1), Route 25 (1), Vermilion Harbor (1) |
| Tuber F | 6 | Route 6 (1), Route 10 (1), Route 12 (1), Route 20 (1), Route 24 (1), Vermilion Harbor (1) |
| Tuber M | 7 | Route 6 (1), Route 10 (1), Route 12 (2), Route 20 (1), Route 21 (1), Vermilion Harbor (1) |
| Twins | 0 |  |
| Youngster F | 4 | Route 1 (1), Route 3 (1), Route 4 (1), Route 7 (1) |
| Youngster M | 4 | Route 1 (1), Route 3 (1), Route 4 (1), Route 7 (1) |

## Coverage

{"maps": 253, "classes": 65, "areas": 49, "encounter_sites": 422, "unplaced_classes": 7, "conditional_sites": 19}

Unresolved map-root scripts: 0. Dynamic battle references: 0. Duplicate trainer-table numeric entries: 0 (last definition wins, matching C).

Exact maps, trainer names, variant IDs, source locations and shared flags are retained in trainer-class-heatmap.json.
