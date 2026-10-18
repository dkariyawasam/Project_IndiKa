"""Review-only palette comparisons, using used pixels and GBA 5-bit colours."""
import colorsys,json,sys
from collections import Counter
from pathlib import Path
from statistics import median
from PIL import Image
from pokedex import catalog
ROOT=Path(__file__).resolve().parents[2]
def gba(rgb):return tuple((v>>3)/31 for v in rgb)
def colors(palette,counts):
    total=sum(counts.values()) or 1
    return [dict(index=int(i),rgb=palette[int(i)],hsv=colorsys.rgb_to_hsv(*gba(palette[int(i)])),share=n/total) for i,n in counts.items() if int(i)!=0 and n]
def metrics(cs):
    chroma=[c for c in cs if c['hsv'][1]>=.15 and c['hsv'][2]>=.15]
    weight=sum(c['share'] for c in chroma)
    saturation=sum(c['hsv'][1]*c['share'] for c in chroma)/weight if weight else None
    ordered=sorted(cs,key=lambda c:c['hsv'][2])
    def quantile(q):
        acc=0
        for c in ordered:
            acc+=c['share']
            if acc>=q:return c['hsv'][2]
        return ordered[-1]['hsv'][2] if ordered else 0
    return dict(saturation=saturation,contrast=quantile(.9)-quantile(.1),chromaticShare=weight)
def compare(cs,refs):
    muted=[]
    for c in cs:
        h,s,v=c['hsv']
        if s<.15 or v<.15 or (lightness(c['rgb'])>=.72 and s<.5):continue
        matches=[r for r in refs if min(abs(r['hsv'][0]-h),1-abs(r['hsv'][0]-h))<=20/360 and abs(r['hsv'][2]-v)<=.15 and r['hsv'][1]>=.15 and abs(lightness(r['rgb'])-lightness(c['rgb']))<=.06]
        if len(matches)<5:continue
        typical=median(r['hsv'][1] for r in matches)
        if typical-s>=.12:
            nearest=min(matches,key=lambda r:abs(r['hsv'][1]-typical)+abs(r['hsv'][2]-v))
            muted.append(dict(index=c['index'],rgb=c['rgb'],share=c['share'],saturation=s,referenceSaturation=typical,referenceSpecies=nearest['key'],referenceRgb=nearest['rgb']))
    return muted

def lightness(rgb):
    values=gba(rgb)
    return (max(values)+min(values))/2

def recommend(palette,indices,refs):
    """Choose actual vanilla colours conservatively; retain pale highlights and ramp order."""
    proposed=[c[:] for c in palette];changes=[]
    used=sorted(set(indices)-{0})
    for i in used:
        rgb=palette[i];h,s,v=colorsys.rgb_to_hsv(*gba(rgb));l=lightness(rgb)
        # Whites, outlines and pastel highlights are intentional material cues.
        if s<.15 or v<.18 or (l>=.72 and s<.5):continue
        choices=[]
        for r in refs:
            rh,rs,rv=r['hsv'];rl=lightness(r['rgb'])
            if min(abs(h-rh),1-abs(h-rh))>15/360:continue
            if abs(rl-l)>.04 or rs-s>.10 or s-rs>.10:continue
            candidate=[(c>>3)*8 for c in r['rgb']]
            # Never merge an existing shade or reverse/collapse a meaningful lightness step.
            if any(gba(candidate)==gba(proposed[j]) for j in used if j!=i):continue
            if any((l-lightness(palette[j]))*(lightness(candidate)-lightness(proposed[j]))<=0 or abs(lightness(candidate)-lightness(proposed[j]))<abs(l-lightness(palette[j]))*.75
                   for j in used if j!=i and abs(l-lightness(palette[j]))>=.04):continue
            distance=sum((a-b)**2 for a,b in zip(gba(rgb),gba(candidate)))
            choices.append((distance,candidate,r['key']))
        if choices:
            _,candidate,source=min(choices,key=lambda x:x[0])
            if gba(candidate)!=gba(rgb):
                proposed[i]=candidate;changes.append(dict(index=i,before=rgb,after=candidate,referenceSpecies=source))
    return proposed,changes

