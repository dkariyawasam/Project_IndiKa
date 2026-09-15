# Route and dungeon trainer-class review — 2026-09-15

Static source audit, not a live traversal. Refreshed script reachability and overworld reports cover 253 compiled maps and 438 encounter sites across the whole project. The table below covers Routes 1–25 and named exploration areas/facilities, including those with no scripted trainer battles. Counts exclude rematch commands; conditional sites need not coexist. Procedural league battles are outside this placement audit.

## Applied changes

1. Route 3 JANAE: Channeler → Scout F; replace the level 28–30 Tower-style ghost party with a route-appropriate party.
2. Route 4 ANGELICA: Channeler → Aroma Lady; align the team with her existing mushroom-hunting dialogue.
3. Mt. Moon KENT: Bug Catcher → Ruin Maniac; align his team with fossil-search dialogue.
4. Mt. Moon ROBBY: Bug Catcher → Hiker; his Geodude/Machop party already supports this role.

Applied with matching battle portraits, overworlds, encounter music and teams. Kent and Robby retain their identities on rematches; Robby now has a separate rematch ID rather than reusing Route 7 CHAD. The main ROM was rebuilt. Route 8 EMILIA now explains her journey to Lavender. Pokémon Breeder is female-only and unsuffixed; MILO/ELI are now MILA/ELIN, using the existing female artwork and corrected female flags.

## Area inventory

| Area | Scripted class sites | Assessment |
| --- | --- | --- |
| Route 1 | Scout F ×1, Scout M ×2, Youngster F ×1, Youngster M ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 2 | Bug Catcher ×3, Bug Maniac ×1, Pokémon Breeder ×1, Pokémon Ranger F ×1, Scout F ×2, Scout M ×2 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 3 | Hiker ×1, Scout F ×1, Youngster F ×1, Youngster M ×1 | JANAE is now Scout F with Pidgey Lv10 and Nidoran F Lv11. |
| Route 4 | Aroma Lady ×1, Ruin Maniac ×1, Scout F ×1, Youngster F ×1, Youngster M ×1 | ANGELICA is now Aroma Lady with Paras Lv12 and Oddish Lv13. |
| Route 5 | Aroma Lady ×1, Bug Catcher ×2, Bug Maniac ×2, Pokémon Breeder ×1, Pokémon Ranger F ×1, Psychic M ×1, Scout F ×1, Scout M ×2 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 6 | Fisherman ×1, Psychic M ×1, Sailor ×1, Tuber F ×1, Tuber M ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 7 | Psychic M ×1, Trendsetter F ×1, Trendsetter M ×1, Youngster F ×1, Youngster M ×1 | Psychic near Saffron, Trendsetters near Celadon and paired Youngsters are coherent. Removed Channeler/Scout F remain absent. |
| Route 8 | Aroma Lady ×1, Channeler ×1, Pokémon Breeder ×1, Psychic M ×1, Scout F ×2, Scout M ×2 | EMILIA remains a Channeler and now explains her journey to Lavender to pray for departed Pokémon. |
| Route 9 | Black Belt F ×1, Black Belt M ×1, Hiker ×3, Ruin Maniac ×1, Scout F ×1, Scout M ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 10 | Fisherman ×2, Hiker ×2, Sailor ×2, Swimmer F ×3, Swimmer M ×1, Tuber F ×1, Tuber M ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 11 | Aroma Lady ×2, Bug Maniac ×3, Pokémon Breeder ×1, Pokémon Ranger M ×1, Ruin Maniac ×1, Scout M ×1 | Ruin Maniac MARCOS is justified by dialogue studying stones at Diglett’s Cave. |
| Route 12 | Fisherman ×6, Sailor ×1, Swimmer F ×3, Triathlete F ×2, Tuber F ×1, Tuber M ×2 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 13 | Biker ×2, Bird Keeper ×2, Hiker ×1, Scout M ×1, Triathlete F ×2 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 14 | Biker ×2, Bird Keeper ×2, Hiker ×1, Scout M ×1, Triathlete F ×2 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 15 | Biker ×2, Bird Keeper ×4, Hiker ×2, Roughneck ×1, Scout F ×1, Scout M ×1, Triathlete F ×2 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 16 | Biker ×2, Roughneck ×2, Trendsetters ×1, Triathlete F ×2 | Trendsetters are on the Celadon approach; cycling classes fit the road. |
| Route 17 | Biker ×5, Roughneck ×5, Triathlete F ×5, Triathlete M ×5 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 18 | Biker ×2, Roughneck ×2, Triathlete F ×2 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 19 | Swim Sibs ×1, Swimmer F ×2, Swimmer M ×4 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 20 | Sailor ×2, Swim Sibs ×1, Swimmer F ×3, Swimmer M ×5, Triathlete F ×2, Tuber F ×1, Tuber M ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 21 | Fisherman ×4, Swim Sibs ×1, Swimmer F ×2, Swimmer M ×4, Tuber M ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 22 | Bird Keeper ×1, Hiker ×1, Scout F ×1, Scout M ×1, Triathlete F ×1, Triathlete M ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 23 | Ace Trainer F ×3, Ace Trainer M ×3, Aces ×1, Bird Keeper ×1, Black Belt F ×1, Hiker ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 24 | Fisherman ×1, Scout F ×1, Swimmer F ×1, Triathlete F ×1, Tuber F ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Route 25 | Aroma Lady ×3, Fisherman ×4, Pokémon Breeder ×3, Scout F ×2, Scout M ×2, Triathlete F ×1, Triathlete M ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Viridian Forest | Aroma Lady ×3, Bug Catcher ×6, Bug Maniac ×1, Leader ×1, Pokémon Breeder ×1, Pokémon Ranger F ×1, Scout M ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Fuchsia Forest | Aroma Lady ×1, Bug Catcher ×1, Bug Maniac ×1, Pokémon Breeder ×1, Pokémon Ranger M ×1, Scout F ×1, Scout M ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Mt. Moon | Hiker ×1, Ruin Maniac ×3 | KENT is now Ruin Maniac with Geodude Lv10/Sandshrew Lv12; ROBBY is Hiker with Geodude Lv10/Machop Lv12. Their rematches use evolved forms at Lv20–22. |
| Rock Tunnel | Black Belt F ×1, Black Belt M ×1, Hiker ×6, Ruin Maniac ×2 | Broad class mix fits the location; no priority class-placement concern identified. |
| Digletts Cave | None | No scripted trainer battles; this is not by itself a placement defect. |
| Cerulean Cave | Ruin Maniac ×3 | Broad class mix fits the location; no priority class-placement concern identified. |
| Celadon Cave | None | No scripted trainer battles; this is not by itself a placement defect. |
| Seafoam Islands | None | No scripted trainer battles; this is not by itself a placement defect. |
| Cinnabar Volcano | Black Belt F ×1, Black Belt M ×1, Expert F ×1, Expert M ×1, Hiker ×2, Kindler ×4, Leader ×1, Ruin Maniac ×1, Scout F ×1, Scout M ×1 | Kindlers, martial specialists, Hikers and Ruin Maniac fit. Scouts MASON/NELLA have hiking/heat dialogue and occupy first-floor corridors; defensible as novice explorers. Optional stricter expedition theme would replace them with specialists. |
| Kanto Victory Road | Ace Trainer F ×4, Ace Trainer M ×4, Aces ×2, Black Belt F ×1, Black Belt M ×1, Expert F ×1, Expert M ×1, Ruin Maniac ×4 | Broad class mix fits the location; no priority class-placement concern identified. |
| Pokémon Tower | Channeler ×5, Leader ×1, Rival Late ×1 | Broad class mix fits the location; no priority class-placement concern identified. |
| Pokémon Mansion | Burglar ×7, Rival Late ×1, Scientist ×7 | Broad class mix fits the location; no priority class-placement concern identified. |
| Power Plant | Electrician ×4, Engineer ×3, Scientist ×2 | Broad class mix fits the location; no priority class-placement concern identified. |
| Silph Co. | Boss ×1, Rival Late ×1, Team Rocket M ×19 | Broad class mix fits the location; no priority class-placement concern identified. |
| Underground Path | Pokéfan F ×3, Pokéfan M ×3, Pokémaniac ×4, Super Nerd ×2 | Pokéfans, Pokémaniacs and Super Nerds fit a meeting/travel corridor; not treated as a natural cave. |
| Kanto Safari Zone | None | No scripted trainer battles; this is not by itself a placement defect. |
| Viridian Channel | None | No scripted trainer battles; this is not by itself a placement defect. |
| S.S. Anne | Gentleman ×4, Lady ×2, Sailor ×7, Trendsetter F ×1, Trendsetter M ×2, Trendsetters ×1 | Broad class mix fits the location; no priority class-placement concern identified. |

