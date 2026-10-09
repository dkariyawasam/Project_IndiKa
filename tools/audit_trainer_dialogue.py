#!/usr/bin/env python3
"""Inventory compiled, map-reachable trainer dialogue for editorial review.

Static reachability includes both sides of conditions, not runtime flag evaluation.
Exact duplicates are candidates, not errors: rematches, roster variants and shared
battle instructions can intentionally reuse text. Semantic fit requires review.
"""
import json
import re
from collections import defaultdict
from pathlib import Path
from audit_trainer_class_heatmap import audit, definitions, resolve
from audit_trainer_overworlds import parse_trainers

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'docs/trainer-dialogue-audit'


def collect():
    files = set()
    def include(p):
        if p in files or not p.exists():
            return
        files.add(p)
        for name in re.findall(r'\.include\s+"([^"]+)"', p.read_text()):
            include(ROOT / name)
    include(ROOT / 'data/event_scripts.s')
    blocks, sources, falls = {}, {}, {}
    for p in sorted(files):
        previous = None
        for n, line in enumerate(p.read_text().splitlines(), 1):
            m = re.match(r'^(\w+)::?\s*(?:@.*)?$', line)
            if m:
                label = m[1]
                if previous is not None:
                    code = [s for s in blocks[previous] if s and not s.startswith('@')]
                    if not code or not re.match(r'^(end|return|goto)\b', code[-1]):
                        if not any(s.startswith(('.string', '.byte', '.2byte', '.4byte')) for s in code):
                            falls[previous] = label
                blocks[label] = []
                sources[label] = f'{p.relative_to(ROOT)}:{n}'
                previous = label
            elif previous:
                blocks[previous].append(line.strip())
    texts = {k: ''.join(re.findall(r'\.string\s+"(.*)"', '\n'.join(v))) for k, v in blocks.items() if any(s.startswith('.string') for s in v)}
    edges = defaultdict(set)
    for label, lines in blocks.items():
        if label in falls:
            edges[label].add(falls[label])
        for line in lines:
            if re.match(r'^(?:goto\w*|call\w*|map_script\w*|trainerbattle\w*)\b', line):
                edges[label].update(t for t in re.findall(r'\b\w+\b', line.split('@')[0]) if t in blocks and t not in texts)
    def walk(roots):
        seen, todo = set(), list(roots)
        while todo:
            label = todo.pop()
            if label in seen:
                continue
            seen.add(label)
            todo.extend(edges[label] - seen)
        return seen
    base = audit()
    ids = definitions(ROOT / 'include/constants/opponents.h')
    entries, missing, used, owners = [], [], set(), defaultdict(set)
    pointer_errors, battle_records = [], {}
    for e in base['encounters']:
        labels = walk(e['roots'])
        refs, battles = set(), []
        for label in sorted(labels):
            for line in blocks.get(label, []):
                if line.startswith('trainerbattle'):
                    args = [a.strip() for a in line.split(None, 1)[1].split(',')]
                    if resolve(args[0], ids) not in e['trainer_ids'] and 'rematch' not in line.split()[0]:
                        continue
                    battles.append({'script': label, 'source': sources[label], 'command': line})
                    op = line.split()[0]
                    positions = {
                        'trainerbattle_single': (1, 2), 'trainerbattle_double': (1, 2, 3),
                        'trainerbattle_rematch': (1, 2), 'trainerbattle_rematch_double': (1, 2, 3),
                        'trainerbattle_no_intro': (1,), 'trainerbattle_no_intro_double': (1, 2),
                        'trainerbattle_earlyrival': (2, 3),
                    }
                    if op not in positions:
                        pointer_errors.append({'source':sources[label], 'command':line, 'error':'unsupported battle macro'})
                    for i in positions.get(op, ()):
                        if i >= len(args) or args[i] not in texts:
                            pointer_errors.append({'source':sources[label], 'command':line, 'error':'battle text does not resolve to a string', 'argument':i})
                        elif not texts[args[i]].endswith('$'):
                            pointer_errors.append({'source':sources[label], 'text':args[i], 'error':'missing terminator'})
                    battle_records[(label, line)] = {'script':label, 'source':sources[label], 'trainer':args[0],
                        'command':line, 'texts':[args[i] for i in positions.get(op, ()) if i < len(args)]}

                if re.match(r'^(?:trainerbattle\w*|msgbox|message|loadword|preparemsg|braillemessage)\b', line):
                    tokens = re.findall(r'\b\w+\b', line)
                    for t in tokens:
                        if t in texts:
                            refs.add(t)
                        elif '_Text_' in t and t not in blocks:
                            missing.append({'map': e['map'], 'script': label, 'text': t})
        for t in refs:
            used.add(t)
            owners[t].add((e['map'], ', '.join(e['names'])))
        entries.append({**e, 'battles': battles, 'dialogue': [{'label':t, 'source':sources[t], 'text':texts[t]} for t in sorted(refs)]})
    duplicates = defaultdict(list)
    for label in used:
        normalized = re.sub(r'\\[nlp]|\$|\s+', ' ', texts[label]).strip()
        normalized = re.sub(r' +', ' ', normalized)
        duplicates[normalized].append(label)
    duplicate_groups = []
    for text, labels in sorted(duplicates.items()):
        speakers = sorted(set().union(*(owners[l] for l in labels)))
        if len(speakers) > 1:
            duplicate_groups.append({'text':text, 'labels':sorted(labels), 'speakers':speakers})
    # Resolve aliases before comparing the identity selected by the VS Seeker.
    trainer_data = {resolve(k, ids):v for k,v in parse_trainers().items()}
    class_defs = definitions(ROOT / 'include/constants/trainers.h')
    active = {tid for e in entries for tid in e['trainer_ids']}
    rematches = (ROOT / 'src/vs_seeker.c').read_text()
    rematches = rematches[rematches.index('static const struct RematchData'):]
    rematches = rematches[:rematches.index('\n};')]
    rematch_identity_mismatches = []
    for group in re.findall(r'\{\s*\{([^}]+)\}', rematches):
        names = [n for n in re.findall(r'TRAINER_\w+', group) if n != 'TRAINER_NONE']
        if not names or resolve(names[0], ids) not in active:
            continue
        before = trainer_data.get(resolve(names[0], ids))
        for token in names[1:]:
            after = trainer_data.get(resolve(token, ids))
            def identity(t):
                return t['name'], resolve(t['class'], class_defs), t['pic']
            if before and after and identity(before) != identity(after):
                rematch_identity_mismatches.append({'base':names[0], 'before':before, 'rematch':token, 'after':after})
    return {'summary':{**base['summary'], 'dialogue_labels':len(used), 'duplicate_groups':len(duplicate_groups), 'battle_commands':len(battle_records)},
            'rematch_identity_mismatches':rematch_identity_mismatches,
            'pointer_errors':pointer_errors, 'battle_commands':list(battle_records.values()),
            'missing_scripts':base['missing_scripts'], 'missing_texts':missing,
            'dynamic_battles':base['dynamic_battles'], 'shared_flags_across_maps':base['shared_flags_across_maps'],
            'duplicates':duplicate_groups, 'encounters':entries}


def main():
    data = collect()
    OUT.mkdir(exist_ok=True)
    (OUT / 'inventory.json').write_text(json.dumps(data, indent=2) + '\n')
    lines = ['# Trainer dialogue inventory', '', 'Static, conditional reachability; editorial findings are separate from this generated inventory.', '']
    for e in data['encounters']:
        lines += [f"## {e['map']} — {', '.join(e['names'])} ({e['class'].removeprefix('CLASS_')})", '']
        for t in e['dialogue']:
            lines.append(f"- `{t['label']}` ({t['source']}): {t['text']}")
        lines.append('')
    (OUT / 'inventory.md').write_text('\n'.join(lines))
    print(json.dumps({k:v for k,v in data.items() if k not in ('encounters','duplicates','battle_commands')}, indent=2))

if __name__ == '__main__':
    main()
