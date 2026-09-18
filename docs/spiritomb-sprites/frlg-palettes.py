from pathlib import Path
normal=[(0,0,0),(139,98,131),(189,148,180),(156,213,74),(189,238,131),(246,246,41),(49,98,41),(82,139,49),(49,65,41),(16,16,16),(115,74,106),(82,49,90),(164,148,123),(123,106,98),(197,180,156),(82,65,74)]
shiny=[(0,0,0),(106,172,197),(156,197,230),(189,74,180),(238,115,156),(255,180,164),(106,49,115),(156,57,164),(74,32,82),(16,16,16),(65,123,156),(49,90,123),(164,148,123),(123,106,98),(197,180,156),(82,65,74)]
def write(path,p):Path(path).write_text('JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,c)) for c in p)+'\n')
write('graphics/pokemon/spiritomb/normal.pal',normal);write('graphics/pokemon/spiritomb/shiny.pal',shiny)
write('graphics/object_events/palettes/spiritomb.pal',[(152,208,160),normal[10],normal[2],normal[1],normal[3],normal[4],normal[7],normal[6],normal[15],normal[13],normal[9],normal[12],(0,0,0),(0,0,0),(0,0,0),(0,0,0)])
