"""Direct native-resolution pixel pass; run from the repository root."""
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont
ROOT = Path('output/aeropteryx')
OUT = ROOT / 'frlg-pass'
src = Image.open(ROOT / 'front.png')
# Shared 4-bit palette: transparent, outline, bone ramp, membrane ramp, accents.
colors = [(42,42,42),(32,32,48),(64,64,88),(104,104,128),(144,144,168),
          (184,184,208),(216,216,232),(240,240,248),(80,56,112),(128,88,160),
          (184,136,208),(216,176,88),(248,224,144),(72,200,184),(160,72,104),(240,144,168)]
im = src.copy()
im.putpalette(sum((list(c) for c in colors), []) + [0]*720)
# Remove painted noise first; deliberate native-pixel clusters follow below.
remap = {2:2,3:3,4:3,5:5,6:5,7:6,8:8,9:9,10:10}
im.putdata([remap.get(v,v) for v in list(src.get_flattened_data())])
d = ImageDraw.Draw(im)
def poly(points,c): d.polygon(points,fill=c)
def line(points,c,width=1): d.line(points,fill=c,width=width)
# Long neck: lit left plane, broad shaded right plane, clean jaw contact shadow.
poly([(44,18),(45,20),(44,26),(42,31),(43,34),(39,35),(35,32),(35,29),(38,25),(40,20)],3)
poly([(42,22),(43,20),(43,27),(40,32),(38,32),(38,29)],4)
line([(40,24),(38,28),(36,30),(37,32)],2)
# Horns and brow. Preserve the asymmetric two-horn crown.
poly([(35,8),(35,6),(37,5),(37,9),(40,10),(43,7),(45,4),(46,3),(46,8),(44,12),(40,14),(35,13),(34,11)],4)
poly([(35,8),(36,7),(36,10),(39,11),(42,10),(44,8),(45,6),(45,9),(42,12),(37,12),(35,11)],5)
line([(35,9),(36,11),(39,11),(42,10),(44,8)],6)
line([(36,6),(36,8)],12)
line([(45,5),(45,7)],12)
line([(44,8),(44,9)],11)
line([(35,12),(37,13),(41,13),(43,12)],3)
# Eye: a dark socket, light brow, and a deliberately legible turquoise iris.
poly([(39,14),(44,12),(44,15),(40,16),(38,15)],1)
line([(40,14),(43,13)],5)
line([(41,14),(41,15)],13)
d.point((42,14),fill=13)
d.point((43,14),fill=6)
# Snout and cheek as a single clean bone plane.
poly([(26,14),(27,15),(34,15),(37,16),(39,16),(42,16),(44,15),(46,16),(48,16),(47,17),(44,17),(43,19),(40,18),(38,18),(36,19),(33,19),(31,21),(29,23),(27,22),(26,19),(25,18),(25,16)],5)
line([(26,15),(32,16),(36,16)],6)
line([(26,16),(26,18)],6)
poly([(26,19),(28,19),(28,21),(29,22),(30,20),(31,19),(33,18),(35,18),(35,19),(32,20),(30,23),(28,22),(27,21)],3)
line([(27,19),(27,20)],2)
line([(32,19),(31,20)],2)
line([(36,18),(38,18),(40,19),(43,18)],2)
line([(44,16),(46,17),(47,17)],6)
# Open mouth: dark cavity behind the pink tongue, strong rim and lower jaw.
poly([(38,19),(41,19),(39,22),(36,25),(34,27),(31,25),(32,23),(34,21)],1)
poly([(37,20),(40,20),(38,22),(35,25),(33,25),(34,23)],14)
poly([(36,22),(38,21),(37,23),(34,25),(33,25)],15)
line([(41,19),(40,22),(37,25),(35,28),(34,29),(32,28),(31,26)],2)
line([(40,20),(39,22),(36,25),(34,27),(33,27),(32,26)],5)
line([(31,25),(32,26),(34,26)],6)
line([(34,28),(33,28)],3)
# Shoulder mantle and chest: one continuous illuminated plate.
poly([(28,30),(31,31),(34,31),(37,33),(40,34),(43,33),(45,33),(45,35),(43,37),(42,40),(40,43),(39,45),(36,44),(33,40),(31,36),(27,36)],5)
line([(28,31),(31,32),(34,32)],6)
poly([(34,34),(38,35),(41,35),(44,34),(42,37),(41,40),(39,43),(37,41),(35,38)],6)
line([(43,37),(41,42),(39,45),(36,44),(34,42)],2)
poly([(29,37),(31,36),(33,40),(36,44),(38,45),(34,46),(31,42)],3)
# Far folded wing to the right: clear horizontal spar over purple membrane.
poly([(40,43),(43,44),(47,44),(51,44),(56,44),(58,44),(55,45),(50,45),(47,46),(43,46),(40,46)],5)
line([(42,44),(48,44),(54,44)],6)
line([(56,44),(57,44)],12)
line([(41,46),(47,46),(50,45),(54,45)],2)
poly([(40,47),(43,47),(46,47),(49,47),(50,49),(49,52),(47,49),(44,49),(42,48)],9)
poly([(42,47),(46,47),(48,48),(49,50),(47,49),(45,48),(43,48)],10)
line([(49,47),(50,49),(49,52)],3)
# Tail: angular bone edge with a single lit ridge.
line([(5,36),(9,34),(11,35),(13,37),(15,37),(17,40),(20,43),(23,45)],5)
line([(7,35),(9,34),(10,35)],6)
line([(14,37),(15,37),(16,39)],6)
line([(18,42),(20,44),(23,46)],3)
# Near folded membrane: simple uninterrupted purple triangle.
poly([(24,40),(27,43),(30,47),(33,52),(33,55),(30,53),(27,51),(24,49),(20,47)],9)
poly([(25,44),(28,47),(31,51),(32,53),(28,51),(25,49),(22,48)],10)
line([(24,40),(25,44),(26,47)],8)
# Small upper inset clearly framed by bone.
poly([(27,37),(30,38),(31,40),(29,39)],10)
line([(27,37),(30,37),(31,40)],2)
# Near wing spar, kept broad enough to read as bone, with gold tip.
poly([(22,36),(25,38),(27,41),(29,44),(32,47),(34,50),(34,53),(32,52),(30,48),(27,45),(25,42),(23,40),(21,37)],5)
line([(22,36),(23,38),(25,40),(27,43),(30,46),(32,49)],6)
line([(24,39),(26,42),(29,45),(31,47),(33,50)],3)
line([(34,50),(34,52)],12)
d.point((33,50),fill=11)
# Three uninterrupted pixel bands define the diagonal foreground wing bone.
for y, x0, x1 in [(36,21,23),(37,22,24),(38,23,25),(39,24,26),(40,25,27),(41,25,28),(42,26,29),(43,27,30),(44,28,31),(45,29,32),(46,30,33),(47,31,33),(48,32,34),(49,32,34),(50,33,34)]:
    line([(x0,y),(x1,y)],5)
    d.point((x0,y),fill=6)
    d.point((x1,y),fill=3)
