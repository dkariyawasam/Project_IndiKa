# Full story playthrough — in progress

Requested September 13, 2026: play through the campaign and check whether dialogue
explains events and guides the player. Small wording fixes are authorized.

## Live status

IN PROGRESS. Computer access resumed after the user unlocked the Mac.
A fresh disposable game reached the forest, defeated Tangrowth after earning its
three clues, found an Energy Root through a wild held item, and completed Erika's
field battle. It then continued to Pewter and Mt. Moon following Brock's aide.
A real save/restart at the forest checkpoint loaded successfully. Brock's exposed
rock produced a Helix Fossil; returning it sent him back to Pewter Gym, where the
test earned its second badge. His post-battle dialogue points to Cinnabar's fossil
lab and Rock Tunnel's ancient Pokémon. The Mt. Moon witness and Cerulean's damaged
house witness supplied two Annihilape accounts. The Route 4 wandering witness was
missed on the first pass and remains outstanding. Following Cerulean's gym aide,
the player walked through Routes 9/10 and Rock Tunnel to Misty's scouting trial.
Misty's three current decisions and generator handoffs then ran through ordinary
movement and interaction. The first wrong direction returned her with an
explanation and a retry prompt. The final generator sent her back to Cerulean;
the aide acknowledged this, explained type advantages, and the victory awarded
the Cascade Badge/Board (third badge). Her Gyarados request was read; the trade
itself remains outstanding. The player then continued south toward Saffron.
Saffron's Cubone, Lavender's boy and the tower channeler supplied all three
Osscythe accounts. The rival battle ran in the tower. Sabrina rejected Charizard
as incompatible; Gastly then learned Dream Eater through the tutor menu. Her aide
arrived, gave type advice and witnessed the battle. The fourth badge and Marsh
Finder were awarded. Osscythe appeared upstairs and was defeated; Oak's response
connected its readings to grief/memory. Mr. Fuji gave the Poké Flute and explained
sleeping obstacles. Back at Saffron Gym, the player used the teleport pads,
confirmed the aide was present, and received Chingling with the correct
friendship/night evolution explanation. The next destination is Vermilion.
Vermilion's aide led to Surge in Diglett's Cave. Pikachu learned Volt Tackle,
Surge returned, the aide gave Ground-type advice, and victory awarded the fifth
badge/Thunder Pass. The rival's Route 11 fieldwork scene, Chaplin's post-battle
account and both cave entrance witnesses led to Mime Sr. Its encounter completed
and Oak explained the barriers as protection for a social group. The test then
returned toward Route 2 to finish the outstanding Mt. Moon account.
The Route 4 witness completed Annihilape's accounts. Its Mt. Moon encounter was
won and Oak linked its power to uncontrolled emotion. Onward travel reached
Celadon, where the rival explained badge-scaled leader teams, Erika's aide
acknowledged the forest victory, and Erika gave Budew with daytime friendship
evolution advice. The party now has six Pokémon, including the two gym gifts.

The introduction, starter selection, first rival battle and first Route 1 attempt
used ordinary input without party changes. An early level-30 Jumpluff encounter
exhausted Tackle, and a subsequent Route 1 trainer defeated the starter. To focus
on dialogue rather than grinding, the test then added level-100 Charizard and
level-50 Pikachu/Gastly, placed Charizard first and provided Flamethrower. Thief was
used temporarily to obtain a wild Energy Root, which was taken through the party
menu. The original starter remains in the party. Some test-team healing uses the
native HealPlayerParty special. Story flags, badges, Apex rumours and trial states
have been earned through map interactions so far. No travel warps have been used.
The exception is a separate cave regression fixture: the test saved an emulator
state, staged the signal flag/map locations to check both entrances before/after
the blast, then restored the exact continuous state. The story's signal flag was
confirmed unset afterward. Those fixture warps are not counted as played travel.
This is an accelerated story playthrough, not a difficulty/balance certification.
Automated name entry named both player and rival AAAAAAA. Text logs may contain
control-code decoding artifacts; screenshots verify the rendered text.

The initial live ROM was `366202f2e2ec069867d97af309aef75de83dc3b26ab832ef0ac1fed70e173475`.
After the forest checkpoint, the test loaded the updated main ROM
`de8f0ed867150ec863a7d520495bf927536cc733de1e969cc441b7fb5c28726a`.

## Live findings fixed

