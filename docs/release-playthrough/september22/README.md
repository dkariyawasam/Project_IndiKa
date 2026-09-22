# Continuous playthrough resumed 22 September

IN PROGRESS. Loaded the 18 September earned campaign save (Pewter Gym, zero badges, Bulbasaur 9/Hoppip 7/Weedle 8) against the main ROM after the evolution-link fixes. Dedicated emulator and save in `/tmp/kanto-release-september22`; no party, inventory, flag or position writes. Movement/battle harness uses button inputs and read-only memory. No rollback for battle outcomes.

[Balance findings for review](balance-review.md) — report-only; no trainer level or team changes.

## Confirmed so far

- Route 3 rival Gym conversation triggers on the approach to Mt. Moon; completed without battle.
- Route 4 Timmy battle won; Bulbasaur learned Vine Whip in place of Growl and reached level 11.
- Kinkabuto wild encounter displays the known question-mark battle sprite. Battle itself completed normally.
- Mt. Moon travel battles reduced Bulbasaur to 3 HP. Four subsequent wild encounters escaped with Retreat, HP unchanged.
- Brock's B1F conversation points to exposed rocks. Found Helix fragment at the nearby rock through normal interaction. Brock accepted it, gave Dart fragment and returned to Pewter Gym.
- Route 4 healing house restored the earned party to full HP.
- Route 3 Greg's level-18 Machop caused Bulbasaur to faint. Weedle poisoned and defeated it; battle won without rollback. Weedle evolved into Kakuna. Party afterward: Bulbasaur 12 (fainted), Hoppip 7, Kakuna 9.
- Pewter Pokémon Centre healed the party.
- Gym aide recognizes Brock's return and advises Water, Grass and Ground moves; dialogue explicitly connects fossil hunting to Brock's return.

Current checkpoint: five badges (Brock, Misty, Surge, Erika and Sabrina), healed and saved in Fuchsia after Routes 12–15. This is an ongoing playthrough, not a completed campaign certification. Koga, Blaine, Giovanni, Apex encounter resolutions, remaining rival story steps, and both Leagues remain. Balance findings are report-only at the user’s request.

## Boulder Badge

Brock's double battle won on the first attempt. Bulbasaur finished level 13 at 26 HP; Hoppip fainted and Kakuna replaced it. Badge/Cinnabar Lab access reward delivered. Follow-up explicitly points to ancient waters beneath Rock Tunnel for the next evolution lesson. No injected party or retries.

## Cerulean Gym softlock found

After travelling normally to Cerulean and healing, first Gym entry locked controls indefinitely before the aide dialogue. Read-only runtime state: player (8,16), aide local ID 2 at (6,14), initial (7,14), movement action 0x12 (walk left), flags 0x103 (active, single movement active, frozen). The aide used WALK_LEFT_AND_RIGHT; the entrance script uses lockall, which freezes NPCs immediately. Its following scripted movement could not complete. Changed the aide to FACE_DOWN so her scripted approach starts from the expected stationary position.

Recovery requires reloading the last normal cartridge save (Brock victory), then replaying the travel with ordinary inputs. This is a bug recovery, not a rollback to improve a battle result. The original attempt earned level-15 Bulbasaur with Sleep Powder and reached Cerulean; its outcomes remain in the logs.

## Fix verified and Route 9 recovery

Rebuilt the main ROM, replayed the journey from the normal Brock save, and saved at Cerulean Pokémon Centre. The fixed aide approached, explained Misty's location and released controls normally.

Route 9 Scout Isaac was defeated; Bulbasaur evolved into Ivysaur at level 16. The following Black Belt F battle (Cora; legacy script label Conner) with a level-22 Mankey defeated the three-Pokémon party. Blackout returned to Cerulean Pokémon Centre and restored the party normally; no reset was used to erase the loss. This exposes a party-strength gap on this path, but is not yet evidence for a balance change: the supporting Pokémon are only levels 7 and 9, and the basic battle pilot does not use status tactics intelligently.

Cora's full roster is Mankey 22 and Machop 25. On the normal retry, Leech Seed enabled Ivysaur to defeat Mankey, then Machop knocked it out. Nearby Scout Isaac's Mankey is level 12. Record this local difficulty contrast for balancing review; no trainer data changed.

## Northern route and capture coverage

Nugget Bridge's first trainer clearly introduces the five-trainer challenge. Ivysaur reached level 17 during that battle, but the level-20 Qwilfish defeated the remaining party. Recovery again worked without a reset. The run is now strengthening the supporting party before progressing. Caught Wurmple 8 on Route 5 using Sleep Powder and two Poké Balls; registration, declining a nickname and return to the overworld worked.

