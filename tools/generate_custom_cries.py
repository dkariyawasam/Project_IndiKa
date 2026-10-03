#!/usr/bin/env python3
"""Derive custom cries from local FRLG recordings. Requires NumPy and SciPy."""
import hashlib
import json
import math
import re
from pathlib import Path
import wave
import numpy as np
from scipy.signal import resample_poly, butter, sosfilt

ROOT = Path(__file__).resolve().parents[1]
DIR = ROOT / 'sound/direct_sound_samples/cries'
RATE = 10512


def load(name):
    with wave.open(str(DIR / (name + '.wav'))) as w:
        assert w.getsampwidth() == 1 and w.getnchannels() == 1
        x = (np.frombuffer(w.readframes(w.getnframes()), dtype=np.uint8).astype(float) - 128) / 128
        rate = w.getframerate()
    g = math.gcd(RATE, rate)
    return resample_poly(x, RATE // g, rate // g)


def pitch(x, factor):
    # Playback-rate transposition deliberately changes both pitch and duration.
    n = round(factor * 1000)
    return resample_poly(x, 1000, n)


def mix(*parts):
    out = np.zeros(max(len(x) + round(delay * RATE) for x, gain, delay in parts))
    for x, gain, delay in parts:
        start = round(delay * RATE)
        out[start:start + len(x)] += gain * x
    return out


def smooth(x, cutoff):
    return sosfilt(butter(2, cutoff, fs=RATE, output='sos'), x)


def finish(x):
    x = x - x.mean()
    fade = min(round(.008 * RATE), len(x) // 4)
    x[:fade] *= np.linspace(0, 1, fade)
    x[-fade:] *= np.linspace(1, 0, fade)
    peak = np.max(np.abs(x))
    x *= .88 / max(peak, .001)
    assert len(x) / RATE < 2.25
    return np.clip(np.rint(x * 128 + 128), 0, 255).astype(np.uint8).tobytes()


def generate():
    recipes = []
    def add(species, sources, description, x):
        recipes.append((species, sources, description, x))
    k, kt, o, ot = (load(s) for s in ['kabuto', 'kabutops', 'omanyte', 'omastar'])
    add('KINKABUTO', ['kabuto', 'kabutops'], 'Lower Kabuto body with a short, higher Kabutops clicking finish.', mix((pitch(k,.87),.85,0),(pitch(kt,1.3)[:2400],.32,.24)))
    x = pitch(o,.9); t = np.arange(len(x))/RATE
    add('AMUNYTE', ['omanyte','omastar'], 'Lower Omanyte with gentle bubbling amplitude pulses and an Omastar undertone.', mix((x*(.85+.15*np.sin(2*np.pi*18*t)),.85,0),(smooth(pitch(ot,.78),2200),.25,.03)))
    add('OMATO', ['omanyte','kabuto'], 'Omanyte-led call with a short Kabuto accent.', mix((pitch(o,1.06),.8,0),(pitch(k,1.12),.3,.04)))
    add('OMATOPS', ['omastar','kabutops'], 'Omastar-led call with a sharper Kabutops edge.', mix((pitch(ot,.96),.8,0),(pitch(kt,1.08),.33,.05)))
    add('KABUSTAR', ['kabuto','omanyte'], 'Kabuto-led call with a bright Omanyte undertone.', mix((pitch(k,1.07),.85,0),(pitch(o,1.16),.32,.03)))
    add('KABUKNIGHT', ['kabutops','omastar'], 'Lower Kabutops-led call with Omastar resonance.', mix((pitch(kt,.91),.85,0),(pitch(ot,.88),.3,.05)))
    x = smooth(pitch(load('aerodactyl'),.94),3100)
    add('AEROPTERYX', ['aerodactyl'], 'Slightly lower screech, softened rasp and a quiet bright overtone.', mix((x,.9,0),(pitch(x,1.2),.12,.02)))
    x = pitch(load('marowak'),.88)
    snap = pitch(load('marowak'),1.55)[-1100:]*np.linspace(1,0,1100)
    add('OSSCYTHE', ['marowak'], 'Hollow, lower call with two short echoes and a sharp final fragment.', mix((x,.8,0),(x,.22,.035),(x,-.12,.065),(snap,.38,max(0,len(x)/RATE-.08))))
    x=load('mr_mime')
    add('MIME_SR', ['mr_mime'], 'Lower theatrical call with a short answering phrase.', mix((pitch(x,.88),.85,0),(pitch(x,1.08)[-2600:],.42,len(x)/RATE+.02)))
    x=load('magikarp')
    add('CINNABAR_MAGIKARP', ['magikarp'], 'Fuller Magikarp call with a low growling undertone.', mix((pitch(x,.93),.8,0),(smooth(pitch(x,.68),1700),.4,.02)))
    x=pitch(load('feebas'),.94);t=np.arange(len(x))/RATE
    shimmer=x*np.sin(2*np.pi*850*t)*np.linspace(0,.22,len(x))
    add('CINNABAR_FEEBAS', ['feebas'], 'Wavering Feebas call with a restrained metallic, icy tail.', mix((x*(.92+.08*np.sin(2*np.pi*7*t)),.9,0),(shimmer,.5,.035)))
    x=load('porygon2');a,b,c=np.array_split(x,3)
    add('PORYGON3', ['porygon2'], 'Three clean ascending phrases derived from Porygon2.', np.concatenate([smooth(pitch(a,.96),3300),np.zeros(180),smooth(pitch(b,1.06),3300),np.zeros(180),smooth(pitch(c,1.18),3300)]))
    for species,base,factor in [('PICHU_ALOLAN','pichu',1.045),('PIKACHU_ALOLAN','pikachu',.96),('EXEGGCUTE_ALOLAN','exeggcute',.955),('CUBONE_ALOLAN','cubone',.95),('KOFFING_GALARIAN','koffing',.94)]:
        x=pitch(load(base),factor)
        add(species,[base],f'Subtle family variation: playback rate {factor}; quiet 18 ms echo.',mix((x,.95,0),(x,.05,.018)))
    records=[]
    for species,sources,description,x in recipes:
        path=DIR/('custom_'+species.lower()+'.wav')
        pcm=finish(x)
        with wave.open(str(path),'wb') as w:
            w.setparams((1,1,RATE,0,'NONE','not compressed'));w.writeframes(pcm)
        records.append(dict(species=species,sources=[dict(path=str((DIR/(s+'.wav')).relative_to(ROOT)),sha256=hashlib.sha256((DIR/(s+'.wav')).read_bytes()).hexdigest()) for s in sources],treatment=description,wav=str(path.relative_to(ROOT)),wav_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),sample_rate=RATE,duration_seconds=round(len(pcm)/RATE,4)))
    tones=re.findall(r'^\tcry (\w+)', (ROOT/'sound/cry_tables.inc').read_text(), re.M)
    for record in records:
        symbol='Cry_Custom_'+record['species']
        if symbol in tones:
            record['cry_id']=tones.index(symbol)
    (ROOT/'docs/custom-cries.json').write_text(json.dumps(dict(method='Derived from existing FRLG game audio; mono unsigned 8-bit PCM, 10512 Hz. Peak limited to 88%, with 8 ms edge fades.',cries=records),indent=2)+'\n')
    print(f'Generated {len(records)} custom cries; longest {max(r["duration_seconds"] for r in records):.2f}s.')


if __name__=='__main__':
    generate()