- At the initial lab visit, the rival said only that Oak was absent. Added a lead
  toward Route 1 at the north end of town, where the opening event occurs.
- Oak now warns that even familiar routes can contain strong evolved Pokémon,
  and distinguishes ordinary wild-battle retreat from the permanent Apex retreat.
- Erika said the Energy Root was somewhere nearby without explaining acquisition.
  Oddish/Bellsprout have it in their common held-item slots and appear in this
  forest. Her request and journal now point to those Pokémon and their held items.
- Brock's request now identifies the exposed tunnel rocks as the fossil search
  targets. The actual Helix Fossil pickup was tested before this wording change.
- Brock's active journal now repeats the location and exposed-rock clue.
- Misty's trade offer now asks whether the player is ready to trade Gyarados,
  avoiding her enthusiastic assumption that it is already present before the
  party-selection check.
- Sabrina's unsuccessful tutor interaction now names Gastly, available in the
  tower and compatible with Dream Eater, instead of only giving an abstract hint.
- Sabrina's aide now acknowledges the witnessed tower victory and directs the
  player to her friendship lesson, replacing an unrelated slots joke.
- Surge now names Volt Tackle before tutoring and explicitly asks for an Electric
  type; that matches his actual tutor eligibility check.

## Progression fixes found live

- **Celadon Cave existed before its creation.** From Route 4 at (93,18), ordinary
  south input entered the cave before Giovanni's signal. The pre-signal map now
  restores a solid cliff on Route 4 and seals the Route 7 opening. The source map
  supplies the open tiles after the signal. Six isolated runtime checks passed:
  both sealed entrances and all four post-signal entry/exit directions. The
  natural later blast scene remains to be reached in this campaign.
- **Stale cave exit:** Celadon Cave's south exit referenced Route 7 warp 2, but
  Route 7 has only entries 0/1. Corrected to 1 and verified the actual exit lands
  at Route 7 (9,23).
- A broader fixed-destination audit found four more stale indices: both Route 16
  gatehouse outside exits, its upstairs return staircase, and Saffron's Trainer
  Fan Club exit. Corrected to their reciprocal entries. All 880 fixed destination
  references pass `verify_warp_targets.py`; live checks of these four remain.

The cave fix and accumulated wording changes were loaded in the test ROM
`b62c6341549869b21d098975e64770057eea55a71278d9f3b6f13598dca25f00`.

These new lines passed the build/width checks. The already-completed opening
branches still need separate rendered-message checks on the updated build.

## Initial source review and changes

Reviewed Oak's opening briefing, Route 2's alternative-route advice, Brock's first
trial/trade handoff, Sabrina's gift and journal states, Mansion/Silph/Route 7/Mewtwo/
Viridian handoffs, and League entry/reward explanations.

- Sabrina's journal now directs the player back to Saffron Gym for Chingling, then
  explains friendship and leveling at night to obtain Chimecho. These match the
  gift script's states 4/5 and the species' evolution method.
- Rocket's receptionist and terminal now warn that beating Viridian Gym is required
  to face its Champion. Previously this requirement was first disclosed at the
  Champion door after the pool battles.
- Rocket's multiplier prompts now describe arena prizes for the run, rather than
  only the next prize. The prize tier is reused by each pool reward and reset when
  the run ends. The board's x1 label now explicitly identifies the base multiplier.

Build and `verify_wording.py` passed. Main save and customization ROM unchanged.
Initial source-review ROM SHA-256:
`366202f2e2ec069867d97af309aef75de83dc3b26ab832ef0ac1fed70e173475`.

## Remaining playthrough coverage

Use a new disposable save and a temporary copy of the current ROM. Follow actual
player-visible leads; record the route taken and any confusion. Cover opening and
research tutorial; gym discoveries, trials and rewards; Apex clue chains and Oak
reports; Giovanni's Mansion → Silph → Route 7 → Cerulean Cave → Viridian sequence;
Rocket and Indigo entries/championships; and final research/story handoffs. Check
journal objectives alongside spoken directions, including returning after a break.
Record any skipped travel, staged conditions or battle shortcuts explicitly. Do not
claim a continuous playthrough for staged tests or certify unvisited branches.

## Continued campaign: Cycling Road, Seafoam, and Cinnabar

