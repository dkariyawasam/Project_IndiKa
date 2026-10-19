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

def hue_distance(a,b):return min(abs(a-b),1-abs(a-b))

def ramp_groups(entries):
    """Group similar hues, then order shades by lightness (dark to light)."""
    groups=[]
    for entry in sorted(entries,key=lambda e:colorsys.rgb_to_hsv(*gba(e['rgb']))[1],reverse=True):
        h,s,v=colorsys.rgb_to_hsv(*gba(entry['rgb']))
        if v<.12 or lightness(entry['rgb'])>.96:continue
        neutral=s<.12
        group=next((g for g in groups if g['neutral']==neutral and (neutral or hue_distance(g['hue'],h)<=.09)),None)
        if group is None:
            group=dict(hue=h,neutral=neutral,entries=[]);groups.append(group)
        group['entries'].append(entry)
    for g in groups:g['entries'].sort(key=lambda e:lightness(e['rgb']))
    return groups

def recommend(palette,indices,refs,artwork=None):
    """Adapt whole shade ramps to one vanilla Pokémon per ramp, never merge shades."""
    from itertools import combinations
    sources={}
    for r in refs:sources.setdefault(r['key'],[]).append(r)
    candidates=[dict(g,key=key) for key,entries in sources.items() for g in ramp_groups(entries)]
    proposed=[c[:] for c in palette];changes=[]
    groups=ramp_groups([dict(index=i,rgb=palette[i]) for i in sorted(set(indices)-{0})])
    for number,g in enumerate(groups):
        original=g['entries'];n=len(original);best=None
        guidance=[]
        for a in original:
            ah,ast,av=colorsys.rgb_to_hsv(*gba(a['rgb']));al=lightness(a['rgb'])
            eligible=[c for c in (artwork or []) if (colorsys.rgb_to_hsv(*gba(c['rgb']))[1]<.15)==g['neutral'] and (g['neutral'] or hue_distance(ah,colorsys.rgb_to_hsv(*gba(c['rgb']))[0])<.12)]
            guidance.append(min(eligible,key=lambda c:3*hue_distance(ah,colorsys.rgb_to_hsv(*gba(c['rgb']))[0])+2*abs(al-lightness(c['rgb']))+.25*abs(ast-colorsys.rgb_to_hsv(*gba(c['rgb']))[1])) if eligible else a)

        # Artwork may legitimately be darker or less saturated than the existing sprite.
        basis=guidance if artwork else original
        for target in candidates:
            if target['neutral']!=g['neutral'] or (not g['neutral'] and hue_distance(target['hue'],g['hue'])>.10):continue
            unique={gba(e['rgb']):e for e in target['entries']}
            entries=sorted(unique.values(),key=lambda e:lightness(e['rgb']))
            if len(entries)<n:continue
            for shades in combinations(entries,n):
                # Preserve each material's hue and avoid trading vivid midtones for dull ramps.
                if any(hue_distance(colorsys.rgb_to_hsv(*gba(a['rgb']))[0],colorsys.rgb_to_hsv(*gba(b['rgb']))[0])>20/360 for a,b in zip(basis,shades) if not g['neutral']):continue
                if not artwork and any(colorsys.rgb_to_hsv(*gba(b['rgb']))[1]<colorsys.rgb_to_hsv(*gba(a['rgb']))[1]-.10 for a,b in zip(original,shades)):continue
                # Highlights must remain pale; distinct shadows must retain their separation.
                if any(lightness(a['rgb'])>=.72 and lightness(b['rgb'])<max(.72,lightness(a['rgb'])-.06) for a,b in zip(basis,shades)):continue
                if any(lightness(shades[i+1]['rgb'])-lightness(shades[i]['rgb'])<max(.02,(lightness(original[i+1]['rgb'])-lightness(original[i]['rgb']))*.65) for i in range(n-1)):continue
                score=sum(3*abs(lightness(a['rgb'])-lightness(b['rgb']))+(.5*hue_distance(colorsys.rgb_to_hsv(*gba(a['rgb']))[0],colorsys.rgb_to_hsv(*gba(b['rgb']))[0]) if not g['neutral'] else 0) for a,b in zip(original,shades))/n
                if artwork:
                    score=.25*score+sum(3*abs(lightness(a['rgb'])-lightness(b['rgb']))+2*hue_distance(colorsys.rgb_to_hsv(*gba(a['rgb']))[0],colorsys.rgb_to_hsv(*gba(b['rgb']))[0])+abs(colorsys.rgb_to_hsv(*gba(a['rgb']))[1]-colorsys.rgb_to_hsv(*gba(b['rgb']))[1]) for a,b in zip(guidance,shades))/n
                score+=2*hue_distance(target['hue'],g['hue']) if not g['neutral'] else 0
                if best is None or score<best[0]:best=(score,target['key'],shades)
        if best is None:continue
        _,source,shades=best
        replacements=[[(v>>3)*8 for v in e['rgb']] for e in shades]
        outside={gba(proposed[i]) for i in set(indices)-{0}-{e['index'] for e in original}}
        if any(gba(rgb) in outside for rgb in replacements):continue
        for entry,rgb in zip(original,replacements):
            i=entry['index']
            if gba(rgb)!=gba(palette[i]):
                proposed[i]=rgb;changes.append(dict(index=i,before=palette[i],after=rgb,referenceSpecies=source,ramp=number+1))
    return proposed,changes