Harness limitation: initial damage-only move selection underused Ivysaur's status moves. Updated the button-input pilot to use Leech Seed against healthy non-Grass trainer opponents. This is a testing strategy change, not a ROM or party-state edit.

## Capture experience did not trigger evolution

Live reproduction: caught Minun 7 on Route 5 with Kakuna as the active Pokémon. Capture experience raised Kakuna from level 9 to 10, but after declining Minun's nickname the game returned directly to the map. Read-only party inspection confirmed Kakuna at level 10.

Cause: `FreeResetData_ReturnToOvOrDoEvolutions` allowed evolution checks only for `B_OUTCOME_WON`, despite the project awarding experience on captures. Updated it to also allow `B_OUTCOME_CAUGHT`; losses, escapes and other results still bypass evolution. Main ROM build passed. Saved the earned five-Pokémon party, healed, then reloaded the new ROM. Live post-fix verification is pending.

Post-fix live verification passed: captured Ariados 23 (party reached six), then Wurmple 8. The latter capture raised Kakuna to level 11; Wurmple was sent to BOX1, and Kakuna immediately evolved into Beedrill. Screenshot `capture-evolution-fixed.png` records the success message. All captures and experience were earned through normal inputs.

## Route 9 rematch roster used on first encounter

Ruin Maniac Jovan at (14,22), legacy trainer ID `TRAINER_YOUNGSTER_EDDIE`, uses `sParty_RematchMtMoonRuinManiac` on its first battle: Clefairy 31, Onix 33, Amunyte 35. This is much higher than Scout Isaac 12 and Cora 22/25 on the same route to Misty's trial. The earned party eventually won: Ariados defeated Clefairy but fainted against Onix; Ivysaur finished the battle and reached level 19. Amunyte still shows its known placeholder battle sprite. The user requested that balance findings be recorded for review. Trainer levels and teams remain unchanged.

Cora was defeated on the subsequent attempt: Ivysaur used Leech Seed, then Beedrill finished Machop after Ivysaur fainted. Beedrill reached level 12. The party was then intercepted by Hiker Brice (Geodude 20) on the route toward the Route 10 healing house while three members had already fainted.

## Route 10 reached

Brice defeated on the healed attempt; Ivysaur reached level 20. Route 10 Tuber Dillon also defeated. Reached Route 10 healing house, accepted a rest and confirmed the six-Pokémon party fully healed. Saved normally there (`route10-healing-house-checkpoint.sav`). Entering Rock Tunnel next.

## Rock Tunnel and healing-house dialogue

First Rock Tunnel trainer defeated, with Ariados finishing the Fighting opponents and reaching level 24. On B1F, Ruin Maniac Winston's level-25 Onix knocked Ariados out with a critical Rock attack while Ivysaur was fainted; the remaining low-level party lost. Recovery correctly placed the player at (4,5) directly in front of the Route 10 house healer. However, her first line incorrectly said "MOM" and "Welcome home": the house recovery script reused the mother's text. Added a separate generic house-healer recovery line. Live verification of the new line is pending; the position was verified on the original recovery.

Continued from the house save on the rebuilt ROM. Winston defeated with Ivysaur; the subsequent Fighting trainer defeated with Ariados (level 25, learned Leech Life in place of Constrict). Used the two remaining Potions during the latter battle. Collected the B1F Revive at (2,37) and used it on Ivysaur; item presentation and recovery worked. Next Hiker defeated; Ivysaur reached level 22 and replaced Vine Whip with Razor Leaf. Reached the northwest 1F ladder via B1F (33,3).

Testing controls now support normal forced replacement after fainting, and use Retreat/Run for incidental wild encounters. Live NPC positions are read to avoid navigation stalls when a trainer has moved from its map-template coordinates. No game-state writes are used. After the dialogue-only build, ROM data addresses used by the pilot were refreshed; a stale address briefly caused repeated Sleep Powder selection against Winston's Onix, a harness issue rather than a game issue.

## Misty trial completed

Reached B2F via the northwest 1F ladder and central B1F ladder. Misty's introduction explains the link between underground currents, generators and the Power Plant's surges. Tested the first wrong direction: she returned, explained the looping current and allowed another choice. Correct directions and both generator handoffs worked; the second interaction explicitly explains that she can hear the player through the wall. Final dialogue states that the plant is fixed, asks the player to return to the Gym and introduces the upcoming evolution lesson. Misty departed and controls returned normally. Read-only runtime check of `VAR_MISTY_TRIAL_STATE` confirmed 9 (the compiled SaveBlock1 vars offset is 0x1010; the source comment 0x1000 is stale).

