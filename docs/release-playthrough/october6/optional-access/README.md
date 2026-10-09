# October 9 optional NPC access checks

Continued the isolated 46-owned campaign through normal buttons, movement, Cascade Board use and paid Safari admission. No debug warps, flag edits, injected items or save-state restores. The user's main save was untouched. The campaign ROM is the fishing-fix build; the later Feebas-table rebuild has not yet replaced this running copy. These NPC scripts are unchanged between those builds.

## Results

- Walked/surfed from Pewter through Viridian Forest, Pallet and Route 21 to Cinnabar. Used the earned Boulder Key to enter the lab and physically reached the Research Room scientist. Received one Upgrade and his Blaine terminal instructions. A second conversation gave instructions without another item; read-only bag inspection confirmed one Upgrade.
- Reached both Nia and Finn on Route 20. Their shared battle was already completed in this campaign, so this pass did not claim a new victory or reset the flag. Each independently gave the correct Clamperl/Deep Sea Scale → Gorebyss or Clamperl/Deep Sea Tooth → Huntail offer. Cancellation released control. No Clamperl was exchanged in this pass; the previous controlled trade/evolution evidence remains separate.
- Defeated intervening Triathlete Missy (Pelipper 31 and Starmie 33) normally while travelling east. The level-49 Venusaur makes this flow evidence, not level-matched balance evidence.
- Entered Safari normally and paid the entry fee. Reached the East trader using the stairs around the raised path. His dialogue explicitly requests Alolan Graveler and explains Alolan Golem. Declining released control. Source inspection confirms three East encounter slots provide Alolan Graveler at levels 25–28. No Safari catch or completed Alolan trade is claimed here.

The initial direct approach to the Safari NPC was on a different elevation. Correcting the temporary movement helper to reject direct elevation-3/elevation-4 steps allowed normal traversal around the stairs. This was a harness issue, not a map defect; no game-code change was made.

Evidence: [lab repeat instructions](lab-upgrade-repeat.png), [Nia offer](nia-offer.png), [Finn offer](finn-offer.png), [Safari trader](safari-trade-intro.png).

This pass verifies access and the listed interactions. The follow-up below covers the remaining lab offers and Rocket’s Disc scientist. Naturally earned prerequisites for every completed trade and a fresh Nia/Finn battle unlock remain outside this pass.

Tested ROM SHA-256: `b32ffca373fc53ab4233a4c6339594140f3a512b8bd66a93f811b96f4e7d1833`.

Left Safari through its normal early-exit dialogue and saved in Fuchsia City, still 46 owned. Upgrade and Missy battle progress are preserved in `/tmp/kanto-collection-oct6/earned-optional-access-oct9.sav`; active campaign save updated. [Save panel](save.png).


## Follow-up: remaining lab offers and Rocket Disc scientist

Continued with normal movement, paid bike rental, surfing and dialogue. Sold one earned Revive to fund the return rental; defeated two intervening Cycling Road trainers without restoring outcomes. The same fishing-fix ROM remained running, so this does not add live coverage of the later Feebas evolution change.

- Rocket League Lobby scientist: started with zero Dubious Discs, received one through his normal gift dialogue, then spoke again. Repeat dialogue explained Porygon2 and the risk without another gift; read-only bag checks confirmed exactly one. Source also guards the gift with `FLAG_GOT_ROCKET_DUBIOUS_DISC`, set only after a successful give-item result.
- Lab Lounge, Clifton: reached normally; his Magmar offer describes the lab-developed Magmarizer and Magmortar. Declining returned field control.
- Lab Lounge, Norma: reached normally; her Scyther offer describes Metal Coat and Scizor. Declining returned field control.
- Lab Lounge, wandering scientist: reached normally; his Onix offer explains Metal Coat bonding with its rocky body to become Steelix, and requests Golduck. Accepted the offer to open Pokémon selection, then cancelled. Field control returned; no Pokémon was exchanged.
- Lab Experiment Room, Garett: reached normally; his Rhydon offer explains the lab-developed Protector and Rhyperior. Declining returned field control.

The four scripts point to their intended trade IDs and have completed-trade guards. These are access/dialogue/cancellation checks, not four new completed trades; earlier controlled trade/evolution tests remain separate. No game-code defect was found or patched in this pass.

Evidence: [Disc introduction](disc-intro.png), [repeat instructions](disc-repeat.png), [Magmar](lab-magmar-intro.png), [Scyther](lab-scyther-intro.png), [Onix introduction](lab-onix-intro.png), [Onix offer](lab-onix-offer.png), [party cancellation entry point](lab-onix-party.png), [Rhydon](lab-rhydon-intro.png). Bag snapshots are alongside these images.

Saved normally inside the Cinnabar lab with 46 owned and one each of Upgrade and Dubious Disc. Earned checkpoint: `/tmp/kanto-collection-oct6/earned-lab-disc-access-oct9.sav`. The user's main save remains untouched.
