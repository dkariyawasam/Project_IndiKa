from pathlib import Path
from PIL import Image, ImageDraw
ROOT=Path(__file__).resolve().parent
source=Path('/tmp/codex-remote-attachments/01a08c66-0028-7a82-a332-3f5f716848ef/9363A63E-DA70-4437-A584-05E2F612830A/1-Pasted-Image-1.jpg')
if not (ROOT/'face-before.png').exists():Image.open(source).convert('RGB').save(ROOT/'face-before.png')
im=Image.open(ROOT/'face-before.png').convert('RGB');d=ImageDraw.Draw(im)
outline=(48,30,32);deep=(72,30,36);middle=(105,44,43);tongue=(158,78,69)
ivory=(255,247,221);ivoryshade=(206,198,173);lip=(238,166,153);liplight=(255,210,183)
# Confine edits to the existing near eye and open mouth. Single native pixels.
d.polygon([(19,26),(21,27),(22,29),(21,31),(19,31),(18,29),(18,28)],fill=outline)
d.polygon([(19,27),(20,27),(21,28),(21,30),(19,30),(19,29),(18,28)],fill=ivoryshade)
d.rectangle((19,27,20,29),fill=ivory);d.point((21,29),fill=ivory)
d.point((19,28),fill=outline);d.point((19,29),fill=outline)
# A coherent lip rim around a dark, uncluttered mouth cavity.
d.line([(11,31),(14,31),(17,33),(19,35),(21,38),(21,41),(20,43),(18,46),(16,48),(14,48)],fill=lip,width=1)
d.line([(12,31),(14,31),(16,32)],fill=liplight,width=1)
d.polygon([(13,33),(15,33),(18,35),(20,38),(20,41),(18,44),(16,46),(14,45),(13,42),(12,37)],fill=deep)
d.polygon([(14,37),(16,36),(18,38),(19,41),(17,44),(15,45),(14,42)],fill=middle)
d.polygon([(14,43),(16,42),(18,42),(18,43),(16,46),(14,45)],fill=tongue)
# Four distinct tapered fangs, with dark space between them.
d.polygon([(13,33),(14,33),(14,36)],fill=ivoryshade);d.line([(13,33),(13,34),(14,35)],fill=ivory)
d.polygon([(17,34),(18,35),(17,37)],fill=ivoryshade);d.line([(17,34),(17,35),(17,36)],fill=ivory)
d.polygon([(14,42),(15,44),(15,45),(14,44)],fill=ivoryshade);d.line([(14,42),(14,43),(14,44)],fill=ivory)
d.polygon([(18,40),(19,42),(18,43),(17,44)],fill=ivoryshade);d.line([(18,40),(18,41),(18,42)],fill=ivory)
im.save(ROOT/'thermikarp-face-v1.png')
old=Image.open(ROOT/'face-before.png');changed=[(x,y) for y in range(64) for x in range(64) if old.getpixel((x,y))!=im.getpixel((x,y))]
assert all((18<=x<=22 and 26<=y<=31) or (11<=x<=21 and 31<=y<=48) for x,y in changed)
preview=Image.new('RGB',(768,416),'#eee9df');draw=ImageDraw.Draw(preview)
for i,(label,pic) in enumerate([('BEFORE',old),('FACE CLEANUP',im)]):
 draw.text((i*384+12,8),label,fill='black');preview.paste(pic.resize((384,384),Image.Resampling.NEAREST),(i*384,32))
preview.save(ROOT/'face-comparison.png');print(len(changed),'face pixels edited; all other pixels unchanged')
