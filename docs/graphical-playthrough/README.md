# Graphical playthrough — 11 September 2026

Historical test record: player customisation was subsequently [shelved](../shelves/player-customisation-2026-09-11/README.md). These screenshots and ROM hash describe the tested build before shelving. The NPC repairs, trainer-card header fix and open Route 7 finding remain relevant; the current rebuilt ROM uses the original player selection and colours.

## Findings

Two issues were repaired. A third, reproducible dynamic-palette capacity issue remains open on Route 7. This pass does **not** give the overworld a clean bill of health.

### Repaired: lab entry freeze

Entering Oak's lab could freeze on a black screen. This was reproduced in mGBA, not inferred from an asset audit. The dynamic palette cleanup encountered a field-effect palette tag absent from the NPC table. The lookup searched for a sentinel which the table did not contain, so its byte-sized index wrapped indefinitely.

`FindObjectEventPaletteIndexByTag` now checks the table bounds and ignores empty entries. Its loading callers also handle an absent palette safely. The rebuilt ROM successfully loads the lab and displays the Rival, Channeler and Scientists with their repaired palettes.

| Before | After |
| --- | --- |
| ![Lab entry freeze](04-lab-entrance.png) | ![Repaired lab](19-lab-targeted.png) |

### Repaired: magenta trainer-card hint strip

The custom Kanto hint-strip tiles use palette 1, colour 8. The original card palette leaves that colour as magenta filler. The card now supplies the shared hint-header blue at that entry after loading its gender-specific background palette. The card face, customised portrait and border retain their existing colours. A fresh emulator launch loaded the in-game save and verified the repaired card.

| Before | After |
| --- | --- |
| ![Magenta hint strip](57-trainer-card.png) | ![Repaired hint strip](66-card-header-fixed.png) |

### Open: Route 7 NPCs use fallback palettes after walking

Reproduction: visit Route 7 at (12,60), walk south to (12,70), return north, then walk south again. This was reproduced on both southward traversals. The Psychic M and Scout F receive fixed fallback slot 2 instead of their custom palettes. Their graphics IDs are 40 and 202; expected tags are `0x1143` and `0x1140`.

At the southern checkpoint, slot 12 belongs to weather (`0x1200`), slot 14 to Trendsetter M (`0x112c`), and slot 15 to grass effects (`0x1005`). Slot 13 becomes free after earlier NPCs despawn, but already-created fallback sprites do not retry allocation. Three custom NPC palettes plus weather and grass also exceed the four-slot dynamic pool even after cleanup.

This requires a palette-budget/allocation change, including recovery from failed allocations. Freeing live weather/grass palettes would corrupt those effects; changing the NPC artwork to disguise it would not repair the allocator. Neither workaround was applied. Route 9 also has five dynamic tags across its full map, but this specific walking failure is confirmed on Route 7 only.

![Route 7 after walking south](62-route7-walk-repeat.png)

[Runtime palette evidence](runtime-palettes.tsv) records 65 active-object observations, including four mismatches across the two southern checkpoints. `FIXED_NOT_CHECKED` means the dynamic-tag check does not apply to that fixed-slot object; it is not an assertion that its RGB colours were compared automatically.

## Method and scope

The game ran in mGBA 0.10.5 using a disposable ROM/save under `/tmp`; no existing player save was changed. Input was driven through mGBA's Lua API. Screenshots are native 240×160 emulator output.

The opening traversal covered the bedroom, stairs, downstairs, Pallet Town and lab entrance. After rebuilding to repair the freeze, an engine-script warp restored the lab test location. Normal controls then completed Oak's Pallet Town introduction, starter selection and the first rival battle. Later map checks use targeted engine-script warps; they are not a complete campaign playthrough. Trainer sight encounters are disabled in RAM only during those targeted visits so NPC rendering can be inspected.

Confirmed visual checks:

- Bedroom, downstairs and Pallet Town: player walking sprite and map transitions.
- Oak's lab: Rival, Channeler, Scientists, Oak, starter balls, dialogue boxes and Pokémon preview.
- First rival battle: customised blonde/blue player back sprite, Pokémon sprites, send-out effects, HP bars, move menu and return to the lab.
- Radial menu, save dialog, completed in-game save, fresh-process Continue/load, and trainer-card portrait/header.
- Targeted visits: northern/southern Route 7, eastern/western Route 9, Celadon's Trendsetters, S.S. Anne corridor/room/deck, Route 19 shore, and the Power Plant entrance preview followed by its tinted interior and Electricians. Map-entry captures did not expose the Route 7 failure; repeated walking did.

![Starter preview](25-starter-preview.png)
![Customised battle back sprite](33-first-battle.png)
![Battle move menu](36-battle-progress.png)
![Celadon Trendsetters](46-celadon-trendsetters.png)
![Power Plant interior](69-power-plant-interior.png)

The final tested `pokefirered.gba` has SHA-1 `fabe6330484b4df12a004126b72ecb1374485612`. The lab/story/map walking checks used the preceding build with the identical overworld fix; the final build adds the card-strip repair and was checked through save reload, both card sides and the Power Plant visit.

## Automated checks

- ROM builds with `make -j4`.
- `tools/test_object_palette_lifetime.py`: real palette lookup rejects absent tags; shared/live palettes, fixed slots and field effects remain protected; 200 NPC reuse cycles pass.
- `tools/test_player_appearance.py`: 245 palette combinations, defaults, invalid values, bounds and save-data round trips pass.
- `tools/test_player_sprite_palettes.py`: all 14 player sheets and compiled tile indices pass.
- `tools/audit_overworld_palettes.py`: 181 sprite/palette pairs and all map object lists pass the permutation/fixed-slot checks.

Those tests do not prove sufficient runtime capacity. The emulator walking test found a failure despite the static and lifetime tests passing.

## Limits

This is a graphical regression pass, not certification of every map or a completed story run. The automated palette tests cover more colour combinations than the visual playthrough. Full-game progression, every battle animation, all player movement modes, every lighting/weather condition and all save/reload combinations remain outside this pass. Whole-map palette counts do not prove that all those NPCs can be visible simultaneously.
