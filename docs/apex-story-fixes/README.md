# Apex story fixes

The main ROM keeps **ROUTE 7 RAMPAGE**. Its objective now reads: “Explore the cave opened by MEWTWO's blast on ROUTE 7.” This preserves the cave's origin as the aftermath of the blast instead of presenting it as a pre-existing destination.

Changes:

- Flashback camera restoration keeps player controls locked until the calling scene finishes its remaining dialogue and progress updates. This covers Lance and the other Route 7 flashbacks using the same restoration helper.
- Corrected the Zapdos, Articuno and Moltres dossier locations and witnesses. Also corrected the Route 11 Bug Maniac and the Annihilape witnesses. Updated stale Zapdos/Moltres summaries to match their current conversations. `compiled-dossiers.tsv` lists the actual strings read from the rebuilt ROM.
- Escaping or teleporting from any of the eight Apex battles permanently resolves that encounter. Tangrowth, Zapdos, Articuno and Mewtwo now follow the policy already used by the other four.
- The shared rumour-reveal helper respects fought flags, preventing repeated rumours from respawning a resolved Apex.
- Mewtwo's escape has its own Giovanni response. Giovanni still leaves and unlocks the Viridian Gym story path, so a permanently lost Mewtwo cannot block progression. Victory/capture retain their existing conclusion.

Validation uses a fresh game in a disposable copy of the rebuilt main ROM. Lance is exercised with A-button presses every 30 frames, including throughout camera restoration. The escape regression checks execute the compiled post-battle escape handlers under staged conditions, inspect their flags, reload each map, and invoke the shared rumour reveal helper. These are targeted runtime checks, not eight naturally played battles. The test fixture explicitly spawns each Apex before exercising its escape handler.

An attempted fixture derived from the previous session's RAM was discarded after failing to load. It was not used for the final validation, and no existing player save was changed.

The standalone customisation ROM remains the previously retained snapshot. These changes are in `pokefirered.gba`.

Main ROM SHA-256: `ad23c19131544eee88e8ab6a33e2af21d6dd869e6d22502d188d3031ce4d6eaa`.

`make -j4` and `git diff --check` pass. The compiled escape handlers were checked for all eight permanent fought/hide flag pairs, with both run and teleport routed to those handlers. The customisation ROM and existing root save retain their original SHA-256 hashes.

Final runtime result: **35 checks passed** — four persistence checks for each of eight Apex encounters, Mewtwo's escape story continuation, and Lance's rapid-advance completion/control-release checks. Final Lance flags are all set and field lock is 0. The temporary QA ROMs were removed after the emulator was closed.
