# Codex Pokemon Mod Session Merge

This file consolidates the useful context from the local Codex sessions related to this Pokemon FireRed mod. It is not a literal Codex UI session merge; it is the practical merged project memory to use going forward.

## Source Sessions

- `019e1407-75cc-7201-a277-9a964bf05d39` - Find unused items, updated 2026-05-10.
- `019eeb56-62ec-7ee0-b0d7-e83a5a88fbf3` - Find unused items / main mod work, updated 2026-06-21.
- `019eeb5b-6f22-7c51-b9e6-f45e049c41b6` - Explain this file, berry plant sprite atlas.
- `019eebb3-19a2-7980-953a-3a402e9c2bf6` - Revert/rebuild start screen logo bin.
- `019ef10f-ebec-70c0-953e-ab4014204f6c` - Access/find-session follow-up, updated 2026-06-22.

## Current Repo Notes

- Branch: `Quest-Log`.
- Recent tracked working tree appeared clean when checked, with only untracked `agbcc/` and `porymap/`.
- Recent `git log` showed the latest commit as `16de508ce removed ruby sapphire unused trainers`.
- Many session logs contain compacted/encrypted chunks, so this file summarizes observable user requests, assistant summaries, and repo state rather than claiming a perfect replay.

## Completed Item Cleanup

- Initial unused item audit found Hoenn/RSE leftovers and unused special/key items.
- Removed:
  - `ITEM_SHOAL_SALT`
  - `ITEM_SHOAL_SHELL`
  - `ITEM_RED_SCARF`
  - `ITEM_BLUE_SCARF`
  - `ITEM_PINK_SCARF`
  - `ITEM_GREEN_SCARF`
  - `ITEM_YELLOW_SCARF`
  - `ITEM_CONTEST_PASS`
  - `ITEM_WAILMER_PAIL`
  - `ITEM_DEVON_GOODS`
  - `ITEM_SOOT_SACK`
  - `ITEM_ROOT_FOSSIL`
  - `ITEM_CLAW_FOSSIL`
  - `ITEM_OAKS_PARCEL`
- Item removal pattern:
  - Delete row from `src/data/items.json`.
  - Remove icon table row from `src/data/item_icon_table.h`.
  - Compact `include/constants/items.h`.
  - Regenerate `src/data/items.h`.
  - Build with `make -j8`.
- `ITEM_OAKS_PARCEL` was a constants-only compatibility slot and was removed from constants, icon table, quest-log item logic, and unused graphics declarations.

## TM And Reward Direction

- TMs are intended to be centralized at the Game Corner / Rocket League TM seller.
- `ITEM_TM39` Rock Tomb was added to the Celadon Game Corner TM prize clerk for `3500` coins.
- Later work moved toward all TMs being available through Game Corner and removed from field/NPC rewards.
- Remaining old TM reward flags/scripts were cleaned in a later pass according to the session log.
- TM field pickups were replaced with useful non-TM items such as healing/status/X items/berries.

## Badge, Gym, And Quest Loop Work

- Brock gives `ITEM_BOULDER_KEY` as part of receiving the Boulder Badge.
- Misty gives `ITEM_CASCADE_BOARD` as part of receiving the Cascade Badge.
- Cinnabar Lab access was blocked by `ITEM_BOULDER_KEY` at one point.
- Later Cinnabar Gym lock/key logic was removed so the gym can be entered straight away.
- Gym leaders and Elite Four were converted to double battles.
- Dynamic badge-count trainer variants were added for leaders including Brock, Misty, Surge, Erika, Koga, Blaine, and Sabrina.
- Badge stat boosts were removed.
- HM/field-move badge requirements were removed.
- Obedience was changed to count total badges instead of specific badge flags.
- Regular gym trainers were removed from Brock/Misty/Surge/Erika/Sabrina/Blaine gyms as requested, and `set_gym_trainers` calls were removed.

## Field Moves And Traversal

- Cut is intended to be a normal TM, not an overworld effect.
- Cut tree placements were removed from map data.
- Cut was removed from the party-menu field-move affordance.
- TM51+ consumption behavior was cleaned so TMs consume normally when taught.
- Snorlax encounters/pokeflute gating were removed or audited.
- Silph Scope requirement for Pokemon Tower ghosts was removed; ghosts and Marowak are visible by default.
- Cascade Board was fixed so bag use and registered SELECT use both trigger boarding at valid water edges.

## Pokedex And Text Polish

- National Dex menu now keeps only National numerical mode, not Kanto numerical mode.
- The top-right Pokedex count panel was simplified to `Seen` and `Owned` using National totals.
- The National Dex menu was rearranged:
  - Empty `Pokemon List` header removed.
  - `SEARCH` renamed to `LISTS`.
  - Mode names changed to `Numerical`, `Alphabetical`, and `Type`.
  - Lightest/smallest modes removed from that menu.
- User-facing `Pokemon`/`POKEMON` spellings were audited and accented where appropriate in displayed text.

## Map And Content Pruning

