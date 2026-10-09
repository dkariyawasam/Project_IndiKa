# October 6 trainer runtime checks

Tested the current build in a separate mGBA window using a disposable copy of the October 3 certificate-earned save. The primary ROM/save session was not used. This is controlled feature QA: debug warps, script entry points, temporary trainer flags, VS Seeker readiness and a fan-club scene variable were used. It is not additional earned campaign progress or a balance playthrough.

## Fixed live findings

The Game Corner and both Pokémon fan clubs could exhaust the dynamic NPC palette pool. NPCs then used the generic fallback colours. Before-fix runtime records identify the affected active objects; the comparison image shows examples.

Assigned Gentleman to reserved slot 11, female civilian worker to slot 6, and Black Belt F to slot 7. These use the existing fixed-palette loading path. No sprite pixels or palette colours changed. The compiled audit checks all map object lists for conflicts with other fixed NPC palettes.

Reloaded the rebuilt ROM and read its graphics descriptors to verify the changed slots were actually loaded. Eight final samples across the three rooms have active NPCs, no dynamic-tag mismatches and correct fixed-slot assignments. The other sampled locations were Celadon City, Routes 4/7/9/23 and Viridian Forest. These samples do not establish coverage of every camera position or weather/reflection combination. One transitional zero-object snapshot in the broader after log is excluded; Vermilion Fan Club was resampled successfully in the final indoor log.

## Trainer checks

- All four Rocket admins and eight Rocket Aces: actual arena entrance scripts, distinct introduction text, overworld appearance and battle portrait transitions checked. See `league-contact.png` and `league-results.json`. These are entrance/transition checks, not twelve completed matches.
- Six enabled rematches: Angelica, Gregory, Rick, Doug, Sammy and Anthony resolved through the engine's rematch command to IDs 72, 74, 75, 76, 77 and 82. Their portraits and names match. The fixture supplies the VS Seeker flag/readiness and original battle text arguments; it does not force the resulting opponent ID. The four S.S. Anne cases return 0 under the existing ship rematch exclusion. Their new table entries therefore remain dormant there; no ship rematches were enabled.
- Eight field encounters sampled: Chaplin (Route 11), Paxton (Route 18), Becky and Inez (Route 24), Finn's duo (Route 20), Pax (Vermilion Harbor), Tobin and Maris (Fuchsia Forest). Introductory dialogue and battle appearances match their configured identities.
- Inez's battle was completed with ordinary battle inputs in the disposable fixture. Her defeat text, money award, unlocked return to the map and subsequent normal-interaction post-battle conversation worked. The high-level campaign party makes this a flow check, not balance evidence.

## Validation

- Main ROM built successfully. Hashes are recorded in `build-hashes.json`.
- Trainer identity/dialogue regression: 446 placed trainer objects, league actors, 20 dedicated IDs and 201 edited text labels passed.
- Trainer wiring: 777 entries, 327 active references and 664 parties passed.
- Overworld palette audit: 181 sprite/palette pairs; no detected index permutations or whole-map fixed-slot conflicts.
- Palette lifetime and recovery host tests passed, including shared palette retention and full-pool recovery.

The isolated harness and intermediate screenshots are under `/tmp/kanto-trainer-live-oct6`. Only selected evidence is retained here. Remaining work includes earned collection/balance testing, the deliberately deferred custom-ability battle tests, and final artwork. This pass does not certify every trainer's full defeat/rematch sequence or every crowded-map camera position.

## Earned continuation

A separate [earned collection checkpoint](earned-collection/README.md) resumes the unmodified campaign save through ordinary inputs. As of October 9, the earned campaign has 46 owned species and Mountain is 15/15. Its habitat reward, repeat-claim protection and persistence through CONTINUE passed. Level-matched balance validation remains unfinished.
