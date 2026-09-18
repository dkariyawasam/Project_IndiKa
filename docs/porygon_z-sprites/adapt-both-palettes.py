from pathlib import Path

def read(path):return [tuple(map(int,l.split())) for l in Path(path).read_text().splitlines()[3:]]
def load(n,kind='normal'):return read(f'graphics/pokemon/{n}/{kind}.pal')
def write(path,p):Path(path).write_text('JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,c)) for c in p)+'\n')
p=load('porygon2');q=load('porygon2','shiny');d=load('dusclops');l=load('lapras');y=load('piloswine');j=load('jynx')
normal={'porygon_z':[(0,0,0),p[4],p[5],p[3],p[4],p[15],p[10],p[2],p[7],p[1],p[11],p[9],y[2],y[5],y[3],(0,0,0)],'dusknoir':[(0,0,0),y[5],y[3],y[2],d[2],d[15],d[4],d[6],d[5],d[4],d[8],(255,0,0),d[2],d[15],(0,0,0),(0,0,0)]}
shiny={'porygon_z':[(0,0,0),q[4],q[5],q[3],q[4],q[15],q[10],q[2],q[7],q[1],q[11],q[9],y[2],y[5],y[3],(0,0,0)],'dusknoir':[(0,0,0),y[5],y[3],y[2],l[14],d[15],l[13],l[11],l[12],l[13],l[11],j[7],l[14],d[15],(0,0,0),(0,0,0)]}
for n in normal:
 old=load(n);ow=read(f'docs/{n}-sprites/source/overworld_normal.pal')
 mapped=[ow[0]]+[normal[n][min(range(1,len(old)),key=lambda k:sum((old[k][c]-v[c])**2 for c in range(3)))] for v in ow[1:]]
 write(f'graphics/object_events/palettes/{n}.pal',mapped)
 write(f'graphics/pokemon/{n}/normal.pal',normal[n]);write(f'graphics/pokemon/{n}/shiny.pal',shiny[n])
