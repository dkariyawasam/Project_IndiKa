#!/usr/bin/env python3
"""Build the bundled regional encounter reference from PokéAPI data/v2/csv.

Usage: python3 tools/debug_dashboard/import_official_encounters.py /path/to/csv
Only reads the supplied tables; never calls the network at dashboard runtime.
"""
import csv
import hashlib
import json
import sys
from collections import defaultdict
from datetime import date
from pathlib import Path

TABLES = ('encounters', 'location_areas', 'locations', 'regions', 'versions',
          'pokemon', 'encounter_slots', 'encounter_methods')

def build(folder):
    tables = {name: list(csv.DictReader((folder/(name+'.csv')).open())) for name in TABLES}
    indexes = {name: {r['id']: r for r in rows} for name, rows in tables.items() if name != 'encounters'}
    grouped = defaultdict(lambda: defaultdict(lambda: defaultdict(set)))
    covered = set()
    for r in tables['encounters']:
        area = indexes['location_areas'][r['location_area_id']]
        location = indexes['locations'][area['location_id']]
        region = indexes['regions'].get(location['region_id'], {}).get('identifier', 'unspecified')
        version = indexes['versions'][r['version_id']]['identifier']
        pokemon = indexes['pokemon'][r['pokemon_id']]['identifier']
        method_id = indexes['encounter_slots'][r['encounter_slot_id']]['encounter_method_id']
        method = indexes['encounter_methods'][method_id]['identifier']
        place = location['identifier'] + ((' / '+area['identifier']) if area['identifier'] else '')
        grouped[pokemon][region][version].add((place,method))
        covered.add(version)
    entries = {}
    for pokemon in indexes['pokemon'].values():
        regions = grouped.get(pokemon['identifier'], {})
        entries[pokemon['identifier']] = [dict(region=region, games=[dict(game=game,
            encounters=[dict(location=place, method=method) for place,method in sorted(encounters)])
            for game,encounters in sorted(games.items())]) for region,games in sorted(regions.items())]
    return dict(source='https://github.com/PokeAPI/pokeapi/tree/master/data/v2/csv',
        documentation='https://pokeapi.co/docs/v2#encounters', retrieved=str(date.today()),
        coverage=sorted(covered),
        fileHashes={name:hashlib.sha256((folder/(name+'.csv')).read_bytes()).hexdigest() for name in TABLES},
        pokemon=entries)

if __name__ == '__main__':
    output = Path(__file__).parent/'official_encounters.json'
    output.write_text(json.dumps(build(Path(sys.argv[1])),separators=(',',':'))+'\n')
    print(output)