- Tooltip / trainer-tip signs were removed.
- Hoenn maps and unused Hoenn/Sevii content were progressively pruned.
- Birth Island, Navel Rock, Faraway Island, Southern Island, Four Island daycare, and Two Island were removed in later cleanup passes.
- Two Island removal included map/layout folders, map group entries, region map/dex area hooks, fly destination, travel text, and stale scene variables.
- Remaining generic Sevii/mapsec infrastructure may still exist where harmless.
- Team Rocket presence was removed from some locations such as Mt. Moon and Cerulean City; other Rocket areas were left/reworked depending on the current design.

## Rocket League

- Trainer Tower lobby was repurposed/renamed as `RocketLeague_Lobby`.
- The Rocket League lobby has/should have:
  - TM prize seller replacing old mart behavior.
  - Game Corner TM prize seller removed from Game Corner prize building so Rocket League lobby can be the only seller.
  - North-door lady explaining single battle format, Team Rocket opponents, coin winnings, and 4 consecutive wins leading to the league champion.
  - YES/NO prompt and auto-trigger behavior near the north door.
- Giovanni is gated from the champion room until his Viridian Gym resolution; before then, the arena reports he is away on important business.

## Trainer Work

- Many trainer cleanup passes were completed:
  - Hoenn-only unused trainer battles removed.
  - Ruby/Sapphire unused trainer classes removed in latest commit.
  - Sevii trainer hooks stripped.
  - Unused trainer entries normalized.
  - Habitat/route trainer distribution reshuffled.
  - VS Seeker/rematch coverage audited.
- Current trainer state from session notes:
  - Trainer systems are mostly done.
  - One-time areas such as SS Anne, Silph Co., and Saffron Dojo were treated as intentional exceptions unless design changes later.
  - Remaining trainer work is mainly optional placement/class/sightline QA.

## Pokemon / Dex / Species Work

- Starly, Staravia, and Staraptor were added to habitat data, then moved to Rough Terrain.
- Missing official Pokemon/forms in the project were filled with official-style base stats, typing, catch/EXP yield, EV yield, egg groups, abilities where supported, and Dex metadata.
- Custom/non-official forms were left as placeholders where appropriate.
- Berry plant sprite atlas session identified:
  - Runtime asset: `graphics/object_events/pics/misc/berry_plant.png`
  - Palette: `graphics/object_events/palettes/berry_plant.gbapal`
  - Wired berry patch subset: Sitrus, Lum, Leppa, Pomeg, Kelpsy, Tamato, plus soil.

## Day/Night And Visuals

- Custom day/night outdoor map list was audited.
- Viridian Forest should be treated like an interior/dungeon, not a day/night outdoor map.
- Saffron City connection and Safari Zone maps were added to day/night list.
- Several day/night palette glitches were investigated and fixed, especially after save load and wild battles.
- Custom NPC palette/sprite issues were investigated for Aroma Lady, Pokemon Ranger, and Triathlete M.
- Triathlete M overworld sheet was rearranged from `160x32` to `16x320` to match stable bespoke sprite frame layout.

## Title Screen Logo

- FireRed title logo assets:
  - `graphics/title_screen/firered/game_title_logo.png`
  - `graphics/title_screen/firered/game_title_logo.pal`
  - `graphics/title_screen/firered/game_title_logo.bin`
  - generated `.8bpp.lz`, `.bin.lz`, `.gbapal`
- Important lesson from the logo session:
  - The `.png` is tile graphics.
  - The `.bin` is the tilemap/placement.
  - Palette index 0 matters for transparent/background areas.
  - The original title screen uses a split-pattern tilemap, not a simple full-width static rectangle.
- The title logo session went through several attempts to place missing `E` and `NS`/final `S` chunks into the `EXPEDITIONS KANTO` subtitle.
- Last known open/fragile task: verify title logo tilemap alignment visually. The repeated "check its all aligning" requests were tied to this.

## Current Remaining Work

The project is in consolidation/QA rather than foundation-building. Best next tasks:

1. Verify and finish title screen logo alignment.
2. Do a new-save playthrough QA for badge order, gym reward loops, quest log phases, and progression gates.
3. Playtest Rocket League four-win champion access after Giovanni's arc completion.
4. Check/polish Blaine's final gym-only flow.
5. QA map traversal:
   - Viridian Channel.
   - Route 18 / Route 21 / Route 20.
   - Vermilion Harbor.
   - Fuchsia Forest.
   - Cascade Board entry/exit on all intended shorelines.
6. Optional cleanup:
   - Old help-system / quest-log references.
   - Generic e-Reader, Mystery Gift, wireless, and Sevii infrastructure.
   - `flags.h` formatting and naming consistency.
   - Final trainer placement, facing, class/overworld mismatch, and sightline audit.

## How To Use This File In Future Sessions

When starting a new Codex thread for this project, ask Codex to read:

`docs/codex-pokemon-session-merge.md`

Then ask it to inspect the current repo state with:

- `git status --short`
- `git log --oneline -n 20`
- targeted `rg` searches for the task at hand

This avoids relying on the large old session logs, some of which contain image payloads or compacted encrypted chunks.
