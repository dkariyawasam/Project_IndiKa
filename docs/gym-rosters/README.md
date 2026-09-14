# Celadon and Saffron training rosters

Added 13 September 2026, following the agreed Aroma Lady/Breeder and mixed-gender Psychic rosters.

Celadon: Aroma Ladies Flora, Daphne, Laurel and Viola; Pokémon Breeders Hazel and Mabel. Teams are level 25–28, principally Grass types, with Chansey supporting the nurturing theme.

Saffron: male Psychics Orin, Alden and Silas; female Psychics Mira, Selene and Lyra. Teams are level 33–36 and use Psychic types. Trainers occupy six separate teleport rooms without standing on warp tiles. Female trainers use the existing female Psychic battle portrait and CIV_WOMAN_1 overworld appearance because there is no dedicated female Psychic overworld asset in the project.

All twelve have distinct initial and post-battle dialogue, normal trainer battles, and explicit VS Seeker rematch branches and table entries. Rematches reuse their original teams. Existing charging, range and willingness rules remain unchanged. No new double-battle class or encounter was added.

Vacant trainer IDs 38–41 and 44–51 were used. These slots had no trainer definitions or active data/source references. NUM_TRAINERS and MAX_TRAINERS_COUNT remain 785, preserving existing save flag offsets and trainers elsewhere. Existing constants for unused imported placeholders remain aliases at those numbers.

The male Psychic sprite sheet also needed a frame-grouped graphics conversion rule, as the Ninja sheet did in the preceding task. Added a 2-by-4-tile rule for its horizontal 16-by-32 frames; no image or palette artwork was changed.

## Validation

- `make -j4` passes.
- Scoped `git diff --check` passes.
- `python3 docs/gym-rosters/verify.py` passes: twelve trainer/party/rematch entries, no overlapping NPC or warp positions, passable placement tiles, 36 dialogue boxes within the text width, and unchanged trainer flag allocation.
- The broad trainer wiring audit reports only the same ten pre-existing unreferenced parties as the preceding task. None were changed here.
- Emulator verification completed on retry in mGBA 0.10.5: all twelve trainers appeared, accepted the VS Seeker and completed an actual rematch victory. All 36 final assertions pass; see `runtime-summary.tsv`.
- Overworld and battle screenshots were inspected for Aroma Lady, Breeder, male Psychic and female Psychic appearances. No ROM changes were needed during the retry.
- The first Orin attempt exposed a harness issue: after a random refusal, its message was not dismissed before another activation. The harness was corrected to close that message and the Saffron cases reran successfully. `runtime-tests.tsv` retains that preliminary failure for transparency; it is not a game failure.
- Tests used staged travel, prior-victory flags, charge and a level-100 test Pokémon. They verify rematch functionality, not normal-level balance or every first-time battle. Test input and callbacks were stopped after completion.
- The user's already-open main game was left running; the test used a second emulator window and disposable ROM. The UI control tool timed out when closing that extra window, so it was left idle. The main save and customization ROM hashes remain unchanged.

The Lua files retain session-specific temporary paths and are not a portable one-command test suite. `roster.json` records names, IDs, teams and placements.

Main ROM SHA-256: `bcd86e4af2c6285fa1f46b9ebf6e509bdf8249b8fc30a50ffa050fcfee991ee9`.
The main save and retained customization ROM remain unchanged.
