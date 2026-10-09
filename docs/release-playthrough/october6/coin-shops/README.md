# Rocket League coin shops — 9 October 2026

Tested both clerks on the isolated earned campaign with normal controls. Travelled from Cinnabar through Route 21 and Cycling Road, defeating three intervening swimmers. No injected coins, Pokémon, flags, debug warps, or emulator save states. Main user save untouched.

## Live results

- Started with 5,550 coins and a full party.
- Attempting Porygon (9,999 coins) rejected the purchase. Cancelling Smeargle’s confirmation retained all coins and storage contents.
- Bought Smeargle for 500 coins: sent to Box 1, slot 10, with valid checksum. Balance 5,050; party unchanged.
- Bought two TM05s at 1,000 coins each: Bag quantity increased from zero to two; balance 3,050. Ordinary money remained 2,481 throughout both transactions.
- Saved normally, loaded the rebuilt ROM, and chose CONTINUE. Party, storage, coins, money, and Bag contents matched the pre-reset snapshots. Pokédex owned count is 51.
- Retested unaffordable Porygon on the rebuilt ROM. Its error now stays visible until dismissed; no purchase occurred.

The pre-travel baseline and post-cancellation party differ only in Porygon-Z’s friendship (71 → 72) from walking. Transaction-specific snapshots have identical parties.

## Fix found

Coin-shortage and full-PC errors lacked `{PAUSE_UNTIL_PRESS}`, causing them to disappear immediately after printing. Added the same acknowledgement control used by the ordinary shop’s insufficient-money message. The live coin-shortage check confirms the corrected behaviour.

## Automated boundary coverage

`python3 tools/test_coin_shop.py` passes using the actual production transaction functions with inventory/gift stubs: full Bag and full PC reject without charging; party/PC delivery; insufficient funds; money shops unaffected; all 59 TM prices match the original clerk. Full-storage rejection was tested in this harness, not by filling all 420 live PC slots. Live party delivery was not repeated because the campaign party was full.

`make -j8` passes. Purchase tests initially ran on the preceding campaign build; persistence and readable-error checks ran on the final rebuilt ROM. Hashes and numeric results are in `results.json`.

Earned checkpoint: `/tmp/kanto-collection-oct6/earned-51-owned-coin-shops-oct9.sav`.
