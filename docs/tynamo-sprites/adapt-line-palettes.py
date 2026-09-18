from pathlib import Path

def read(path):return [tuple(map(int,l.split())) for l in Path(path).read_text().splitlines()[3:]]
def load(n):return read(f'graphics/pokemon/{n}/normal.pal')
def write(path,p):Path(path).write_text('JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,c)) for c in p)+'\n')
i=load('glalie');l=load('lapras');c=load('chinchou');a=load('lanturn');h=load('huntail');v=load('venusaur');b=load('bellsprout');e=load('eevee');j=load('chimecho')
normal={'tynamo':[(0,0,0),i[3],i[4],i[1],i[5],c[15],i[2],a[14],a[15],c[8],a[9],v[9]],'eelektrik':[(0,0,0),e[3],e[5],e[2],c[15],l[14],h[9],l[3],v[8],v[9],a[10],a[9],i[1],i[3]],'eelektross':[(0,0,0),l[14],i[15],h[9],h[7],c[15],e[5],e[3],a[10],a[9],e[2],v[9],v[8],i[5],i[1]]}
shiny={'tynamo':[(0,0,0),h[7],h[8],i[1],h[9],c[15],h[5],j[11],j[12],c[9],a[14],j[2]],'eelektrik':[(0,0,0),b[10],b[2],b[1],c[15],c[7],a[6],v[5],v[6],v[9],b[11],b[9],i[1],i[3]],'eelektross':[(0,0,0),v[1],i[15],v[2],v[4],c[15],h[8],h[7],b[10],b[9],h[5],j[2],j[12],i[5],i[1]]}
for n in normal:
 old=load(n);ow=read(f'docs/{n}-sprites/source/overworld_normal.pal');normal[n]+=[(0,0,0)]*(16-len(normal[n]));shiny[n]+=[(0,0,0)]*(16-len(shiny[n]))
 mapped=[ow[0]]+[normal[n][min(range(1,len(old)),key=lambda k:sum((old[k][c]-v[c])**2 for c in range(3)))] for v in ow[1:]]
 write(f'graphics/object_events/palettes/{n}.pal',mapped);write(f'graphics/pokemon/{n}/normal.pal',normal[n]);write(f'graphics/pokemon/{n}/shiny.pal',shiny[n])
