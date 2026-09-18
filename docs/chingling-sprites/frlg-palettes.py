from pathlib import Path
normal=[(0,0,0),(123,41,65),(197,74,98),(246,90,115),(197,230,255),(115,148,180),(16,16,16),(106,65,0),(255,213,90),(246,189,90),(255,230,148),(172,115,74),(255,255,255),(0,0,0),(0,0,0),(0,0,0)]
shiny=[(0,0,0),(148,74,0),(197,106,24),(255,148,24),(205,255,172),(98,189,98),(16,16,16),(106,65,0),(255,238,115),(222,197,115),(246,255,189),(197,148,41),(255,255,255),(0,0,0),(0,0,0),(0,0,0)]
def write(path,pal):Path(path).write_text('JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,c)) for c in pal)+'\n')
write('graphics/pokemon/chingling/normal.pal',normal);write('graphics/pokemon/chingling/shiny.pal',shiny)
ow=[(152,208,160),(65,74,106),normal[1],normal[3],normal[6],normal[5],normal[4],normal[8],normal[9],normal[11],normal[10],normal[9],normal[12],normal[7],(0,0,0),(0,0,0)]
write('graphics/object_events/palettes/chingling.pal',ow)
