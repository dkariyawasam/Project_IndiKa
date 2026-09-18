from pathlib import Path

def load(name):
    return [tuple(map(int,line.split())) for line in Path(f'graphics/pokemon/{name}/normal.pal').read_text().splitlines()[3:]]
p=load('piloswine');l=load('lapras');v=load('victreebel');s=load('seel');b=load('venusaur')
# Every visible colour is taken directly from an existing FRLG species palette.
normal=[(0,0,0),p[5],p[4],p[6],l[10],p[1],p[15],l[14],l[12],p[12],p[14],p[2],p[6],p[3],s[8],s[9]]
shiny=[(0,0,0),v[11],v[10],v[12],l[10],p[1],p[15],b[1],b[3],p[12],p[14],v[6],v[12],v[7],s[8],s[9]]
def write(path,pal):
    Path(path).write_text('JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,c)) for c in pal)+'\n')
write('graphics/pokemon/mamoswine/normal.pal',normal)
write('graphics/pokemon/mamoswine/shiny.pal',shiny)
write('graphics/object_events/palettes/mamoswine.pal',[(152,208,160),normal[6],normal[3],normal[1],normal[2],normal[12],normal[9],normal[10],normal[4],normal[7],normal[5],normal[11],normal[8],normal[13],normal[14],normal[15]])
