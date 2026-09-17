# Swimmer and Triathlete movement

Audited all 69 placed Swimmer/Triathlete graphics across active maps. Retained 11 existing walking/wandering patterns and gave 58 stationary placements short, axis-aligned back-and-forth patrols. Each new patrol has at least two collision-free tiles at the matching elevation. Routes avoid other NPC starting tiles, map triggers/warps, and previously assigned new patrol lanes. Existing collision handling stops NPCs at the player or other NPCs.

Four harbor placements were incorrectly using land elevation on water. Corrected their elevation; both land-form Swimmers there now use their water forms. Route 17 NICO moved from (11,91) to (11,93), and Route 20 DARRIN from (43,10) to (43,12), to reach viable ground/water respectively. Trainer parties, scripts, sight ranges and rematch identity are unchanged.

Exact before/after records and verified lane tiles are in changes.json. Terrain/elevation checks, ROM build and trainer audits passed, with only the pre-existing three Saffron Gym sprite placeholders and Celadon habitat allowlist warning. Live traversal and movement timing are not yet verified.