Exited B2F through the Power Plant, collected the Max Potion and reached Route 10 on foot. Returning to the healing house and Cerulean for Misty's Gym follow-up.

## Cascade Badge

Returned to Cerulean normally after healing. The aide recognized Misty’s return and recommended Grass and Electric attacks. Misty’s double battle was won on the first attempt: BEACON (Chinchou 17), PUDDLES (Poliwhirl 20), and TWINKLE (Staryu 23). Ivysaur 25 and Ariados 26 survived. Cascade Badge and Cascade Board were awarded, with instructions for using the board at water. Misty clearly requested Gyarados for her held-item evolution trade; declined for now. Saved normally (`misty-earned-checkpoint.sav`). Two badges earned; the full campaign remains in progress.

## Southern route difficulty

Healed in Cerulean, purchased 12 Great Balls (plus the normal Premier Ball bonus) with earned money. An attempted Super Potion purchase did not complete; the missing stock was discovered during Erika’s third attempt. Route 5’s Bug Catcher and subsequent route trainer were defeated normally. Psychic Preston at the southern junction uses the shared `sParty_GymAdjacentPsychicB`: Drowzee 30, Kadabra 32, Xatu 34. This is another early-route level spike, encountered while travelling toward Vermilion with two badges. No balance data changed.

Preston defeated the party; normal recovery returned to Cerulean and healed it. Continued without resetting. The southern edge of Route 5 permits a route around his sightline to the Underground Path entrance, so this is not a hard progression block. Defeated additional Bug/Picnicker trainers en route; tunnel entry worked. A later party-selection stall was in the test harness: voluntary battle selection uses action 0 as well as action 1. Updated the input handler; no corresponding game change.

## Vermilion and Surge trial

Traversed the Underground Path and Route 6 through normal encounters. Porygon and transformed Ditto rendered correctly in the tunnel battle. Healed at Vermilion. Gym aide correctly approached, directed the player to Diglett’s Cave, returned and released control.

Triggered the rival’s Route 11 scene outside the cave: he names Mime Sr., mentions invisible walls and missing Mr. Mime, and directs the player to compare three local accounts in the Logbook. The reflection emphasizes evolution without a Trainer. No rival battle; departure and control release worked. The cave entrance can be reached from the west before crossing the rival’s trigger tiles; noted as an optional encounter/presentation detail, not a confirmed defect.

Found Surge in the cave. His tutor selection correctly marked only Minun eligible in this party. Minun learned Volt Tackle, Surge explained the move and invited the player to Vermilion Gym, then departed normally.

## Thunder Badge

The returning-Surge aide dialogue explicitly recommends Ground types, explains Electric immunity and Ground effectiveness, warns about paralysis, and states the doubles requirement. Surge defeated on the first attempt using the earned Ivysaur 28/Ariados 27 pair. Thunder Badge and Thunder Pass delivered; follow-up explains expanded Mart stock and Raichu/Electabuzz-with-Electirizer trade. Entered then cancelled the trade selection successfully; no Pokémon exchanged. Saved normally (`surge-earned-checkpoint.sav`). Three badges; the campaign remains in progress.

## Celadon rival and Erika’s forest challenge

Celadon rival battle triggered immediately on the Gym approach and was won. Follow-up clearly contrasts trainer-assisted evolution with wild evolution and explains that Gym teams adjust to badge count. The carried-over September save already has Erika’s forest introduction (trial state 2), so the Celadon aide is correctly absent pending the field challenge; this entrance introduction was not replayed.

Collected all three Mime Sr. accounts, including Chaplin’s post-battle conversation. Logbook feedback states the three-account requirement and permanent consequence of fleeing. The active confirmed Mime Sr. lead and its witness dossier open correctly. Its Apex silhouette remains unrecorded. Traversed Diglett’s Cave normally to Route 2.

Caught Bellsprout 7 in Viridian Forest (second Great Ball, following Sleep Powder). Pokédex registration worked. Took its Energy Root through the party Item menu. Showing the root to Erika triggered the aide’s arrival, type advice, and explicit official-challenge explanation. Erika won the first battle; normal blackout returned to Celadon. Her 3-badge roster is Victreebel 29, Tangela 24, Vileplume 29, Exeggcute 32. The party had poor offensive coverage and suffered sleep/setup; no balance change made.

