# Battle Tower retirement

Battle Tower is retired as a player-facing facility. The September 13, 2026 audit
found it already absent from the current map registry, map events, dialogue and
shared scripts; there was no remaining entrance to remove. Battle Frontier and
Trainer Tower were also absent. No gameplay or ROM edit was necessary.

`python3 docs/battle-tower-retirement/verify.py` checks those sources for facility
references and script specials implemented by the legacy Battle Tower module.
League specials already occupy some former facility slots and must remain intact.

Retain legacy engine functions, special-table slots, save structures, link/record
compatibility and shared assets. Removing them wholesale would require a separate
engine cleanup and save/link compatibility review, rather than removing a facility.
The old Easy Chat BATTLE TOWER word is already disabled. Shield descriptions and
other residual strings belong to dormant legacy data, not accessible facility content.
Indigo and Rocket remain the active League challenges; their terminals and records
are unchanged. The main save and customization ROM are unchanged.

## Implementation cleanup

Subsequent cleanup removed 31 Tower functions, its party-selection entry/callback,
and the level-50/100 opponent datasets. Removed challenge specials now point to
NullFieldSpecial in their original table slots. StartSpecialBattle selector 0 returns
to the script without launching the retired facility; secret-base/e-Reader selectors
remain intact. Legacy trainer display helpers, shared party restrictions and e-Reader
support remain because the shared engine references them. Save structures are unchanged.

Validation: ROM build passed; retained special IDs matched the pre-cleanup linked
ROM and removed function symbols were absent. Actual agbcc save-layout checks,
league record tests, Indigo battle scaling/restoration tests, terminal validation and
Indigo map/script validation all passed. No new live emulator playthrough was run
for this code cleanup. Main save and retained customization ROM hashes were unchanged.
Updated main ROM SHA-256:
`cb775bf3e2d9f7e6a5e94f1cce76c8e00f48726981d32da3dc60f028920bd258`.
