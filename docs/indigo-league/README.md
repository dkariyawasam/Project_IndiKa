# Indigo League single-room challenge

The Pokémon Centre north entrance already led to the arena (internally named
PokemonLeague_BrunosRoom). Its existing saved challenge chooses four different
trainers from twelve: Lorelei, Bruno, Agatha, Lance and the eight Kanto Gym Leaders.
The fifth opponent is the rival Champion, selected for the starter and game-clear
state. These choices and trainer teams are preserved.

This update adds a visible opponent using each selected trainer's overworld sprite.
They enter the battle area and leave after their round. The Champion door stays
visibly closed and impassable for rounds zero through three, opens after the fourth
victory, and restores its correct state when loading the map. The existing Champion
entry script additionally checks the active Indigo challenge. Progress messages
count victories and direct the player north after four wins.

The arena requires two usable Pokémon before starting its double battles. Talking
to the lobby aide explains the pool and that losing or returning to the Pokémon
Centre resets the run. The lobby transition now also resets an Indigo run, covering
both scripted exits and ordinary door warps. Existing whiteout reset remains.

Validation: main ROM build passed; verify.py passed connection, twelve opponent
branch/sprite, floor collision and game-font dialogue-width checks. The closed/open
map renders were generated from game tiles and reviewed. They are map previews,
not emulator screenshots. Live playthrough remains unverified: mGBA computer-use
access timed out before interaction, so no user emulator session was altered.

## Regulated battle rules

Indigo now forces doubles for every pool opponent and Champion. Both parties are
recalculated at level 50 after the enemy party is created. All six party slots remain
available, with moves, EVs, IVs and species retained. Player Bag actions and opponent
trainer items are disabled; held-item effects remain available. Existing Indigo EXP
suppression remains in place.

A 600-byte EWRAM party snapshot is taken before scaling. On battle return, including
losses, it restores the original party and then heals HP, status and PP. Consumed or
stolen held items and original party order are restored too. The snapshot is outside
the resettable battle heap and does not change the save format. Rocket battles do
not use these rules.

Validation: build and dialogue checks passed. `test_battle_rules.py` executes the
production scaling/restoration functions with a host Pokémon API fixture, checking
six-member preservation, both teams at 50, restoration after party reordering and
item mutations, healing, repeated calls, both Indigo rooms and exclusion of Rocket,
link battles, inactive challenges and other maps. These are logic tests, not a live
emulator playthrough or a test of the game's stat calculation implementation.

## Live emulator validation — 2026-09-13

Completed in mGBA 0.10.5 using disposable ROM/save fixtures. The final tested ROM
SHA-256 is `e1e6ab60403d826e7f889d0e4496228cbd26833e50c3add8387514cfd9176696`.

The live test found and fixed:

- The south entrance tiles blocked movement, trapping arrivals. Their collision is
  now passable; normal walking from the Pokémon Centre into the arena passed.
- The scripted exit used obsolete Pokémon Centre coordinates. It now returns to
  the correct door at (4, 2); the actual exit warp passed.
- Defeated opponents could respawn when the camera moved, blocking the Champion
  doorway. A temporary hide flag now prevents this. The final four-round run and
  normal approach to the Champion door passed.
- The build omitted layout binaries from its map dependencies. Map and border
  edits now rebuild maps.o, and verify.py checks the built arena bytes against source.

Verified live: both parties at level 50, four battlers, player Bag rejection,
zero opponent trainer items, four-round progression, Champion doubles and Hall of
Fame transition, loss reset, re-entry, locked door after reset, and safe lobby exit.
All six Pokémon's decoded records matched their pre-battle baseline after each of
four final-build rounds, the Champion, and the loss: levels, EXP, moves, PP, held
items, friendship, EVs, IVs, HP and status. Baselines were fully healed.

These were staged mechanics tests, not a difficulty/balance playthrough. The first
battle included ordinary turns with reduced opponent HP; win/loss outcomes were
then injected to exercise every completion path. The final four-round run used
six valid Pokémon at levels 12, 35, 50, 62, 85 and 100. An earlier setup accidentally
used the unused species slot 143; it was replaced with Dragonite before the final
run. Main save and retained player-customisation ROM hashes remained unchanged.

Evidence is in `live/`: screenshots, dialogue/runtime logs and party comparisons.
Earlier notes above describing live verification as pending are superseded by this
completed run.
