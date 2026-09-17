# Trainer palette audit

Checked the built ROM, not only source declarations.

- 181 overworld sprite/palette pairs: no detected scrambled colour-index mappings. Shared palettes allow deliberate small colour substitutions.
- All overworld graphics checked against standard fixed slots 2–5. Found and fixed Electrician: its custom palette tag was assigned to the generic blue slot. Uses dedicated bespoke slot 10 now, with no fixed-slot conflicts in its five placed maps and no extra dynamic-pool demand.
- 87 battle sprite/palette table pairs: every used opaque PNG colour matches the assigned decompressed ROM palette at GBA precision. Details in battle-palettes.json.
- All map object lists checked for fixed bespoke-slot conflicts. No conflicts detected.
- Palette lifetime/recovery regression tests passed, including 200 reuse cycles, shared ownership, pool exhaustion and subsequent recovery.
- Main ROM rebuilt successfully.

Route 9 still has five dynamic tags across the entire map. This is a visibility-dependent demand warning, not evidence of five simultaneous allocations; the prior live palette stress pass covered that route. This audit did not repeat a live traversal of every map or verify every day/night rendering state. Do not treat static checks as a guarantee against every possible runtime palette problem.

Reusable checks: tools/audit_overworld_palettes.py now rejects custom palette tags assigned to the preloaded standard slots; tools/audit_trainer_battle_palettes.py checks battle portrait palettes against their source PNGs.
