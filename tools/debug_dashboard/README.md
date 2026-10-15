# Pokemon Expedition: Dashboard

A local browser dashboard connected to mGBA's Lua scripting interface. It uses the built ROM's symbol table and compiled save-layout descriptor. There are no remote services or JavaScript package dependencies.

## Start

1. Build the current main ROM and ELF:
   ```sh
   DEVKITARM=/opt/devkitpro/devkitARM make -j4
   ```
2. Double-click **Launch.command** in this folder, or run:
   ```sh
   python3 tools/debug_dashboard/server.py
   ```
3. The dashboard opens at **http://127.0.0.1:8765**. **Launch.command** also starts the test game; when starting the server directly, click **Launch / connect game** or pass `--launch-game`.
4. If the detected mGBA supports `--script`, the bridge loads automatically. mGBA 0.10.5 needs one manual step per emulator launch: **Tools → Scripting**, paste the connection guide command and click **Run**. Continue the game normally. The status turns connected only after the bridge heartbeat arrives.

Use a separate mGBA instance if another game session is already running. The session directory contains a copy of the main ROM and save, plus an untouched original-save backup. Open the displayed test ROM to keep normal gameplay saves separate from the main save. Attaching the bridge directly to another matching ROM cannot prove which save path the emulator uses.

The dashboard resumes its previous test session when both the ROM and ELF still match, preserving its test save and an existing running bridge connection. After rebuilding, it creates a new session; launch that new test game. Startup compares the ELF's binary content with the ROM, and the bridge compares the entire loaded ROM with its session copy before allowing writes. A paused emulator is shown as disconnected/stale because it cannot process commands.

Set `KANTO_MGBA` to choose a different mGBA executable; otherwise it detects mGBA on PATH or the macOS app. Launches use the session save directory, disable save-state autoload, and keep running when focus moves to the dashboard. Repeated button clicks do not launch duplicate children within the same server process.

Options: `--new-session` explicitly creates a new test copy; `--launch-game` starts mGBA; `--port 8766` chooses another localhost port; `--fresh` omits the copied save; `--no-open` leaves browser opening to you. Ctrl-C stops the server. Reload the displayed bridge to replace a previous bridge in the same emulator.

## Controls

- **Overview:** live screen, controller buttons, map/coordinates, party summary, last buffered text (not a complete dialogue history) and action acknowledgements.
- **Tile atlas:** real ROM artwork rendered from GBA tiles, metatiles and palettes, with map-edge connections stitched using their tile offsets. Zoom/pan, native pixel scale, names/boundaries and doorway navigation are available. The connected outdoor region is shown together; other maps are separate. “Selected map & exact adjoining edges” shows local joins when global offsets conflict. Placement checks list inconsistent cycles and overlapping map extents. Static artwork excludes NPCs, animation, weather, time-of-day tint and runtime replacements.
- **Story state:** live named flags and variables, filtered for Apex, gyms, Giovanni/Rocket, rival and leagues. These are individual values, not complete story presets: dependent events may require several values and a map reload.
- **Party & items:** add active species at a chosen level, heal, assign moves, set friendship, give items and set money. Pokémon generation and editing use the game's own routines, including checksums and stats. A full party is rejected instead of sending a debug Pokémon to storage.
- **Battle lab:** start generic wild battles or a trainer's actual roster. To test a story encounter, trial or league sequence, warp to its location and use the normal event instead. Trainer battles may set normal battle progression flags; restore a checkpoint afterwards when needed.
- **Pokédex sprites:** all 399 active species with searchable front/back artwork, normal/shiny palettes and single/double-battle position previews. Uses source sprite tables, palette aliases, Y offsets, enemy elevation and the battle anchor table. Shows a neutral 240×160 canvas with a message-area boundary, optional sprite bounds/centres and numeric positions. The selected species occupies both sides for comparison. Previews use the first artwork frame; no transforms, personality patterns, runtime effects or background scenery. Reload source refreshes changes without a ROM rebuild or emulator connection.
- **Overworld animations:** all registered source overworlds, searchable by name or graphics ID, with south/north/west/east walking loops and a rechallenge comparison shown together. The rechallenge preview alternates standing and the declared raised-hand pose once per second at normal speed; this is an inspection loop, not a simulation of map hopping/swimming. Missing raised-hand definitions are labelled. Uses actual frame-table references, frame durations and flips; repeated frames stay visible. Pause/play, half/normal/double speed and Reload source sheets work without an emulator connection or ROM rebuild. Source palette previews omit runtime lighting and player accents. Static objects and special-action sheets without a four-direction walk table are labelled; missing source/frame definitions are reported. Only visible cards draw, and playback stops while the tab is hidden.
- **Appearance:** style, both clothing accents and matching UI frame. Reload the current map to update the overworld avatar. Open the relevant menus to test the theme. The palette inspector displays all 16 BG and 16 OBJ palette slots.
- **Project checks:** allowlisted repository tests and audits with captured output. A successful audit is labelled **report ready**, not **passed**, because reports can contain findings. Existing audit scripts refresh their reports under `docs/`. Pillow-dependent checks use the available project image Python environment when present; missing dependencies appear in output.
- **Checkpoints & evidence:** emulator states including savedata, and screenshot/state/note evidence bundles.

