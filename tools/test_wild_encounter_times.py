#!/usr/bin/env python3
"""Validate global activity periods and engine-compatible day/night table pairs."""
import collections
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
data = json.loads((ROOT / 'src/data/wild_encounters.json').read_text())
policy = json.loads((ROOT / 'tools/wild_encounter_times.json').read_text())['species']
seen = set()
for group in data['wild_encounter_groups']:
    if not group['for_maps']:
        continue
    lengths = {field['type']: len(field['encounter_rates']) for field in group['fields']}
    positions = collections.defaultdict(list)
    labels = set()
    for index, entry in enumerate(group['encounters']):
        positions[entry['map']].append(index)
        assert entry['base_label'] not in labels
        labels.add(entry['base_label'])
    for map_name, indices in positions.items():
        assert len(indices) in (1, 2), map_name
        if len(indices) == 2:
            assert indices[1] == indices[0] + 1, map_name
        for period, index in enumerate(indices):
            entry = group['encounters'][index]
            for method, count in lengths.items():
                if method not in entry:
                    continue
                table = entry[method]
                # Some legacy tables contain unused trailing slots; the engine reads only count.
                assert len(table['mons']) >= count, (map_name, method)
                assert table['encounter_rate'] > 0
                for mon in table['mons'][:count]:
                    species = mon['species']
                    assert 1 <= mon['min_level'] <= mon['max_level'] <= 100
                    if species in policy:
                        seen.add(species)
                        assert len(indices) == 2 and policy[species] == ('day', 'night')[period], (map_name, species)
assert seen == set(policy), 'A restricted species has become unobtainable.'
print(f'PASS: {len(seen)} activity rules, table adjacency, slot counts and encounter levels')
