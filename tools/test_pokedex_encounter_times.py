#!/usr/bin/env python3
"""Exercise the production time-mask scan with paired and all-day encounter headers."""
from pathlib import Path
import subprocess
import tempfile
root = Path(__file__).resolve().parents[1]
source = (root / 'src/wild_pokemon_area.c').read_text()
function = source[source.index('u8 GetSpeciesPokedexEncounterTimes(u16 species)'):]
code = '''
#include <assert.h>
typedef unsigned char u8;
typedef unsigned short u16;
#define DEX_ENCOUNTER_DAY 1
#define DEX_ENCOUNTER_NIGHT 2
#define MAP_UNDEFINED 255
#define MAP_GROUP(x) (x)
struct WildPokemonHeader {u8 mapGroup, mapNum; u16 species;};
static const struct WildPokemonHeader gWildMonHeaders[] = {
    {0, 0, 1}, {0, 0, 2},
    {0, 1, 3},
    {0, 2, 4}, {0, 2, 4},
    {0, 3, 5}, {0, 3, 0},
    {0, 4, 0}, {0, 4, 5},
    {255, 255, 0}
};
static int IsSpeciesOnMap(const struct WildPokemonHeader *header, u16 species) {
    return header->species == species;
}
'''+function+'''
int main(void) {
    assert(GetSpeciesPokedexEncounterTimes(1) == 1);
    assert(GetSpeciesPokedexEncounterTimes(2) == 2);
    assert(GetSpeciesPokedexEncounterTimes(3) == 3);
    assert(GetSpeciesPokedexEncounterTimes(4) == 3);
    assert(GetSpeciesPokedexEncounterTimes(5) == 3);
    assert(GetSpeciesPokedexEncounterTimes(6) == 0);
    return 0;
}
'''
with tempfile.TemporaryDirectory() as tmp:
    path = Path(tmp)
    (path / 'test.c').write_text(code)
    subprocess.run(['cc', '-Wall', '-Wextra', '-Werror', str(path / 'test.c'), '-o', str(path / 'test')], check=True)
    subprocess.run([str(path / 'test')], check=True)
print('PASS: day, night, unpaired, both, mixed locations and absent species')
