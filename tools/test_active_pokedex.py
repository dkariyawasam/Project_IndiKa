#!/usr/bin/env python3
"""Host regression tests for the actual Dex membership/count/completion functions."""
import re, subprocess, tempfile
from pathlib import Path
from sync_active_pokedex import ROOT, outputs, read

def function(source,name):
    match=re.search(r'\w+ '+name+r'\([^;]*?\)\n\{',source)
    assert match,name
    end=match.end();depth=1
    while depth:
        depth+=(source[end]=='{')-(source[end]=='}');end+=1
    return source[match.start():end]
files,active=outputs()
for p,s in files.items(): assert read(p)==s,p
assert len(active)==len(set(active))
assert {'EEVEE','SYLVEON','BULBASAUR','GALLADE','TYNAMO','EELEKTROSS','CHIKORITA'}<=set(active)
assert {'ROSERADE','KABUSTAR','BRONZOR','RATTATA_ALOLAN'}<=set(active)
assert not {'TREECKO','TORCHIC','MUDKIP','REGIROCK','REGICE','REGISTEEL'}&set(active)
source=read('src/pokedex.c')
code='''#include <assert.h>
#include <stdint.h>
#include <stdio.h>
typedef uint8_t bool8;
typedef uint16_t bool16;
typedef uint16_t u16;
typedef uint8_t u8;
#define TRUE 1
#define FALSE 0
#include "include/constants/pokedex.h"
enum {FLAG_GET_SEEN, FLAG_GET_CAUGHT, FLAG_SET_SEEN, FLAG_SET_CAUGHT};
#include "src/data/pokemon/active_pokedex.h"
static u8 seen[1024], caught[1024];
static int GetSetPokedexFlag(u16 n, u8 which) {
    if (which == FLAG_SET_SEEN) seen[n]=1;
    if (which == FLAG_SET_CAUGHT) caught[n]=1;
    return which == FLAG_GET_SEEN ? seen[n] : caught[n];
}
'''
for n in ('IsActivePokedexSpecies','GetActivePokedexCount','GetNationalPokedexCount','GetKantoPokedexCount','CompletePokedex','HasAllKantoMons','HasAllMons'):
    code+=function(source,n)+'\n'
code+='''int main(void) {
    assert(!IsActivePokedexSpecies(0));
    assert(!IsActivePokedexSpecies(65535));
    assert(IsActivePokedexSpecies(NATIONAL_DEX_SYLVEON));
    assert(!IsActivePokedexSpecies(NATIONAL_DEX_TREECKO));
    assert(IsActivePokedexSpecies(NATIONAL_DEX_ROSERADE));
    assert(IsActivePokedexSpecies(NATIONAL_DEX_KABUSTAR));
    assert(IsActivePokedexSpecies(NATIONAL_DEX_RATTATA_ALOLAN));
    assert(GetNationalPokedexCount(FLAG_GET_CAUGHT)==0);
    assert(!HasAllMons());
    CompletePokedex();
    assert(GetNationalPokedexCount(FLAG_GET_CAUGHT)==GetActivePokedexCount());
    assert(GetKantoPokedexCount(FLAG_GET_SEEN)==GetActivePokedexCount());
    assert(caught[NATIONAL_DEX_ROSERADE]);
    assert(!caught[NATIONAL_DEX_TREECKO]);
    assert(HasAllMons());
    caught[NATIONAL_DEX_MEW]=0;
    assert(HasAllMons());
    caught[NATIONAL_DEX_SYLVEON]=0;
    assert(!HasAllMons());
    caught[NATIONAL_DEX_SYLVEON]=1;
    caught[NATIONAL_DEX_CHIKORITA]=0;
    assert(!HasAllKantoMons());
    caught[NATIONAL_DEX_CHIKORITA]=1;
    caught[NATIONAL_DEX_TREECKO]=1;
    assert(GetKantoPokedexCount(FLAG_GET_CAUGHT)==GetActivePokedexCount()-1);
    printf("Membership, seen/caught counts, restored placeholders and retired exclusions, and completion: %u species passed\\n",GetActivePokedexCount());
}
'''
with tempfile.TemporaryDirectory(prefix='dex-tests-') as tmp:
    p=Path(tmp);(p/'test.c').write_text(code)
    subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-I',str(ROOT),str(p/'test.c'),'-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],check=True)