def family_key(key, reference_keys):
    for suffix in ('_ALOLAN','_GALARIAN','_PALDEAN','_HISUIAN'):
        if key.endswith(suffix) and key[:-len(suffix)] in reference_keys:
            return key[:-len(suffix)]
    return key if key in reference_keys else None

def approved_palettes(root):
    path=root/'tools/debug_dashboard/approved_family_palettes.json'
    return json.loads(path.read_text()).get('species',{}) if path.exists() else {}

def studies(root):
    path=root/'tools/debug_dashboard/curated_palette_studies.json'
    return json.loads(path.read_text()) if path.exists() else {}

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
            original=mon['key'] in by_key
            used=set(counts)|set(Image.open(root/mon['back']).getdata());used.discard(0)
            same=original and all(gba(palette[int(i)])==gba(by_key[mon['key']][variant][int(i)]) for i in used)
            reasons=[]
            if not original and share>=.1:reasons.append(f'{share:.0%} of visible pixels use colours at least 12 saturation points below comparable vanilla hues.')
            if not original and stats['contrast']<threshold:reasons.append('Value contrast falls below the vanilla 10th percentile.')
            if original:
                muted=[]
                reasons=['Matches this species’ own vanilla FireRed colours.' if same else 'Modified from this species’ own vanilla FireRed palette. This is a customisation, not a style error.']
            rows.append(dict(original=original,key=mon['key'],name=mon.get('name',mon['key']),metrics=stats,muted=muted,reasons=reasons,status=('Vanilla match' if same else 'Modified from vanilla') if original else 'FireRed adaptation',colors=cs))
        except (ValueError,OSError,IndexError) as e:rows.append(dict(key=mon['key'],name=mon.get('name',mon['key']),status='Unavailable',reasons=[str(e)],colors=[],muted=[],metrics={}))
    art_path=root/'tools/debug_dashboard/official_artwork_colours.json'
    art=json.loads(art_path.read_text()).get('species',{}) if art_path.exists() else {}
    curated=studies(root);approved=approved_palettes(root)
    for row in rows:
        row['familyReference']=family_key(row['key'],by_key)
        row['approvedFamily']=variant=='normal' and row['key'] in approved
        row['curatedStudy']=variant=='normal' and row['key'] in curated
        a=art.get(row['key'],{})
        row['modernArtwork']=variant=='normal' and a.get('status')=='available'
    return dict(rows=rows,variant=variant,reference={k:reference[k] for k in ('repository','revision')},contrastThreshold=threshold,note='Heuristic review, not a style verdict. Compares first-frame front-sprite colours after GBA 5-bit conversion with 386 vanilla FireRed Pokémon. Transparent and unused indices are excluded; neutral colours are excluded from saturation comparisons. Comparable colours are within 20° hue and 15 value points. Saturation comparisons also require similar lightness; pale highlights are protected. Vanilla colour coverage measures exact used-colour matches at GBA precision, not artistic quality. Shiny palettes use a separate shiny reference. Scanning changes no colours.')


