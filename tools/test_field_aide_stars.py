#!/usr/bin/env python3
"""Run the production card star/completion functions against controlled save data."""
import subprocess
import tempfile
from pathlib import Path
from sync_obtainable_pokedex import generate

ROOT = Path(__file__).resolve().parents[1]

def function(path, signature):
    text = (ROOT / path).read_text()
    start = text.index(signature + '\n{')
    end = text.index('\n}', start) + 2
    return text[start:end]

files, evidence = generate()
for path, contents in files.items():
    assert (ROOT / path).read_text() == contents, path
required = evidence['sources']
for name in ('MEW', 'MEWTWO', 'SMEARGLE', 'PORYGON_Z', 'KABUKNIGHT', 'GOLEM_ALOLAN', 'HUNTAIL', 'GOREBYSS', 'TYROGUE', 'HITMONTOP'):
    assert name in required, name
for name in ('PORYGON3', 'GLACEON', 'ROSERADE', 'CHIKORITA', 'TYNAMO'):
    assert name not in required, name
harness = r'''
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef bool bool8;
typedef bool bool16;
#define TRUE true
#define FALSE false
#define FLAG_GET_CAUGHT 1
#define LEAGUE_CHALLENGE_ROCKET 2
#include "include/constants/pokedex.h"
#include "src/data/pokemon/obtainable_pokedex.h"
static bool caught[NATIONAL_DEX_COUNT + 1];
static bool GetSetPokedexFlag(u16 id, int mode) { assert(mode == FLAG_GET_CAUGHT); return caught[id]; }
struct TrainerCard { struct { u16 hofDebutHours; u8 hofDebutMinutes, hofDebutSeconds; } rse; };
struct LeagueRecord { u16 championships; } record;
static struct LeagueRecord *GetLeagueRecord(u8 type) { assert(type == LEAGUE_CHALLENGE_ROCKET); return &record; }
'''
harness += function('src/pokedex.c', 'bool16 HasAllObtainableMons(void)') + '\n'
harness += function('src/trainer_card.c', 'static u8 GetFieldAideStarCount(struct TrainerCard *trainerCard)') + '\n'
harness += r'''
int main(void) {
    struct TrainerCard card = {0};
    for (int hof = 0; hof < 2; hof++)
    for (int collection = 0; collection < 2; collection++)
    for (int rocket = 0; rocket < 2; rocket++) {
        memset(caught, 0, sizeof(caught));
        if (collection)
            for (int i = 1; i <= NATIONAL_DEX_COUNT; i++) caught[i] = sObtainablePokedexSpecies[i];
        card.rse.hofDebutSeconds = hof;
        record.championships = rocket;
        assert(GetFieldAideStarCount(&card) == hof + collection + rocket);
    }
    // Each required species matters; unobtainable entries never block completion.
    for (int i = 1; i <= NATIONAL_DEX_COUNT; i++) caught[i] = sObtainablePokedexSpecies[i];
    assert(HasAllObtainableMons());
    for (int i = 1; i <= NATIONAL_DEX_COUNT; i++) {
        if (!sObtainablePokedexSpecies[i]) continue;
        caught[i] = false;
        assert(!HasAllObtainableMons());
        caught[i] = true;
    }
    record.championships = 65535;
    assert(GetFieldAideStarCount(&card) == 3);
    return 0;
}
'''
with tempfile.TemporaryDirectory() as tmp:
    source = Path(tmp) / 'stars.c'
    binary = Path(tmp) / 'stars'
    source.write_text(harness)
    subprocess.run(['cc', '-std=c99', '-Wall', '-Werror', '-I', str(ROOT), str(source), '-o', str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
print(f"PASS: eight milestone combinations, all {len(required)} required species, exclusions and repeated championship wins")
