from pathlib import Path
from PIL import Image,ImageDraw
import shutil,re
ROOT=Path(__file__).resolve().parents[3];DOC=Path(__file__).resolve().parent;OUT=ROOT/'graphics/pokemon/cinnabar_magikarp'
for name in ('front.png','normal.pal'):
 if not (DOC/('before-'+name)).exists():shutil.copyfile(OUT/name,DOC/('before-'+name))
im=Image.open(DOC/'before-front.png').copy();orig=im.copy()
# Use the existing indexed palette so the other battle view remains unchanged.
# Simplify the dorsal fin to gold with a coherent pale leading edge.
for y in range(6,32):
 for x in range(18,50):
  v=orig.getpixel((x,y))
  if v in (8,10,13) and (y<17 or x>=(y-17)//2+27):
   im.putpixel((x,y),8)
   if any(orig.getpixel((max(0,x-d),y))==0 for d in (1,2)) or (y in (7,8,12) and v==13):im.putpixel((x,y),13 if v==13 else 10)
# Short, deliberate fin highlights instead of the original scattered patches.
for y,xs in {9:range(29,33),10:range(28,32),11:range(27,31),12:range(25,30),13:range(24,28),14:range(23,26),15:range(22,24),19:range(40,44),20:range(39,43),21:range(38,41)}.items():
 for x in xs:
  if im.getpixel((x,y))==8:im.putpixel((x,y),10)
# Fin white/grey clusters: follow each fin's shape, preserving every outer pixel.
for y in range(33,44):
 for x in range(25,45):
  v=orig.getpixel((x,y))
  if v in (12,13,14,15):
   im.putpixel((x,y),13 if y<=34 else (12 if y<=37 or x<34 else 14))
for x,y in [(29,36),(30,36),(31,37),(32,37),(33,38),(34,38),(35,39),(36,39)]:
 if orig.getpixel((x,y)) in (12,13,14,15):im.putpixel((x,y),15)
for y in range(32,55):
 for x in range(44,60):
  v=orig.getpixel((x,y))
  if v in (12,13,14,15):
   im.putpixel((x,y),13 if y<40 and x>53 else 12)
   if orig.getpixel((min(x+1,63),y))==0:im.putpixel((x,y),1)
   elif orig.getpixel((min(x+2,63),y))==0:im.putpixel((x,y),14)
# One curved division in the tail.
for x,y in [(53,40),(52,41),(51,42),(50,43),(49,44),(48,45),(48,46)]:
 if orig.getpixel((x,y)) in (12,13,14,15):im.putpixel((x,y),15)
# Remove tiny checkerboard patches on the rear body and simplify its lower shadow.
for y in range(38,45):
 for x in range(29,43):
  v=orig.getpixel((x,y))
  if v in (4,5,6,8,9,10): im.putpixel((x,y),5 if y<43 else 4)
# Keep a small warm fissure between plates.
for x,y in [(32,39),(33,40),(34,40),(35,41)]:
 if orig.getpixel((x,y)):im.putpixel((x,y),8)
# Soften internal scale outlines while preserving dark exterior edges.
for y in range(19,33):
 for x in range(23,39):
  if orig.getpixel((x,y)) in (2,3) and all(orig.getpixel((x+dx,y+dy)) for dx,dy in ((1,0),(-1,0),(0,1),(0,-1))):im.putpixel((x,y),4)
# Sharpen the near eye without increasing its footprint.
d=ImageDraw.Draw(im)
d.polygon([(19,26),(21,26),(22,28),(22,30),(20,31),(18,30),(18,28)],fill=4)
d.polygon([(19,27),(20,27),(21,28),(21,30),(19,30),(18,29)],fill=13)
d.point((21,30),fill=12) # Preserve Thermikarp's vacant, pupil-less eye.
# The mouth stays dark, with four separated ivory fangs and a small tongue.
for y in range(34,44):
 for x in range(10,19):
  if orig.getpixel((x,y)) in (2,3,4):im.putpixel((x,y),2 if y<41 else 3)
for points in [[(10,33),(11,33),(10,36)],[(15,34),(16,34),(15,37)],[(11,40),(12,43),(11,43)],[(16,40),(16,43),(15,43)]]:d.polygon(points,fill=13)
d.point((11,43),fill=12);d.point((15,43),fill=12)
# Gold whisker highlights are limited to their light-facing curl edges.
for y in range(48,59):
 for x in range(21,35):
  if orig.getpixel((x,y))==10:im.putpixel((x,y),8)
for x,y in [(27,50),(28,50),(29,50),(26,51),(26,52),(28,53),(29,53)]:
 if orig.getpixel((x,y)):im.putpixel((x,y),10)
# Prevent accidental silhouette changes from hand-placed detail pixels.
for y in range(64):
 for x in range(64):
  if not orig.getpixel((x,y)):im.putpixel((x,y),0)
im.info['transparency']=0;im.save(OUT/'front.png');im.save(DOC/'front.png')
mask=Image.new('L',(64,64));mask.putdata([255 if p else 0 for p in im.get_flattened_data()]);a,b,c,e=mask.getbbox()
p=ROOT/'src/data/pokemon_graphics/front_pic_coordinates.h';s=p.read_text();s=re.sub(r'(\[SPECIES_CINNABAR_MAGIKARP\]\s*=\s*\{).*?(\n    \})',lambda m:m[1]+f'\n        .size = MON_COORDS_SIZE({(c-a+7)//8*8}, {(e-b+7)//8*8}),\n        .y_offset = {64-e},'+m[2],s,flags=re.S);p.write_text(s)
preview=Image.new('RGB',(768,416),'#e8e0c8');draw=ImageDraw.Draw(preview)
for i,(label,pic) in enumerate([('BEFORE',orig),('FRLG CLEANUP',im)]):
 pic.info['transparency']=0;pic=pic.convert('RGBA').resize((384,384),Image.Resampling.NEAREST);preview.paste(pic,(384*i,32),pic);draw.text((384*i+12,10),label,fill='black')
preview.save(DOC/'comparison.png')
print('Preserved silhouette and shared palette; bounds',mask.getbbox())
