from pathlib import Path
import re, runpy
root=Path(__file__).resolve().parents[2]
font=runpy.run_path(str(root/'docs/league-terminals/verify.py'))
for name,labels in {
 'CeladonCity':['ErikaBudewAdvice','ErikaPostBattle'],
 'FuchsiaCity':['KogaGolbatAdvice'],
 'VermilionCity':['LtSurgeElectabuzzTradeComplete','LtSurgeElectabuzzTradeDone'],
}.items():
 text=(root/f'data/maps/{name}_Gym/text.inc').read_text()
 for label in labels:
  block=text.split(f'{name}_Gym_Text_{label}::')[1].split('::')[0]
  for s in re.findall(r'\.string "(.*)"',block):
   for line in re.split(r'\\[npl]|\$',s):
    assert sum(font['w'][font['chars'][c]] for c in line)<=208,line
print('PASS: updated leader guidance fits the dialogue window')