def proposal(root,species,key,variant,mode=None):
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
    own=next((r for r in reference['species'] if r['key']==key),None)
    art_path=root/'tools/debug_dashboard/official_artwork_colours.json'
    art=json.loads(art_path.read_text()).get('species',{}).get(key,{}) if art_path.exists() else {}
    available=variant=='normal' and art.get('status')=='available'
    mode=mode or ('restore' if own else ('modern' if available else 'adapt'))
    if mode not in ('restore','modern','adapt','study','family'):raise ValueError('Unknown adaptation mode')
    if mode=='modern' and not available:raise ValueError('No verified normal artwork reference for this Pokémon. Choose FireRed adaptation instead.')
    if mode=='restore' and not own:raise ValueError('No original FireRed palette for this Pokémon')

    if mode=='family':
        family=family_key(key,{r['key'] for r in reference['species']})
        saved=approved_palettes(root).get(key) if variant=='normal' else None
        if saved:
            proposed=[c[:] for c in palette];changes=[]
            for i in sorted(used-{0}):
                rgb=saved[i];proposed[i]=rgb[:]
                if gba(rgb)!=gba(palette[i]):changes.append(dict(index=i,before=palette[i],after=rgb,referenceSpecies=key,ramp='approved family'))
            family_message='Uses your approved family palette. No changes means this Pokémon already matches the accepted treatment.'
        else:
            if not family:raise ValueError('No vanilla family reference is available for this species')
            proposed,changes=recommend(palette,used,[r for r in refs if r['key']==family])
            family_message='Suggested using vanilla '+family.replace('_',' ')+' shade ramps. Only compatible colour groups are adapted; distinctive regional colours without a suitable family ramp are retained. This is a starting point for visual review, not automatic material recognition.'
    elif mode=='study':
        study=studies(root).get(key)
        if not study or variant!=study['variant']:raise ValueError('No material study for this palette')
        if hashlib.sha256(raw).hexdigest()!=study['sourcePaletteSha256']:raise ValueError('Palette changed since this study. Review the study before applying it.')
        proposed=[c[:] for c in palette];changes=[]
        baseline={r['key']:r for r in reference['species']}
        for edit in study['changes']:
            i=edit['index'];rgb=edit.get('rgb',baseline[edit['referenceSpecies']][variant][edit['referenceIndex']])
            if len(rgb)!=3 or any(type(c) is not int or not 0<=c<=255 for c in rgb):raise ValueError('Invalid study RGB colour')
            if i==0 or i not in used:raise ValueError('Study refers to a transparent or unused index')
            proposed[i]=rgb[:]
            if gba(rgb)!=gba(palette[i]):changes.append(dict(index=i,before=palette[i],after=rgb,referenceSpecies=edit['referenceSpecies'],ramp=edit['material']))
    elif mode=='restore':
        proposed=[c[:] for c in palette];changes=[]
        for i in sorted(used-{0}):
            rgb=own[variant][i]
            if gba(palette[i])!=gba(rgb):
                proposed[i]=rgb[:];changes.append(dict(index=i,before=palette[i],after=rgb,referenceSpecies=key,ramp='original'))
    else:proposed,changes=recommend(palette,used,refs,art.get('colours') if mode=='modern' else None)

    affected=[m for m in mons if m[variant]==mon[variant]]
    previews=[];digest=hashlib.sha256(raw)
    digest.update(json.dumps(proposed).encode());digest.update(mode.encode())
    if mode=='modern':digest.update(art['sha256'].encode())
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
    artwork=None
    if mode in ('modern','study') and available:
        cached=root/'.cache/debug-dashboard/official-artwork'/(key+'.png')
        thumbnail=None
        if cached.exists():
            pic=Image.open(cached);pic.thumbnail((192,192));buf=io.BytesIO();pic.save(buf,format='PNG');thumbnail='data:image/png;base64,'+base64.b64encode(buf.getvalue()).decode()
        artwork=dict(url=art['url'],image=thumbnail,colours=art['colours'],note=('Family study: colours may be blended from the vanilla family to retain regional identity. Inspect both sprite views before approval.' if mode=='study' else 'Official artwork mirrored by PokéAPI guides colour identity. Output colours still come exclusively from the 386-species vanilla FireRed palettes. Hue grouping is approximate; inspect small details before approval.'))
    return dict(mode=mode,artwork=artwork,key=key,variant=variant,path=mon[variant],revision=digest.hexdigest(),changes=changes,previews=previews,palette=proposed,
                message=(family_message if mode=='family' else study['description'] if mode=='study' else 'Restoration to this species’ own vanilla FireRed palette. All shared-palette users are previewed below; recoloured or reindexed artwork may require manual review.' if mode=='restore' else 'FireRed palette adaptation maps each hue group to an ordered shade ramp from one vanilla Pokémon. Pale highlights and meaningful lightness gaps are retained; shades are never merged. Black/white endpoints remain unchanged. Reference Pokémon are named for each changed ramp. Groups without a suitable ramp remain unchanged. This changes the registered battle palette, including every sprite shown below; menu icons use separate palettes.'))

def apply_proposal(root,species,payload):
    from datetime import datetime
    p=proposal(root,species,payload.get('key'),payload.get('variant','normal'),payload.get('mode'))
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
        if action=='proposal':result=proposal(ROOT,payload['species'],payload.get('key'),payload.get('variant','normal'),payload.get('mode'))
        elif action=='apply':result=apply_proposal(ROOT,payload['species'],payload)
        else:result=check(ROOT,payload['species'],payload.get('variant','normal'))
        print(json.dumps(result))
    except ValueError as e:print(json.dumps(dict(error=str(e))))
