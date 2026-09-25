# Debug dashboard validation

Validated 25 September 2026 against mGBA 0.10.5 on macOS and the main ROM SHA-256 `0c280598b475c4448eb9328dd4dac63ffe76e2fbe83e4c9a9bf123036f6781ca`.

## Automated checks

- Main ROM builds successfully with `DEVKITARM=/opt/devkitpro/devkitARM make -j4`.
- Eleven dashboard regression tests pass, covering catalog namespaces, English character decoding, argument bounds, active species, stale/mismatched/busy sessions, acknowledgement identity, checkpoint lookup, audit allowlists and all 24 encrypted Pokémon substructure orders.
- Existing player accent regression test passes through the dashboard audit runner.
- `git diff --check` passes.

## Live integration checks

Used a separate Kanto Playtest mGBA instance with copies of the ROM and save under `.cache/debug-dashboard/`. The main save remains byte-identical to the original-save copies made at session startup.

Verified controller input, screen snapshots, map position and encrypted party decoding; creating a level-33 Pikachu; setting Thunderbolt and friendship with a valid Pokémon checksum; healing, money and item grants; writing and reading the Apex Mewtwo flag and a temporary variable; changing style and both accents; matching the green UI frame; warping to Pallet Town; launching a wild battle and Lorelei's trainer battle; restoring checkpoints from both battles; executing an audit; capturing a screenshot/state/note evidence bundle; restoring the initial test party.

A command without the session token was rejected. Desktop layout was checked at 1360 px with no horizontal document overflow, and the narrow layout was inspected. UI navigation, story filtering and a party command were exercised in the browser.

The completion protocol publishes its result only after the engine action finishes. This prevents a frame callback from interpreting an intermediate return value as a rejection. Snapshots are refreshed before publishing mutation acknowledgements. Idle facing animations are permitted; actual walking, locked controls, active scripts and fades block edits.

## Scope and limitations

This validates the dashboard controls, not every possible trainer, map, battle outcome or story-state combination. Generic battle launch does not reproduce a story encounter's surrounding scripts. Story flags and variables are individual edits, not complete quest presets. Appearance changes need a map reload to update the overworld avatar. The text inspector shows the last shared text buffer, not a complete dialogue transcript.

Default warp coordinates are source-derived and may require choosing another walkable tile. Avoid changing story state without a checkpoint. Each game-data command creates one automatically. A paused emulator cannot process commands and becomes stale in the dashboard.

Follow [the dashboard guide](../tools/debug_dashboard/README.md) to start a fresh isolated session. Rebuild and restart the dashboard after source changes; it checks ROM/ELF agreement and compares the emulator's loaded ROM to its session copy.

## Connected-map view

The catalog contains all 253 maps, 944 directed map connections/warps and 12 dynamic or unresolved destinations. Repeated warps to the same map are grouped visually, with each source warp preserved in the details. Tests verify complete link accounting, destination resolution, Pallet Town’s Route 1 connection and house warp, and dynamic-destination handling. Browser checks verified map search, connection filtering, neighbourhood scope, zoom/fit, map-inspector navigation and the rendered nodes/links. The view is read-only; it never warps the player when selecting a node.

## Tile atlas

All 253 maps were rendered from the session ROM/ELF with no exceptions. The renderer decodes GBA LZ77 tiles, 4bpp tile pixels, horizontal/vertical flips, primary/secondary palette slots and both metatile layers. Visual checks covered Pallet Town and its adjoining Route 1/Route 21 tiles, plus doorway navigation to the player's house. Native-pixel zoom and region/local view controls were exercised. Unit checks cover negative offsets, reciprocal north/south joins, separate components, conflict detection and compressed backreferences.

There is one 39-map component joined through cardinal map connections, plus 214 maps without cardinal joins. Warps are navigable links rather than an inferred geographic alignment. Four source-offset conflicts were found: Celadon City/Route 16, Pewter City/Route 2, Route 12/Route 13 and Route 24/Route 25. With the deterministic spanning layout, Route 17/Viridian Channel and Celadon City/Route 16 overlap. These issues are displayed in placement checks; the atlas does not edit map data to hide them. Use the local-edge view to inspect the selected map's outgoing joins (incoming joins are used only where there is no outgoing definition).
