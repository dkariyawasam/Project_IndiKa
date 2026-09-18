from pathlib import Path

def read(path):return [tuple(map(int,l.split())) for l in Path(path).read_text().splitlines()[3:]]
def load(n):return read(f'graphics/pokemon/{n}/normal.pal')
def write(path,p):Path(path).write_text('JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,c)) for c in p)+'\n')
e=load('eevee');v=load('venusaur');b=load('bellsprout');g=load('gardevoir');i=load('glalie');l=load('lapras');c=load('chimecho')
normal={'leafeon':[(0,0,0),g[14],g[12],g[13],e[10],e[14],e[13],e[14],e[5],g[13],e[2],e[3],e[5],e[4],e[1],e[10]],'glaceon':[(0,0,0),l[14],l[13],c[4],e[10],i[6],i[9],l[12],i[15],l[14],l[1]]+[(0,0,0)]*5}
shiny={'leafeon':[(0,0,0),v[1],v[4],v[3],e[10],e[14],e[13],e[14],e[5],b[10],e[2],e[11],e[5],e[12],e[1],e[10]],'glaceon':[(0,0,0),l[14],i[7],i[2],e[10],i[6],i[9],g[9],i[15],i[9],l[1]]+[(0,0,0)]*5}
for n in normal:
 old=load(n);ow=read(f'docs/{n}-sprites/source/overworld_normal.pal')
 mapped=[ow[0]]+[normal[n][min(range(1,len(old)),key=lambda k:sum((old[k][c]-v[c])**2 for c in range(3)))] for v in ow[1:]]
 write(f'graphics/object_events/palettes/{n}.pal',mapped)
 write(f'graphics/pokemon/{n}/normal.pal',normal[n]);write(f'graphics/pokemon/{n}/shiny.pal',shiny[n])
