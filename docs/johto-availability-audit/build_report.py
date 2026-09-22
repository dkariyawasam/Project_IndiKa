from pathlib import Path
import csv,json,re,sys,collections
ROOT=Path(__file__).resolve().parents[2];DOC=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'tools'));from sync_active_pokedex import derive
load=lambda n:list(csv.DictReader(open(DOC/'reference'/(n+'.csv'))))
D=json.loads((DOC/'data.json').read_text())
active=derive()[0]
wild=set(re.findall(r'SPECIES_(\w+)',(ROOT/'src/data/wild_encounters.json').read_text()))
trainers=set(re.findall(r'SPECIES_(\w+)',(ROOT/'src/data/trainer_parties.h').read_text()))
scripted=set()
gifts=set()
for script in (ROOT/'data').rglob('*.inc'):
 for line in script.read_text().splitlines():
  if re.search(r'\b(seteventmon|setwildbattle)\b',line):
   scripted.update(re.findall(r'SPECIES_(\w+)',line))
  gifts.update(re.findall(r'\b(?:givemon|giveegg)\s+SPECIES_(\w+)',line))
initial=set(active)-wild-trainers
sp={r['identifier'].upper().replace('-','_'):r for r in load('pokemon_species')};ids={r['id']:n for n,r in sp.items()}
loc={r['id']:r for r in load('locations')};areas={r['id']:r for r in load('location_areas')};pk={r['id']:r['identifier'].upper().replace('-','_') for r in load('pokemon')};ver={r['id']:r['identifier'] for r in load('versions')};slots={r['id']:r for r in load('encounter_slots')};methods={r['id']:r['identifier'] for r in load('encounter_methods')}
evidence=collections.defaultdict(set)
safari_evidence=collections.defaultdict(set)
for e in load('encounters'):
 if ver[e['version_id']] not in ('gold','silver','crystal','heartgold','soulsilver'):continue
 l=loc[areas[e['location_area_id']]['location_id']]
 if l['region_id']!='2' or l['identifier'].startswith('unknown'):continue
 n=pk[e['pokemon_id']];method=methods[slots[e['encounter_slot_id']]['encounter_method_id']]
 target=safari_evidence if 'safari' in l['identifier'] else evidence
 target[n].add((ver[e['version_id']],l['identifier'],method))
