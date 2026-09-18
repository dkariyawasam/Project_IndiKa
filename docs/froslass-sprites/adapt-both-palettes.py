from pathlib import Path

def read(path):return [tuple(map(int,l.split())) for l in Path(path).read_text().splitlines()[3:]]
def load(n,kind='normal'):return read(f'graphics/pokemon/{n}/{kind}.pal')
def write(path,p):Path(path).write_text('JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,c)) for c in p)+'\n')
g=load('gardevoir');s=load('gardevoir','shiny');i=load('glalie');j=load('jynx');c=load('chimecho')
normal={'froslass':[(0,0,0),i[9],i[8],i[6],i[5],i[1],i[3],j[5],j[2],g[15],j[12],j[11],g[6],g[8],g[7],i[6]],'gallade':[(0,0,0),g[14],g[15],g[13],g[4],g[11],g[14],g[12],g[4],g[2],g[1],g[10],g[8],g[3],g[7],g[5]]}
shiny={'froslass':[(0,0,0),i[9],i[8],i[6],i[5],i[1],i[3],j[4],j[2],g[15],j[12],j[11],c[12],c[11],c[2],i[6]],'gallade':[(0,0,0),s[14],s[15],s[13],i[5],i[2],i[6],s[12],s[4],s[2],s[1],g[10],s[8],s[3],s[7],s[5]]}
for n in normal:
 old=load(n);ow=read(f'docs/{n}-sprites/source/overworld_normal.pal')
 mapped=[ow[0]]+[normal[n][min(range(1,len(old)),key=lambda k:sum((old[k][c]-v[c])**2 for c in range(3)))] for v in ow[1:]]
 write(f'graphics/object_events/palettes/{n}.pal',mapped)
 write(f'graphics/pokemon/{n}/normal.pal',normal[n]);write(f'graphics/pokemon/{n}/shiny.pal',shiny[n])