The corrected Route 16 gatehouse stairs and both exits passed live round trips.
Rented a bicycle through the guard, rode Cycling Road to Fuchsia, and received
Janine's Cinnabar Volcano directions at the gym. Launched and registered Misty's
Cascade Board through the Bag, then traveled Route 19/20 normally. Collected all
three Articuno accounts (Mariner, Seafoam entrance witness, wandering Camper).
The fast-water passage refused board use; deferred the boulder puzzle until
obtaining Strength from the Rocket League lobby's TM counter. Continued west by sea to Cinnabar.
Several scripted navigation stalls here were shore dismounts and trainer movement,
not confirmed game defects; routes were corrected with ordinary directional input.

The Cinnabar rival scene explicitly assumed ferry travel despite this sea arrival.
Changed its greeting to “So you made it to CINNABAR after all.” The Mansion/lab
lead that follows remains intact. Also added clear Bag-use instructions to Misty's
board handoff and Strength/current-blocking directions to Route 20's Mariner.
These three wording changes await loading into the live test ROM.

Koga's volcano request, checkpoint, water-source scene, Janine's arrival, shared
stone retrieval, type advice, witnessed battle, Soul Badge/Pass reward, and return
to Fuchsia instruction all played. This is badge six. A manual party replacement
was needed when the test Pikachu fainted; this remains a story test, not a balance
assessment. Collected the volcano Kindler's Moltres account after his battle.
Cinnabar lab, Blaine, Mansion, later Apex encounters and leagues remain in progress.

Moltres's remaining accounts came from Cinnabar's old man and the lab researcher,
with Boulder Key access working. The volcano encounter resolved and Oak linked it
to renewal. Blaine's aide, battle, seventh badge and Porygon machine handoff played.
The generator accepted Yes/No/Yes, gave Porygon and transferred it to the PC because
the party was full. Saved outside Cinnabar Gym before entering the Mansion.

The Mansion statue, rival battle and Giovanni scene ran. Giovanni explicitly names
Silph in Saffron and an infallible Ball as his next objective. Returning south over
his still-active coordinate trigger then blocked all directional movement. A raw
held direction reproduced the stall. In an isolated savestate diagnostic, setting
only VAR_TEMP_0 to 1 disabled that completed trigger and immediately allowed travel
from (7,21) to (7,26). Restored the original state after this diagnostic.

Fixed the trigger to consume its temporary scene variable when entered, and set it
on map transition when the persistent Giovanni-seen flag is already set. Rebuilt
and resumed the real Cinnabar save to replay the scene and verify the fix. The new
ROM also includes the three travel/greeting wording fixes above. SHA-256:
`4b2bfa13d02218d5bb498c4eaf5c65fa20178d40d60aa832cae1eec2cc6af89a`.
Protected main save and player-customization ROM hashes remain unchanged.

The rebuilt Mansion scene passed normal completion and southward crossing, then
a basement exit/reentry and both-direction crossing with the persistent seen flag.
Returned to Celadon via Route 21 and Cycling Road. Bought coins, defeated the
Game Corner poster guard, revealed the stairs, and bought TM54 (Strength) at the
Rocket League lobby counter. The earlier draft direction to the prize building
was corrected to the Rocket League lobby: that is the project's sole TM seller.
Read the receptionist's complete rules, including the Viridian Gym requirement,
and declined entry for now. Added a short explanation of the poster's stair switch.
These last two wording changes are pending the next build.

Silph's elevator, 5F Card Key, 3F locked door, 7F rival battle, transfer tiles and
11F locked door have played. The rival connects the Master Ball to control and
directs the player upstairs. The boardroom scene recognizes the Mansion meeting
and explains Giovanni's plan to manufacture Master Balls to reclaim Mewtwo.
The central battle and president's follow-up are in progress.

After restoring the pre-boardroom checkpoint and healing the exhausted test party,
Giovanni was defeated. His withdrawal explicitly names Route 7 and signal research.
The president gave the Master Ball, explained the research theft and added the
second Mewtwo account. Saffron Fan Club's corrected exit passed an actual round
trip. Saved in Saffron before continuing.

All three Route 7 aftermath witnesses played their flashbacks in order. Agatha
explicitly identifies the cave as opened by Mewtwo's blast. Lance's cave flashback
completed once; a repeat interaction gave only the Cerulean Cave directions. His
account completed the Mewtwo rumors. Traveled through the northern Route 4 spur,
Cerulean Cave's river and upper maze, and reached Giovanni. His failure/step-aside
scene played, Mewtwo was defeated, Oak's report played, and Giovanni explicitly
returned to Viridian Gym. Runtime flags and Mewtwo's won outcome were checked.

