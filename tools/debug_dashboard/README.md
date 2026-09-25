# Expedition Kanto debug desk

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
3. The dashboard opens at **http://127.0.0.1:8765**. Its connection guide gives a separate test ROM path and a `dofile(...)` command.
4. In mGBA, open that test ROM. Then open **Tools → Scripting**, paste the connection command and click **Run**. Continue the game normally.

Use a separate mGBA instance if another game session is already running. The session directory contains a copy of the main ROM and save, plus an untouched original-save backup. Open the displayed test ROM to keep normal gameplay saves separate from the main save. Attaching the bridge directly to another matching ROM cannot prove which save path the emulator uses.

Restart the dashboard and reconnect after rebuilding. Startup compares the ELF's binary content with the ROM, and the bridge compares the entire loaded ROM with its session copy before allowing writes. A paused emulator is shown as disconnected/stale because it cannot process commands.

Options: `--port 8766` chooses another localhost port; `--fresh` omits the copied save; `--no-open` leaves browser opening to you. Ctrl-C stops the server. Reload the displayed bridge to replace a previous bridge in the same emulator.

## Controls

- **Overview:** live screen, controller buttons, map/coordinates, party summary, last buffered text (not a complete dialogue history) and action acknowledgements.
- **World & maps:** searchable map catalog, source-defined objects/visibility flags, running permission, coordinate warp and current-map reload. Default coordinates use the first warp where possible; otherwise choose a walkable tile explicitly.
- **Tile atlas:** real ROM artwork rendered from GBA tiles, metatiles and palettes, with map-edge connections stitched using their tile offsets. Zoom/pan, native pixel scale, names/boundaries and doorway navigation are available. The connected outdoor region is shown together; other maps are separate. “Selected map & exact adjoining edges” shows local joins when global offsets conflict. Placement checks list inconsistent cycles and overlapping map extents. Static artwork excludes NPCs, animation, weather, time-of-day tint and runtime replacements.
- **Connection diagram:** searchable, pannable and zoomable diagram of every catalog map. Filter outdoor connections or warps; inspect incoming/outgoing links and jump to map inspection. The emulator location is highlighted. Disconnected maps remain visible, and dynamic/unresolved destinations are listed separately. Scripted travel and story access conditions are not inferred. This is a connectivity diagram rather than a geographic map.
- **Story state:** live named flags and variables, filtered for Apex, gyms, Giovanni/Rocket, rival and leagues. These are individual values, not complete story presets: dependent events may require several values and a map reload.
- **Party & items:** add active species at a chosen level, heal, assign moves, set friendship, give items and set money. Pokémon generation and editing use the game's own routines, including checksums and stats. A full party is rejected instead of sending a debug Pokémon to storage.
- **Battle lab:** start generic wild battles or a trainer's actual roster. To test a story encounter, trial or league sequence, warp to its location and use the normal event instead. Trainer battles may set normal battle progression flags; restore a checkpoint afterwards when needed.
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
```

The unit suite covers catalog parsing, active-species selection, argument bounds, unknown commands, stale/busy/mismatched sessions, acknowledgement identity, checkpoint lookup and Pokémon decryption across all 24 substructure orders. Live smoke-test results are recorded in `docs/debug-dashboard-validation.md`.