Every game-data edit first creates a checkpoint. Restore replaces the test emulator's state and savedata with that checkpoint; it is not a merge. Input presses are not checkpointed. Mutations are accepted only in the idle overworld; menus, dialogue, transitions and battles must finish first. Queued commands expire after five seconds and are never evaluated as Lua code. The server reports queued, applied, rejected and timeout separately. “Applied” means the engine accepted the action; a launched battle or warp may still be running.

## Files and implementation

`server.py`, `catalog.py` and `static/` provide the local UI and API. `bridge.lua` polls a private per-session mailbox at four times per second and sends snapshots. `src/debug_dashboard.c` provides a dormant command mailbox, activated only by the matching bridge; the normal main loop never exposes it as an in-game menu. Game save offsets come from `gDebugDashboardLayout`, compiled with the ROM. Party decoding validates the encrypted record checksum.

All sessions, save copies, checkpoints, logs and evidence live under `.cache/debug-dashboard/` (git-ignored). `current.json` identifies the current session. No dashboard action silently writes changes into the original main save. Normal in-game saving still uses whichever ROM/save the emulator actually has open, so use the provided test copy.

The web server listens only on 127.0.0.1 and checks Host, Origin, per-session tokens, request size and action/value allowlists. There is no arbitrary shell, memory-write or Lua endpoint. Audit subprocesses are fixed commands with timeouts.

mGBA scripting API reference: https://mgba.io/docs/scripting.html

## Tests

```sh
python3 tools/debug_dashboard/test_dashboard.py
python3 tools/debug_dashboard/test_overworlds.py
python3 tools/debug_dashboard/test_pokedex.py
```

The unit suite covers catalog parsing, active-species selection, argument bounds, unknown commands, stale/busy/mismatched sessions, acknowledgement identity, checkpoint lookup and Pokémon decryption across all 24 substructure orders. Live smoke-test results are recorded in `docs/debug-dashboard-validation.md`.

The Pokédex sprite tab includes front/back Y offset and enemy elevation editors. Edits preview immediately and remain drafts per species until **Save offsets to source** is clicked. **Reset preview** restores loaded values; **Reload source** discards drafts and rereads files. Saves reject stale coordinates, preserve other species, and update the normal-form runtime tables for Castform. Rebuild the ROM for in-game changes; the dashboard does not alter the running ROM or save file.

Species details also show the full source cry with audio controls, Pokédex text and measurements, registered footprint, normal/shiny palette indices, and a clickable evolution family tree with table conditions. Script-only transformations are not represented by the evolution table.

**Item catalogue:** searchable source item descriptions, pockets, base prices, field/battle/held usage, TM move descriptions and evolution links. Defaults to items referenced in game source, with an all-defined-items option and expandable file/line evidence. References are not a complete obtainability audit; runtime checks and conditional or unused content can appear. Reload items rereads source without rebuilding.

Item cards render their registered icon with its actual palette. Obtainability is a conservative source audit: map-linked gifts/pickups/marts, hidden items, wild held items, Pickup, Rocket coin-shop TMs and literal engine awards. Cards list acquisition evidence separately from general references. Story gates and dynamic scripted awards may need manual verification; missing evidence is labelled Unconfirmed, not unavailable. Patch Disc is labelled intentionally undistributed unless an acquisition source is added.

## Quest catalogue and pixel UI

**Quests & research** reads the nine main quests, 30 objectives, 21 Field Promises and dynamic objective dialogue from `src/quests.c` and `src/strings.c`. Search by quest, objective or location. Progression notes come from `docs/logbook-quests.md`. This is a source catalogue including undiscovered content, not live save progress. Porygon research remains under Evolution by Design.

The dashboard uses a local pixel font derived from `graphics/fonts/latin_normal.png`, with square panels and hard shadows. The checked-in `static/pixel.ttf` needs no runtime dependencies or external font requests. To regenerate it, run `build_pixel_font.py` in a Python environment with Pillow and fontTools. The glyph artwork retains its existing game-asset attribution; this conversion does not claim new ownership.