line([(34,50),(34,52)],12)
# Join the lower membrane into clean clusters instead of isolated paint flecks.
for y, x0, x1 in [(47,22,25),(48,22,27),(49,23,28),(50,25,29),(51,27,30),(52,28,31),(53,30,31)]:
    line([(x0,y),(x1,y)],10)
line([(23,47),(25,49),(28,51),(30,52)],9)
# Feet with flat highlights and dark toes; avoid noisy grey pixels.
line([(31,54),(32,55),(32,57),(31,58)],3)
line([(31,56),(32,56)],5)
line([(29,57),(31,58),(29,59)],2)
line([(49,51),(49,53),(50,54)],3)
line([(48,53),(48,54),(50,54)],4)
# Preserve the supplied silhouette exactly; keep its original dark outer edge.
pix, original = im.load(), src.load()
for y in range(64):
    for x in range(64):
        if original[x,y] == 0:
            pix[x,y] = 0
        elif original[x,y] == 1 and any(0 <= x+dx < 64 and 0 <= y+dy < 64 and original[x+dx,y+dy] == 0 for dx,dy in [(1,0),(-1,0),(0,1),(0,-1)]):
            pix[x,y] = 1
im.save(OUT/'front.png', transparency=0, bits=4)
(OUT/'normal.pal').write_text('JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,c)) for c in colors)+'\n')
# Preview and before/after sheet use nearest-neighbour scaling only.
bg = (238,234,219)
def framed(sprite,scale):
    tile=Image.new('RGB',(64,64),bg)
    rgba=sprite.convert('RGBA')
    tile.paste(rgba,(0,0),rgba)
    return tile.resize((64*scale,64*scale),Image.Resampling.NEAREST)
framed(im,8).save(OUT/'front-preview.png')
canvas=Image.new('RGB',(820,490),bg)
draw=ImageDraw.Draw(canvas)
draw.text((20,12),'BEFORE',fill=(40,40,50))
draw.text((420,12),'FRLG PIXEL PASS',fill=(40,40,50))
canvas.paste(framed(src,6),(16,36))
canvas.paste(framed(im,6),(416,36))
canvas.paste(framed(src,1),(180,424))
canvas.paste(framed(im,1),(580,424))
canvas.save(OUT/'comparison.png')
print('Saved 64x64 indexed sprite:',OUT/'front.png')
print('Used palette entries:',len(set(list(im.get_flattened_data()))))