## Checks and limits

- Youngster M and F are both present on Routes 1, 3, 4 and 7: the established route-pairing rule holds.
- No Trendsetter encounter on Route 8; no active deprecated-class violation reported.
- Route/dungeon objects produced no class–overworld mismatch in the current alias-based checker. Across all maps, 344 of 347 trainer objects match; the three exceptions are female Psychics MIRA, SELENE and LYRA in Saffron Gym using CIV_WOMAN_1. There is no dedicated PSYCHIC_F overworld constant, so this needs an asset/intent decision rather than substituting a Channeler.
- Habitat automation reported one false positive: Celadon’s approved TRENDSETTERS double class is missing from its urban allowlist. Its broad route allowlist also misses the substantive Channeler issues above.
- Trainer wiring validation reports ten unreferenced party definitions. These are unused-data findings, not proof of misplaced active trainers; no cleanup performed.
- Selected suspicious trainers had dialogue and parties checked directly. This is not an exhaustive validation of every party, dialogue line, walkable tile or visible palette.

## Validation

- Main ROM build passed (`DEVKITARM=/opt/devkitpro/devkitARM make -j4`).
- Overworld audit: 344/347 matched; only the three previously documented Saffron Gym placeholders remain.
- Habitat audit: zero hard/deprecated-class violations; the existing Celadon Trendsetters allowlist warning remains.
- Heatmap regenerated with one Pokémon Breeder row (11 encounter sites).
- Leader nickname regression checks passed for 51 parties / 208 Pokémon.
- No live playtest performed for these changes.
