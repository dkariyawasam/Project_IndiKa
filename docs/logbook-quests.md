# Logbook progression

The logbook has nine main entries: The Nature of Evolution, Gym Leader Trials,
Apex Pokémon, Giovanni's Ambition, Rocket League, Indigo League, Expedition
Homecoming, Field Promises, and Living Relics.

## Research and story

Nature has three research categories, each completing at two distinct discoveries.
Counters remain uncapped: Instinct can display 8/2, Design 13/2, and Bond 33/2
with the currently defined evolution routes. Repeating the same route counts once.

Instinct records the eight scripted Apex encounters. Design records creating
Porygon; evolving Porygon to Porygon2 and Porygon2 to Porygon-Z or Porygon3;
reviving Omanyte, Omato, Kabuto, Kabustar and Aerodactyl; and evolving the four
revived species with evolutions. Porygon3 is supported, but its Patch Disc is
still intentionally undistributed.

Bond records all other trade / held-item trade evolutions and friendship
evolutions (including time-of-day and move-type variants). Electivire, Steelix,
Scizor, Rhyperior and Magmortar count toward Bond, not Design. Wild catches,
ordinary gifts, failed or cancelled evolutions do not count. Successful evolution
hooks and successful fossil gifts record distinct milestones in unused save flags.
The source lists are in `src/data/nature_research.h`.

Old completed NPC trades, Porygon creation and Amber revival remain recognised.
Oak's acknowledged Bond/Design categories retain a minimum of two credits.
Older saves do not contain reliable evolution history, so other past evolutions
or revivals cannot be reconstructed from Pokédex ownership and must be witnessed
again to contribute to the detailed counter. Save structure size is unchanged.

Porygon research lives in Evolution by Design. Its objective changes with
Blaine's gift, the lab's Up-Grade, Porygon2, the Rocket scientist's Dubious Disc,
and Porygon-Z. The undistributed Patch Disc is not advertised.

The status readers provide the same live completion result to rows, counts,
footers, and gameplay consumers. League reception unlocks its entry; saved
League records determine completion. Existing Indigo Hall of Fame saves count
as complete even without newer League records.

Expedition Homecoming appears after Oak acknowledges the Nature research.
It follows the existing expedition prerequisites and becomes reward-ready when
the final lab conversation is available. Receiving the certificate completes it.

## Field Promises

The list records all sixteen NPC trade offers, the Dojo Black Belt, and the
unclaimed Budew, Golbat, Chingling and Porygon rewards from the leaders. Trade
requests are discovered by speaking to the trader; defeating the appropriate
trainer reveals the reward reminder. Unknown requests are omitted entirely.
Completing all known requests marks the entry done; discovering another pending
request makes it active again. This is a list of known commitments, not a claim
that every possible trade has been discovered.

Old completed trades and collected gifts are recognised from their existing
flags. Old uncompleted offers need another conversation to record discovery,
because earlier saves did not store whether those offers had been heard.

## Save compatibility and verification

The nine main quests still use the existing six-byte quest array. The original
24 subquest bits and their offsets remain unchanged. Field Promises reads
existing completion flags and newly named, previously unused discovery flags;
it does not allocate new subquest save bits. Compile-time checks guard these
limits.

Run `python3 tools/test_logbook_progression.py` for synthetic old/new-save checks
of production state readers, League records, discoveries, reward completion,
research categories and the finale. `python3 tools/test_oak_expedition.py` checks
Oak's actual expedition gate and reaction ordering. These source-level tests do
not replace an emulator playthrough of every dialogue or menu state.

## Living Relics

This private field investigation appears after seeing any of deep-form Magikarp,
deep-form Feebas, Relicanth, Kinkabuto, Amunyte or Aeropteryx. Unseen species
remain unnamed with question-mark icons. Each entry completes when caught;
all six complete the quest. Existing Pokédex records count. No NPC claims prior
knowledge of their survival, and no extra item reward is promised.

Quest 8 uses the remaining five-bit slot in the existing six-byte quest storage.
The allocation is fixed rather than rounded up from the quest count; save offsets
and the original 24 subquest bits remain unchanged. Its six objectives are derived
from Pokédex records and allocate no subquest bits.
