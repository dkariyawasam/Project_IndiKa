# Map collision audit — 27 September 2026

Static pass over all 253 map headers in the current built ROM, including the retired Route 21 South compatibility slot. No terrain tiles were changed by this pass.

## Fixed

- Vermilion Harbour: Kai and Mina now use water tuber sprites and elevation 1; Rory now uses water elevation 1.
- Route 12: Vera moved from the bridge at (16,114) to water at (15,114), elevation 1, with a two-tile northward patrol that stops before the bridge.
- Route 24: Becky moved one tile east from the bridge to water at (14,22), elevation 1.
- Route 20: Amira moved from sand to nearby water at (66,3), elevation 1.
- Route 23: Ernest moved to the island at (9,101); Zeek moved to dry ground at (15,76). Both previously occupied water with land sprites and elevation 3.

## Checks passed after fixes

- No trainer starts on a collision-blocked tile.
- No trainer start has a nonzero elevation inconsistent with its tile.
- No land/water trainer sprite mismatches under the audit's classification.
- All ordinary object and warp coordinates are within map bounds.
- All fixed warp targets resolve to existing destination warp entries (dynamic 127/255 targets excluded).
- All maps, including enlarged Route 18, fit the map buffer with connection margins.
- ROM build and `git diff --check` pass.

## Remaining review candidates — not automatically treated as bugs

89 unblocked water-behavior tiles have elevations other than 0, 1 or 15: 4 on S.S. Anne Deck, 44 on Route 10, 12 on Route 14 and 29 on Route 25. Many are shoreline or custom metatiles; changing elevation without testing Surf and adjacent terrain could introduce a regression.

82 tiles use collision settings different from at least 99% of uses of the same metatile. These include open rock tiles and blocked ground. Script-controlled barriers, scenery and intentional paths can legitimately differ. Detailed locations follow.

Three connected map pairs have no direct crossing according to collision/elevation data: Harbour–Route 15 (forest warp route), Route 22–23 (gate warp route), and Route 4–7 (requires further design review). A geometric connection alone does not establish that it should be walkable.

Route 21–Cinnabar has two elevation changes at Route 21 (14,99)/(15,99), opposite Cinnabar (38,0)/(39,0). These are water-to-land candidates, not proof of a broken seam. Other crossing tiles remain available.

## Limits

This is not a full emulator playthrough or a proof that every collision is correct. The audit reads compiled tiles, behavior attributes, collisions, elevations, map connections and source event data. It does not simulate Surf transitions, ledges, directional barriers, script-driven tile changes, puzzle states, moving objects or progression gates. Movement checks were made for the relocated swimming triathletes; every NPC patrol was not exhaustively simulated. The unusual tiles and gates above still need targeted live checks.

Re-run after building with `python3 tools/audit_map_collisions.py`. Machine-readable results are written to `/tmp/expedition-collision-*.json`.

## Water-elevation candidates

| Map | Coordinates and tile value |
| --- | --- |
| Route10 | (19,14) `0x31fe`, (19,15) `0x3202`, (19,16) `0x3202`, (19,17) `0x3202`, (19,18) `0x3202`, (19,19) `0x3202`, (19,20) `0x3202`, (19,21) `0x3202`, (19,22) `0x3202`, (19,23) `0x3202`, (19,24) `0x3202`, (19,25) `0x3202`, (19,26) `0x3202`, (19,27) `0x3202`, (19,28) `0x3202`, (19,29) `0x3202`, (19,30) `0x3202`, (19,31) `0x3202`, (19,32) `0x3202`, (19,33) `0x3202`, (19,34) `0x3202`, (19,35) `0x3202`, (19,36) `0x3202`, (19,37) `0x3202`, (19,38) `0x3202`, (19,39) `0x3202`, (19,40) `0x3202`, (19,41) `0x3202`, (19,42) `0x3202`, (19,43) `0x3202`, (19,44) `0x3202`, (19,45) `0x3202`, (19,46) `0x3202`, (19,47) `0x3202`, (19,48) `0x3202`, (19,49) `0x3202`, (19,50) `0x3202`, (19,51) `0x3202`, (19,52) `0x3202`, (19,53) `0x3202`, (19,54) `0x3202`, (19,55) `0x3202`, (18,56) `0x31fb`, (19,56) `0x320a` |
| Route14 | (22,51) `0x312a`, (23,51) `0x31d9`, (22,52) `0x312a`, (23,52) `0x31d9`, (22,53) `0x312a`, (23,53) `0x31d9`, (22,54) `0x312a`, (23,54) `0x31d9`, (22,55) `0x312a`, (23,55) `0x31d9`, (22,56) `0x312a`, (23,56) `0x31d9` |
| Route25 | (62,4) `0x31fe`, (62,5) `0x3202`, (62,6) `0x3202`, (62,7) `0x3202`, (62,8) `0x3202`, (62,9) `0x3202`, (62,10) `0x3202`, (62,11) `0x3202`, (62,12) `0x3202`, (62,13) `0x3202`, (41,14) `0x31fa`, (42,14) `0x31fb`, (43,14) `0x31fb`, (44,14) `0x31fb`, (45,14) `0x31fb`, (46,14) `0x31fb`, (47,14) `0x31fb`, (48,14) `0x31fb`, (49,14) `0x31fb`, (53,14) `0x31fb`, (54,14) `0x31fb`, (55,14) `0x31fb`, (56,14) `0x31fb`, (57,14) `0x31fb`, (58,14) `0x31fb`, (59,14) `0x31fb`, (60,14) `0x31fb`, (61,14) `0x31fb`, (62,14) `0x320a` |
| SSAnne_Deck | (1,14) `0x312b`, (1,15) `0x312b`, (1,16) `0x312b`, (2,16) `0x312b` |

