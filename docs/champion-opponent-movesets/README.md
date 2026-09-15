# Champion opponent movesets

Updated the actual opponent parties in `src/data/trainer_parties.h` and rebuilt the main ROM. This is separate from `docs/champion-movesets`, whose earlier experiments modified only disposable player teams.

Scope: all three Indigo starter variants, their three rematch variants, and Rocket League Champion Giovanni. Changes affect 39 of 42 Pokémon entries. Species, order, levels, IVs, held items, battle rules, and AI flags are unchanged. Exact before/after moves are recorded in `changes.json`.

Indigo gains Steel Wing/Double-Edge on Pidgeot, special Shock Wave on Alakazam, Rock Slide on Rhydon/Heracross/Tyranitar, Ice Beam on Gyarados/Blastoise/Tyranitar, Psychic on Exeggutor, and Sludge Bomb on Venusaur. Several doubles sets gain Protect; unnecessary Earthquake use on rematch partners is reduced. Rhydon retains Earthquake and its opening Flying partner. Rematch Gyarados retains Dragon Dance with physical Return. Alakazam's former physical Shadow Ball is replaced with a special attack, respecting this engine's type-based damage categories.

Giovanni: Persian receives Shadow Ball and Thunderbolt; Crobat receives Sludge Bomb and Shadow Ball; Nidoqueen receives Ice Beam and Thunderbolt; Nidoking receives Thunderbolt; Rhyperior receives Brick Break. Honchkrow retains its existing set. Coverage complements existing Ground/Poison/Flying attacks without raising levels.

Validation: `make -j4` passed; `verify.py` checked all 168 compiled Champion move slots against source and protected file hashes. Indigo arena/terminal and Apex readiness regression checks passed. Main save and separate player-customisation ROM are unchanged. These opponent revisions have not yet been live balance-tested; the older player-team win rates do not apply to these revised opponents.
