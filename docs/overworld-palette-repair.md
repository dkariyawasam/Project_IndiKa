# Overworld palette repair

The reported NPC colour corruption had two independently confirmed causes:

- Forty-five NPC sprite sheets had reordered PNG colour indices which no longer matched their runtime palettes. Their indices and embedded palettes were reordered together, preserving every original RGB pixel. This includes Scientists and Channelers in Oak's lab.
- Dynamic NPC palettes stayed allocated after the last sprite using them despawned. The four unreserved slots could therefore fill with palettes from NPCs no longer on screen. Cleanup now releases unused object palettes after despawning and before another dynamic allocation. Live NPCs, reflection sprites, fixed slots and palettes owned by field effects are protected.

Oak's lab requires two dynamic palette tags (Scientist and Rival). Its map does not inherently exceed the four-slot pool. Route 7 and Route 9 each have five different dynamic palettes across their whole map, making palette reuse necessary as the player moves; that count is not a claim that all five are simultaneously visible.

Petrel also shared fixed slot 10 with Ariana in the Rocket League arena. Petrel now uses a dynamic slot so the two palettes cannot overwrite one another.

Validation:

- `make -j4`: ROM builds.
- `python3 -B tools/test_object_palette_lifetime.py`: shared/live ownership, field-effect protection, fixed-slot protection and 200 allocation/release cycles.
- `python3 -B tools/audit_overworld_palettes.py` (Pillow and ARM nm required): 181 sprite/palette pairs and all map object lists; no detected index permutations or whole-map fixed-slot collisions. Small intentional shared-palette colour differences are allowed by this audit.
- Existing player appearance palette tests continue to pass.

The hardware still has 16 object palettes. Cleanup prevents stale allocations; it does not provide unlimited capacity for simultaneously visible NPCs and effects. The subsequent playthrough confirmed that Route 7 still exceeds its dynamic budget while walking near Trendsetter M, Psychic M and Scout F with grass effects active. Weather and grass consume two of the four slots; two NPCs fall back to slot 2 and do not recover when a slot later becomes free. This is an open bug, documented with runtime evidence in the playthrough report.

The subsequent emulator playthrough reproduced a black-screen freeze when entering Oak's lab. The palette lookup expected a tag sentinel, but the table actually ends with a zeroed entry. Looking up an unknown field-effect tag during cleanup could therefore loop indefinitely. The lookup is now bounded by the table length, ignores empty entries, and its loading callers reject missing tags. The lifetime regression test exercises unknown tags using the real lookup implementation.

After rebuilding, the emulator entered the lab successfully. The Rival, Channeler and Scientists were visually checked, followed by starter selection and the first rival battle. See [graphical playthrough](graphical-playthrough/README.md) for screenshots and the exact scope of verification.