A second isolated revisit diagnostic reproduced the same completed-trigger lock
at Cerulean Cave (7,16). Disabling only VAR_TEMP_0 allowed movement again. Restored
the exact pre-diagnostic campaign state, moved the trigger's temporary-variable
assignment ahead of its completed-state guards, saved, rebuilt and resumed.
The new ROM includes the corrected Rocket League TM-counter hint and poster-switch
explanation. SHA-256:
`32cfef7056c65294262e40637090243bfbd148f72e969f98a4d6895012aee159`.
The cave fix's rebuilt live check and remaining campaign are still in progress.

The rebuilt Cerulean Cave trigger passed both directions after loading the real
save. Traveled to Route 10 and the Power Plant. Wade stood at (33,31), the only
passage to the upper floor; even after defeat he blocked Baily and Zapdos.
Moved him to the wider floor at (32,32), preserving his battle. The new collision
reachability check passes with all NPCs present. Rebuilt and resumed outside the
plant; physically crossed the passage, battled Baily, heard his account, then
reached and defeated Zapdos. Oak's electricity explanation played; won outcome
was checked. Current ROM SHA-256:
`ffe14040ab6bfdd58b8e5817c51d1df2e862f0eb992cd92eb57f2b2883f68120`.

Koga's returned aide recognized the volcano trial. Koga gave Golbat through the
full-party PC branch and explained friendship evolution. Learned purchased TM54
Strength through the Bag, replacing Fire Spin on the test Charizard. Returned to
Seafoam, activated Strength and pushed two boulders into the B3F holes. Both
vanished, but neither lower-floor blocker appeared: the pushable objects' reveal
flags had been replaced with TRAINER_TYPE_NONE. Restored the reveal metadata on
all four Seafoam floors and added verify_seafoam_boulders.py. Replayed from the
pre-puzzle checkpoint, saved upstairs, rebuilt and reentered the cave. Repeated
the actual pushes; B4F's current-stopped flag now set, the Cascade Board launched,
and Articuno was reached and defeated. Oak's temperature/adaptation report played.
The earlier failed current check is retained in runtime-tests.tsv, followed by
the passing rebuilt check. No story flags were injected to bypass the puzzle.

Returned through Cinnabar and Pallet. Oak's normal Dex review played. All eight
Apex encounters have now been completed in the accelerated campaign; this does
not imply a complete Pokedex or every optional friendship/trade quest.

Giovanni's final Gym introduction recalled the earlier story. Won the double
battle after replacing fainted partners, received the Earth Badge, and heard his
trust-focused change of course. Added explicit west-to-Route-22 / north-through-
Route-23-and-Victory-Road directions and Ground-type counter advice. Changed the
Rocket Champion's closing line to match that resolution. The first badge runtime
assertion used the vanilla system-flag base and was wrong; the corrected project
address and defeated-leader flag passed. The earlier failed assertion is retained
with the correction in runtime-tests.tsv.

Route 23's earned-badge checks passed. In Victory Road, collision/push-state
analysis showed Ivo's (18,18) position blocked the first-floor switch puzzle even
when the other trainer was ignored. Moved Ivo to (15,18), keeping his battle.
The portable push-state regression test passes with all trainers present. Rebuilt,
entered normally, used Strength and solved the switch puzzle; the runtime scene
variable reached 100. The second-floor switch was also solved normally.
Inspection found Victory Road's falling boulder also lacked its lower-floor
reveal flag, like Seafoam. Restored it and saved/rebuilt before entering 3F.
That final drop and both Leagues remain in progress.


## Completion

Completed Victory Road’s third-floor drop and lower switch through actual pushes. Won Indigo’s four pool matches (Brock, Koga, Lorelei, Agatha), then the Champion. Fixed the Hall of Fame trainer animation crash, and verified the corrected presentation and credits by explicit native diagnostic replay. Returned to Pallet. Walked back to Celadon and completed Rocket’s four matches (Sable, Ariana, Shion, Viper), then Giovanni. Received 5,000 coins. Both record screens show five wins and one championship. Rocket reward/prize-board menus were read; the multiplier purchase was declined. See README.md for the final scope, limitations and build.