## Collision-pattern candidates

| Map | Coordinates and tile value |
| --- | --- |
| CeruleanCity | (21,3) `0x408` (usual collision 0), (21,4) `0x408` (usual collision 0), (25,3) `0x409` (usual collision 0), (25,4) `0x409` (usual collision 0), (42,11) `0x40e` (usual collision 0) |
| CinnabarIsland | (25,13) `0x52b` (usual collision 0), (26,13) `0x52b` (usual collision 0) |
| DiglettsCave_B2F | (72,27) `0x6d4` (usual collision 0), (71,29) `0x6d4` (usual collision 0), (72,31) `0x6d4` (usual collision 0), (71,33) `0x6d4` (usual collision 0), (72,35) `0x6d4` (usual collision 0) |
| Route11 | (36,14) `0x40d` (usual collision 0), (36,15) `0x40d` (usual collision 0), (6,4) `0x3071` (usual collision 1), (7,4) `0x3071` (usual collision 1), (6,5) `0x3071` (usual collision 1), (7,5) `0x3071` (usual collision 1) |
| Route17 | (15,0) `0x30f4` (usual collision 1), (15,1) `0x30f4` (usual collision 1) |
| Route22 | (45,0) `0x3071` (usual collision 1), (46,0) `0x3071` (usual collision 1), (47,0) `0x3071` (usual collision 1), (43,1) `0x3071` (usual collision 1), (45,1) `0x3071` (usual collision 1), (46,1) `0x3071` (usual collision 1), (47,1) `0x3071` (usual collision 1), (43,2) `0x3071` (usual collision 1), (47,2) `0x3071` (usual collision 1), (43,3) `0x3071` (usual collision 1), (44,3) `0x3071` (usual collision 1), (45,3) `0x3071` (usual collision 1), (43,4) `0x3071` (usual collision 1), (44,4) `0x3071` (usual collision 1), (45,4) `0x3071` (usual collision 1), (46,4) `0x3071` (usual collision 1), (47,4) `0x3071` (usual collision 1) |
| Route4 | (93,19) `0x3069` (usual collision 1) |
| SaffronCity | (4,22) `0x410` (usual collision 0), (6,22) `0x410` (usual collision 0), (5,22) `0x411` (usual collision 0), (7,22) `0x411` (usual collision 0), (57,50) `0x71` (usual collision 1), (58,50) `0x71` (usual collision 1), (57,51) `0x79` (usual collision 1), (58,51) `0x79` (usual collision 1) |
| VermilionCity | (44,29) `0x40e` (usual collision 0) |
| VermilionHarbor | (42,1) `0x52b` (usual collision 0), (43,1) `0x52b` (usual collision 0), (44,1) `0x52b` (usual collision 0), (45,1) `0x52b` (usual collision 0), (46,1) `0x52b` (usual collision 0), (49,1) `0x52b` (usual collision 0), (50,1) `0x52b` (usual collision 0), (51,1) `0x52b` (usual collision 0), (46,29) `0x52b` (usual collision 0), (47,30) `0x52b` (usual collision 0), (55,1) `0x3071` (usual collision 1), (55,2) `0x3071` (usual collision 1), (55,3) `0x3071` (usual collision 1) |
| ViridianChannel | (4,6) `0x52b` (usual collision 0), (14,6) `0x52b` (usual collision 0), (30,6) `0x52b` (usual collision 0), (31,6) `0x52b` (usual collision 0), (6,7) `0x52b` (usual collision 0), (7,7) `0x52b` (usual collision 0), (8,7) `0x52b` (usual collision 0), (9,7) `0x52b` (usual collision 0), (10,7) `0x52b` (usual collision 0), (11,7) `0x52b` (usual collision 0), (16,7) `0x52b` (usual collision 0), (17,7) `0x52b` (usual collision 0), (18,7) `0x52b` (usual collision 0), (19,7) `0x52b` (usual collision 0), (20,7) `0x52b` (usual collision 0), (21,7) `0x52b` (usual collision 0), (24,7) `0x52b` (usual collision 0), (25,7) `0x52b` (usual collision 0), (26,7) `0x52b` (usual collision 0), (27,7) `0x52b` (usual collision 0), (28,7) `0x52b` (usual collision 0), (29,7) `0x52b` (usual collision 0) |
