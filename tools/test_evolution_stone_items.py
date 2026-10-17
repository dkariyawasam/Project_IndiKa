"""Compile the actual item gate/effect data to catch unusable evolution stones."""
import pathlib
import subprocess
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[1]


class EvolutionStoneItemsTest(unittest.TestCase):
    def test_stones_reach_evolution_effect(self):
        source = r'''
#include <assert.h>
#include <stddef.h>
typedef unsigned char u8;
#include "constants/items.h"
#include "constants/item_effects.h"
#include "data/pokemon/item_effects.h"
int main(void) {
    const unsigned stones[] = {ITEM_MOON_STONE, ITEM_SUN_STONE,
        ITEM_FIRE_STONE, ITEM_WATER_STONE, ITEM_THUNDER_STONE,
        ITEM_LEAF_STONE, ITEM_SHINY_STONE, ITEM_DUSK_STONE,
        ITEM_DAWN_STONE, ITEM_ICE_STONE};
    for (unsigned i = 0; i < sizeof(stones) / sizeof(stones[0]); i++) {
        unsigned item = stones[i];
        assert(IS_POKEMON_ITEM(item));
        assert(item - ITEM_POTION < sizeof(gItemEffectTable) / sizeof(gItemEffectTable[0]));
        assert(gItemEffectTable[item - ITEM_POTION] != NULL);
        assert(gItemEffectTable[item - ITEM_POTION][4] & ITEM4_EVO_STONE);
    }
    assert(!IS_POKEMON_ITEM(ITEM_POKE_BALL));
    assert(!IS_POKEMON_ITEM(ITEM_METAL_COAT));
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as tmp:
            c = pathlib.Path(tmp) / 'stones.c'
            exe = pathlib.Path(tmp) / 'stones'
            c.write_text(source)
            subprocess.run(['cc', '-std=c99', '-I'+str(ROOT/'include'),
                            '-I'+str(ROOT/'src'), str(c), '-o', str(exe)], check=True)
            subprocess.run([str(exe)], check=True)


if __name__ == '__main__':
    unittest.main()