def check(root,species,variant='normal'):
    if variant not in ('normal','shiny'):raise ValueError('Unknown palette variant')
    reference=json.loads((root/'tools/debug_dashboard/firered_palette_reference.json').read_text())
    refs=[];by_key={};contrasts=[]
    for row in reference['species']:
        cs=colors(row[variant],row['counts']);refs.extend(dict(c,key=row['key']) for c in cs);by_key[row['key']]=row;contrasts.append(metrics(cs)['contrast'])
    typical={gba(r['rgb']) for r in refs}
    threshold=sorted(contrasts)[int(len(contrasts)*.1)];rows=[]
    for mon in catalog(root,species)['species']:
        try:
            im=Image.open(root/mon['front']).crop((0,0,64,64))
            if im.mode!='P':raise ValueError('Front sprite is not indexed')
            counts=Counter(im.getdata());counts.pop(0,None)
            palette=[list(map(int,l.split())) for l in (root/mon[variant]).read_text().splitlines()[3:19]]
            cs=colors(palette,counts);stats=metrics(cs);stats['vanillaColourShare']=sum(c['share'] for c in cs if gba(c['rgb']) in typical);muted=compare(cs,refs);share=sum(c['share'] for c in muted)
            same=mon['key'] in by_key and all(gba(palette[int(i)])==gba(by_key[mon['key']][variant][int(i)]) for i in counts)
            reasons=[]
            if not same and share>=.1:reasons.append(f'{share:.0%} of visible pixels use colours at least 12 saturation points below comparable vanilla hues.')
            if not same and stats['contrast']<threshold:reasons.append('Value contrast falls below the vanilla 10th percentile.')
            rows.append(dict(key=mon['key'],name=mon.get('name',mon['key']),metrics=stats,muted=muted,reasons=reasons,status='Vanilla match' if same else ('Review' if reasons else 'No flags'),colors=cs))
        except (ValueError,OSError,IndexError) as e:rows.append(dict(key=mon['key'],name=mon.get('name',mon['key']),status='Unavailable',reasons=[str(e)],colors=[],muted=[],metrics={}))
    return dict(rows=rows,variant=variant,reference={k:reference[k] for k in ('repository','revision')},contrastThreshold=threshold,note='Heuristic review, not a style verdict. Compares first-frame front-sprite colours after GBA 5-bit conversion with 151 vanilla Kanto Pokémon. Transparent and unused indices are excluded; neutral colours are excluded from saturation comparisons. Comparable colours are within 20° hue and 15 value points. Saturation comparisons also require similar lightness; pale highlights are protected. Vanilla colour coverage measures exact used-colour matches at GBA precision, not artistic quality. Shiny palettes use a separate shiny reference. Scanning changes no colours.')


def proposal(root,species,key,variant):
    import hashlib,base64,io
    data=check(root,species,variant)
    row=next((r for r in data['rows'] if r['key']==key),None)
    if row is None or row['status']=='Unavailable':raise ValueError('Unknown or unavailable Pokémon')
    mons=catalog(root,species)['species'];mon=next(m for m in mons if m['key']==key)
    path=root/mon[variant];raw=path.read_bytes()
    palette=[list(map(int,l.split())) for l in raw.decode().splitlines()[3:19]]
    reference=json.loads((root/'tools/debug_dashboard/firered_palette_reference.json').read_text())
    refs=[dict(c,key=r['key']) for r in reference['species'] for c in colors(r[variant],r['counts'])]
    used=set()
    for view in ('front','back'):
        used.update(Image.open(root/mon[view]).getdata())
    proposed,changes=recommend(palette,used,refs) if row['status']!='Vanilla match' else ([c[:] for c in palette],[])
    affected=[m for m in mons if m[variant]==mon[variant]]
    previews=[];digest=hashlib.sha256(raw)
    digest.update(json.dumps(proposed).encode())
    for m in affected:
        digest.update(m['key'].encode());entry=dict(key=m['key'],name=m.get('name',m['key']),views=[])
        for view in ('front','back'):
            source=root/m[view];digest.update(str(source.relative_to(root)).encode());digest.update(source.read_bytes())
            im=Image.open(source).crop((0,0,64,64))
            if im.mode!='P':raise ValueError('Cannot preview non-indexed sprite')
            pair=dict(view=view)
            for label,pal in [('before',palette),('after',proposed)]:
                out=im.copy();out.putpalette([round(c*255) for rgb in pal for c in gba(rgb)]+[0]*(768-48))
                out.info['transparency']=0;buf=io.BytesIO();out.convert('RGBA').save(buf,format='PNG')
                pair[label]='data:image/png;base64,'+base64.b64encode(buf.getvalue()).decode()
            entry['views'].append(pair)
        previews.append(entry)
    return dict(key=key,variant=variant,path=mon[variant],revision=digest.hexdigest(),changes=changes,previews=previews,palette=proposed,
                message='Suggestions use actual vanilla colours close in hue, saturation and lightness. Pale highlights and neutral colours are protected; shade merging, lightness reversal and large ramp compression are rejected. Some palettes need manual editing. This changes the registered battle palette, including every sprite shown below; menu icons use separate palettes.')

def apply_proposal(root,species,payload):
    from datetime import datetime
    p=proposal(root,species,payload.get('key'),payload.get('variant','normal'))
    if p['revision']!=payload.get('revision'):raise ValueError('Palette or artwork changed. Generate a new preview before approving.')
    if not p['changes']:raise ValueError('No suggested colour changes to apply')
    path=root/p['path'];original=path.read_bytes()
    backup=root/'.cache/debug-dashboard/source-backups'/datetime.now().strftime('%Y%m%d-%H%M%S-%f')
    backup.mkdir(parents=True);(backup/path.name).write_bytes(original)
    (backup/'palette-change.json').write_text(json.dumps(dict(source=p['path'],changes=p['changes'],affected=[m['key'] for m in p['previews']]),indent=2)+'\n')
    text='JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,c)) for c in p['palette'])+'\n'
    temp=path.with_suffix('.pal.tmp');temp.write_text(text);temp.replace(path)
    return dict(message='Approved colours saved. Rebuild the ROM to apply them in-game.',backup=str(backup),affected=[m['key'] for m in p['previews']])

if __name__=='__main__':
    payload=json.load(sys.stdin)
    try:
        action=payload.get('action','check')
        if action=='proposal':result=proposal(ROOT,payload['species'],payload.get('key'),payload.get('variant','normal'))
        elif action=='apply':result=apply_proposal(ROOT,payload['species'],payload)
        else:result=check(ROOT,payload['species'],payload.get('variant','normal'))
        print(json.dumps(result))
    except ValueError as e:print(json.dumps(dict(error=str(e))))
