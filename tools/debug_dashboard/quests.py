"""Read the logbook catalogue from game source, without inventing save progress."""
import json
import re
from pathlib import Path


def array(source, name):
    return re.search(r'\b' + name + r'\[[^]]*\]\s*=\s*\{(.*?)\n\};', source, re.S).group(1)


def calls(body, macro):
    # The outer macro arguments contain identifiers and NELEMS(...), not strings.
    for match in re.finditer(r'\b' + macro + r'\(', body):
        start = at = match.end()
        depth = 1
        args = []
        while depth:
            char = body[at]
            if char == '(':
                depth += 1
            elif char == ')':
                depth -= 1
            if (char == ',' and depth == 1) or depth == 0:
                args.append(body[start:at].strip())
                start = at + 1
            at += 1
        yield args


def catalog(root):
    source = (root / 'src/quests.c').read_text()
    strings = {}
    for path in ('src/strings.c', 'src/quests.c'):
        text = (root / path).read_text()
        for match in re.finditer(r'\b(\w+)\[\]\s*=\s*_\(((?:\s*"(?:[^"\\]|\\.)*")+?)\s*\)', text):
            strings[match[1]] = ''.join(json.loads(s.replace(r"\p", r"\n").replace(r"\l", r"\n")) for s in re.findall(r'"(?:[^"\\]|\\.)*"', match[2])).replace('\\p', '\n').replace('\\l', '\n')
    def resolve(key):
        if key not in strings:
            raise ValueError('Unresolved quest text: ' + key)
        return strings[key]
    promises = re.findall(r'\{(\w+),\s*(\w+)\}', array(source, 'sFieldPromises'))
    quests = []
    for i, args in enumerate(calls(array(source, 'sSideQuests'), 'side_quest')):
        name, desc, done, location, sprite, kind, children, count = args
        entry = dict(id=i, name=resolve(name), description=resolve(desc), completed=resolve(done), location=resolve(location), objectives=[], source='src/quests.c')
        if children != 'NULL':
            for j, row in enumerate(calls(array(source, children), 'sub_quest')):
                _, title, detail, place, sprite, kind, status = row
                objective = dict(name=resolve(title), description=resolve(detail), location=resolve(place), statusLabel=resolve(status))
                if children == 'sPromiseQuests':
                    heard, finished = promises[j]
                    objective['discovery'] = heard if heard != '0' else 'Defeat the Dojo master (Koichi)'
                    objective['completion'] = finished
                entry['objectives'].append(objective)
        quests.append(entry)
    # Include every objective variant actually referenced by the dynamic reader.
    start = re.search(r'const u8 \*GetDynamicSubquestDesc\([^;]+?\)\s*\{', source).start()
    dynamic = source[start:]
    dynamic = dynamic[:dynamic.index('\n}') + 2]
    variants = []
    for key in dict.fromkeys(re.findall(r'\b(?:sText_|gText_)\w+', dynamic)):
        if key in strings and strings[key].strip():
            variants.append(dict(key=key, text=strings[key]))
    research_source = (root / 'src/data/nature_research.h').read_text()
    research = []
    for title, table in [('Bond', 'sBondMilestones'), ('Design', 'sDesignMilestones')]:
        rows = re.findall(r'\{SPECIES_(\w+), SPECIES_(\w+), (FLAG_\w+), (\w+), (TRUE|FALSE)\}', array(research_source, table))
        milestones = [dict(name=a.replace('_', ' ') + ' → ' + b.replace('_', ' '), flag=flag,
                           kind='Trade evolution' if trade == 'TRUE' else 'Evolution') for a,b,flag,legacy,trade in rows]
        if title == 'Design':
            for species in re.findall(r'FLAG_RESEARCH_REVIVED_(\w+)', array(research_source, 'sRevivalMilestones')):
                milestones.append(dict(name='Revive ' + species, flag='FLAG_RESEARCH_REVIVED_' + species, kind='Revival'))
            milestones.append(dict(name='Create PORYGON', flag='FLAG_GOT_PORYGON_FROM_BLAINE', kind='Creation'))
        research.append(dict(name=title, target=2, milestones=milestones))
    apex_flags = re.findall(r'\bFLAG_INTERACTED_APEX_\w+\b', source[source.index('static u8 CountApexInteractionsForNatureQuest(void)\n{'):].split('\n}', 1)[0])
    research.append(dict(name='Instinct', target=2, milestones=[dict(name=f.removeprefix('FLAG_INTERACTED_APEX_').replace('_',' '), flag=f, kind='Apex encounter') for f in apex_flags]))
    notes = (root / 'docs/logbook-quests.md').read_text().split('## Save compatibility')[0]
    return dict(quests=quests, research=research, variants=variants, notes=notes, sourceOnly=True,
                counts=dict(main=len(quests), objectives=sum(len(q['objectives']) for q in quests if q['id'] != 7), promises=len(promises)))