Deposited Hoppip, Wurmple and Beedrill normally in the PC before the forest task. After the loss, caught Ninetales 27 on Route 8 (Sleep Powder, one Great Ball); registration worked. Healed and moved it to the lead position alongside Ariados. Preparing a retry using Ember/Safeguard; ordinary button inputs only.

## Rainbow Badge

Second attempt lost, but trial state correctly restored the aide in the forest and offered a direct retry. Third attempt won with Ninetales/Ariados: used the collected Max Potion on Ninetales and manually focused both attackers on the Swords Dance Tangela. The pilot previously focused the other enemy and allowed Tangela to build up; these losses are not by themselves evidence for weakening Erika. Read-only battle HP and party-menu HP agreed.

Victory explicitly confirms the witnessed official Gym challenge, awards the Rainbow Badge and Rainbow Key, points to the Celadon friendship reward, and explains Berry Patch access. Erika and aide departed, controls returned. Saved normally (`erika-earned-checkpoint.sav`). Four badges; reward follow-up and later campaign content remain.

## Route 6 trainer blocks the northern opening

After Psychic Lucian’s approach/battle (won), he remained at (9,6), directly south of Route 6’s only traversable opening on row 5, at (9,5). The player at (10,6) could no longer reach Saffron by that route. Screenshot `route6-trainer-chokepoint.png` and live object inspection confirm the position. His original (6,6) location and LOOK_AROUND/range-4 behavior allow this sideways approach. Changed only his movement type to FACE_DOWN so his approach cannot occupy the northern choke. Main build passed; live post-fix verification pending. Took the ordinary Underground Path detour without resetting.

Reloaded the rebuilt ROM from the normal Celadon save; addresses used by the read-only harness were rechecked and unchanged. Walked Route 6 from (10,6) through (11,0) into Saffron successfully. Lucian was already defeated in this save, so this confirms restored traversal, not a fresh battle-approach replay.

Erika’s Celadon gift follow-up passed: received Budew 30 with the standard name; dialogue explicitly says strong friendship plus a daytime level-up evolves it to Roselia. The gift evolution itself remains to be completed in this run.

## Sabrina lead and Route 8

Saffron Gym aide introduction directs the player to Pokémon Tower and releases control. Route 8 Psychic Johan intercepted the crossing. The battle pilot repeatedly selected a disabled Quick Attack; this was a harness limitation, corrected by excluding the current disabled move from its scoring. No game balance or battle mechanics changed.

Johan and the subsequent Route 8 Scout were defeated without a reset. Lavender Healing House restored the party normally. Pokémon Tower rival scene passed: references Osscythe, recalls Mime Sr., proposes habitat as a theory, and directs the player to accounts/Logbook. No rival battle, and departure/control release worked.

Sabrina tutor correctly marked the current six Pokémon ineligible; cancelling provided the explicit Gastly hint and released control. Caught Chingling 6 (one Great Ball), registered it, and sent it to BOX1. Returned normally to Saffron, healed, deposited Minun and withdrew that Chingling, then saved. Existing BOX1 Gengar 100 was observed but not withdrawn or used. Current party: Ninetales 29, Ariados 30, Bellsprout 12, Ivysaur 31, Budew 30, Chingling 6.

Usability review: a full-party player catching a tutor-compatible Pokémon at the tower must return to a PC; Lavender Healing House has none. This is a pacing observation, not a confirmed defect.

## Marsh Badge

Purchased 10 Hyper Potions and 3 Revives at Saffron Mart, with quantity/cost confirmation. Chingling 6 successfully learned Dream Eater from Sabrina. Aide arrival, explicit witness explanation, Bug/Ghost/Dark advice, Dark immunity warning, and challenge prompt all passed.

Won Sabrina’s 4-badge challenge on the first attempt with Ninetales/Ariados, using six Hyper Potions (one on the wrong target due to pilot selection). Used Safeguard and focused the second enemy position. No trainer teams or levels changed. Victory awarded the Marsh Badge, directed the player back to Saffron Gym, removed Sabrina/aide, and released control. Saved normally (`sabrina-earned-checkpoint.sav`). Five badges; gift follow-up and remaining campaign still pending.

Sabrina’s Gym follow-up passed through the normal teleport-pad route. With a full party, her Chingling gift transferred to BOX1 using its standard name; follow-up explains strong friendship and a nighttime level-up. Gift evolution remains pending. The tower Channeler’s Osscythe account was also recorded, with the permanent-flee warning displayed.