D['johto']={n:sorted({x[1] for x in records}) for n,records in sorted(evidence.items())}
D['excluded_johto_safari']={n:sorted({x[1] for x in records}) for n,records in sorted(safari_evidence.items())}
walker=set('CARVANHA CASTFORM DELCATTY FROSLASS GLALIE KECLEON SHARPEDO SKITTY SNORUNT SPIRITOMB TROPIUS WAILMER WAILORD'.split())
custom=set('KABUKNIGHT KABUSTAR OMATO OMATOPS'.split());later=set('TYNAMO EELEKTRIK EELEKTROSS FARFETCHD_GALARIAN'.split())
kanto=set('ANORITH ARMALDO LILEEP CRADILY GOREBYSS HUNTAIL LOPUNNY PORYGON2 PORYGON_Z'.split())
pre=collections.defaultdict(list)
text=(ROOT/'src/data/pokemon/evolution.h').read_text()
for n,body in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{(.*?)(?=\n\s*\[|\n};)',text,re.S):
 for target in re.findall(r'\{EVO_\w+,\s*[^,]+,\s*SPECIES_(\w+)\}',body):pre[target].append(n)
# Follow project evolution rules, not the official species ancestry used below.
# Include explicit gifts as evolution starting points; gift-only roots stay in the report.
wild_reachable=set(wild)|scripted|gifts
paths={n:[n] for n in wild_reachable}
changed=True
while changed:
 changed=False
 for target,parents in pre.items():
  if target in wild_reachable:continue
  for parent in parents:
   if parent in wild_reachable:
    wild_reachable.add(target)
    paths[target]=paths[parent]+[target]
    changed=True
    break
previous_missing=initial-scripted
evolution_excluded={n for n in previous_missing & wild_reachable if len(paths[n])>1}
report_excluded=(wild|scripted)|evolution_excluded
D['missing']=sorted(previous_missing-report_excluded)
D['gift_evolution_sources']=sorted(gifts)
D['static_excluded']=sorted(initial&scripted)
D['evolution_excluded']={n:paths[n] for n in sorted(evolution_excluded)}
(DOC/'data.json').write_text(json.dumps(D,indent=2)+'\n')
rows=[]
for n in D['missing']:
 chain=[];parent=sp.get(n,{}).get('evolves_from_species_id')
 while parent:
  q=ids[parent];chain.append(q);parent=sp[q]['evolves_from_species_id']
 proof='';cat=''
 if n in custom:cat='Custom species';proof='No official-game counterpart.'
 elif n in later:cat='Later generation/form';proof='Introduced after the Johto games; no GSC/HGSS encounter.'
 elif n in ('LEAFEON','GLACEON'):cat='Requires external evolution';proof='HGSS lacks the required moss/ice rock; trade from DPPt.'
 elif n in ('REGICE','REGIROCK','REGISTEEL'):cat='External transfer';proof='Not obtainable natively in GSC/HGSS.'
 elif n in walker:cat='Pokéwalker line';proof='Pokéwalker capture or evolution of one; not a mainland Johto wild encounter.'
 elif n in kanto:cat='HGSS Kanto source or revival';proof={'ANORITH':'Claw Fossil can be found in Johto Cliff Cave; revival is in Pewter City, Kanto.','LILEEP':'Root Fossil can be found in Johto Cliff Cave; revival is in Pewter City, Kanto.','HUNTAIL':'Evolve Clamperl from Kanto Route 19.','GOREBYSS':'Evolve Clamperl from Kanto Route 19.','LOPUNNY':'Evolve Buneary from Kanto Route 25; also Pokéwalker Buneary.'}.get(n,'Evolution of a Pokémon obtained or revived in Kanto in HGSS.')
 elif n in evidence:
  ms={x[2] for x in evidence[n]}
  cat='Johto gift' if ms<= {'gift','gift-egg','npc-trade'} else 'Johto event' if n=='CELEBI' else 'Johto encounter'
  proof='; '.join(' / '.join(x) for x in sorted(evidence[n]))
 elif any(x in evidence for x in chain):cat='Evolution of Johto source';proof='Evolve '+next(x for x in chain if x in evidence)+'. Evolution may require trading or an item; this does not claim a wild encounter.'
 elif n in safari_evidence or any(x in safari_evidence for x in chain):cat='Excluded Johto Safari source';proof='Only Johto Safari Zone evidence found for this Pokémon or its pre-evolutions; excluded by the project source rule.'
 else:cat='Needs further verification';proof='No direct Johto record located.'
 project='Evolves from '+', '.join(pre[n]) if pre[n] else 'No incoming evolution in project table'
 if n in ('KABUSTAR','OMATO'):project+='; Cinnabar fossil gift exists'
 if n=='AMBIPOM':project='AIPOM learns DOUBLE HIT at 33; level up knowing it in FUCHSIA FOREST. No discovery required; no wild Apex encounter.'
 if n=='LICKILICKY':project='LICKITUNG learns ROLLOUT at 33; level up knowing it in CERULEAN CAVE after defeating or catching MEWTWO. Fleeing/losing does not qualify. No wild LICKILICKY encounter.'
 if n in ('SWINUB','PILOSWINE','MAMOSWINE'):
  project+='; Johto line by design; all three grouped in non-native habitat.'
 if n=='MAMOSWINE':project+=' No Kanto evolution from PILOSWINE is configured.'

 rows.append({'species':n,'category':cat,'official_evidence':proof,'project_note':project})
counts=collections.Counter(r['category'] for r in rows)
intro=f'''# Pokémon encounter gaps and official Johto availability

Scope: the {len(active)}-species active project roster. Exclude species that appear in wild encounter tables, trainer-party definitions, or explicit scripted wild battles. Also exclude any Pokémon reachable through one or more implemented evolution steps starting from a Kanto wild encounter Pokémon (including scripted wild encounters) or an explicitly scripted gift Pokémon. This includes Eevee’s stone/friendship evolutions and Porygon’s trade evolutions. Evolution routes count even when they require items, trading, moves, discovery, or habitat gates. This checks configured evolution routes, not full item accessibility or runtime reachability.

The previous direct-encounter rule identified {len(previous_missing)} candidates. The new evolution rule removes {len(evolution_excluded)}, leaving **{len(rows)} report candidates**. Gift-only starting Pokémon remain candidates under the encounter-focused rule, but their implemented evolutions are excluded. This is a static source audit, not a live proof of every reachable encounter: unused trainer definitions can conceal additional gaps, and variable-driven encounters need separate tracing. It is not a definitive unobtainable-species count.

Official comparison: Gold, Silver, Crystal, HeartGold and SoulSilver, restricted to Johto locations **excluding the Johto Safari Zone**. Pokémon with another qualifying Johto location remain eligible; Safari-only species and evolutions do not. Kanto locations within those games, external transfers and Pokéwalker are separate. Celebi's Crystal encounter requires its event/Virtual Console unlock. No Pokémon GO geographic spawns, arbitrary player trades, or player-imported Pokémon counted as native Johto availability. Radio, swarms and other conditions still apply; the location evidence is not a promise of an ordinary unconditional grass encounter.

PokéAPI's community encounter data provides game/location/method evidence; it is not an official publisher dataset. Evolution classifications are inferred from its ancestry data with explicit exceptions for Leafeon/Glaceon. Project evolution notes record table links, not a full obtainable-item/ancestry proof. Active placeholders are included, deliberately retired species excluded. No game assets or encounters changed.

## Updated project decisions

- AIPOM can evolve into AMBIPOM in FUCHSIA FOREST with DOUBLE HIT; no wild Apex has developed there, so no discovery gate applies.
- LICKITUNG can evolve into LICKILICKY in CERULEAN CAVE with ROLLOUT only after MEWTWO is defeated or caught. Mewtwo occupies its potential Apex habitat. Cerulean Cave is distinct from Celadon Cave.
- SWINUB, PILOSWINE and MAMOSWINE belong to Johto and remain together in the non-native Pokédex habitat. MAMOSWINE has no Kanto evolution rule.
- TANGROWTH, ANNIHILAPE, MIME SR. and OSSCYTHE retain their scripted encounters; their pre-evolutions now evolve after discovery, knowing ANCIENTPOWER / RAGE FIST / BARRIER / BONE RUSH respectively, and levelling up in Viridian Forest / Mt. Moon / Diglett's Cave / Pokémon Tower. Their required moves, plus DOUBLE HIT and ROLLOUT above, are learned at level 33.
- ARTICUNO, ZAPDOS, MOLTRES and MEWTWO are also Apex species. All eleven have the Pokédex Apex marker/highlight; that UI classification does not imply an evolution or local encounter.
- The count now excludes evolutions reachable from wild Pokémon or explicit gifts. AMBIPOM and LICKILICKY qualify through their implemented habitat evolution routes. MAMOSWINE has no player evolution route, but is now excluded because Rocket League Champion Giovanni uses it.
- Existing saves that resolved Mewtwo before the new victory flag was added cannot reliably distinguish a past win from escape. See [Apex evolution rules](../apex-evolution/README.md).

Trainer exclusion was rechecked against every species in `src/data/trainer_parties.h`: none of the remaining candidates appears in a defined trainer team. This is broader than verified Kanto trainer encounters because unused party definitions are also excluded.

Lorelei’s active six-member team includes PILOSWINE and WEAVILE. The Pokéwalker-only Snorunt line has been retired. MAMOSWINE belongs exclusively to Rocket League Champion Giovanni, replacing Nidoqueen. All three are excluded from the candidate count. Mamoswine remains non-native and cannot be evolved locally by the player.

## Counts

'''
intro+=''.join(f'- {k}: {v}\n' for k,v in sorted(counts.items()))
intro+='''
## Sources

- [Encounter/location/species dataset](https://github.com/PokeAPI/pokeapi/tree/master/data/v2/csv) (downloaded 2026-09-19; relevant evidence retained in data.json and report rows).
- [HGSS Safari Zone](https://www.serebii.net/heartgoldsoulsilver/safarizone.shtml) (excluded source; retained for provenance)
- [Hoenn/Sinnoh radio encounters](https://www.serebii.net/heartgoldsoulsilver/hoennsinnoh.shtml)
- [Pokéwalker species list](https://bulbapedia.bulbagarden.net/wiki/Pokemon_found_in_Pokewalker)
- [Eevee evolution restrictions](https://www.serebii.net/games/eevee.shtml)
- [HGSS unobtainable Pokémon](https://www.serebii.net/heartgoldsoulsilver/unobtainable.shtml)
- [Anorith](https://www.serebii.net/pokedex-dp/347.shtml), [Lileep](https://www.serebii.net/pokedex-dp/345.shtml), [Clamperl](https://www.serebii.net/pokedex-dp/366.shtml), [Buneary](https://www.serebii.net/pokedex-dp/427.shtml), [Porygon](https://www.serebii.net/pokedex-dp/137.shtml).

[Crystal Celebi](https://www.serebii.net/crystal/celebi.shtml); [Johto fossil locations](https://www.serebii.net/heartgoldsoulsilver/rocksmash.shtml).

## Every candidate

| Pokémon | Official Johto status | Evidence | Project evolution/gift note |
|---|---|---|---|
'''
intro+=''.join(f"| {r['species']} | {r['category']} | {r['official_evidence']} | {r['project_note']} |\n" for r in rows)
intro+='\n## Excluded through evolution from Kanto wild Pokémon or gifts\n\n'+''.join('- '+ ' → '.join(paths[n])+'\n' for n in sorted(evolution_excluded))
(DOC/'report.md').write_text(intro);(DOC/'classified.json').write_text(json.dumps(rows,indent=2));print(dict(counts))
assert len(rows)==len(set(active)-wild-trainers-scripted-report_excluded)
assert not evolution_excluded & set(D['missing'])
assert {'AMBIPOM','LICKILICKY'} <= evolution_excluded
assert 'MAMOSWINE' not in pre  # No player evolution configured.

assert {'LEAFEON','GLACEON'} <= wild_reachable
assert not {'LEAFEON','GLACEON'} & set(D['missing'])
