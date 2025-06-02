#include "game/characterinfo.h"
#include "game/gamemem.h"

characterinfo characterinfo_list[3];

Mempool *characterinfo::GetMempoolForSpecies() {
    if (character_info_is_main_player_species(species)) {
        for (int i = 0; i < 3; i++) {
            if (characterinfo_list[i].unk0) {
                return &mempool;
            }
        }
    }
    if (species < 9 || species > 21) {
        return Gamemem_info.GetPerlevelMempool();
    }
    return Gamemem_info.GetSummonMempool();
}

void characterinfo::LoadCharacterInfo() {
    Mempool *pool = GetMempoolForSpecies();
    int size;
    set_current_mempool(pool);
    size = pool->getPoolSpaceLeft();
}

bool characterinfo::character_info_is_main_player_species(int species) {
    bool is_main_player_species = false;
    if (species > -1 && species < 8) {
        is_main_player_species = true;
    }
    return is_main_player_species;
}

void DisplayCharacterMeminfo(char *buffer) {}
