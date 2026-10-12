# Prepared-party checks — 12 October 2026

Follow-up to the Koga tier 6 and Erika tier 7 basic-party losses. One seed-11 native battle per leader on the disposable emulator fixture; campaign save is not changed.

Same six evolution families and per-species levels as the basic trials. Graveler, Growlithe and Pikachu become Golem, Arcanine and Raichu. Explicit moves replace native default moves (see `cases.json`). Koga's lead pair changes to Raichu/Golem to answer Crobat; Erika retains Pidgeot/Arcanine as the opening pair. Levels are 40–41 for Koga and 45–46 for Erika. All parties have zero EVs, native random IVs/natures, no held items, no Bag use and no proactive switching. Attack selection and forced replacements use the existing input controller.

This changes evolution stages, moves and Koga's opening order together. It tests whether practical preparation helps, not which individual change caused an outcome. Same seed does not guarantee identical IVs or subsequent RNG across differently ordered scripts. These are two individual samples, not general win-rate estimates or a natural-progression availability audit.

`verify-fixtures.py`, run from the repository root, checks Pokémon checksums, species, levels, zero EVs, all four moves, and opposing species/levels. `driver.lua` requires the QA callback harness and the current ELF symbols in `qa.sym`. TSV columns: case, native outcome (1 win / 2 loss), completed-turn counter, survivors, remaining party HP, total maximum party HP.

ROM SHA-256: `ad3b40c98111ef824c4348cd446815969df3ab9676cc2fb36b47d0821299a2f1`.

No production balance changes are included in these checks.

## Results

| Leader | Basic party | Prepared party | Completed turns | Survivors | Party HP |
|---|---|---|---:|---:|---:|
| Koga tier 6 | Loss | Win | 28 | 3 | 195/710 |
| Erika tier 7 | Loss | Win | 5 | 6 | 614/799 |

All 12 starting Pokémon passed checksum, species, level, zero-EV and four-move validation. Both enemy rosters matched expectations. Both battles completed and returned to an unlocked overworld. The controller was stopped afterward.

Koga's Crobat/Arbok opening became manageable with the revised lead and coverage, but Muk and Swalot extended the endgame substantially. The observed live battle included poison/paralysis and continued normal attack selection; no freeze occurred. This is a pacing observation, not enough evidence to alter the roster. Erika was comfortable for a Fire/Flying opening with deliberate moves. That is an expected favourable matchup, not evidence by itself that her team needs strengthening.

Recommendation: retain both rosters. The basic-party losses do not establish overtuning; these prepared parties won without Bag use or proactive switching. Additional samples with different preparation/tactics would be required for a broader difficulty conclusion. No production files or balance values changed in this task.

Earned campaign save SHA-256 remains `001a04fc396009ff0a8fecd353eba53a721b32f6577dc2c19950472aa66b4ac4`.
