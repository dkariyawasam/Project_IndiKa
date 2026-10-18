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


## iPhone Home Screen app

Run the dashboard on the Mac with its private Wi-Fi address:

```sh
python3 tools/debug_dashboard/server.py --no-open --lan-host "$(ipconfig getifaddr en0)"
```

If Wi-Fi uses a different interface, use the IPv4 address shown in macOS Wi-Fi settings instead. The terminal prints the iPhone URL and a pairing code. Open that URL in iPhone Safari on the same trusted Wi-Fi and enter the code. Use Share → Add to Home Screen, with Open as Web App enabled. If the installed app asks again, enter the same code there.

The phone has a section picker containing all dashboard tabs, touch-sized controls, and a Home Screen icon. mGBA, repository data and edits still run on the Mac. Keep the Mac awake and the server running; there is no offline emulator or cached project data. The local connection uses HTTP, so use trusted private Wi-Fi only, without public port forwarding. A Mac firewall prompt may require allowing Python incoming connections. Guest-network device isolation can block connections.

LAN access is opt-in: without `--lan-host` the server remains localhost-only. LAN requests must use the specified address and pair before reading project data or invoking commands. Pairing cookies are HttpOnly and SameSite=Strict; mutations also require the existing session token and matching origin. Pairing persists when restarting the same test session. A new session or deleting its phone-pairing.key file resets pairing. The IP address may change when switching networks; reopen the new address and add it to the Home Screen again if needed.


## Official-game regional encounter reference

The Pokédex includes a bundled PokéAPI encounter snapshot in `official_encounters.json`, grouped by actual location region, not the game’s primary region. It contains direct encounters, gifts and NPC trades where upstream records them. It does not infer evolution, breeding or transfer availability; missing data is explicitly not an absence claim. Custom species/forms do not inherit the official base species’ encounters.

Source: https://github.com/PokeAPI/pokeapi/tree/master/data/v2/csv (PokéAPI community data). Snapshot metadata includes retrieval date, covered versions and input SHA-256 hashes. To refresh, download the tables listed in `import_official_encounters.py` from that source into one directory, then run `python3 tools/debug_dashboard/import_official_encounters.py /path/to/csv`. The dashboard does not require internet access to display the reference. Encounter coverage is incomplete for some games, and several recent games have no records in the snapshot.

### Encounter planner and assets checklist

The Encounter planner reads `src/data/wild_encounters.json`. Search by map or species,
edit the existing slots and level ranges, review, then save. Saves validate species,
levels, slot count and source revision, and back up the JSON under
`.cache/debug-dashboard/source-backups/`. Rebuild the ROM after saving. Configured
weights are shown separately from actual engine odds; slot-count mismatches and
known fishing/Rock Smash selector differences are flagged. Gifts and scripted
encounters are outside this editor.

The Assets checklist scans registered Pokémon front/back art, normal/shiny palettes,
menu icons, footprints and cries, plus overworld source sheets and animation
references. Filter by group, name or source issues and expand an entry for paths.
Rescan after editing files. Passing means source checks pass, not that artwork is
visually finished; placeholders and intentionally shared assets require review.

### Workspaces

Navigation groups the tools into six workspaces on desktop and phone:
Playtest (live view, party, battles, checkpoints), Pokémon (Pokédex and items),
Encounters (trainers, wild encounters and tile atlas), Story (quests, rivals and
flags), Art studio (overworlds and live palettes), and Project checks (assets and
audits). The workspace remembers the last tool selected during the current page
session. Open Connection guide in the header when setting up the emulator.

### FireRed colour checker

Art studio → FireRed colours compares used first-frame front-sprite colours with
151 vanilla Kanto Pokémon from pret/pokefirered revision
`037335f4c725d7c9aecdac87066f2002b4bd7e14`. The frozen reference JSON is generated by
`build_palette_reference.py` (requires Pillow and that local Git revision).
Normal and shiny palettes have separate references. RGB is converted to GBA
5-bit values before measuring HSV saturation and 10th–90th percentile value
contrast. Transparent index 0 and unused colours do not contribute. Near-neutral
or near-black colours are excluded from saturation comparisons.

A review flag indicates at least 10% of visible pixels use chromatic colours
12 saturation points below the median of vanilla colours within 20 degrees of
hue and 15 value points, or contrast below the vanilla 10th percentile. At least
five comparable reference colours are required. These thresholds are heuristics,
not artistic requirements. Vanilla matching means matching used palette indices
at GBA precision, not matching sprite geometry. No flags is not a certification.
The scan never changes artwork. It does not inspect back/icon palettes or
claim that every later-generation colour is less saturated than FireRed.


#### Preview and approve recommendations

Expand a flagged species and choose **Preview recommendation**. Front and back
comparisons are rendered from the exact proposed palette at GBA colour precision.
Recommendations select nearby actual vanilla colours within 15° hue, 10 saturation
points and 4 HSL-lightness points. Neutral colours and pale highlights (lightness
≥72%, saturation <50%) are retained. Candidates that merge shades, reverse lightness
ordering or compress meaningful lightness gaps by more than 25% are rejected.
Vanilla colour coverage shows exact used-colour matches after GBA quantization; it
is not a style score. Contrast-only problems can still require manual editing. All species sharing that palette are
listed and previewed; menu icons use separate palettes.

**Approve and apply** saves only the selected normal/shiny palette, with a backup
and change manifest in `.cache/debug-dashboard/source-backups/`. A fingerprint of
the palette, proposed colours, affected species and sprite files prevents applying
an outdated preview. Rebuild the ROM after approval. Browsing previews does not
change any source artwork.