## Coastal rental and Route 12 patrol clearance

The Route 12 gate correctly offered a ¥500 rental, explained coastal-route use/return, charged the fee and placed the player outside on the bike. A Route 12 fishing battle was won.

The navigator initially attempted a tile-elevation mismatch at the pier; its movement rules now check elevations. Separately, Gia and Jes patrol the two lanes at x=8 and x=9, y=53–56 and repeatedly blocked forward passage. Moved Gia’s starting x from 9 to 13 on the adjacent pier, preserving her vertical patrol, team and level. Build passed (`/tmp/route12-patrol-clearance-build.log`). Reloaded the normal saved game; map re-entry is required to refresh saved live object positions. Live traversal verification follows below.

After normal map exit/re-entry, live objects confirmed Gia patrolling x=13 and Jes x=8. Traversed the x=9 lane south from (9,53) to (9,59), back north, and south again successfully. Both patrols remain active. Main ROM includes the verified clearance fix.

The run continues south on the coastal rental. Chip, Pia, Cora and Maris battles were won; Ninetales fainted naturally and Ariados carried subsequent fights, reaching level 33. Budew moved to the lead for normal friendship-evolution experience testing. No balance edits.

Vera’s first attempt was lost (Pelipper 31, Starmie 33, Lanturn 35). Budew fainted against Pelipper; the party was already depleted from travel, including a fainted Ninetales and low attack PP on Ariados. Ariados reached 34 and learned Pin Missile over String Shot during the battle, but the remaining party lost. Normal blackout returned to Saffron and healed everyone. No rollback. Retrying with Ivysaur leading and full PP. This outcome is recorded for review, not used to alter balance.

Vera retry won with full PP, Ivysaur leading, and two Hyper Potions. Ivysaur reached 32 and evolved into Venusaur normally after battle. Cleared Route 12 and entered Route 13. The rental was renewed after blackout; the gate explanation and transition worked again.

## Fuchsia checkpoint

Cleared the traversed Route 13/14/15 trainer encounters and reached Fuchsia. Used the remaining two Hyper Potions and the collected Energy Root during this stretch; three Revives remain unused. The party was healed and saved normally (`fuchsia-earned-checkpoint.sav`): Venusaur 34, Ariados 34, Budew 30, Ninetales 30, gifted Chingling 30. Budew and Chingling friendship evolutions remain pending.

Fuchsia Gym’s Janine introduction triggered at the entrance. Her purple-haired sprite rendered correctly, and the dialogue explicitly directs the player to Cinnabar Volcano for Koga’s volcanic-rock task. Screenshot `fuchsia-janine-introduction.png`.

Purchased 20 Super Potions, 10 Full Heals, 10 Ultra Balls and 5 Max Repels in Fuchsia; remaining money ¥5,988. Route 19 shore trainer defeated. The Route 19 ferry is initially hidden by design (source verified); activated the earned Cascade Board from the Bag and entered the water normally, with control restored. Southern traversal is in progress.

## Seafoam crossing

Reached Route 20 by normal surfing; sea-route battles won, Venusaur reached level 35. Entered Seafoam from the east entrance, descended from 1F (28,19) to B2F (25,19), crossed to B2F (17,9), and returned to western 1F (10,6). Exited the western entrance successfully without Strength or boulders. The western witness delivered the lava-to-cold rumour, recorded it, explained the three-account Logbook requirement and permanent flee consequence, then released control. Articuno discovery/battle remains pending.

Reached Cinnabar via Route 20, healed at its Pokémon Centre and saved normally (`cinnabar-earned-checkpoint.sav`). Five badges remain the current count. No trainer balance changes made. Koga’s volcano sequence is next.

Koga’s 1F introduction and Left Corridor 2F checkpoint both progressed and released control. Reached the Right Corridor 3F fountain: Janine arrived, the three characters freed the lavic stone, Koga explained her role as official witness, and Janine provided Psychic/Ground/Flying-immunity/antidote advice. Accepted the field battle at five badges; opponent opening pair Crobat 38 and Arbok 39, player Venusaur 36 and Ariados 34. Battle result pending.

Koga first attempt lost after four Super Potions. Crobat defeated; remaining observed opponents Muk 40 and Arbok 39. Normal blackout returned to Cinnabar Pokémon Centre. No rollback. Koga reward/return-to-gym transition is still unverified in this continuous run; the trial cutscene and witness advice were verified before the loss. See balance review for party/controller limitations.
